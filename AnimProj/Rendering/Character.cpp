#include "pch.h"
#include "Character.h"
#include "../AcclaimMotionCapture/AcclaimMotion.h"
#include "../AcclaimMotionCapture/AcclaimSkeleton.h"

#include "../Animation/AnimationPlayer.h"
#include "../Animation/AcclaimImporter.h"
#include "../Animation/CompactAnimation.h"
#include "../Animation/AnimationBuilder.h"

#include "../Rendering/Mesh.h"
#include "../Rendering/StickMesh.h"
#include "../Rendering/CubeMesh.h"
#include "../Keyboard.h"

pa::Character::Character(ID3D11Device* device)
{
	std::wstring asfFilePath = _SOLUTIONDIR;

	Acclaim::Skeleton acclaimSkeleton(asfFilePath + LR"(Assets\ASFAMC\131-dance\131-dance.asf)");

	_skeleton = new Skeleton();
	AcclaimImporter::createSkeleton(acclaimSkeleton, _skeleton);
	_skeleton->generateBoneMasks();
	
	std::wstring amcDirectory = _SOLUTIONDIR;
	amcDirectory += LR"(Assets\ASFAMC\)";
	{
		Acclaim::Motion acclaimMotion(&acclaimSkeleton, amcDirectory + LR"(131-dance\131_04-dance.amc)");
		//Acclaim::Motion acclaimMotion(&acclaimSkeleton, amcDirectory + LR"(135-martialArts\135_06-martialArts.amc)");
		RawAnimation rawAnimation = {};
		AcclaimImporter::createRawAnimation(acclaimMotion, *_skeleton, &rawAnimation);
		AnimationBuilder animationBuilder(*_skeleton, rawAnimation);

		static CompactAnimation animationUpperBody = animationBuilder.createUpperBodyAnimation();
		_animationPlayers.push_back(AnimationPlayer(animationUpperBody));

		static CompactAnimation animationLowerBody = animationBuilder.createLowerBodyAnimation();
		_animationPlayers.push_back(AnimationPlayer(animationLowerBody));
	}

	{
		Acclaim::Motion acclaimMotion(&acclaimSkeleton, amcDirectory + LR"(07-walk\07_05-walk.amc)");
		RawAnimation rawAnimation = {};
		AcclaimImporter::createRawAnimation(acclaimMotion, *_skeleton, &rawAnimation);
		AnimationBuilder animationBuilder(*_skeleton, rawAnimation);

		static CompactAnimation animationUpperBody = animationBuilder.createUpperBodyAnimation();
		_animationPlayers.push_back(AnimationPlayer(animationUpperBody));

		static CompactAnimation animationLowerBody = animationBuilder.createLowerBodyAnimation();
		_animationPlayers.push_back(AnimationPlayer(animationLowerBody));
	}

	_jointTransforms.resize(_skeleton->getBoneCount());
	_boneStickTransforms.resize(_skeleton->getBoneCount());
	
	//_poseScales.resize(_skeleton->getBoneCount());
	_poseRotations.resize(_skeleton->getBoneCount());
	_poseTranslations.resize(_skeleton->getBoneCount());

	
	_boneMesh = new StickMesh(device);
	_jointMesh = new CubeMesh(device, 0.25f);
}

pa::Character::~Character()
{
	AcclaimImporter::destroySkeleton(_skeleton);
	_skeleton = nullptr;

	if (nullptr != _boneMesh)
	{
		delete _boneMesh;
		_boneMesh = nullptr;
	}

	if (nullptr != _jointMesh)
	{
		delete _jointMesh;
		_jointMesh = nullptr;
	}
}

void pa::Character::update(float deltaTime, ID3D11DeviceContext* deviceContext)
{
	for (auto& animationPlayer : _animationPlayers)
	{
		animationPlayer.update(deltaTime);
	}

	using namespace DirectX;
	for (const size_t boneIndex : _skeleton->getHierarchy())
	{
		XMVECTOR animationRotation;
		if (_skeleton->getUpperBodyMask()[boneIndex])
			animationRotation = _animationPlayers[0].getBoneRotation(boneIndex);
		else
			animationRotation = _animationPlayers[3].getBoneRotation(boneIndex);


		XMMATRIX animationMatrix	= XMMatrixRotationQuaternion(XMQuaternionNormalize(animationRotation));

		const size_t parentBoneIndex			= _skeleton->getParentBoneID(boneIndex);
		const XMMATRIX& parentWorldTransform	= (boneIndex != 0) ?
			_jointTransforms[parentBoneIndex] : XMMatrixIdentity();

		XMMATRIX boneMatrix = _skeleton->getBoneMatrix(boneIndex);

		XMVECTOR boneTranslation		= {};
		XMVECTOR dummyVector			= {};
		XMMatrixDecompose(&dummyVector, &dummyVector, &boneTranslation, boneMatrix);


		_jointTransforms[boneIndex] = animationMatrix * boneMatrix * parentWorldTransform;

		const float boneStickScale = XMVectorGetX(XMVector3Length(boneTranslation));
		if (boneStickScale <= 0)
		{
			_boneStickTransforms[boneIndex] = XMMATRIX{};
			continue;
		}

		const XMVECTOR vec0 = XMVECTOR{ 0.0f, 1.0f, 0.0f, 0.0f };
		const XMVECTOR vec1 = XMVector3Normalize(boneTranslation);

		const float		dotProduct = XMVectorGetX(XMVector3Dot(vec0, vec1));
		const float		angle = std::acosf(dotProduct);
		const XMVECTOR	rotationAxis = XMVector3Cross(vec0, vec1);


		_boneStickTransforms[boneIndex] =
			XMMatrixScaling(0.15f, boneStickScale, 0.15f) * XMMatrixRotationAxis(rotationAxis, angle)
			* XMMatrixTranslation(0.f, 0.f, 0.f) * parentWorldTransform;
	}


	_boneMesh->updateInstanceData(deviceContext, _boneStickTransforms.data(), _boneStickTransforms.size());
	_jointMesh->updateInstanceData(deviceContext, _jointTransforms.data(), _jointTransforms.size());
}

void pa::Character::render(ID3D11DeviceContext* deviceContext)
{
	_boneMesh->drawInstanced(deviceContext, static_cast<UINT>(_skeleton->getBoneCount()));
	_jointMesh->drawInstanced(deviceContext, static_cast<UINT>(_skeleton->getBoneCount()));
}

void pa::Character::processInput(float deltaTime)
{

	if (Keyboard::get()->getKeyState(32 /* space bar */))
	{
		for (auto& animationPlayer : _animationPlayers)
		{
			animationPlayer.play();
		}
	}


}
