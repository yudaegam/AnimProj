// author: Changwan Yu
#pragma once

namespace pa
{
	class SkeletonComponent;
	class Mesh;
	class SkeletonRenderingSystem
	{
	public:
		SkeletonRenderingSystem();
		~SkeletonRenderingSystem();
		SkeletonRenderingSystem(SkeletonRenderingSystem const&)	= delete;
		SkeletonRenderingSystem(SkeletonRenderingSystem&&)		= delete;

	public:
		void update(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
		void render(ID3D11DeviceContext* deviceContext, const Mesh* boneMesh, const Mesh* boneToBoneMesh);

	private:
		std::vector<ComPtr<ID3D11Buffer>>	_boneWorldCBuffers;
		std::vector<ComPtr<ID3D11Buffer>>	_boneToBoneWorldCBuffers;

	private:
	};
}
