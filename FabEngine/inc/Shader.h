#pragma once

#include <DirectXComponentsPCH.h>

using namespace DirectX;

namespace Fab
{
	class D3D11RenderSystem;

	class Shader
	{
	public:
		Shader(WCHAR* szFileName);
		~Shader();
		HRESULT              Compile();
		ID3DBlob**           GetVertexShaderBlob();
		ID3DBlob**           GetPixelShaderBlob();
		ID3D11VertexShader** GetVertexShader();
		ID3D11PixelShader**  GetPixelShader();
		ID3D11InputLayout**  GetVertexLayout();
		void                 Use();

	private:
		HRESULT              CompileVertexShader();
		HRESULT              CompilePixelShader();
		HRESULT              CompileShader(ID3DBlob** shaderBlob, LPCSTR szEntryPoint, LPCSTR szShaderModel);

	public:
		static const D3D11_INPUT_ELEMENT_DESC VertexData[5];

	private:
		D3D11RenderSystem&  _renderSystem;

		WCHAR*              _szFileName;

		ID3DBlob*           _pVertexShaderBlob;
		ID3DBlob*           _pPixelShaderBlob;
		ID3D11VertexShader* _pVertexShader;
		ID3D11PixelShader*  _pPixelShader;
		ID3D11InputLayout*  _pInputLayout;
	};

	typedef std::shared_ptr<Shader> ShaderPtr;
}