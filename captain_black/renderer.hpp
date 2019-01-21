#pragma once
#include <d3d11.h>

class renderer
{
public:
	void update_io();
	void render();

	bool& initialised();
	ID3D11RenderTargetView*& render_target_view();
	HWND& window_handle();

private:
	bool m_initialised;
	HWND m_window_handle;
	ID3D11RenderTargetView* m_render_target_view;
};