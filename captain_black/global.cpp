#include "global.hpp"
#include <iostream>
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"

bdo::cheat bdo::global::cheat = {};
//bdo::engine::lua::do_string_t bdo::global::do_string_original = nullptr;
bdo::engine::lua::gettop_t bdo::global::gettop_original = nullptr;
bdo::global::present_t bdo::global::present_original = nullptr;

bool bdo::global::run_lua_from_disk = false;

//
//
//void* __fastcall bdo::global::do_string_hook(std::int64_t state, const char* command, std::int64_t length)
//{
//	if (state && command)
//	{
//		if (!dump)
//		{
//			constexpr auto dump_command = "dofile('D:/BDO/Extract_Tables_Globals.lua')";
//			auto result = bdo::global::do_string_original(state, dump_command, sizeof(dump_command));
//			printf("Dumped! - %p\n", result);
//			dump = true;
//		}
//
//		printf("[lua::do_string] %s - (%p)\n", command, reinterpret_cast<void*>(state));
//	}
//
//	return bdo::global::do_string_original(state, command, length);
//}

bool running_script = false;

void* __fastcall bdo::global::gettop_hook(std::int64_t state)
{
	if (state)
	{
		//printf("[lua::gettop] %p\n", state);
		

		if (global::run_lua_from_disk && !running_script)
		{
			running_script = true;

			constexpr auto dump_command = "dofile(\"D:/BDO/test.lua\")";
			auto result = bdo::global::cheat.lua().do_string(state, dump_command, strlen(dump_command));
			printf("Dumped! - %p\n", result);

			running_script = false;
			global::run_lua_from_disk = false;
		}

	}

	return bdo::global::gettop_original(state);
}

bool should_initialise = true;
ID3D11RenderTargetView* render_target_view = nullptr;
HWND window_handle = NULL;
HRESULT __stdcall bdo::global::present_hook(IDXGISwapChain* swapchain_pointer, UINT sync_interval, UINT flags)
{
	printf("Present called - %X\n", GetTickCount());

	ID3D11Device* device;
	ID3D11DeviceContext *device_context;
	swapchain_pointer->GetDevice(__uuidof(device), reinterpret_cast<void**>(&device));
	device->GetImmediateContext(&device_context);

	if (should_initialise)
	{
		window_handle = FindWindowW(L"BlackDesertWindowClass", NULL);

		ID3D11Texture2D* render_target_texture = nullptr;
		if (SUCCEEDED(swapchain_pointer->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&render_target_texture))))
		{
			device->CreateRenderTargetView(render_target_texture, NULL, &render_target_view);
			render_target_texture->Release();
		}

		ImGui_ImplDX11_Init(window_handle, device, device_context);
		ImGui_ImplDX11_CreateDeviceObjects();

#pragma region ImGui Theme
		auto& style = ImGui::GetStyle();

		style.ChildWindowRounding = 3.f;
		style.GrabRounding = 0.f;
		style.WindowRounding = 0.f;
		style.ScrollbarRounding = 3.f;
		style.FrameRounding = 3.f;
		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

		style.Colors[ImGuiCol_Text] = ImVec4(0.73f, 0.73f, 0.73f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.26f, 0.26f, 0.26f, 0.95f);
		style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_ComboBg] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_Column] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_CloseButton] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
		style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.32f, 0.52f, 0.65f, 1.00f);
		style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);

		auto& io = ImGui::GetIO();

		io.Fonts->AddFontDefault();
#pragma endregion

		should_initialise = false;
	}

	device_context->OMSetRenderTargets(1, &render_target_view, NULL);

	ImGui_ImplDX11_NewFrame();


	// UPDATE IO
	POINT mouse_pos;
	GetCursorPos(&mouse_pos);
	ScreenToClient(window_handle, &mouse_pos);

	// SAVE MOUSE INFO
	ImGui::GetIO().MousePos.x = static_cast<float>(mouse_pos.x);
	ImGui::GetIO().MousePos.y = static_cast<float>(mouse_pos.y);
	ImGui::GetIO().MouseDown[0] = GetAsyncKeyState(VK_LBUTTON);

	ImGui::Begin("CaptainBlack v1");
	ImGui::End();

	// MENU
	//if (global::cheat.keyboard().pressed(VK_INSERT, true))
	//	Settings::DisplayMenu = !Settings::DisplayMenu;
	//
	//if (Settings::DisplayMenu)
	//{
	//	Renderer::UpdateIO(ImGui::GetIO(), window_handle);
	//	Renderer::DrawMainMenu();
	//}

	ImGui::Render();

	return bdo::global::present_original(swapchain_pointer, sync_interval, flags);
}
