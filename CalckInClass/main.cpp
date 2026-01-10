#undef UNICODE
#include <Windows.h>
#include "resource.h"

#define g_i_BUTTON_SIZE				50
#define g_i_INTERVAL				2
#define g_i_DOUBLE_BUTTON_SIZE		(g_i_BUTTON_SIZE*2 + g_i_INTERVAL)

#define g_i_START_X					10
#define g_i_START_Y					10

#define g_i_DISPLAY_WIDTH			(g_i_BUTTON_SIZE*5 + g_i_INTERVAL * 4)
#define g_i_DISPLAY_HEIGTH			22

#define g_i_BUTTON_START_X			g_i_START_X
#define g_i_BUTTON_START_Y			(g_i_START_Y + g_i_DISPLAY_HEIGTH + g_i_INTERVAL)

#define g_i_BUTTON_X_POSITION(SHIFT)	g_i_BUTTON_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL) * (SHIFT)
#define g_i_BUTTON_Y_POSITION(SHIFT)	g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * (SHIFT)

CONST CHAR g_OPERATION[] = "+-*/";
 





CONST CHAR g_sz_WINDOW_CLASS[] = "Calc PV_522";

LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow) //точка запуска
{
	//регистрируем класс окна ......................................................................................

	WNDCLASSEX wClass;

	ZeroMemory(&wClass, sizeof(wClass)); //очистка памяти класса

	// 2. Стиль окна
	wClass.style = NULL;

	// 3. Устанавливаем размер структуры
	wClass.cbSize = sizeof(wClass);

	// 4. Дополнительная память для класса и окон
	wClass.cbClsExtra = 0;
	wClass.cbWndExtra = 0;

	// 5. Иконки, курсор, фон
	wClass.hIcon = LoadIcon(hInstance, IDI_APPLICATION); //тконка в верху окна
	wClass.hIconSm = LoadIcon(hInstance, IDI_APPLICATION); //маленькая иконка для панели задач
	wClass.hCursor = LoadCursor(hInstance, IDC_ARROW); 
	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW; //цвет окна 

	// 6. Дескриптор экземпляра приложения
	wClass.hInstance = hInstance;

	// 7. Имя класса (уникальный идентификатор)
	wClass.lpszClassName = g_sz_WINDOW_CLASS;

	// 8. Меню (если есть)
	wClass.lpszMenuName = NULL;

	// 9. Оконная процедура (обрботчик)
	wClass.lpfnWndProc = WndProc;

	//в скобках RegisterClassEx(&wClass) - команда регистрации класса
	if (!RegisterClassEx(&wClass)) //проверка прошла ли регистрация окна
	{
		MessageBox(NULL, "Class regestration failed", NULL, MB_OK | MB_ICONERROR);
		return 0;
	}




	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 2. Создание окна
	//

	HWND hwnd = CreateWindowEx
	(
		NULL, //exStyles
		g_sz_WINDOW_CLASS, //ClassName
		g_sz_WINDOW_CLASS, //Window title
		WS_OVERLAPPEDWINDOW, //Стиль окна. Набор стилей всегда зависит от класса окна. 
		//Стиль главного окна всегда WS_OVERLAPPEDWINDOW
		CW_USEDEFAULT, CW_USEDEFAULT, //начальная позиция окна при запуске
		CW_USEDEFAULT, CW_USEDEFAULT, //размер окна 
		NULL, //Parent window
		NULL, //hMenu - для главного окна этот параметр определяет главное меню.
		//Для дочернего окна (Control) - содержит ResorceID дочернего окна
		//Благодаря hMenu дочернее окно всегда можно найти при помощи функции GetDlgItem();
		hInstance, //Дескриптор экземпляра приложения
		NULL //lParam
	);

	if (hwnd == NULL) //проверка удалось ли создать окно
	{
		MessageBox(NULL, "Window creation failed", NULL, MB_OK | MB_ICONERROR);
		return 0;
	}


	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);








//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 3.Запуск цикла сообщений
//

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}



LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
	case WM_CREATE: 
	{
		HWND hEdit = CreateWindowEx(
			NULL, "Edit", "0", 
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
			g_i_START_X, g_i_START_Y,
			g_i_DISPLAY_WIDTH, g_i_DISPLAY_HEIGTH,
			hwnd, 
			(HMENU)IDC_DISPLAY,
			GetModuleHandle(NULL),  
			NULL
		);

		CHAR sz_digit[2] = {};

		for (int i = 6; i >= 0; i -= 3)
		{
			for (int j = 0; j < 3; j++)
			{
				sz_digit[0] = i + j + '1';

				CreateWindowEx(
					NULL, "BUTTON", sz_digit,
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					g_i_BUTTON_X_POSITION(j),
					g_i_BUTTON_Y_POSITION(2 - i / 3),
					//g_i_BUTTON_START_X +(g_i_BUTTON_SIZE + g_i_INTERVAL) * j, //x position
					//g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * (2-i/3), //y position
					g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
					hwnd,
					(HMENU)(ID_BUTTON_1 + i +j),
					GetModuleHandle(NULL),
					NULL
				);
			}
		}
	
		//кнопка нуля 
		CreateWindowEx(
			NULL, "BUTTON", "0",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			g_i_BUTTON_X_POSITION(0),
			g_i_BUTTON_Y_POSITION(3),
			g_i_DOUBLE_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)ID_BUTTON_0,
			GetModuleHandle(NULL),
			NULL
		);


		//кнопка точки........................................................................
		CreateWindowEx (
			NULL, "BUTTON", ".",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			g_i_BUTTON_X_POSITION(2),
			g_i_BUTTON_Y_POSITION(3),
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)ID_BUTTON_POINT,
			GetModuleHandle(NULL),
			NULL
		);



		//кнопка операций ..................................................................
		CHAR sz_operation[2] = {};
		for (int i = 0; i < 4; i++)
		{
			sz_operation[0] = g_OPERATION[3 - i];
			CreateWindowEx(
				NULL, "BUTTON", sz_operation,
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				g_i_BUTTON_X_POSITION(3),
				g_i_BUTTON_Y_POSITION(i),
				g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
				hwnd,
				(HMENU)(ID_BUTTON_SLASH - i),
				GetModuleHandle(NULL),
				NULL
			);
		}


		//кнопка BSP ..................................................................
		CreateWindowEx(
			NULL, "BUTTON", "<-",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			g_i_BUTTON_X_POSITION(4),
			g_i_BUTTON_Y_POSITION(0),
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)(ID_BUTTON_BSP),
			GetModuleHandle(NULL),
			NULL
		);



		//кнопка CLR ..................................................................
		CreateWindowEx(
			NULL, "BUTTON", "C",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			g_i_BUTTON_X_POSITION(4),
			g_i_BUTTON_Y_POSITION(1),
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)(ID_BUTTON_CLR),
			GetModuleHandle(NULL),
			NULL
		);

		//кнопка =  ..................................................................
		CreateWindowEx(
			NULL, "BUTTON", "=",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			g_i_BUTTON_X_POSITION(4),
			g_i_BUTTON_Y_POSITION(2),
			g_i_BUTTON_SIZE, g_i_DOUBLE_BUTTON_SIZE,
			hwnd,
			(HMENU)(ID_BUTTON_EQUAL),
			GetModuleHandle(NULL),
			NULL
		);

	
	
	
	
	}break;
	case WM_COMMAND: {} break;
	case WM_DESTROY: { PostQuitMessage(0); } break;
	case WM_CLOSE: { DestroyWindow(hwnd); } break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return FALSE;

}