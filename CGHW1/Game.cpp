#include "Game.h"

Game::Game() : Gizmos(this), Light(this)
{
	//...
}

bool Game::Initialize()
{
	Display = new DisplayWin32(*this, 1200, 700, Name);

	Camera::Main = new Camera(this);
	Camera::Main->Transform.SetPosition(Vector3(0.0f, 0.0f, -15.0f));
	Camera::Main->SetAspectRatio(static_cast<float>(Display->ClientWidth) / static_cast<float>(Display->ClientHeight));

	Input = new InputDevice(this);

	if (!Gfx.Initialize(Display->hWnd, Display->ClientWidth, Display->ClientHeight))
	{
		return false;
	}

	if (!Resources.InitializeResources(Gfx.GetDevice()))
	{
		return false;
	}

	if (!Light.Initialize())
	{
		return false;
	}

	ImGUI.Initialize(Gfx.GetDevice(), Gfx.GetContext(), Display->hWnd);
	Gizmos.Initialize();

	for (int i = 0; i < Components.size(); i++) 
	{
		Components[i]->Initialize();
	}

	isInitialized = true;

	return true;
}

bool Game::ResizeWindow(UINT width, UINT height)
{
	if (!isInitialized)
		return false;

	if (!Gfx.Resize(width, height))
	{
		return false;
	}

	return true;
}

void Game::DestroyResources()
{
	for (int i = 0; i < Components.size(); i++) 
	{
		Components[i]->DestroyResources();
		delete Components[i];
	}

	Gfx.DestroyResources();
	ImGUI.DestroyResouces();
}

void Game::Draw()
{
	Gfx.PrepareFrame();

	RenderJob shadowJob;
	shadowJob.SetQueueIndex(0);
	shadowJob.AddBindable(Resources.GetResource<VertexShader>("VS_Depth"));
	shadowJob.AddBindable(Resources.GetResource<PixelShader>("PS_Depth"));

	for (int i = 0; i < Components.size(); i++)
	{
		shadowJob.AddComponent(Components[i]);
	}

	cascadeShadowMapPass.AddRenderJob(&shadowJob);
	cascadeShadowMapPass.BindDirectionalLight(*dynamic_cast<DirectionalLightComponent*>(Light.GetLightComponent(0)));
	cascadeShadowMapPass.BindMainCamera(*Camera::Main);
	cascadeShadowMapPass.Execute(Gfx);

	Gfx.GetContext()->OMSetRenderTargets(0, nullptr, nullptr);

	//shadowMapPass.AddRenderJob(&shadowJob);
	//shadowMapPass.BindLightSource(*Light.GetLightComponent(1));
	//shadowMapPass.Execute(Gfx);

	Gfx.GetContext()->OMSetRenderTargets(0, nullptr, nullptr);

	Light.Bind(Gfx.GetContext());
	UINT materialsCount = Resources.GetCount<Material>();
	for (int i = 0; i < materialsCount; i++)
	{
		renderQueuePass.AddRenderJob(Resources.GetResource<Material>(i));
	}

	renderQueuePass.Execute(Gfx);

	//Draw debug
	Gizmos.Draw();

	//Draw ImGui
	ImGUI.Draw();

	Gfx.EndFrame();

	renderQueuePass.Clear();
	shadowMapPass.Clear();
	cascadeShadowMapPass.Clear();
}

void Game::Exit()
{
	this->isExitRequested = true;
}

void Game::Pause()
{
	this->isPauseRequested = true;
}

void Game::MessageHandler()
{
	MSG msg{};
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) 
	{

		if (msg.message == WM_KEYDOWN || msg.message == WM_KEYUP)
		{
			InputDevice::KeyboardInputEventArgs args{
				0,
				msg.message,
				msg.wParam,
				msg.message
			};
			Input->OnKeyDown(args);
		}
		if (msg.message == WM_MOUSEMOVE)
		{
			InputDevice::RawMouseEventArgs args
			{
				msg.message,
				msg.wParam,
				0,
				0,
				0,
				LOWORD(msg.lParam),
				HIWORD(msg.lParam)
			};
			Input->OnMouseMove(args);
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT) {
			this->Exit();
		}
	}
}

void Game::RestoreTarget()
{
	//...
}

void Game::Run()
{
	if (!Initialize())
	{
		return;
	}

	prevTime = std::chrono::steady_clock::now();

	while (!isExitRequested) 
	{
		auto curTime = std::chrono::steady_clock::now();
		DeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - prevTime).count() / 1000000.0f;
		prevTime = curTime;

		MessageHandler();
		UpdateInternal();
		Update();
		Draw();

		if (isPauseRequested) 
		{
			system("pause");
			isPauseRequested = false;
		}
	}
	DestroyResources();
}

void Game::Update()
{
	for (int i = 0; i < Components.size(); i++)
	{
		if (!Components[i]->IsEnabled())
			continue;

		Components[i]->Update();
	}
}

void Game::UpdateInternal()
{
	//Update components
	for (int i = 0; i < Components.size(); i++)
	{
		if (!Components[i]->IsEnabled())
			continue;

		Components[i]->FixedUpdate();
	}

	//Handle collisions
	for (int i = 0; i < Components.size(); i++)
	{
		for (int j = 0; j < Components.size(); j++)
		{
			if (i == j)
				continue;

			if (!Components[i]->IsEnabled() || !Components[j]->IsEnabled())
				continue;

			ColliderBase* c1 = Components[i]->Collider;
			ColliderBase* c2 = Components[j]->Collider;

			if (c1 == nullptr || c2 == nullptr)
				continue;

			c1->HandleCollision(c2);
		}
	}
}
