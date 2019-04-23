#pragma once
#define NOMINMAX
#include <d3d11.h>
#include "captain.hpp"
#include "renderer.hpp"
#include "settings.hpp"
#include "cache.hpp"
#include "directx.hpp"

namespace global
{
	extern captain cpt;
	extern renderer menu;
	extern settings options;
	extern cache_t cache;
	extern directx direct;

	//extern engine::lua::do_string_t do_string_original;
	//extern void* __fastcall do_string_hook(std::int64_t state, const char* command, std::int64_t length);

	// LUA HOOKS
	extern engine::lua::gettop_t gettop_original;
	extern void* __fastcall gettop_hook(std::int64_t state);
	extern bool run_lua_from_disk;

	// DX HOOKS
	
	// PRESENT
	using present_t = HRESULT(__stdcall*)(IDXGISwapChain* swapchain_pointer, UINT sync_interval, UINT flags);
	extern present_t present_original;
	extern HRESULT __stdcall present_hook(IDXGISwapChain* swapchain_pointer, UINT sync_interval, UINT flags);

	// DRAW INDEXED
	using draw_indexed_instanced_t = void(__stdcall*)(ID3D11DeviceContext* context, UINT index_count_per_instance, UINT instance_count, UINT start_index_location, INT base_vertex_location, UINT start_instance_location);
	extern draw_indexed_instanced_t draw_indexed_instanced_original;
	extern void __stdcall draw_indexed_instanced_hook(ID3D11DeviceContext* context, UINT index_count_per_instance, UINT instance_count, UINT start_index_location, INT base_vertex_location, UINT start_instance_location);

	// CREATE QUERY
	using create_query_t = void(__stdcall*)(ID3D11Device* device, const D3D11_QUERY_DESC* query_description, ID3D11Query** query);
	extern create_query_t create_query_original;
	extern void __stdcall create_query_hook(ID3D11Device* device, const D3D11_QUERY_DESC* query_description, ID3D11Query** query);

}