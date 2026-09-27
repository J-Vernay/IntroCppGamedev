#include <jv/x64-windows/jv_win64_impl.h>

constexpr char jv::win64::kVertexShader[] = R"(
cbuffer cb
{
    float2 half_vp_size;
    float2 tex_size;
    float2 offset;
    float2 rot_cos_sin;
    float2 scale;
}

struct VSInput
{
    float2 pos: POSITION0;
    float2 uv: TEXCOORD0;
    float4 color: COLOR0;
};

struct VSOutput
{
    float4 pos: SV_Position;
    float2 uv: TEXCOORD0;
    float4 color: COLOR0;
};

VSOutput main(VSInput input)
{
    VSOutput output = (VSOutput)0;

    float2 pos;
    pos.x = input.pos.x * rot_cos_sin.x - input.pos.y * rot_cos_sin.y;
    pos.y = input.pos.x * rot_cos_sin.y + input.pos.y * rot_cos_sin.x;
    pos *= scale;
    pos += offset;
    pos /= half_vp_size;
    pos -= float2(1, 1);

    // Invert y-axis to have topleft = (0,0)
    output.pos = float4(pos.x, -pos.y, 0, 1);
    output.uv = input.uv / tex_size;
    output.color = input.color;
    return output;
}
)";

constexpr char jv::win64::kPixelShader[] = R"(
sampler Sampler : register(s0);
Texture2D Texture : register(t0);

struct PSInput
{
    float4 pos: SV_Position;
    float2 uv: TEXCOORD0;
    float4 color: COLOR0;
};

struct PSOutput
{
    float4 color: SV_Target0;
};

PSOutput main(PSInput input)
{
    PSOutput output = (PSOutput)0;
    output.color = Texture.Sample(Sampler, input.uv);
    output.color *= input.color;
    return output;
}
)";

#pragma endregion
