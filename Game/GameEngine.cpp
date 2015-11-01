#include "GameEngine.h"
#include "..\Timer\Timer.h"
#include "..\Graphics\D3D11Renderer.h"
#include "..\Memory\MemoryManager.h"
#include "..\Debug\Debug.h"
#include "..\Font\Font.h"
#include "..\Object\ObjectLoader.h"
#include "..\Physics\cdCollisionWorld.h"
#include "..\Physics\cdSphere.h"
#include "..\Physics\cdObject.h"
#include "..\Physics\cdAabb.h"

typedef SIMDVector3 Vector3;

long GameEngine::cursor_pos[2] = { 0, 0 };

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

	Vector3 origin1(0.0f, 5.0f, 0.0f);
	Vector3 origin2(0.0f, -5.0f, 0.0f);
	Vector3 dimension(1.0f, 1.0f, 1.0f);

	float aabb[6];
	aabb[0] = origin1.GetX() + dimension.GetX() / 2;
	aabb[1] = origin1.GetY() - dimension.GetY() / 2;
	aabb[2] = origin1.GetZ() + dimension.GetZ() / 2;
	aabb[3] = origin1.GetX() - dimension.GetX() / 2;
	aabb[4] = origin1.GetY() + dimension.GetY() / 2;
	aabb[5] = origin1.GetZ() - dimension.GetZ() / 2;

	Vector3 min1(aabb[0], aabb[1], aabb[2]);
	Vector3 max1(aabb[3], aabb[4], aabb[5]);

	float aabb1[6];
	aabb1[0] = origin2.GetX() + dimension.GetX() / 2;
	aabb1[1] = origin2.GetY() - dimension.GetY() / 2;
	aabb1[2] = origin2.GetZ() + dimension.GetZ() / 2;
	aabb1[3] = origin2.GetX() - dimension.GetX() / 2;
	aabb1[4] = origin2.GetY() + dimension.GetY() / 2;
	aabb1[5] = origin2.GetZ() - dimension.GetZ() / 2;

	Vector3 min2(aabb1[0], aabb1[1], aabb1[2]);
	Vector3 max2(aabb1[3], aabb1[4], aabb1[5]);

	Debug debug;
	//Two vertices

/**
	debug.draw_line(vertices[0]);
	debug.draw_line(vertices[1]);
	//Vector3(center_x, center_y, center_z), Vector3(width, height depth), vertexPerCrossSection, num_slices)
	debug.draw_prism(vertices[2][0], vertices[2][1], Primitives::CYLINDER);
	debug.draw_pyramid(vertices[3][0], vertices[3][1], Primitives::CONE, 30);
	debug.draw_pyramid(vertices[4][0], vertices[4][1], Primitives::RECTANGULAR_PYRAMID, 30);
	debug.draw_ellipsoid(vertices[5][0], vertices[5][1], Primitives::ELLIPSOID, 30);
	float translate = 0.0f;
	for (int i = 0; i < 100; i++)
	{
		translate += i;
		debug.draw_prism(Vector3(translate, translate, translate), vertices[2][1], Primitives::CYLINDER);
	}
*/
	debug.draw_ellipsoid(Vector3(5.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Primitives::SPHERE, 30);
	debug.draw_ellipsoid(Vector3(-5.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Primitives::SPHERE, 30);
	
//	debug.draw_prism(origin1, dimension, Primitives::RECTANGULAR_PRISM);
//	debug.draw_prism(origin2, dimension, Primitives::RECTANGULAR_PRISM);

	// GameObject* array[1]
	//array[0] = new GamObject(blah blah blah);
	

	// collision world testing start
	CollisionWorld world;
	Sphere sphere1(Vector3(5.0f, 0.0f, 0.0f), 1.0f);
	Sphere sphere2(Vector3(-5.0f, 0.0f, 0.0f), 1.0f);
	CollidableObject object1(&sphere1, 0);
	CollidableObject object2(&sphere2, 1);
	world.addObject(object1);
	world.addObject(object2);
	Vector3 velocity1(-0.5f, 0.0f, 0.0f);
	Vector3 velocity2(0.5f, 0.0f, 0.0f);
	
	Vector3 velocity4(0.0f, 0.5f, 0.0f);


	AABB aabb_1(min1, max1);
	AABB aabb_2(min2, max2);
//	CollidableObject object3(&aabb_1, 2);
//	CollidableObject object4(&aabb_2, 3);
	//world.addObject(object3);
	//.addObject(object4);


	// collision world testing end


	Font font;
	char s[64], p[64];

	sprintf(s, "%f, %f, %f, %f, %f, %f", min1.GetX(), min1.GetY(), min1.GetZ(),
		max1.GetX(), max1.GetY(), max1.GetZ());
	sprintf(p, "%f, %f, %f, %f, %f, %f", min2.GetX(), min2.GetY(), min2.GetZ(),
		max2.GetX(), max2.GetY(), max2.GetZ());
	font.write(s, 6.0f, 6.0f);
	font.write(p, 6.0f, 4.0f);

//	font.write("JACKHSK", 6.0f, -2.0f);	 //All supported characters
//	font.write("Elegant Engine", 0.0f, 0.0f);
/**
	//Import .obj object
	ObjectLoader objectLoader("CityColony");
	if (objectLoader.LoadWaveFrontObject(0.01f)) {
		objectLoader.Draw();
	}
*/
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
	const float FPS = 5.0f;
	float elaspedTime = 0.0f;

	// m0, m1 -> sphere1, sphere2
	MeshInstance* m0 = D3D11Renderer::GetInstance()->GetMeshInstanceList().at(0);
	MeshInstance* m1 = D3D11Renderer::GetInstance()->GetMeshInstanceList().at(1);

	// m2, m3 -> box1, box2
//	MeshInstance* m2 = D3D11Renderer::GetInstance()->GetMeshInstanceList().at(2);
//	MeshInstance* m3 = D3D11Renderer::GetInstance()->GetMeshInstanceList().at(3);

	// Memory
	//MemoryManager::GetInstance()->Construct();
	Vector3 velocity3(0.0f, -0.5f, 0.0f);
	// enter the main game loop
	bool bQuit = false;
	while (!bQuit)
	{

		if (elaspedTime >= 1.0 / FPS)
		{
			m0->Transform(
				new float(1.0f),						//scaling
				&(Vector3(0.0f, 0.0f, 0.0f)),			//rotation
				&(velocity1)			//translation
			);
			sphere1.translate(Vector3(-0.5f, 0.0f, 0.0f));
			
			m1->Transform(
				new float(1.0f),						//scaling
				&(Vector3(0.0f, 0.0f, 0.0f)),			//rotation
				&(velocity2)			//translation
			);
			sphere2.translate(Vector3(0.5f, 0.0f, 0.0f));
/**			

			m2->Transform(
				new float(1.0f),						//scaling
				&(Vector3(0.0f, 0.0f, 0.0f)),			//rotation
				&(velocity3)			//translation
				);
			aabb_1.translate(velocity3);
			m3->Transform(
				new float(1.0f),						//scaling
				&(Vector3(0.0f, 0.0f, 0.0f)),			//rotation
				&(velocity4)			//translation
				);
			aabb_2.translate(velocity4);
*/		
			world.computeCollision();
			sprintf(p, "%d", world.getCollideSize());
			font.write(p, 6.0f, 6.0f);

			if (world.getCollideSize() == 1) {
//			if (count)
				font.write("JACKHSK", 0.0f, 0.0f);
				velocity3.SetY(0.5f);
				velocity4.SetY(-0.5f);
				
			}

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