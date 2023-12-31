cbuffer CameraCBuffer : register(b0)
{
	float4x4 View;
	float4x4 Projection;
};

//cbuffer WorldCBuffer : register(b1)
//{
//	float4x4 World[1024];
//};

StructuredBuffer<float4x4> worldSBuffer : register(t0);

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

VertexShaderOutput main(VertexShaderInput vsInput, uint instanceID : SV_InstanceID)
{
	VertexShaderOutput vsOut = (VertexShaderOutput)0;


	const float4x4 matrixViewProjection = mul(Projection, View);

	vsOut.position = vsInput.position;
	vsOut.position = mul(worldSBuffer[instanceID], vsOut.position);
	vsOut.position = mul(matrixViewProjection, vsOut.position);

	vsOut.color = vsInput.color;
	return vsOut;
}