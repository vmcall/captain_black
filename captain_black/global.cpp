#define NOMINMAX
#include "global.hpp"
#include "logger.hpp"
#include <iostream>
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"

captain global::cpt = {};
renderer global::menu = {};
settings global::options = {};
cache_t global::cache = {};
directx global::direct = {};

//engine::lua::do_string_t global::do_string_original = nullptr;

engine::lua::gettop_t global::gettop_original = nullptr;
global::present_t global::present_original = nullptr;
global::draw_indexed_t global::draw_indexed_original = nullptr;
global::create_query_t global::create_query_original = nullptr;

bool global::run_lua_from_disk = false;

//
//
//void* __fastcall global::do_string_hook(std::int64_t state, const char* command, std::int64_t length)
//{
//	if (state && command)
//	{
//		if (!dump)
//		{
//			constexpr auto dump_command = "dofile('D:/BDO/Extract_Tables_Globals.lua')";
//			auto result = global::do_string_original(state, dump_command, sizeof(dump_command));
//			printf("Dumped! - %p\n", result);
//			dump = true;
//		}
//
//		printf("[lua::do_string] %s - (%p)\n", command, reinterpret_cast<void*>(state));
//	}
//
//	return global::do_string_original(state, command, length);
//}

bool running_script = false;

void* __fastcall global::gettop_hook(std::int64_t state)
{
	if (state)
	{
		//printf("[lua::gettop] %p\n", state);
		
		if (global::run_lua_from_disk && !running_script)
		{
			running_script = true;

			constexpr auto dump_command = "dofile(\"D:/BDO/test.lua\")";
			auto result = global::cpt.lua().do_string(state, dump_command, strlen(dump_command));
			printf("Running - %p\n", result);

			running_script = false;
			global::run_lua_from_disk = false;
		}

	}

	return global::gettop_original(state);
}

HRESULT __stdcall global::present_hook(IDXGISwapChain* swapchain_pointer, UINT sync_interval, UINT flags)
{
	//printf("Present called - %X\n", GetTickCount());

	swapchain_pointer->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&global::direct.device()));
	global::direct.device()->GetImmediateContext(&global::direct.device_context());

	if (!global::menu.initialised())
	{
		logger::log("initialising present...");
		global::menu.window_handle() = FindWindowW(L"BlackDesertWindowClass", NULL);

		ID3D11Texture2D* render_target_texture = nullptr;
		if (SUCCEEDED(swapchain_pointer->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&render_target_texture))))
		{
			global::direct.device()->CreateRenderTargetView(render_target_texture, NULL, &global::menu.render_target_view());
			render_target_texture->Release();
		}

		ImGui_ImplDX11_Init(global::menu.window_handle(), global::direct.device(), global::direct.device_context());
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


		// DRAW INDEXED STUFF
		D3D11_DEPTH_STENCIL_DESC  stencilDesc;
		stencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		stencilDesc.StencilEnable = true;
		stencilDesc.StencilReadMask = 0xFF;
		stencilDesc.StencilWriteMask = 0xFF;
		stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		
		stencilDesc.DepthEnable = true;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		global::direct.device()->CreateDepthStencilState(&stencilDesc, &directx::g_depth_stencil_states[static_cast<int>(directx::depth_state::enabled)]);
		
		stencilDesc.DepthEnable = false;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		global::direct.device()->CreateDepthStencilState(&stencilDesc, &directx::g_depth_stencil_states[static_cast<int>(directx::depth_state::disabled)]);
		
		stencilDesc.DepthEnable = false;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		stencilDesc.StencilEnable = false;
		stencilDesc.StencilReadMask = UINT8(0xFF);
		stencilDesc.StencilWriteMask = 0x0;
		global::direct.device()->CreateDepthStencilState(&stencilDesc, &directx::g_depth_stencil_states[static_cast<int>(directx::depth_state::no_read_no_write)]);
		
		stencilDesc.DepthEnable = true;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		stencilDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
		stencilDesc.StencilEnable = false;
		stencilDesc.StencilReadMask = UINT8(0xFF);
		stencilDesc.StencilWriteMask = 0x0;
		
		stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		
		stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
		global::direct.device()->CreateDepthStencilState(&stencilDesc, &directx::g_depth_stencil_states[static_cast<int>(directx::depth_state::read_no_write)]);
		
		D3D11_RASTERIZER_DESC rwDesc;
		global::direct.device_context()->RSGetState(&global::direct.rw_state());
		global::direct.rw_state()->GetDesc(&rwDesc);
		rwDesc.FillMode = D3D11_FILL_WIREFRAME;
		rwDesc.CullMode = D3D11_CULL_NONE;
		global::direct.device()->CreateRasterizerState(&rwDesc, &global::direct.rw_state());
		
		D3D11_RASTERIZER_DESC rsDesc;
		global::direct.device_context()->RSGetState(&global::direct.rs_state());
		global::direct.rs_state()->GetDesc(&rsDesc);
		rsDesc.FillMode = D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_NONE;
		global::direct.device()->CreateRasterizerState(&rsDesc, &global::direct.rs_state());


		global::direct.create_shader(1.f, 0.2f, 0.2f, &global::direct.crimson_shader());
		global::direct.create_shader(1.f, 0.6f, 0.0f, &global::direct.yellow_shader());

		global::menu.initialised() = true;
		logger::log("initialised present..!");
	}

	global::direct.device_context()->OMSetRenderTargets(1, &global::menu.render_target_view(), NULL);

	ImGui_ImplDX11_NewFrame();

	// RENDER MENU

	// UPDATE IO
	global::menu.update_io();

	if (global::options.show_menu)	
		global::menu.render();

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

	// HANDLE CHEAT
	global::cpt.handle_loop();


	if (global::cpt.keyboard().pressed(VK_INSERT, true))
	{
		global::options.show_menu = !global::options.show_menu;
	}

	if (global::cpt.keyboard().pressed(VK_END, true))
	{
		global::cpt.stop();
	}

	return global::present_original(swapchain_pointer, sync_interval, flags);
}

void __stdcall global::draw_indexed_hook(ID3D11DeviceContext* context, UINT index_count, UINT start_index_location, INT base_vertex_location)
{
	UINT Stride;
	ID3D11Buffer *veBuffer;
	UINT veBufferOffset = 0;
	context->IAGetVertexBuffers(0, 1, &veBuffer, &Stride, &veBufferOffset);


	if (Stride == 36)
	{
		global::direct.set_depth_stencil_state(directx::depth_state::disabled);
		context->PSSetShader(global::direct.yellow_shader(), NULL, NULL);

		global::draw_indexed_original(context, index_count, start_index_location, base_vertex_location);

		global::direct.set_depth_stencil_state(directx::depth_state::enabled);
		context->PSSetShader(global::direct.crimson_shader(), NULL, NULL);

		context->PSSetShaderResources(0, 1, &ShaderResourceView);
	}

	return global::draw_indexed_original(context, index_count, start_index_location, base_vertex_location);
}

void __stdcall global::create_query_hook(ID3D11Device* device, const D3D11_QUERY_DESC* query_description, ID3D11Query** query)
{

	//if (query_description->Query == D3D11_QUERY_OCCLUSION)
	//{
	//	D3D11_QUERY_DESC o_query_desc = CD3D11_QUERY_DESC();
	//	(&o_query_desc)->MiscFlags = query_description->MiscFlags;
	//	(&o_query_desc)->Query = D3D11_QUERY_TIMESTAMP;
	//
	//	return global::create_query_original(device, &o_query_desc, query);
	//}

	return global::create_query_original(device, query_description, query);
}
