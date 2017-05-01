// CalendarDemo.cpp : ����Ӧ�ó������ڵ㡣
//

#include "NUIMainView.h"
#include "Resource.h"
#include "mainforwindows.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
HWND g_hWnd = 0;
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

int g_iScreenWidth = 1024; // ��Ļ����
int g_iScreenHeight = 768; // ��Ļ�߶�
bool		g_bMoveWindow = FALSE;
int g_mouse_down_flag = FALSE;
CNUIInstance  g_nui_instance;

// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: �ڴ˷��ô��롣
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_NUIApp, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDD_NUIApp_DIALOG));

	// NUI�ӹ�����Ϣѭ��:
	//while (GetMessage(&msg, NULL, 0, 0))
	//{
	//	if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
	//	{
	//		TranslateMessage(&msg);
	//		DispatchMessage(&msg);
	//	}
	//}

	// ����Ϣѭ��:
	kn_bool bExit = FALSE;
	g_nui_instance.RunEventLoop(bExit);

	g_nui_instance.release();
	return  0;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴��������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NUIApp));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	//wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_HMI);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_NUIApp));

	return RegisterClassEx(&wcex);
}
void CreateView()
{
	////  ��������


	KMainView_PTR main_view = KMainView_PTR(new KMainView() );
	main_view->Create(RERect::MakeXYWH(0, 0, (SkScalar)g_iScreenWidth , (SkScalar)g_iScreenHeight));
	g_nui_instance.getScreen()->AddView( main_view );	
	main_view->init();


}
//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hWnd =  CreateWindow(
	   szWindowClass,
	   szTitle,
	   WS_POPUP,
	   (GetSystemMetrics(SM_CXSCREEN) - g_iScreenWidth) / 2,
	   (GetSystemMetrics(SM_CYSCREEN) - g_iScreenHeight) / 2,
	   g_iScreenWidth, g_iScreenHeight,
	   NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   KNUIInitPara para;
   para.m_w = g_iScreenWidth;
   para.m_h = g_iScreenHeight;
   para.m_wnd = hWnd;
   para.m_render_mode = SCREEN_RENDER_MULTI_THREAD;

   para.m_device_type = WIN_DEVICE;


   g_nui_instance.create(para );

   CreateView();
   g_hWnd = hWnd;

   g_nui_instance.getScreen()->SetRenderFlag(TRUE);
   ShowWindow( hWnd, nCmdShow );
   UpdateWindow( hWnd );

   return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch ( message )
	{
		//case WM_PAINT:
		//	hdc = BeginPaint(hWnd, &ps);
		//	EndPaint(hWnd, &ps);
		//	break;
	case WM_DESTROY:
		//�첽��Ⱦ��Ҫ�ڴ˽�����Ⱦ�߳�
		PostQuitMessage( 0 );
		break;
	case WM_KEYDOWN:


		break;
	default:
		break;
	}

	g_nui_instance.NUIWndProc( hWnd, message, wParam, lParam );

	return DefWindowProc( hWnd, message, wParam, lParam );
}

// �����ڡ������Ϣ��������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}