#include <d3d9.h>

#pragma comment(lib, "d3d9.lib")					// 包含d3d库文件
#pragma comment(lib, "d3dx9.lib")					// 包含d3dx扩展库文件

#define WINDOW_CLASS L"DirectXGO"
#define WINDOW_TITLE L"港通图形轻量化引擎"

LPDIRECT3D9 g_D3D = NULL;							// D3D对象指针
LPDIRECT3DDEVICE9 g_D3dDevice = NULL;				// D3D设备对象指针

bool InitializeD3D(HWND hWnd);						// 初始化D3D
void Shutdown();									// 释放D3d设备对象和D3D对象
void RenderScene();									// 渲染场景

// 窗口过程回调函数
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
		}
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// 四个参数：窗口句柄，弃用的16位窗口句柄，命令行参数，显示设置
int APIENTRY wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int iCmdShow)
{
	// 定义窗口类
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);					// 窗口类大小
	wc.style = CS_CLASSDC;							// 窗口样式
	wc.lpfnWndProc = MsgProc;						// 指向窗口过程的指针
	wc.cbClsExtra = 0;								// 按窗口类结构分配的额外字节数
	wc.cbWndExtra = 0;								// 要在窗口实例之后分配的额外字节数
	wc.hInstance = hInst;							// 窗口句柄
	wc.hIcon = NULL;								// 图标句柄
	wc.hCursor = NULL;								// 鼠标句柄
	wc.hbrBackground = NULL;						// 背景画刷
	wc.lpszMenuName = NULL;							// 菜单
	wc.lpszClassName = WINDOW_CLASS;				// 窗口类名
	wc.hIconSm = NULL;								// 小图标句柄

	// 注册窗口类
	RegisterClassEx(&wc);

	// 创建窗口参数：窗口类名、窗口标题、窗口样式、LEFT、TOP、宽、高、父窗口、菜单、窗口句柄、对象指向要传递到窗口的值的指针
	HWND hWnd = CreateWindow(WINDOW_CLASS, WINDOW_TITLE, WS_OVERLAPPEDWINDOW, 100, 100, 1024, 768, NULL, NULL, hInst, NULL);

	// 初始化D3D
	if (InitializeD3D(hWnd))
	{
		ShowWindow(hWnd, SW_SHOWDEFAULT);
		UpdateWindow(hWnd);

		// 创建消息并清零
		MSG msg;
		ZeroMemory(&msg, sizeof(msg));

		while (msg.message != WM_QUIT)
		{
			// 从消息队列中取出消息赋值给 msg
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);						// 翻译消息
				DispatchMessage(&msg);						// 分派消息，转给wc.lpfnWndProc(本例为 MsgProc)
			}
			else
			{
				RenderScene();
			}
		}
	}
	
	Shutdown();												// 释放D3d设备对象和D3D对象

	UnregisterClass(WINDOW_CLASS, hInst);					// 注销窗口
}

bool InitializeD3D(HWND hWnd)
{
	g_D3D = Direct3DCreate9(D3D_SDK_VERSION);				// 创建D3D对象
	if (g_D3D == NULL) return false;

	D3DDISPLAYMODE displayMode;								// 显示模式
	if (FAILED(g_D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode))) return false;		// 获取主显卡显示模式

	// 创建D3D设备对象用的显示参数
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	d3dpp.Windowed = true;
	d3dpp.BackBufferFormat = displayMode.Format;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	// 创建D3D设备对象
	if (FAILED(g_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_D3dDevice))) return false;

	return true;
}

void Shutdown()
{
	if (g_D3dDevice != NULL) g_D3dDevice->Release();
	if (g_D3D != NULL) g_D3D->Release();
}

void RenderScene()
{
	// 1. 清屏
	g_D3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	// 2. 开始场景
	g_D3dDevice->BeginScene();

	// 3. 输出显示3D图形

	// 4. 结束场景
	g_D3dDevice->EndScene();

	// 5. 显示
	g_D3dDevice->Present(NULL, NULL, NULL, NULL);
}
