#include <d3d9.h>

#pragma comment(lib, "d3d9.lib")					// ����d3d���ļ�
#pragma comment(lib, "d3dx9.lib")					// ����d3dx��չ���ļ�

#define WINDOW_CLASS L"DirectXGO"
#define WINDOW_TITLE L"��ͨͼ������������"

LPDIRECT3D9 g_D3D = NULL;							// D3D����ָ��
LPDIRECT3DDEVICE9 g_D3dDevice = NULL;				// D3D�豸����ָ��

bool InitializeD3D(HWND hWnd);						// ��ʼ��D3D
void Shutdown();									// �ͷ�D3d�豸�����D3D����
void RenderScene();									// ��Ⱦ����

// ���ڹ��̻ص�����
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

// �ĸ����������ھ�������õ�16λ���ھ���������в�������ʾ����
int APIENTRY wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int iCmdShow)
{
	// ���崰����
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);					// �������С
	wc.style = CS_CLASSDC;							// ������ʽ
	wc.lpfnWndProc = MsgProc;						// ָ�򴰿ڹ��̵�ָ��
	wc.cbClsExtra = 0;								// ��������ṹ����Ķ����ֽ���
	wc.cbWndExtra = 0;								// Ҫ�ڴ���ʵ��֮�����Ķ����ֽ���
	wc.hInstance = hInst;							// ���ھ��
	wc.hIcon = NULL;								// ͼ����
	wc.hCursor = NULL;								// �����
	wc.hbrBackground = NULL;						// ������ˢ
	wc.lpszMenuName = NULL;							// �˵�
	wc.lpszClassName = WINDOW_CLASS;				// ��������
	wc.hIconSm = NULL;								// Сͼ����

	// ע�ᴰ����
	RegisterClassEx(&wc);

	// �������ڲ������������������ڱ��⡢������ʽ��LEFT��TOP�����ߡ������ڡ��˵������ھ��������ָ��Ҫ���ݵ����ڵ�ֵ��ָ��
	HWND hWnd = CreateWindow(WINDOW_CLASS, WINDOW_TITLE, WS_OVERLAPPEDWINDOW, 100, 100, 1024, 768, NULL, NULL, hInst, NULL);

	// ��ʼ��D3D
	if (InitializeD3D(hWnd))
	{
		ShowWindow(hWnd, SW_SHOWDEFAULT);
		UpdateWindow(hWnd);

		// ������Ϣ������
		MSG msg;
		ZeroMemory(&msg, sizeof(msg));

		while (msg.message != WM_QUIT)
		{
			// ����Ϣ������ȡ����Ϣ��ֵ�� msg
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);						// ������Ϣ
				DispatchMessage(&msg);						// ������Ϣ��ת��wc.lpfnWndProc(����Ϊ MsgProc)
			}
			else
			{
				RenderScene();
			}
		}
	}
	
	Shutdown();												// �ͷ�D3d�豸�����D3D����

	UnregisterClass(WINDOW_CLASS, hInst);					// ע������
}

bool InitializeD3D(HWND hWnd)
{
	g_D3D = Direct3DCreate9(D3D_SDK_VERSION);				// ����D3D����
	if (g_D3D == NULL) return false;

	D3DDISPLAYMODE displayMode;								// ��ʾģʽ
	if (FAILED(g_D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode))) return false;		// ��ȡ���Կ���ʾģʽ

	// ����D3D�豸�����õ���ʾ����
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	d3dpp.Windowed = true;
	d3dpp.BackBufferFormat = displayMode.Format;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	// ����D3D�豸����
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
	// 1. ����
	g_D3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	// 2. ��ʼ����
	g_D3dDevice->BeginScene();

	// 3. �����ʾ3Dͼ��

	// 4. ��������
	g_D3dDevice->EndScene();

	// 5. ��ʾ
	g_D3dDevice->Present(NULL, NULL, NULL, NULL);
}
