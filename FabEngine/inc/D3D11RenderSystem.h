#pragma once

#include <DirectXComponentsPCH.h>
#include "ShaderManager.h"
#include "Application.h"
#include "Vertex.h"

using namespace DirectX;

namespace Fab
{
	class Application;

	struct ObjectConstantBuffer
	{
		XMMATRIX World;

		XMFLOAT4 SpecularColor;
		float    SpecularPower;
	};

	struct FrameConstantBuffer
	{
		XMMATRIX View;
		XMMATRIX Projection;

		XMFLOAT4 AmbientColor;
		XMFLOAT4 LightColor;
		XMFLOAT3 LightDirection;

		XMFLOAT3 CameraPosition;
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
		ID3D11Buffer**            GetFrameConstantBuffer();
		FrameConstantBuffer*      GetFrameConstantBufferUpdate();
		ID3D11Buffer**            GetObjectConstantBuffer();
		ObjectConstantBuffer*     GetObjectConstantBufferUpdate();
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
		ID3D11Buffer*             _pFrameConstantBuffer;
		ID3D11Buffer*             _pObjectConstantBuffer;
		D3D11_VIEWPORT            _screenViewport;
		ID3D11DepthStencilView*   _depthStencilView;
		ID3D11Texture2D*          _depthStencilBuffer;

		FrameConstantBuffer       _pFrameConstantBufferUpdate;
		ObjectConstantBuffer      _pObjectConstantBufferUpdate;

		UINT                      _windowHeight;
		UINT                      _windowWidth;
		UINT                      _4xMsaaQuality;
	};
}