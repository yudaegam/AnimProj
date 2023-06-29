cbuffer CameraConstantBuffer : register(b0)
{
	float4x4 View;
	float4x4 Projection;
};

cbuffer MeshConstantBuffer : register(b1)
{
	float4x4 World[100];
};


struct VertexShaderInput
{
	float4 position	: POSITION;
	float4 color	: COLOR;
};

struct VertexShaderOutput
{
	float4 position	: SV_POSITION;
	float4 color	: COLOR;
};

VertexShaderOutput main(VertexShaderInput vsInput)
{
	VertexShaderOutput vsOut = (VertexShaderOutput)0;

	const float4x4 matrixViewProjection = mul(Projection, View);
	vsOut.position = mul(matrixViewProjection, vsInput.position);

	vsOut.color = vsInput.color;
	return vsOut;
}