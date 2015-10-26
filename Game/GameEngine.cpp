#include "GameEngine.h"
#include "..\Timer\Timer.h"
#include "..\Graphics\D3D11Renderer.h"
#include "..\Memory\MemoryManager.h"
#include "..\Debug\Debug.h"
#include "..\Font\Font.h"
#include "..\Object\ObjectLoader.h"

typedef SIMDVector3 Vector3;

long GameEngine::cursor_pos[2] = { 0.0f, 0.0f };

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}
	else if (msg == WM_PAINT) {
		ValidateRect(hWnd, NULL);
		return 0;
	}
	else {
		if (msg >= WM_MOUSEFIRST && msg <= WM_MOUSELAST)
			GameEngine::MouseEventHandeler(hWnd, msg, wParam, lParam);
		else if (msg >= WM_KEYFIRST && msg <= WM_KEYLAST)
			GameEngine::KeyboardEventHandeler(hWnd, msg, wParam, lParam);

		InvalidateRect(hWnd, NULL, TRUE);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

GameEngine::GameEngine()
{
}

GameEngine::~GameEngine()
{
}

void GameEngine::SetCursorPosition(long x, long y) {
	cursor_pos[0] = x;
	cursor_pos[1] = y;
}

void GameEngine::MouseEventHandeler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	static bool firstMove = false;
	if (!firstMove) {
		GameEngine::SetCursorPosition(LOWORD(lParam), HIWORD(lParam));
		firstMove = true;
		return;
	}

	if (msg == WM_MOUSEMOVE && 
		D3D11Renderer::GetInstance()->GetCameraType() == CameraType::DEGREE360_CAMERA && 
		(GetAsyncKeyState(VK_LBUTTON) || GetAsyncKeyState(VK_RBUTTON))
	) {
		D3D11Renderer::GetInstance()->GetCamera()->rotateVPos(
			(LOWORD(lParam) - cursor_pos[0]) / 150.0f,
			(HIWORD(lParam) - cursor_pos[1]) / 150.0f
		);
	}

	GameEngine::SetCursorPosition(LOWORD(lParam), HIWORD(lParam));
}

void GameEngine::KeyboardEventHandeler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_KEYDOWN) {
		if (GetAsyncKeyState(VK_1)) {
			D3D11Renderer::GetInstance()->SetCamera(CameraType::MOVE_CAMERA);
		} else if (GetAsyncKeyState(VK_2)) {
			D3D11Renderer::GetInstance()->SetCamera(CameraType::DEGREE360_CAMERA);
		} else if (GetAsyncKeyState(VK_3)) {
			D3D11Renderer::GetInstance()->SetCamera(CameraType::TOP_VIEW_CAMERA);
		} else if (GetAsyncKeyState(VK_4)) {
			D3D11Renderer::GetInstance()->SetCamera(CameraType::BOTTOM_VIEW_CAMERA);
		} else if (GetAsyncKeyState(VK_5)) {
			D3D11Renderer::GetInstance()->SetCamera(CameraType::LEFT_VIEW_CAMERA);
		} else if (GetAsyncKeyState(VK_6)) {
			D3D11Renderer::GetInstance()->SetCamera(CameraType::RIGHT_VIEW_CAMERA);
		} else if (GetAsyncKeyState(VK_7)) {
			D3D11Renderer::GetInstance()->SetCamera(CameraType::FRONT_VIEW_CAMERA);
		} else if (GetAsyncKeyState(VK_8)) {
			D3D11Renderer::GetInstance()->SetCamera(CameraType::BACK_VIEW_CAMERA);
		}

		if (D3D11Renderer::GetInstance()->GetCameraType() == CameraType::MOVE_CAMERA) {
			if (GetAsyncKeyState(VK_W) || GetAsyncKeyState(VK_S)) {
				D3D11Renderer::GetInstance()->GetCamera()->move(
					GetAsyncKeyState(VK_W) ? CameraMove::FORWARD : CameraMove::BACKWARD, 0.4f
				);
			}
			else if (GetAsyncKeyState(VK_A) || GetAsyncKeyState(VK_D)) {
				D3D11Renderer::GetInstance()->GetCamera()->move(
					GetAsyncKeyState(VK_A) ? CameraMove::LEFT : CameraMove::RIGHT, 0.4f
				);
			} else if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(VK_RIGHT)) {
				D3D11Renderer::GetInstance()->GetCamera()->rotateVLookAt(
					GetAsyncKeyState(VK_LEFT) ? CameraMove::ROTATE_LEFT : CameraMove::ROTATE_RIGHT, 0.03f
				);
			} else if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(VK_DOWN)) {
				D3D11Renderer::GetInstance()->GetCamera()->rotateVLookAt(
					GetAsyncKeyState(VK_DOWN) ? CameraMove::ROTATE_DOWN : CameraMove::ROTATE_UP, 0.03f
				);
			}
		}
	}
}

void GameEngine::Start(HINSTANCE hInst)
{
	UNREFERENCED_PARAMETER(hInst);

	// Register the window class
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		"Game class", NULL
	};
	RegisterClassEx(&wc);

	// Create the application's window
	HWND hWnd = CreateWindow("Game class", "Game Engine",
		WS_OVERLAPPEDWINDOW, 0, 0, 1024, 768,
		NULL, NULL, wc.hInstance, NULL);

	// Setup our GameWorld and GraphicsDevice singletons
	D3D11Renderer::GetInstance()->ConstructWithWindow(hWnd);

	Vertex1P vertices[][2] = {
		//	VertexA, VertexB
		{ Vector3(3.0f, 3.0f, 1.0f),		Vector3(3.0f, 2.0f, 1.0f) },
		{ Vector3(-3.0f, 3.0f, 1.0f),		Vector3(-3.0f, 2.0f, 1.0f) },
		//	Vertex(center), Size(width, height, depth)
		{ Vector3(3.0f, 0.0f, 1.0f),		Vector3(4.0f, 4.0f, 4.0f) },
		{ Vector3(-3.0f, 0.0f, 1.0f),		Vector3(1.0f, 1.5f, 2.0f) },
		{ Vector3(3.0f, -3.0f, 1.0f),		Vector3(1.0f, 1.5f, 2.0f) },
		{ Vector3(-3.0f, -3.0f, 1.0f),		Vector3(1.0f, 1.5f, 2.0f) }
	};

	Debug debug;
	//Two vertices
//	debug.draw_line(vertices[0]);
//	debug.draw_line(vertices[1]);
	//Vector3(center_x, center_y, center_z), Vector3(width, height depth), vertexPerCrossSection, num_slices)
	debug.draw_prism(vertices[2][0], vertices[2][1], Primitives::CYLINDER);
//	debug.draw_pyramid(vertices[3][0], vertices[3][1], Primitives::CONE, 30);
//	debug.draw_pyramid(vertices[4][0], vertices[4][1], Primitives::RECTANGULAR_PYRAMID, 30);
//	debug.draw_ellipsoid(vertices[5][0], vertices[5][1], Primitives::ELLIPSOID, 30);
	

	Font font;
	font.write("JACKHSK", 6.0f, -2.0f);	 //All supported characters
	font.write("Elegant Engine", 0.0f, 0.0f);

	//Import .obj object
	ObjectLoader objectLoader("CityColony");
	if (objectLoader.LoadWaveFrontObject(0.01f)) {
		objectLoader.Draw();
	}

	/*
	for (std::vector<MeshInstance*>::iterator itr = D3D11Renderer::GetInstance()->GetMeshInstanceList().begin();
		itr < D3D11Renderer::GetInstance()->GetMeshInstanceList().end();
		itr++
	) {
		(*itr)->Transform(
			new float(1.0f),						//scaling
			&(Vector3(0.0f, 0.0f, 0.0f)),			//rotation
			&(Vector3(0.0f, 0.0f, -15.0f))			//translation
		);
	}
	*/

	// Show the window
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	/// Timer
	Timer m_Timer;
	const float FPS = 30.0f;
	float elaspedTime = 0.0f;

	// Memory
	//MemoryManager::GetInstance()->Construct();

	// enter the main game loop
	bool bQuit = false;
	while (!bQuit)
	{
		if (elaspedTime >= 1.0 / FPS)
		{
			// Update the game world based on delta time
			D3D11Renderer::GetInstance()->Update();

			// Render this frame
			D3D11Renderer::GetInstance()->Render();

			// Debug text
			std::stringstream str;
			str << "FPS: " << 1.0f / elaspedTime;
			SetWindowText(hWnd, str.str().c_str());
			elaspedTime = 0.0f;
		}

		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				bQuit = true;
				break;
			}
		}

		m_Timer.tick();
		elaspedTime += m_Timer.getDeltaTime();
	}

	// Cleanup the GameWorld and GraphicsDevice singletons
	D3D11Renderer::GetInstance()->DestructandCleanUp();
	MemoryManager::GetInstance()->DestructandCleanUp();

	UnregisterClass("Game class", wc.hInstance);
}