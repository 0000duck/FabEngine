#pragma once

#include <DirectXComponentsPCH.h>
#include "DDSTextureLoader.h"
#include "ShaderManager.h"
#include "Application.h"
#include "Vertex.h"

using namespace DirectX;

namespace Fab
{
	class Application;

	struct FrameConstantBuffer
	{
		XMMATRIX View;
		XMMATRIX Projection;
		XMFLOAT4 AmbientColor;
		XMFLOAT4 Padding;
		XMFLOAT4 CameraPosition; //float3

		XMMATRIX World;

		XMFLOAT4 SpecularColor;
		XMFLOAT4 SpecularPower;   //float
		XMFLOAT4 HasMaterial;
		XMFLOAT4 HasSpecular;
		XMFLOAT4 HasNormal;

		XMFLOAT4 DirectionalColor;
		XMFLOAT4 DirectionalDirection; //float3

		XMFLOAT4 LightColor;
		XMFLOAT4 LightDirection;  //float3

		XMFLOAT4 LightRadius;     //float
		XMFLOAT4 LightPosition;   //float3

		XMFLOAT4 LightType;       //int

		XMFLOAT4 FogEnabled;
		XMFLOAT4 FogColor;
		XMFLOAT4 FogStart;
		XMFLOAT4 FogRange;

		XMFLOAT4 FilterColorEnabled;
		XMFLOAT4 FilterColor;

		XMFLOAT4 BlurEnabled;
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
		D3D11_VIEWPORT            _screenViewport;
		ID3D11DepthStencilView*   _depthStencilView;
		ID3D11Texture2D*          _depthStencilBuffer;

		FrameConstantBuffer       _pFrameConstantBufferUpdate;

		UINT                      _windowHeight;
		UINT                      _windowWidth;
		UINT                      _4xMsaaQuality;

		ID3D11SamplerState *      _pColorSampler;

		ID3D11RasterizerState*     _backFaceCulling;
	};
}