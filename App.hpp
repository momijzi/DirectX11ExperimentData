#pragma once

#define OEMRESOURCE
//久保田_04_10------------------------------------------
#define PI 3.141592653589793
//------------------------------------------久保田_04_10

#include <forward_list>
#include <fstream>
#include <memory>
#include <vector>
#include <Windows.h>
#include <atlbase.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <Wincodec.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define MAIN()	APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)

#include "Utility.hpp"

class App final//final調べろー
{
public:
	#include"Window.hpp"
	#include"Graphics.hpp"
	#include"Input.hpp"

	static constexpr wchar_t* NAME = L"DirectX11";

	App() = delete;
	//すべての初期化が完了したときのみtrueを返す
	static bool Refresh()
	{
		GetGraphics().Update();
		GetInput().Update();
		return GetWindow().Update();
	}
	//初期化関数
	static void Initialize()
	{
		static bool isInitializes = false;

		if (!isInitializes)
		{
			CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
			isInitializes = true;
		}
	}
	//ウィンドウ----------------------------------------
	static HWND GetWindowHandle()
	{
		return GetWindow().GetHandle();
	}
	static DirectX::XMINT2 GetWindowSize()
	{
		return GetWindow().GetSize();
	}
	static void SetWindowSize(int width,int height)
	{
		return GetWindow().SetSize(width,height);
	}
	static wchar_t* const GetTitle()
	{
		return GetWindow().GetTitle();
	}
	static void SetTitle(const wchar_t* const title)
	{
		GetWindow().SetTitle(title);
	}
	static void SetFullScreen(bool isFullScreen)
	{
		GetWindow().SetFullScreen(isFullScreen);
	}
	static void AddProcedure(Window::Proceedable* const procedure)
	{
		GetWindow().AddProcedure(procedure);
	}
	static void RemoveProcedure(Window::Proceedable* const procedure)
	{
		GetWindow().RemoveProcedure(procedure);
	}
	//グラフィック--------------------------------------
	static ID3D11Device& GetGraphicsDevice()
	{
		return GetGraphics().GetDevice();
	}
	static ID3D11DeviceContext& GetGraphicsContext()
	{
		return GetGraphics().GetContext();
	}
	static IDXGISwapChain& GetGraphicsMemory()
	{
		return GetGraphics().GetMemory();
	}
	//キー入力-------------------------------------------
	static bool GetKey(int VK_CODE)
	{
		return GetInput().GetKey(VK_CODE);
	}
	static bool GetKeyUp(int VK_CODE)
	{
		return GetInput().GetKeyUp(VK_CODE);
	}
	static bool GetKeyDown(int VK_CODE)
	{
		return GetInput().GetKeyDown(VK_CODE);
	}
	static Float2 GetMousePosition()
	{
		return GetInput().GetMousePosition();
	}
	static void SetMousePosition(float x, float y)
	{
		GetInput().SetMousePosition(x, y);
	}
	static void SetShowCursor(bool isShowCursor)
	{
		GetInput().SetShowCursor(isShowCursor);
	}


private:
	//ウィンドウのデータ取得用ポインタ
	static Window& GetWindow()
	{
		static std::unique_ptr<Window> window(new Window());
		return *window.get();
	}
	//グラフィックのデータ取得用ポインタ
	static Graphics& GetGraphics()
	{
		static std::unique_ptr<Graphics> graphics(new Graphics());
		return *graphics.get();
	}
	//キー入力のデータ取得用ポインタ
	static Input& GetInput()
	{
		static std::unique_ptr<Input> input(new Input());
		return *input.get();
	}
};
#include"Map.hpp"
#include"Texture.hpp"
#include"Material.hpp"
#include"Camera.hpp"
#include"Mesh.hpp"
#include"Sprite.hpp"
#include"OBB.hpp"
#include"Player.hpp"