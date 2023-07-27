// author: Chanwan Yu
#pragma once


namespace pa
{
	class TwoBoneIKSolver
	{
	public:
		bool solve(DirectX::XMVECTOR A, DirectX::XMVECTOR B, DirectX::XMVECTOR C, DirectX::XMVECTOR Target, DirectX::XMVECTOR Q0, DirectX::XMVECTOR Q1);

		DirectX::XMFLOAT4 Qa = { 0.0f, 0.0f, 0.0f, 1.0f };
		DirectX::XMFLOAT4 Qb = { 0.0f, 0.0f, 0.0f, 1.0f };
	};

}

