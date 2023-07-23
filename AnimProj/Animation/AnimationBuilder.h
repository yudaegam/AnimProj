// author: Changwan Yu
#pragma once
#include "RawAnimation.h"
namespace pa
{
	class Skeleton;
	class CompactAnimation;
	class AnimationBuilder
	{
		struct ExtendedKey
		{
			int		 prevKeyTime;
			uint16_t keyTime;
			uint16_t trackID		: 15;
			uint16_t isQuaternion	: 1;
			DirectX::XMFLOAT4 value;
		};

	public:
		AnimationBuilder(const Skeleton& skeleton, const RawAnimation& rawAnimation);
		~AnimationBuilder() = default;

		void						createCompactAnimation(CompactAnimation* comapactAnimation);
	private:
		static AnimationTrack		removeDuplicateFrame(AnimationTrack const& track);
		static AnimationTrack		fitCurveWithCatmullRom(AnimationTrack const& track, float threshold = 0.0001f);
		static CompactAnimation		buildCompactAnimation(std::vector<AnimationTrack> const& tracks);
		static bool					SortingKeyframeBuilderLess(const ExtendedKey& a, const ExtendedKey& b);
		
		static std::vector<ExtendedKey>	mergeTracks(std::vector<AnimationTrack> const& tracks);

	private:
		const Skeleton&		_skeleton;
		const RawAnimation& _rawAnimation;

		std::vector<AnimationTrack> _tracks;
	};
}
