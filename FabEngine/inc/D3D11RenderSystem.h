#pragma once

#include <DirectXComponentsPCH.h>
#include "ShaderManager.h"
#include "Application.h"
#include "Vertex.h"

using namespace DirectX;

namespace Fab
{
	class Application;

	struct ConstantBuffer
	{
		XMMATRIX _world;
		XMMATRIX _view;
		XMMATRIX _projection;
	};

	class D3D11RenderSystem
	{
	public:
		D3D11RenderSystem(UINT windowWidth, UINT windowHeight);
		~D3D11RenderSystem();

		static D3D11RenderSystem& GetRenderSystem();
		static D3D11RenderSystem* GetRenderSystemPtr();

		HRESULT                   InitDevice(HWND hWnd);
		HRESULT                   ResetDevice();
		void                      CleanUp();
		void                      Draw();
		void                      Swap();

		ID3D11Device**            GetPd3dDevice();
		ID3D11DeviceContext**     GetPImmediateContext();
		ID3D11Buffer**            GetConstantBuffer();
		ConstantBuffer*           GetConstantBufferUpdate();
		UINT                      GetWindowHeight();
		UINT                      GetWindowWidth();

		ShaderManager&            GetShaderManager();

	private:
		D3D11RenderSystem(D3D11RenderSystem const&) = delete;
		D3D11RenderSystem& operator=(D3D11RenderSystem const&) = delete;

	private:
		static D3D11RenderSystem* _pRenderSystem;

		ShaderManager             _shaderManager;

		D3D_DRIVER_TYPE           _driverType;
		D3D_FEATURE_LEVEL         _featureLevel;
		ID3D11Device*             _pd3dDevice;
		ID3D11DeviceContext*      _pImmediateContext;
		IDXGISwapChain*           _pSwapChain;
		ID3D11RenderTargetView*   _pRenderTargetView;
		ID3D11Buffer*             _pConstantBuffer;
		D3D11_VIEWPORT            _screenViewport;
		ID3D11DepthStencilView*   _depthStencilView;
		ID3D11Texture2D*          _depthStencilBuffer;

		ConstantBuffer            _pConstantBufferUpdate;

		UINT                      _windowHeight;
		UINT                      _windowWidth;
		UINT                      _4xMsaaQuality;
	};
}