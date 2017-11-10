#include <DirectXComponentsPCH.h>
#include "D3D11RenderSystem.h"

namespace Fab
{
	D3D11RenderSystem* D3D11RenderSystem::_pRenderSystem = nullptr;

	D3D11RenderSystem::D3D11RenderSystem(UINT windowWidth, UINT windowHeight)
		: _windowHeight(windowHeight)
		, _windowWidth(windowWidth)
		
	{
		_driverType = D3D_DRIVER_TYPE_HARDWARE;
		_featureLevel = D3D_FEATURE_LEVEL_11_0;
		_pd3dDevice = nullptr;
		_pImmediateContext = nullptr;
		_pSwapChain = nullptr;
		_pRenderTargetView = nullptr;
		_pConstantBuffer = nullptr;

		_pRenderSystem = static_cast<D3D11RenderSystem*>(this);
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

		// Create the constant buffer
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(ConstantBuffer);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		hr = _pd3dDevice->CreateBuffer(&bd, nullptr, &_pConstantBuffer);

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
		SafeReleaseCom(_pConstantBuffer);
		SafeReleaseCom(_pRenderTargetView);
		SafeReleaseCom(_pSwapChain);
		SafeReleaseCom(_pd3dDevice);
		SafeReleaseCom(_depthStencilView);
		SafeReleaseCom(_depthStencilBuffer);

		SafeRelease(_pConstantBuffer);
	}

	void D3D11RenderSystem::Draw()
	{
		_pImmediateContext->ClearRenderTargetView(_pRenderTargetView, reinterpret_cast<const float*>(&Colors::LightSteelBlue));
		_pImmediateContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		_pImmediateContext->VSSetConstantBuffers(0, 1, &_pConstantBuffer);
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

	ID3D11Buffer** D3D11RenderSystem::GetConstantBuffer()
	{
		return &_pConstantBuffer;
	}

	ConstantBuffer* D3D11RenderSystem::GetConstantBufferUpdate()
	{
		return &_pConstantBufferUpdate;
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