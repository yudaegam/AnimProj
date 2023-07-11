// author: Changwan Yu
#include "pch.h"
#include "Camera.h"

pa::Camera::Camera(ID3D11Device* device)
{
	{
		// Create camera constant buffer
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(pa::Camera::Matrices);
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		checkResult(device->CreateBuffer(&bufferDesc, nullptr, &_cameraConstantBuffer));
	}

	initialize();
}

void pa::Camera::initialize()
{
	constexpr float aspectRatio = 1280.0f / 720.0f;
	_aspectRatio = aspectRatio;

	using namespace DirectX;
	XMStoreFloat4(&_eyePosition, XMVECTOR{ 0.0f, 0.0f, -3.0f, 1.0f });
	XMStoreFloat4(&_focusPosition, XMVECTOR{ 0.0f, 0.0f,  0.0f, 1.0f });

	calculateMatrices();
}


void pa::Camera::setAspectRatio(float aspectRatio)
{
	_aspectRatio = aspectRatio;
	_dirtyBit = true;
}

void pa::Camera::setEyePosition(const DirectX::XMVECTOR& eyePosition)
{
	DirectX::XMStoreFloat4(&_eyePosition, eyePosition);
	_dirtyBit = true;
}

void pa::Camera::setFocusPosition(const DirectX::XMVECTOR& focusPosition)
{
	DirectX::XMStoreFloat4(&_focusPosition, focusPosition);
	_dirtyBit = true;
}

void pa::Camera::update(ID3D11DeviceContext* deviceContext)
{
	calculateMatrices();
	deviceContext->UpdateSubresource(_cameraConstantBuffer.Get(), 0, nullptr, &_matrices, 0, 0);
	deviceContext->VSSetConstantBuffers(0, 1, _cameraConstantBuffer.GetAddressOf());
}

void pa::Camera::calculateMatrices()
{
	using namespace DirectX;

	const XMVECTOR upDirection{ 0.0f, 1.0f, 0.0f, 0.0f };
	XMStoreFloat4x4(&_matrices.View,
		XMMatrixLookAtLH(XMLoadFloat4(&_eyePosition), XMLoadFloat4(&_focusPosition), upDirection));

	constexpr float nearZ = 0.01f;
	constexpr float farZ = 100.0f;
	XMStoreFloat4x4(&_matrices.Projection,
		XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, _aspectRatio, nearZ, farZ));

	_dirtyBit = false;
}

const pa::Camera::Matrices& pa::Camera::getMatrices(void)
{
	calculateMatrices();
	return _matrices;
}
