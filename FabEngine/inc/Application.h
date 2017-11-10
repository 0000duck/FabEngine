#pragma once

#include <DirectXComponentsPCH.h>
#include "Timer.h"
#include "SceneManager.h"
#include "IComponent.h"
#include "Keyboard.h"
#include "Mouse.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

namespace Fab
{
	class Application
	{
	public:
		Application(std::wstring mainWndCaption, UINT windowWidth, UINT windowHeight);
		~Application();
		static Application& GetApplication();
		static Application* GetSingletonPtr();

		LRESULT CALLBACK    MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		HRESULT             Initialise(HINSTANCE hInstance, int nCmdShow);
		int                 Run();

		HINSTANCE&          GetHInst();
		HWND&               GetHWnd();
		UINT                GetWindowHeight();
		UINT                GetWindowWidth();
		float               GetAspectRatio() const;
		bool                GetAppPaused();
		SceneManager&       GetSceneManager();

		IComponent&         GetComponent(ComponentType type);

	private:
		Application(Application const&) = delete;
		Application& operator=(Application const&) = delete;

		HRESULT       InitWindow(HINSTANCE hInstance, int nCmdShow);
		HRESULT       InitRenderSystem();

		void          CalculateFrameStats();

		void          Update(float deltaTime, float totalTime);
		void          Draw();
		void          OnResize();

		HRESULT       KeyEventHandler(MSG* message);
		HRESULT       MouseEventHandler(MSG* message);

		void          InsertComponent(std::unique_ptr<IComponent> component);

	private:
		static Application* _pApplication;
		D3D11RenderSystem&  _renderSystem;

		HINSTANCE           _hInst;
		HWND                _hWnd;
		UINT                _windowHeight;
		UINT                _windowWidth;

		Timer               _timer;

		bool                _appPaused;
		bool                _minimized;
		bool                _maximized;
		bool                _resizing;
		bool                _initialized;

		std::wstring        _mainWndCaption;
		SceneManager        _sceneManager;

		std::map<ComponentType, std::unique_ptr<IComponent>> _components;
	};
}