#define NOMINMAX
#include "global.hpp"
#include "renderer.hpp"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"

void renderer::update_io()
{
	POINT mouse_pos = {};
	GetCursorPos(&mouse_pos);
	ScreenToClient(this->window_handle(), &mouse_pos);

	// SAVE MOUSE INFO
	ImGui::GetIO().MousePos.x = static_cast<float>(mouse_pos.x);
	ImGui::GetIO().MousePos.y = static_cast<float>(mouse_pos.y);
	ImGui::GetIO().MouseDown[0] = GetAsyncKeyState(VK_LBUTTON);
}

void renderer::render()
{
	ImGui::Begin("Captain Black");
	
	ImGui::Checkbox("Attack speed hack", &global::options.attack_speed);
	ImGui::Checkbox("Movement speed hack", &global::options.movement_speed);
	ImGui::Checkbox("Cast speed hack", &global::options.cast_speed);
	ImGui::Checkbox("Low speed", &global::options.speed_low);
	//ImGui::SliderInt("Value", &global::options.attack_speed_value, 0, std::numeric_limits<std::int32_t>::max());

	ImGui::Separator();

	ImGui::Text("Object scene info");
	ImGui::Checkbox("Step", &global::options.step);
	ImGui::Checkbox("Camera distance", &global::options.camera_distance);

	if (ImGui::Button("Patch"))
	{
		global::cpt.handle_object_scene_info();
	}

	ImGui::Separator();
	ImGui::Text("Lua");

	if (ImGui::Button("Run test.lua"))
	{
		global::run_lua_from_disk = true;
	}

	ImGui::End();
}

bool& renderer::initialised()
{
	return this->m_initialised;
}

ID3D11RenderTargetView*& renderer::render_target_view()
{
	return this->m_render_target_view;
}

HWND& renderer::window_handle()
{
	return this->m_window_handle;
}
