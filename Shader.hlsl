cbuffer Object : register(b0)
{
    matrix world;
};
cbuffer Camera : register(b1)
{
    matrix view;
    matrix projection;
};
Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);
struct Vertex
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};
struct Pixel
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};
Pixel VS(Vertex vertex)
{
    Pixel output;
    output.position = mul(vertex.position, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    output.normal = mul(vertex.normal, (float3x3)world);
    output.uv = vertex.uv;
    return output;
}
float4 PS(Pixel pixel) : SV_TARGET
{
    float3 normal = normalize(pixel.normal);
    float3 lightDirection = normalize(float3(0.25, -1.0, 0.5));
    float3 lightColor = float3(1.0, 1.0, 1.0);
    float4 diffuseColor = texture0.Sample(sampler0, pixel.uv);
    float3 diffuseIntensity = dot(-lightDirection, normal) * lightColor;
    float3 ambientIntensity = lightColor * 0.2;
    return diffuseColor * float4(diffuseIntensity + ambientIntensity, 1);
}
//cbuffer Constant : register(b0)
//{
//    matrix world;
//    matrix view;
//    matrix projection;
//};
//struct Vertex
//{
//    float4 position : POSITION;
//    float3 normal : NORMAL;
//};
//struct Pixel
//{
//    float4 position : SV_POSITION;
//    float3 normal : NORMAL;
//};
//Pixel VS(Vertex vertex)
//{
//    Pixel output;
//    output.position = mul(vertex.position, world);
//    output.position = mul(output.position, view);
//    output.position = mul(output.position, projection);
//    output.normal = mul(vertex.normal,(float3x3)world);
//    return output;
//}
//float4 PS(Pixel pixel) : SV_TARGET
//{
//	float3 normal = normalize(pixel.normal);
//	float3 lightDirection = normalize(float3(0.25, -1.0, 0.5));
//	float3 lightColor = float3(0.0, 0.0, 1.0);
//
//	float3 diffuseIntensity = dot(-lightDirection, normal)*lightColor;
//	float3 ambientIntensity = lightColor * 0.2;
//	return float4(diffuseIntensity + ambientIntensity, 1);
//}