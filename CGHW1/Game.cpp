#include "Game.h"

Game::Game() : Gizmos(this), RenderSystem(*this)
{
	//...
}

bool Game::Initialize()
{
	Display = new DisplayWin32(*this, 1200, 700, Name);

	Camera::Main = new Camera(this);
	Camera::Main->Transform.SetPosition(Vector3(0.0f, 5.0f, -15.0f));
	Camera::Main->Transform.SetRotation(Vector3(30.0f, 0.0f, 0.0f));
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

	ImGUI.Initialize(Gfx.GetDevice(), Gfx.GetContext(), Display->hWnd);
	Gizmos.Initialize();

	for (int i = 0; i < Components.size(); i++) 
	{
		Components[i]->Initialize();
	}

	if (!RenderSystem.Initialize())
	{
		return false;
	}

	for (int i = 0; i < Components.size(); i++)
	{
		MeshComponent* mesh = dynamic_cast<MeshComponent*>(Components[i]);
		if (mesh != nullptr)
		{
			RenderSystem.RegisterMesh(mesh);
			continue;
		}
		LightComponent* light = dynamic_cast<LightComponent*>(Components[i]);
		if (light != nullptr)
		{
			RenderSystem.RegisterLight(light);
			continue;
		}
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

	RenderSystem.OpaquePass();
	RenderSystem.LightPass();

	//for (int i = 0; i < Components.size(); i++)
	//{
	//	ParticleSystem* ps = dynamic_cast<ParticleSystem*>(Components[i]);
	//	if (ps != nullptr)
	//	{
	//		ps->Bind();
	//		ps->Draw();
	//	}
	//}

	//Draw debug
	Gizmos.Draw();

	//Draw ImGui
	ImGUI.Draw();

	Gfx.EndFrame();
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

		//int a;
		//std::cin >> a;

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
