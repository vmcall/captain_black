#include "directx.hpp"
#include <d3dcompiler.h>
#include <iostream>

ID3D11DepthStencilState* directx::g_depth_stencil_states[static_cast<int>(depth_state::_depth_count)] = {};

ID3D11Device*& directx::device()
{
	return this->m_device;
}

ID3D11DeviceContext*& directx::device_context()
{
	return this->m_device_context;
}

bool directx::world_to_screen(math::vector3d in, math::vector2d out)
{
	if (this->device() == nullptr)
		return false;

	return true;
}

bool directx::create_shader(float r, float g, float b, ID3D11PixelShader** shader)
{
	char structure[] = "struct VS_OUT"
		"{"
		" float4 Position : SV_Position;"
		" float4 Color : COLOR0;"
		"};"

		"float4 main( VS_OUT input ) : SV_Target"
		"{"
		" float4 fake;"
		" fake.a = 1.0f;"
		" fake.r = %f;"
		" fake.g = %f;"
		" fake.b = %f;"
		" return fake;"
		"}";

	char shader_data[250];

	sprintf_s(shader_data, structure, r, g, b);

	ID3DBlob* error_message_blob;

	ID3D10Blob* blob;
	auto compiled = D3DCompile(shader_data, sizeof(shader_data), "shader", NULL, NULL, "main", "ps_4_0", NULL, NULL, &blob, &error_message_blob);

	if (FAILED(compiled))
		return false;

	auto created = this->device()->CreatePixelShader((DWORD*)blob->GetBufferPointer(), blob->GetBufferSize(), NULL, shader);

	return !FAILED(created);
}

void directx::set_depth_stencil_state(depth_state state)
{
	return this->device_context()->OMSetDepthStencilState(directx::g_depth_stencil_states[state], 1);
}

ID3D11RasterizerState*& directx::rw_state()
{
	return this->m_rw_state;
}

ID3D11RasterizerState*& directx::rs_state()
{
	return this->m_rs_state;
}

ID3D11PixelShader*& directx::crimson_shader()
{
	return this->m_color_crimson;
}

ID3D11PixelShader*& directx::yellow_shader()
{
	return this->m_color_yellow;
}
