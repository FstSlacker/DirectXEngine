#include "Game.h"
#include "InputDevice.h"

Game::Game() : Gizmos(this)
{
	MainCamera = new Camera(this, 1, 1);
}

bool Game::Initialize()
{
	Display = new DisplayWin32(1000, 800, Name);

	MainCamera->SetAspectRatio(static_cast<float>(Display->ClientWidth) / static_cast<float>(Display->ClientHeight));

	Input = new InputDevice(this);

	if (!Gfx.Initialize(Display->hWnd, Display->ClientWidth, Display->ClientHeight))
	{
		return false;
	}

	ImGUI.Initialize(Gfx.GetDevice(), Gfx.GetContext(), Display->hWnd);
	Gizmos.Initialize();

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
	for (int i = 0; i < Components.size(); i++) 
	{
		if (!Components[i]->IsEnabled)
			continue;

		Components[i]->Draw();
	}

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

		if (msg.message == WM_CLOSE) {
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
		if (!Components[i]->IsEnabled)
			continue;

		Components[i]->Update();
	}
}

void Game::UpdateInternal()
{
	//Update components
	for (int i = 0; i < Components.size(); i++)
	{
		if (!Components[i]->IsEnabled)
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

			if (!Components[i]->IsEnabled || !Components[j]->IsEnabled)
				continue;

			ColliderBase* c1 = Components[i]->Collider;
			ColliderBase* c2 = Components[j]->Collider;

			if (c1 == nullptr || c2 == nullptr)
				continue;

			c1->HandleCollision(c2);
		}
	}
}
