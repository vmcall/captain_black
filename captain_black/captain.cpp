#define NOMINMAX

#include "captain.hpp"
#include "logger.hpp"
#include "global.hpp"
#include "anticheat.hpp"
#include "variable_container.hpp"
#include "Minhook/include/MinHook.h"

#include <windows.h>
#include <cstdint>
#include <chrono>
#include <thread>
#include <numeric>


captain::captain() noexcept :
	m_base_address(reinterpret_cast<std::byte*>(GetModuleHandleW(nullptr))), 
	m_lua(reinterpret_cast<std::uint64_t>(this->base()))
{

}

void captain::start()
{
	// SPAWN CONSOLE
	logger::initialise();
	logger::log("Welcome to CaptainBlack v1");
	logger::log_pointer("Base", this->base());
	logger::log_pointer("SelfPlayerActorProxy", this->local_player());

	// SETUP HOOKS
	this->setup_hooks();

	// DO CODE PATCHES
	this->handle_code_patches();
}

void captain::stop()
{
	// RELEASE CONSOLE
	logger::release();

	// RELEASE HOOKS
	this->release_hooks();
}

void captain::setup_hooks()
{
	logger::log("Hooking...");

	// SETUP MINHOOK
	MH_Initialize();

	// HOOK LUA::DOSTRING
	MH_CreateHook(this->lua().gettop, global::gettop_hook, reinterpret_cast<void**>(&global::gettop_original));
	MH_EnableHook(this->lua().gettop);

	// HOOK	PRESENT
	//auto present = this->overlay_helper().get_present();
	//
	//logger::log_pointer("present", reinterpret_cast<void*>(present));
	//MH_CreateHook(reinterpret_cast<void*>(present), global::present_hook, reinterpret_cast<void**>(&global::present_original));
	//MH_EnableHook(reinterpret_cast<void*>(present));

	logger::log("Hooked..!");


	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	HWND window_handle = GetForegroundWindow();

	ID3D11Device* device = NULL;
	ID3D11DeviceContext* context = NULL;
	IDXGISwapChain* swap_chain = NULL;

#pragma region Initialise DXGI_SWAP_CHAIN_DESC
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(scd));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scd.OutputWindow = window_handle;
	scd.SampleDesc.Count = 1; // Set to 1 to disable multisampling
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Windowed = ((GetWindowLongPtr(window_handle, GWL_STYLE) & WS_POPUP) != 0) ? false : true;
	scd.BufferDesc.Width = 1;
	scd.BufferDesc.Height = 1;
	scd.BufferDesc.RefreshRate.Numerator = 0;
	scd.BufferDesc.RefreshRate.Denominator = 1;
#pragma endregion

	if (FAILED(D3D11CreateDeviceAndSwapChain(
		NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		NULL, &featureLevel, 1, D3D11_SDK_VERSION,
		&scd, &swap_chain,
		&device, NULL, &context
	)))
	{
		logger::log("Failed to create device");
		return;
	}

	// GET VTABLE POINTERS
	auto swapchain_vmt = *reinterpret_cast<void***>(swap_chain);

	// GET FUNCTION POINTERS
	global::cache.d3d11_present = swapchain_vmt[8];
	global::cache.d3d11_draw_indexed = swapchain_vmt[12];
	global::cache.d3d11_create_query = swapchain_vmt[24];

	// HOOK PRESENT
	MH_CreateHook(global::cache.d3d11_present, global::present_hook, reinterpret_cast<void**>(&global::present_original));
	MH_EnableHook(global::cache.d3d11_present);

	// HOOK DRAW INDEXED
	MH_CreateHook(global::cache.d3d11_draw_indexed, global::draw_indexed_hook, reinterpret_cast<void**>(&global::draw_indexed_original));
	MH_EnableHook(global::cache.d3d11_draw_indexed);

	// HOOK CREATE QUERY
	MH_CreateHook(global::cache.d3d11_create_query, global::create_query_hook, reinterpret_cast<void**>(&global::create_query_original));
	MH_EnableHook(global::cache.d3d11_create_query);

	// CLEAN UP
	CloseHandle(window_handle);
}

void captain::release_hooks()
{
	// UNHOOK LUA::DOSTRING
	MH_DisableHook(this->lua().gettop);

	// UNHOOK PRESENT
	//auto present = this->overlay_helper().get_present();
	//MH_DisableHook(reinterpret_cast<void*>(present));

	// D3D11
	MH_DisableHook(global::cache.d3d11_present);
	MH_DisableHook(global::cache.d3d11_draw_indexed);
	MH_DisableHook(global::cache.d3d11_create_query);
}

void captain::handle_loop()
{
	// HANDLE LOCAL-ACTOR PATCHES
	this->handle_local_patches();
}

void captain::handle_code_patches()
{
	logger::log("Patching anti-cheat...");

	// XignCode::SPEEDCAP FORCES NORMAL-RANGE VALUES FOR SPEED MODIFIERS
	auto speedcap_function = this->base() + anticheat::speedcap_function_offset;
	logger::log_pointer("game::cap_speed", speedcap_function);

	// WRITE RET INSTRUCTION TO PATCH IT :')
	constexpr auto ret_opcode = 0xC3;
	*speedcap_function = static_cast<std::byte>(ret_opcode);

	logger::log("Patched..!");
}

void captain::handle_object_scene_info()
{
	logger::log("Dumping ObjectSceneInfo...");

	constexpr auto object_scene_info_offset = 0x327EF30;
	auto raw_container = reinterpret_cast<engine::pa_container*>(this->base() + object_scene_info_offset);
	auto object_scene_info = engine::container_wrapper(raw_container);

	//for (auto entry : field_effect_info.variables())
	//{
	//	printf("[%04llX] %s\n", entry.second.offset, entry.first.c_str());
	//}

	logger::log("Dumped..!");
	logger::log("Patching...");

	// CAMERA DISTANCE
	*object_scene_info.get<float>("cam_maxDistanceFromCharacter") = global::options.camera_distance ? 9999.f : 1000.f;
	
	// STEP HEIGHT
	*object_scene_info.get<float>("ch_stepOffset") = global::options.step ? 9999.f : 30.f;

	logger::log("Patched..!");

	// CLIFF LEAN
	//*object_scene_info.get<float>("_cc_nearCliffCheckDistance") = 0.f;
	
}

void captain::handle_local_patches()
{
	if (this->local_player())
	{
		auto val = global::options.speed_low ? 9999999 : std::numeric_limits<std::int32_t>::max();

		// ATTACK SPEED
		if (global::options.attack_speed)
			this->local_player()->attack_speed = val;

		// CAST SPEED
		if (global::options.cast_speed)
			this->local_player()->cast_speed = val;

		// MOVEMENT SPEED
		constexpr auto max_movement = std::numeric_limits<std::int32_t>::max();
		if (global::options.movement_speed)
			this->local_player()->movement_speed = max_movement;
	}
}

std::byte* captain::base()
{
	return this->m_base_address;
}

engine::actor* captain::local_player()
{
	return *reinterpret_cast<engine::actor**>(this->base() + engine::actor::local_offset);
}

engine::lua& captain::lua()
{
	return this->m_lua;
}

engine::command_helper& captain::command_helper()
{
	return this->m_command_helper;
}

native::keyboard_input& captain::keyboard()
{
	return this->m_keyboard;
}

steam::overlay_helper& captain::overlay_helper()
{
	return this->m_overlay_helper;
}
