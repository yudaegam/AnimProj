// author: Changwan Yu
#pragma once
namespace pa
{
	class Skeleton
	{

		friend class ASF;
		friend class AcclaimImporter;

	public:
		Skeleton()	= default;
		~Skeleton() = default;

	public:
		void generateBoneMasks();
		void generateBoneToBoneLTs();


		DirectX::XMMATRIX getBoneMatrix(size_t index) const;
		DirectX::XMMATRIX getBoneToBoneLT(size_t index) const;

		inline size_t						getBoneCount() const { return _boneMatrices.size(); }
		inline size_t						getParentBoneID(size_t index) const { return _parents[index]; }
		inline std::vector<uint16_t> const& getHierarchy() const { return _hierarchy; }
		inline bool							isInUpperBody(size_t boneID) const { return _upperBodyMask[boneID]; }
		inline bool							isInLowerBody(size_t boneID) const { return _lowerBodyMask[boneID]; }

	private:
		std::vector<DirectX::XMFLOAT4X4> _boneMatrices;
		std::vector<DirectX::XMFLOAT4X4> _boneToBoneLTs;

		std::vector<std::uint16_t> _parents;
		std::vector<std::uint16_t> _hierarchy;

		std::vector<bool> _upperBodyMask;
		std::vector<bool> _lowerBodyMask;
	};
}
