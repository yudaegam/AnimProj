// author: Changwan Yu
#pragma once
#include "App/Win32Application.h"
#include "Timer.h"
#include "Rendering/Character.h"
namespace pa
{
	class Camera;
	class Mesh;
	class ImGuiManager;
	class SkeletonRenderer;

	class MyApplication : public Win32Application
	{
	public:
		MyApplication();
		~MyApplication();
		virtual void	onUpdate(void) override;
		virtual void	onRender(void) override;
		virtual void	onPostResize(void) override;
		void			renderScene(void);
		void			renderImGui(void);

	private:
		void initializeD3dDevices(HWND hWnd);
		void initializeGraphicsPipeline();
		void createRenderTarget();
		void processInput(float deltaTime);

	private:
		ComPtr<ID3D11Device>				_device;
		ComPtr<ID3D11DeviceContext>			_deviceContext;
		ComPtr<IDXGISwapChain>				_swapChain;
		ComPtr<ID3D11RenderTargetView>		_renderTargetView;
		ComPtr<ID3D11DepthStencilView>		_depthStencilView;
		ComPtr<ID3D11DepthStencilState>		_depthStencilState;
		D3D11_VIEWPORT						_viewport;

		ComPtr<ID3D11InputLayout>			_inputLayout;
		ComPtr<ID3D11VertexShader>			_vertexShader;
		ComPtr<ID3D11PixelShader>			_pixelShader;
		ComPtr<ID3D11RasterizerState>		_rasterizerState;

		std::unique_ptr<SkeletonRenderer>	_skeletonRenderer;
		std::unique_ptr<ImGuiManager>		_imguiManager;

	private:
		//ComPtr<ID3D11Buffer>				_boneWorldCBuffer;
		//ComPtr<ID3D11Buffer>				_boneToBoneWorldCBuffer;

		// These would be object pools
		//std::vector<DirectX::XMFLOAT3>		_objectPositions;
		//std::vector<DirectX::XMFLOAT4>		_objectRotations;
		//std::vector<DirectX::XMFLOAT4X4>	_boneWorldMatrices;
		//std::vector<DirectX::XMFLOAT4X4>	_boneToBoneWorldMatrices;

	private:
		Timer	_timer;
		float	_clearColor[4]			= { 0.2f, 0.4f, 0.6f, 1.0f };
		

		const Mesh*						_boneMesh	= nullptr;
		const Mesh*						_boneToBoneMesh	= nullptr;
		Camera*							_camera		= nullptr;
		std::vector<Character>			_characters;
	};
}

