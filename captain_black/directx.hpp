#pragma once
#include <d3d11.h>
#include "vector.hpp"

class directx
{
public:
	enum depth_state
	{
		enabled,
		disabled,
		read_no_write,
		no_read_no_write,
		_depth_count
	};


	ID3D11Device*& device();
	ID3D11DeviceContext*& device_context();

	bool world_to_screen(math::vector3d in, math::vector2d out);

	bool create_shader(float r, float g, float b, ID3D11PixelShader** shader);

	static ID3D11DepthStencilState* g_depth_stencil_states[static_cast<int>(depth_state::_depth_count)];
	void set_depth_stencil_state(depth_state state);

	ID3D11RasterizerState*& rw_state();
	ID3D11RasterizerState*& rs_state();

	ID3D11PixelShader*& crimson_shader();
	ID3D11PixelShader*& yellow_shader();

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_device_context;
	ID3D11PixelShader* m_color_crimson;
	ID3D11PixelShader* m_color_yellow;
	ID3D11ShaderResourceView* m_shader_resource_view;
	ID3D11RasterizerState* m_rw_state;
	ID3D11RasterizerState* m_rs_state;
};