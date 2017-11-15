#include <DirectXComponentsPCH.h>
#include "D3D11RenderSystem.h"

namespace Fab
{
	D3D11RenderSystem* D3D11RenderSystem::_pRenderSystem = nullptr;

	D3D11RenderSystem::D3D11RenderSystem(UINT windowWidth, UINT windowHeight)
		: _windowHeight(windowHeight)
		, _windowWidth(windowWidth)
	{
		_driverType           = D3D_DRIVER_TYPE_HARDWARE;
		_featureLevel         = D3D_FEATURE_LEVEL_11_0;
		_pd3dDevice           = nullptr;
		_pImmediateContext    = nullptr;
		_pSwapChain           = nullptr;
		_pRenderTargetView    = nullptr;
		_pFrameConstantBuffer = nullptr;

		_pColorSampler        = nullptr;

		_pRenderSystem        = static_cast<D3D11RenderSystem*>(this);

		tinyxml2::XMLDocument doc;
		doc.LoadFile("data/game.xml");

		_windowWidth = doc.FirstChildElement("application")->FirstChildElement("window")->IntAttribute("width", 1280);
		_windowHeight = doc.FirstChildElement("application")->FirstChildElement("window")->IntAttribute("height", 720);
	}

	D3D11RenderSystem::~D3D11RenderSystem()
	{
		CleanUp();
	}

	D3D11RenderSystem& D3D11RenderSystem::GetRenderSystem(void)
	{
		assert(_pRenderSystem); return (*_pRenderSystem);
	}

	D3D11RenderSystem* D3D11RenderSystem::GetRenderSystemPtr(void)
	{
		assert(_pRenderSystem); return _pRenderSystem;
	}

	HRESULT D3D11RenderSystem::InitDevice(HWND hWnd)
	{
		HRESULT hr = S_OK;
		UINT createDeviceFlags = 0;

#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};

		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};

		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		hr = D3D11CreateDevice(nullptr, _driverType, nullptr, createDeviceFlags, featureLevels, 
			numFeatureLevels, D3D11_SDK_VERSION, &_pd3dDevice, &_featureLevel, &_pImmediateContext);

		if (FAILED(hr))
			return hr;

		_pd3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &_4xMsaaQuality);
		assert(_4xMsaaQuality > 0);

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = _windowWidth;
		sd.BufferDesc.Height = _windowHeight;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 0;
		sd.BufferDesc.RefreshRate.Denominator = 0;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	
		sd.OutputWindow = hWnd;
		sd.Windowed = TRUE;
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = _4xMsaaQuality - 1;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		IDXGIDevice* dxgiDevice = 0;
		hr = _pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

		IDXGIAdapter* dxgiAdapter = 0;
		hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

		IDXGIFactory* dxgiFactory = 0;
		hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

		hr = dxgiFactory->CreateSwapChain(_pd3dDevice, &sd, &_pSwapChain);

		SafeReleaseCom(dxgiDevice);
		SafeReleaseCom(dxgiAdapter);
		SafeReleaseCom(dxgiFactory);

		// Create a render target view
		ID3D11Texture2D* pBackBuffer = nullptr;
		hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		
		if (FAILED(hr))
			return hr;
		
		hr = _pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_pRenderTargetView);
		pBackBuffer->Release();

		if (FAILED(hr))
			return hr;

		//Init depth stencil and buffer
		D3D11_TEXTURE2D_DESC depthStencilDesc;

		depthStencilDesc.Width = _windowWidth;
		depthStencilDesc.Height = _windowHeight;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = _4xMsaaQuality - 1;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		_pd3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &_depthStencilBuffer);
		_pd3dDevice->CreateDepthStencilView(_depthStencilBuffer, nullptr, &_depthStencilView);

		_pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView, _depthStencilView);

		// Setup the viewport
		_screenViewport.Width = (FLOAT)_windowWidth;
		_screenViewport.Height = (FLOAT)_windowHeight;
		_screenViewport.MinDepth = 0.0f;
		_screenViewport.MaxDepth = 1.0f;
		_screenViewport.TopLeftX = 0;
		_screenViewport.TopLeftY = 0;
		_pImmediateContext->RSSetViewports(1, &_screenViewport);

		// Set primitive topology
		_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Create constant buffers
		D3D11_BUFFER_DESC bdFrame;
		ZeroMemory(&bdFrame, sizeof(bdFrame));
		bdFrame.Usage = D3D11_USAGE_DEFAULT;
		bdFrame.ByteWidth = sizeof(FrameConstantBuffer);
		bdFrame.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bdFrame.CPUAccessFlags = 0;
		hr = _pd3dDevice->CreateBuffer(&bdFrame, nullptr, &_pFrameConstantBuffer);

		//Set constant buffers for vertex_shader and pixel_shader
		_pImmediateContext->VSSetConstantBuffers(0, 1, &_pFrameConstantBuffer);
		_pImmediateContext->PSSetConstantBuffers(0, 1, &_pFrameConstantBuffer);

		//Create Color Sampler
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

		hr =_pd3dDevice->CreateSamplerState(&sampDesc, &_pColorSampler);
		_pImmediateContext->PSSetSamplers(0, 1, &_pColorSampler);

		return S_OK;
	}

	HRESULT D3D11RenderSystem::ResetDevice()
	{
		assert(_pImmediateContext);
		assert(_pd3dDevice);
		assert(_pSwapChain);

		SafeReleaseCom(_pRenderTargetView);
		SafeReleaseCom(_depthStencilView);
		SafeReleaseCom(_depthStencilBuffer);

		_windowHeight = Application::GetApplication().GetWindowHeight();
		_windowWidth = Application::GetApplication().GetWindowWidth();

		// Resize the swap chain and recreate the render target view.
		HR(_pSwapChain->ResizeBuffers(1, _windowWidth, _windowHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
		ID3D11Texture2D* pBackBuffer = nullptr;
		HR(_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer));
		HR(_pd3dDevice->CreateRenderTargetView(pBackBuffer, 0, &_pRenderTargetView));
		SafeReleaseCom(pBackBuffer);

		//Init depth stencil and buffer
		D3D11_TEXTURE2D_DESC depthStencilDesc;

		depthStencilDesc.Width = _windowWidth;
		depthStencilDesc.Height = _windowHeight;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = _4xMsaaQuality - 1;;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		_pd3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &_depthStencilBuffer);
		_pd3dDevice->CreateDepthStencilView(_depthStencilBuffer, nullptr, &_depthStencilView);

		_pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView, _depthStencilView);

		// Setup the viewport
		_screenViewport.TopLeftX = 0;
		_screenViewport.TopLeftY = 0;
		_screenViewport.Width = static_cast<float>(_windowWidth);
		_screenViewport.Height = static_cast<float>(_windowHeight);
		_screenViewport.MinDepth = 0.0f;
		_screenViewport.MaxDepth = 1.0f;
		_pImmediateContext->RSSetViewports(1, &_screenViewport);
		
		return S_OK;
	}

	void D3D11RenderSystem::CleanUp()
	{
		_pSwapChain->SetFullscreenState(FALSE, NULL);

		if (_pImmediateContext)
			_pImmediateContext->ClearState();

		SafeReleaseCom(_pImmediateContext);
		SafeReleaseCom(_pFrameConstantBuffer);
		SafeReleaseCom(_pRenderTargetView);
		SafeReleaseCom(_pSwapChain);
		SafeReleaseCom(_pd3dDevice);
		SafeReleaseCom(_depthStencilView);
		SafeReleaseCom(_depthStencilBuffer);

		SafeReleaseCom(_pColorSampler);
	}

	void D3D11RenderSystem::Draw()
	{
		_pImmediateContext->ClearRenderTargetView(_pRenderTargetView, reinterpret_cast<const float*>(&Colors::LightSteelBlue));
		_pImmediateContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		//_pFrameConstantBufferUpdate.LightType = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		_pImmediateContext->UpdateSubresource(_pFrameConstantBuffer, 0, nullptr, &_pFrameConstantBufferUpdate, 0, 0);
	}

	void D3D11RenderSystem::Swap()
	{
		_pSwapChain->Present(1, 0);
	}

	ID3D11Device** D3D11RenderSystem::GetPd3dDevice()
	{
		return &_pd3dDevice;
	}

	ID3D11DeviceContext** D3D11RenderSystem::GetPImmediateContext()
	{
		return &_pImmediateContext;
	}

	ID3D11Buffer** D3D11RenderSystem::GetFrameConstantBuffer()
	{
		return &_pFrameConstantBuffer;
	}

	FrameConstantBuffer* D3D11RenderSystem::GetFrameConstantBufferUpdate()
	{
		return &_pFrameConstantBufferUpdate;
	}

	UINT D3D11RenderSystem::GetWindowHeight()
	{
		return _windowHeight;
	}

	UINT D3D11RenderSystem::GetWindowWidth()
	{
		return _windowWidth;
	}

	ShaderManager& D3D11RenderSystem::GetShaderManager()
	{
		return _shaderManager;
	}
}