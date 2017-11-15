#include <DirectXComponentsPCH.h>
#include "Shader.h"

namespace Fab
{
	const D3D11_INPUT_ELEMENT_DESC Shader::VertexData[6] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	Shader::Shader(WCHAR* szFileName)
		: _renderSystem(D3D11RenderSystem::GetRenderSystem())
		, _pVertexShaderBlob(nullptr)
		, _pVertexShader(nullptr)
		, _pPixelShader(nullptr)
		, _pInputLayout(nullptr)
		, _szFileName(szFileName)
	{
	}

	HRESULT Shader::Compile()
	{
		HRESULT hr = S_OK;
		ID3D11DeviceContext** pContext = _renderSystem.GetPImmediateContext();
		ID3D11Device** pd3dDevice      = _renderSystem.GetPd3dDevice();

		//Create Vertex SHader
		hr = CompileVertexShader();
		hr = (*pd3dDevice)->CreateVertexShader(_pVertexShaderBlob->GetBufferPointer(), _pVertexShaderBlob->GetBufferSize(), nullptr, &_pVertexShader);
		if (FAILED(hr)) return hr;

		//Create PixelShader
		hr = CompilePixelShader();
		hr = (*pd3dDevice)->CreatePixelShader(_pPixelShaderBlob->GetBufferPointer(), _pPixelShaderBlob->GetBufferSize(), nullptr, &_pPixelShader);
		if (FAILED(hr)) return hr;

		UINT numberElements = ARRAYSIZE(Shader::VertexData);

		// Create the input layout
		hr = (*pd3dDevice)->CreateInputLayout(
			Shader::VertexData,
			numberElements,
			_pVertexShaderBlob->GetBufferPointer(),
			_pVertexShaderBlob->GetBufferSize(),
			&_pInputLayout);
		if (FAILED(hr)) return hr;

		return hr;
	}

	HRESULT Shader::CompileVertexShader()
	{
		return CompileShader(&_pVertexShaderBlob, "vertex_shader", "vs_4_0");
	}

	HRESULT Shader::CompilePixelShader()
	{
		return CompileShader(&_pPixelShaderBlob, "pixel_shader", "ps_4_0");
	}

	HRESULT Shader::CompileShader(ID3DBlob** shaderBlob, LPCSTR szEntryPoint, LPCSTR szShaderModel)
	{
		HRESULT hr = S_OK;

		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
		dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

		ID3DBlob* pErrorBlob = nullptr;
		hr = D3DCompileFromFile(_szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
			dwShaderFlags, 0, shaderBlob, &pErrorBlob);

		if (FAILED(hr))
		{
			if (pErrorBlob != nullptr)
				OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());

			if (pErrorBlob) pErrorBlob->Release();

			return hr;
		}

		if (pErrorBlob) pErrorBlob->Release();

		return hr;
	}

	void Shader::Use()
	{
		ID3D11DeviceContext** pContext = _renderSystem.GetPImmediateContext();

		(*pContext)->IASetInputLayout(_pInputLayout);
		(*pContext)->VSSetShader(_pVertexShader, nullptr, 0);
		(*pContext)->PSSetShader(_pPixelShader, nullptr, 0);
	}

	ID3DBlob** Shader::GetVertexShaderBlob()
	{
		return &_pVertexShaderBlob;
	}

	ID3DBlob** Shader::GetPixelShaderBlob()
	{
		return &_pPixelShaderBlob;
	}

	ID3D11VertexShader** Shader::GetVertexShader()
	{
		return &_pVertexShader;
	}

	ID3D11PixelShader** Shader::GetPixelShader()
	{
		return &_pPixelShader;
	}

	ID3D11InputLayout** Shader::GetVertexLayout()
	{
		return &_pInputLayout;
	}

	Shader::~Shader()
	{
		SafeReleaseCom(_pVertexShaderBlob);
		SafeReleaseCom(_pVertexShader);
		SafeReleaseCom(_pPixelShader);
		SafeReleaseCom(_pInputLayout);
	}
}