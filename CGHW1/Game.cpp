#include "Game.h"
#include "InputDevice.h"

Game::Game() 
{
	//...
}

bool Game::Initialize()
{
	Display = new DisplayWin32(1000, 800, Name);


	MainCamera = new Camera(Transform3D(Vector3(0.0f, 0.0f, -15.0f), Vector3::Zero, Vector3::One), Display->ClientWidth, Display->ClientHeight);

	Input = new InputDevice(this);

	if (!Gfx.Initialize(Display->hWnd, Display->ClientWidth, Display->ClientHeight))
	{
		return false;
	}

	ImGUI.Initialize(Gfx.GetDevice(), Gfx.GetContext(), Display->hWnd);

	for (int i = 0; i < Components.size(); i++) 
	{
		Components[i]->Initialize();
	}

	return true;
}

void Game::DestroyResources()
{
	for (int i = 0; i < Components.size(); i++) 
	{
		Components[i]->DestroyResources();
	}

	Gfx.DestroyResources();
	ImGUI.DestroyResouces();
}

void Game::Draw()
{
	Gfx.PrepareFrame();

	//Draw components
	for (int i = 0; i < Components.size(); i++) {
		Components[i]->Draw();
	}

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
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {

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
				/*MOUSE_MOVE_RELATIVE*/
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

		if(msg.message == WM_MOUSEHOVER)

		if (msg.message == WM_QUIT) {
			isExitRequested = true;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
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
		Components[i]->Update();
	}
}

void Game::UpdateInternal()
{
	//Update components
	for (int i = 0; i < Components.size(); i++)
	{
		Components[i]->FixedUpdate();
	}

	//Handle collisions
	for (int i = 0; i < Components.size(); i++)
	{
		for (int j = 0; j < Components.size(); j++)
		{
			if (i == j)
				continue;

			ColliderBase* c1 = Components[i]->Collider;
			ColliderBase* c2 = Components[j]->Collider;

			if (c1 == nullptr || c2 == nullptr)
				continue;

			c1->HandleCollision(c2);
		}
	}
}
