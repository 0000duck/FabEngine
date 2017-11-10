#include <DirectXComponentsPCH.h>
#include "Object.h"

namespace Fab
{
	Object::Object()
	{
		XMStoreFloat4x4(&_position, XMMatrixIdentity());
	}

	Object::~Object()
	{
		SafeReleaseCom(_meshData.PVertexBuffer);
		SafeReleaseCom(_meshData.PIndexBuffer);
	}

	HRESULT Object::Build()
	{
		HRESULT hr;
		D3D11RenderSystem&    renderSystem    = D3D11RenderSystem::GetRenderSystem();
		ID3D11DeviceContext** pContext        = renderSystem.GetPImmediateContext();
		ID3D11Device**        pd3dDevice      = renderSystem.GetPd3dDevice();

		//############# Set vertex buffer
		D3D11_BUFFER_DESC vbd;
		ZeroMemory(&vbd, sizeof(vbd));
		vbd.Usage = D3D11_USAGE_DEFAULT;
		vbd.ByteWidth = sizeof(VertexColor) * _meshData.Vertices.size();
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA vInitData;
		ZeroMemory(&vInitData, sizeof(vInitData));
		vInitData.pSysMem = &_meshData.Vertices[0];
		hr = (*pd3dDevice)->CreateBuffer(&vbd, &vInitData, &_meshData.PVertexBuffer);
		if (FAILED(hr)) return hr;

		//############# Set index buffer
		D3D11_BUFFER_DESC ibd;
		ZeroMemory(&ibd, sizeof(ibd));

		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.ByteWidth = sizeof(WORD) * _meshData.Indices.size();
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA iInitData;
		ZeroMemory(&iInitData, sizeof(iInitData));
		iInitData.pSysMem = &_meshData.Indices[0];
		hr = (*pd3dDevice)->CreateBuffer(&ibd, &iInitData, &_meshData.PIndexBuffer);
		if (FAILED(hr)) return hr;

		//############# Set shader
		_shader = renderSystem.GetShaderManager().GetShaderPtr("color");

		return S_OK;
	}

	void Object::Draw()
	{
		D3D11RenderSystem&    renderSystem      = D3D11RenderSystem::GetRenderSystem();
		ID3D11DeviceContext** pContext          = renderSystem.GetPImmediateContext();
		ID3D11Buffer**        pConstantBuffer   = renderSystem.GetConstantBuffer();
		ConstantBuffer*       pConstantBufferUpdate = renderSystem.GetConstantBufferUpdate();

		XMMATRIX position = XMLoadFloat4x4(&_position);
		pConstantBufferUpdate->_world = XMMatrixTranspose(position);

		_shader->Use();

		UINT stride = sizeof(VertexColor);
		UINT offset = 0;
		(*pContext)->IASetVertexBuffers(0, 1, &_meshData.PVertexBuffer, &stride, &offset);
		(*pContext)->IASetIndexBuffer(_meshData.PIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	
		(*pContext)->UpdateSubresource(*pConstantBuffer, 0, nullptr, pConstantBufferUpdate, 0, 0);
		(*pContext)->DrawIndexed(_meshData.Indices.size(), 0, 0);
	}

	void Object::Update(float deltaTime, float totalTime)
	{
	}

	MeshData& Object::GetMeshData()
	{
		return _meshData;
	}

	XMFLOAT4X4& Object::GetPosition()
	{
		return _position;
	}
}