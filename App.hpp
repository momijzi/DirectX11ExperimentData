#pragma once

#define OEMRESOURCE

#include <forward_list>
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

class App final//final���ׂ�[
{
public:
	#include "Window.hpp"
	#include"Graphics.hpp"

	static constexpr wchar_t* NAME = L"DirectX11";

	App() = delete;
	//���ׂĂ̏����������������Ƃ��̂�true��Ԃ�
	static bool Refresh()
	{
		GetGraphics().Update();
		return GetWindow().Update();
	}
	//�������֐�
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
	//�E�B���h�E----------------------------------------
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
	//�O���t�B�b�N--------------------------------------
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
private:
	//�E�B���h�E�̃f�[�^���擾���邽�߂̃|�C���^�\
	static Window& GetWindow()
	{
		static std::unique_ptr<Window> window(new Window());
		return *window.get();
	}
	//�O���t�B�b�N�̃f�[�^���擾���邽�߂̃|�C���^�\
	static Graphics& GetGraphics()
	{
		static std::unique_ptr<Graphics> graphics(new Graphics());
		return *graphics.get();
	}
};
#include"Texture.hpp"
#include"Material.hpp"
#include"Camera.hpp"
#include"Mesh.hpp"