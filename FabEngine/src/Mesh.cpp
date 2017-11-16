#include <DirectXComponentsPCH.h>
#include "Mesh.h"

namespace Fab
{
	const XMFLOAT4 Mesh::DefaultSpecularColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	const float Mesh::DefaultSpecularPower = 64.0f;

	Mesh::Mesh()
		: _renderSystem(D3D11RenderSystem::GetRenderSystem())
		, _specularColor(DefaultSpecularColor)
		, _specularPower(DefaultSpecularPower)
		, _texture(nullptr)
	{
		XMStoreFloat4x4(&_world, XMMatrixIdentity());
		UpdateLocalPosition();
	}

	Mesh::~Mesh()
	{
		SafeReleaseCom(_pVertexBuffer);
		SafeReleaseCom(_pIndexBuffer);
	}

	HRESULT Mesh::Build()
	{
		HRESULT hr;
		D3D11RenderSystem&    renderSystem    = D3D11RenderSystem::GetRenderSystem();
		ID3D11DeviceContext** pContext        = renderSystem.GetPImmediateContext();
		ID3D11Device**        pd3dDevice      = renderSystem.GetPd3dDevice();

		//############# Set vertex buffer
		D3D11_BUFFER_DESC vbd;
		ZeroMemory(&vbd, sizeof(vbd));
		vbd.Usage = D3D11_USAGE_DEFAULT;
		vbd.ByteWidth = sizeof(VertexData) * _meshData.Vertices.size();
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA vInitData;
		ZeroMemory(&vInitData, sizeof(vInitData));
		vInitData.pSysMem = &_meshData.Vertices[0];
		hr = (*pd3dDevice)->CreateBuffer(&vbd, &vInitData, &_pVertexBuffer);
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
		hr = (*pd3dDevice)->CreateBuffer(&ibd, &iInitData, &_pIndexBuffer);
		if (FAILED(hr)) return hr;

		//############# Set shader
		_shader = renderSystem.GetShaderManager().GetShaderPtr("default");

		return S_OK;
	}

	void Mesh::Initialise()
	{
	}

	void Mesh::Draw()
	{
		ID3D11DeviceContext** pContext                    = _renderSystem.GetPImmediateContext();
		ID3D11Buffer** pFrameConstantBuffer              = _renderSystem.GetFrameConstantBuffer();
		FrameConstantBuffer* pFrameConstantBufferUpdate = _renderSystem.GetFrameConstantBufferUpdate();

		XMMATRIX world = XMLoadFloat4x4(&_world);
		pFrameConstantBufferUpdate->World = XMMatrixTranspose(world);
		pFrameConstantBufferUpdate->SpecularColor = _specularColor;
		pFrameConstantBufferUpdate->SpecularPower = XMFLOAT4(_specularPower, 0.0f, 0.0f, 0.0f);

		_shader->Use();

		if (_texture != nullptr)
			pFrameConstantBufferUpdate->HasMaterial = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
		else
			pFrameConstantBufferUpdate->HasMaterial = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

		if (_specular != nullptr)
			pFrameConstantBufferUpdate->HasSpecular = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
		else
			pFrameConstantBufferUpdate->HasSpecular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

		if (_normal != nullptr)
			pFrameConstantBufferUpdate->HasNormal = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
		else
			pFrameConstantBufferUpdate->HasNormal = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

		UINT stride = sizeof(VertexData);
		UINT offset = 0;
		(*pContext)->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);
		(*pContext)->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	
		(*pContext)->UpdateSubresource(*pFrameConstantBuffer, 0, nullptr, pFrameConstantBufferUpdate, 0, 0);
		(*pContext)->DrawIndexed(_meshData.Indices.size(), 0, 0);
	}

	void Mesh::Update(float deltaTime, float totalTime)
	{
	
	}

	void Mesh::UpdateLocalPosition()
	{
		XMMATRIX worldInverse = XMMatrixInverse(nullptr, XMLoadFloat4x4(&_world));
		XMVECTOR position = XMVector3Transform(XMLoadFloat3(&_position), XMLoadFloat4x4(&_world));
		XMStoreFloat3(&_position, position);
	}

	void Mesh::Transform(XMMATRIX matrix)
	{
		XMStoreFloat4x4(&_world, XMLoadFloat4x4(&_world) * matrix);
		UpdateLocalPosition();
	}

	void Mesh::TransformLocal(XMMATRIX matrix)
	{
		XMMATRIX worldInverse = XMMatrixInverse(nullptr, XMLoadFloat4x4(&_world));
		XMStoreFloat4x4(&_world, XMMatrixInverse(nullptr, worldInverse * matrix));
		UpdateLocalPosition();
	}

	void Mesh::TransformAround(XMMATRIX matrix, XMFLOAT3 around)
	{
		XMVECTOR translationVector = XMVectorSet(
			_position.x - around.x,
			_position.y - around.y,
			_position.z - around.z,
			0.0f
		);

		XMVECTOR translationVectorInverted = XMVectorSet(
			around.x - _position.x,
			around.y - _position.y,
			around.z - _position.z,
			0.0f
		);

		XMMATRIX translationMatrix = XMMatrixTranslationFromVector(translationVector);
		XMMATRIX translationMatrixInverted = XMMatrixTranslationFromVector(translationVectorInverted);

		Transform(translationMatrix);
		Transform(matrix);
		Transform(translationMatrixInverted);

		UpdateLocalPosition();
	}

	void Mesh::SetMeshData(MeshData& meshData)
	{
		_meshData = meshData;
	}

	void Mesh::SetSpecularColor(XMFLOAT4 specularColor)
	{
		_specularColor = specularColor;
	}

	void Mesh::SetSpecularPower(float specularPower)
	{
		_specularPower = specularPower;
	}

	void Mesh::SetTexture(TexturePtr texture)
	{
		_texture = texture;
	}

	void Mesh::SetSpecular(TexturePtr specular)
	{
		_specular = specular;
	}

	void Mesh::SetNormal(TexturePtr normal)
	{
		_normal = normal;
	}

	MeshData& Mesh::GetMeshData()
	{
		return _meshData;
	}

	XMFLOAT4X4& Mesh::GetWorld()
	{
		return _world;
	}

	XMFLOAT3& Mesh::GetPosition()
	{
		return _position;
	}
}