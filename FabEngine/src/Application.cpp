#include <DirectXComponentsPCH.h>
#include "Application.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return Fab::Application::GetApplication().MsgProc(hWnd, message, wParam, lParam);
}

namespace Fab
{
	Application* Application::_pApplication = nullptr;

	Application::Application(std::wstring mainWndCaption)
		: _renderSystem(D3D11RenderSystem::GetRenderSystem())
		, _mainWndCaption(mainWndCaption)
		, _windowWidth(_renderSystem.GetWindowWidth())
		, _windowHeight(_renderSystem.GetWindowHeight())
		, _appPaused(false)
		, _minimized(false)
		, _maximized(false)
		, _resizing(false)
		, _initialized(true)
		, _hInst(nullptr)
		, _hWnd(nullptr)
	{
		_pApplication = static_cast<Application*>(this);
	}

	Application::~Application()
	{
		assert(_pApplication); _pApplication = 0;
	}

	Application& Application::GetApplication(void)
	{
		assert(_pApplication); return (*_pApplication);
	}

	Application* Application::GetSingletonPtr(void)
	{
		assert(_pApplication); return _pApplication;
	}

	LRESULT CALLBACK Application::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		HDC hdc;

		switch (message)
		{
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_ACTIVATE:
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				_appPaused = true;
				_timer.Stop();
			}
			else
			{
				_appPaused = false;
				_timer.Start();
			}
			break;

		case WM_ENTERSIZEMOVE:
			_appPaused = true;
			_resizing = true;
			_timer.Stop();
			break;

		case WM_EXITSIZEMOVE:
			_appPaused = false;
			_resizing = false;
			_timer.Start();
			OnResize();
			break;

		case WM_GETMINMAXINFO:
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = 640;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = 365;
			return 0;

			// WM_SIZE is sent when the user resizes the window.  
		case WM_SIZE:
			// Save the new client area dimensions.
			_windowWidth = LOWORD(lParam);
			_windowHeight = HIWORD(lParam);

			if (_renderSystem.GetPd3dDevice())
			{
				if (wParam == SIZE_MINIMIZED)
				{
					_appPaused = true;
					_minimized = true;
					_maximized = false;
				}
				else if (wParam == SIZE_MAXIMIZED)
				{
					_appPaused = false;
					_minimized = false;
					_maximized = true;
					OnResize();
				}
				else if (wParam == SIZE_RESTORED)
				{
					// Restoring from minimized state?
					if (_minimized)
					{
						_appPaused = false;
						_minimized = false;
						OnResize();
					}

					// Restoring from maximized state?
					else if (_maximized)
					{
						_appPaused = false;
						_maximized = false;
						OnResize();
					}
					else if (_resizing)
					{
						// If user is dragging the resize bars, we do not resize 
						// the buffers here because as the user continuously 
						// drags the resize bars, a stream of WM_SIZE messages are
						// sent to the window, and it would be pointless (and slow)
						// to resize for each WM_SIZE message received from dragging
						// the resize bars.  So instead, we reset after the user is 
						// done resizing the window and releases the resize bars, which 
						// sends a WM_EXITSIZEMOVE message.
					}
					else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
					{
						OnResize();
					}
				}
			}
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return 0;
	}

	HRESULT Application::Initialise(HINSTANCE hInstance, int nCmdShow)
	{
		if (FAILED(InitWindow(hInstance, nCmdShow)))
		{
			return E_FAIL;
		}

		std::unique_ptr<IComponent> keyboard(new Keyboard());
		std::unique_ptr<IComponent> mouse(new Mouse());
		
		InsertComponent(std::move(keyboard));
		InsertComponent(std::move(mouse));

		_sceneManager.Initialise();

		_initialized = true;

		return S_OK;
	}

	HRESULT Application::InitWindow(HINSTANCE hInstance, int nCmdShow)
	{
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_ICON1);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = L"Fab Engine";
		wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_ICON1);

		if (!RegisterClassEx(&wcex))
			return E_FAIL;

		_hInst = hInstance;
		RECT rc = { 0, 0, (LONG)_windowWidth, (LONG)_windowHeight };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
		_hWnd = CreateWindow(L"Fab Engine", _mainWndCaption.c_str(), WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
			nullptr);

		if (!_hWnd)
			return E_FAIL;

		HRESULT hr = InitRenderSystem();
		if (FAILED(hr)) return hr;

		ShowWindow(_hWnd, nCmdShow);

		return S_OK;
	}

	HRESULT Application::InitRenderSystem()
	{
		HRESULT hr;

		hr = _renderSystem.InitDevice(_hWnd);
		if (FAILED(hr)) return hr;

		_renderSystem.GetShaderManager().Initialise();

		return hr;
		
	}

	int Application::Run()
	{
		MSG msg = { 0 };

		_timer.Reset();

		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = _hWnd;
		tme.dwFlags = TME_HOVER | TME_LEAVE;
		tme.dwHoverTime = HOVER_DEFAULT;
		TrackMouseEvent(&tme);

		while (msg.message != WM_QUIT)
		{
			// If there are Window messages then process them.
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_KEYUP || msg.message == WM_KEYDOWN)
					HRESULT hr = KeyEventHandler(&msg);

				if (msg.message == WM_MOUSEMOVE || msg.message == WM_LBUTTONDOWN || msg.message == WM_RBUTTONDOWN || 
					msg.message == WM_LBUTTONUP || msg.message == WM_RBUTTONUP || msg.message == WM_MOUSEHOVER || msg.message == WM_MOUSELEAVE)
					HRESULT hr = MouseEventHandler(&msg);

				TranslateMessage(&msg);
				DispatchMessage(&msg);

				Keyboard& keyboard = static_cast<Keyboard&>(GetComponent(ComponentType::KEYBOARD));

				if (keyboard.IsKeyPressed(KeyName::ESCAPE))
				{
					return (int)msg.wParam;
				}
			}
			// Otherwise, do animation/game stuff.
			else
			{
				_timer.Tick();

				if (!_appPaused && _initialized)
				{
					CalculateFrameStats();
					Update(_timer.DeltaTime(), _timer.TotalTime());
					Draw();
				}
				else
				{
					Sleep(100);
				}
			}
		}

		return (int)msg.wParam;
	}

	void Application::CalculateFrameStats()
	{
		static int frameCnt = 0;
		static float timeElapsed = 0.0f;

		frameCnt++;

		if ((_timer.TotalTime() - timeElapsed) >= 1.0f)
		{
			float fps = (float)frameCnt;
			float mspf = 1000.0f / fps;

			std::wostringstream outs;
			outs.precision(6);
			outs << _mainWndCaption << L"    "
				<< L"FPS: " << fps << L"    "
				<< L"Frame Time: " << mspf << L" (ms)";
			SetWindowText(_hWnd, outs.str().c_str());

			frameCnt = 0;
			timeElapsed += 1.0f;
		}
	}

	void Application::Draw()
	{
		_renderSystem.Draw();
		_sceneManager.Draw();
		_renderSystem.Swap();
	}

	void Application::Update(float deltaTime, float totalTime)
	{
		_sceneManager.Update(deltaTime, totalTime);
	}

	void Application::OnResize()
	{
		_renderSystem.ResetDevice();
	}

	HINSTANCE& Application::GetHInst()
	{
		return _hInst;
	}

	HWND& Application::GetHWnd()
	{
		return _hWnd;
	}

	UINT Application::GetWindowHeight()
	{
		return _windowHeight;
	}

	UINT Application::GetWindowWidth()
	{
		return _windowWidth;
	}

	float Application::GetAspectRatio() const
	{
		return static_cast<float>(_windowWidth) / _windowHeight;
	}

	bool Application::GetAppPaused()
	{
		return _appPaused;
	}

	SceneManager& Application::GetSceneManager()
	{
		return _sceneManager;
	}

	HRESULT Application::KeyEventHandler(MSG* message)
	{
		HRESULT hr = S_OK;

		Keyboard& keyboard = static_cast<Keyboard&>(GetComponent(ComponentType::KEYBOARD));
		keyboard.Update(message, _timer.DeltaTime(), _timer.TotalTime());

		return hr;
	}

	HRESULT Application::MouseEventHandler(MSG* message)
	{
		HRESULT hr = S_OK;

		Mouse& mouse = static_cast<Mouse&>(GetComponent(ComponentType::MOUSE));
		mouse.Update(message, _timer.DeltaTime(), _timer.TotalTime());

		return hr;
	}

	void Application::InsertComponent(std::unique_ptr<IComponent> component)
	{
		auto inserted = _components.insert(std::make_pair(component->GetType(), std::move(component)));
		assert(inserted.second);
	}

	IComponent& Application::GetComponent(ComponentType type)
	{
		auto found = _components.find(type);
		assert(found != _components.end());
		return *found->second;
	}
}