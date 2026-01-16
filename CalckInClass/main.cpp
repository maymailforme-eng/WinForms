#define _CRT_SECURE_NO_WARNINGS
#undef UNICODE
#include <Windows.h>
#include<cstdio>
#include<iostream>
#include <commctrl.h>
#include "resource.h"

#define g_i_BUTTON_SIZE				50
#define g_i_INTERVAL				2
#define g_i_DOUBLE_BUTTON_SIZE		(g_i_BUTTON_SIZE*2 + g_i_INTERVAL)

#define g_i_BUTTON_IMAGE_SIZE				46
#define g_i_DOUBLE_BUTTON_IMAGE_SIZE		(g_i_BUTTON_SIZE*2 - 2)



#define g_i_START_X					10
#define g_i_START_Y					10

#define g_i_DISPLAY_WIDTH			(g_i_BUTTON_SIZE*5 + g_i_INTERVAL * 4)
#define g_i_DISPLAY_HEIGTH			22

#define g_i_BUTTON_START_X			g_i_START_X
#define g_i_BUTTON_START_Y			(g_i_START_Y + g_i_DISPLAY_HEIGTH + g_i_INTERVAL)

#define g_i_WINDOW_WIDTH			g_i_DISPLAY_WIDTH + g_i_START_X * 2 + 16
#define g_i_WINDOW_HEIGTH			g_i_DISPLAY_HEIGTH + g_i_START_Y * 2 + 4 * (g_i_BUTTON_SIZE + g_i_INTERVAL) + 38

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
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, //Стиль окна. Набор стилей всегда зависит от класса окна. 
		//Стиль главного окна всегда WS_OVERLAPPEDWINDOW
		CW_USEDEFAULT, CW_USEDEFAULT, //начальная позиция окна при запуске
		g_i_WINDOW_WIDTH, g_i_WINDOW_HEIGTH, //размер окна 
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
		AllocConsole();

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

		CHAR pathBegin[] = "C:\\Users\\mayma\\OneDrive\\Рабочий стол\\Академия ТОП\\01_Учеба\\07_WPF, WinForms\\Домашняя работа\\Кнопки\\";
		CHAR pathButtonPrefics[] = "Button_";
		CHAR pathEnd[] = ".bmp";
		CHAR path[256] = {};

		CHAR sz_digit[2] = {};

		for (int i = 6; i >= 0; i -= 3)
		{
			for (int j = 0; j < 3; j++)
			{
				memset(path, 0, sizeof(path)); //сброс строки

				sz_digit[0] = i + j + '1';

				
				INT NUM = i + j + 1;
				CHAR numChar = char(NUM + 48);
				sprintf(path, "%s%s%c%s", pathBegin, pathButtonPrefics, numChar, pathEnd);

				HBITMAP hBitmap = (HBITMAP)LoadImage(
					NULL,						// HINSTANCE (NULL для файла)
					path,						// путь к файлу
					IMAGE_BITMAP,				// тип изображения
					g_i_BUTTON_IMAGE_SIZE, g_i_BUTTON_IMAGE_SIZE,						// размер (0 = оригинальный)
					LR_LOADFROMFILE | LR_CREATEDIBSECTION         // флаги
				);


				HWND hButton = CreateWindowEx(
					NULL, "BUTTON", "",
					WS_CHILD | WS_VISIBLE | BS_BITMAP,
					g_i_BUTTON_X_POSITION(j),
					g_i_BUTTON_Y_POSITION(2 - i / 3),
					g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
					hwnd,
					(HMENU)(IDC_BUTTON_1 + i + j),
					GetModuleHandle(NULL),
					NULL);


				SendMessage(hButton, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);



			}
		}






		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//кнопка нуля............................................................................................
		//memset(path, 0, sizeof(path)); //сброс строки
		sprintf(path, "%s%s%s", pathBegin, "Button_0", pathEnd);

		HBITMAP hBitmap = (HBITMAP)LoadImage(
			NULL,						// HINSTANCE (NULL для файла)
			path,						// путь к файлу
			IMAGE_BITMAP,				// тип изображения
			g_i_DOUBLE_BUTTON_IMAGE_SIZE, g_i_BUTTON_IMAGE_SIZE,						// размер (0 = оригинальный)
			LR_LOADFROMFILE | LR_CREATEDIBSECTION         // флаги
		);


		HWND hButton_0 = CreateWindowEx(
			NULL, "BUTTON", "",
			WS_CHILD | WS_VISIBLE | BS_BITMAP,
			g_i_BUTTON_X_POSITION(0),
			g_i_BUTTON_Y_POSITION(3),
			g_i_DOUBLE_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_0,
			GetModuleHandle(NULL),
			NULL
		);

		SendMessage(hButton_0, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//кнопка точки........................................................................
		CreateWindowEx(
			NULL, "BUTTON", ".",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			g_i_BUTTON_X_POSITION(2),
			g_i_BUTTON_Y_POSITION(3),
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_POINT,
			GetModuleHandle(NULL),
			NULL
		);


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//кнопка операций ..................................................................
		CHAR sz_operation[2] = {};
		CONST CHAR* name[] = {"Div", "Multy", "Minus", "Plus", };

		for (int i = 0; i < 4; i++)
		{
			sprintf(path, "%s%s%s", pathBegin, name[i], pathEnd);

			HBITMAP hBitmap = (HBITMAP)LoadImage(
				NULL,						// HINSTANCE (NULL для файла)
				path,						// путь к файлу
				IMAGE_BITMAP,				// тип изображения
				g_i_BUTTON_IMAGE_SIZE, g_i_BUTTON_IMAGE_SIZE,						// размер (0 = оригинальный)
				LR_LOADFROMFILE | LR_CREATEDIBSECTION         // флаги
			);




			sz_operation[0] = g_OPERATION[3 - i];
			HWND hButton = CreateWindowEx(
				NULL, "BUTTON", "",
				WS_CHILD | WS_VISIBLE | BS_BITMAP,
				g_i_BUTTON_X_POSITION(3),
				g_i_BUTTON_Y_POSITION(i),
				g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
				hwnd,
				(HMENU)(IDC_BUTTON_SLASH - i),
				GetModuleHandle(NULL),
				NULL
			);

			SendMessage(hButton, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);
		}


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//кнопка BSP ..................................................................

		sprintf(path, "%s%s%s", pathBegin, "Back", pathEnd);

		 hBitmap = (HBITMAP)LoadImage(
			NULL,						// HINSTANCE (NULL для файла)
			path,						// путь к файлу
			IMAGE_BITMAP,				// тип изображения
			g_i_BUTTON_IMAGE_SIZE, g_i_BUTTON_IMAGE_SIZE,						// размер (0 = оригинальный)
			LR_LOADFROMFILE | LR_CREATEDIBSECTION         // флаги
		);



		 HWND hButtonBack = CreateWindowEx(
			NULL, "BUTTON", "",
			WS_CHILD | WS_VISIBLE | BS_BITMAP,
			g_i_BUTTON_X_POSITION(4),
			g_i_BUTTON_Y_POSITION(0),
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)(IDC_BUTTON_BSP),
			GetModuleHandle(NULL),
			NULL
		);

		SendMessage(hButtonBack, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//кнопка CLR ..................................................................


		CreateWindowEx(
			NULL, "BUTTON", "C",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			g_i_BUTTON_X_POSITION(4),
			g_i_BUTTON_Y_POSITION(1),
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)(IDC_BUTTON_CLR),
			GetModuleHandle(NULL),
			NULL
		);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//кнопка =  ..................................................................

		sprintf(path, "%s%s%s", pathBegin, "Equals", pathEnd);

		hBitmap = (HBITMAP)LoadImage(
			NULL,						// HINSTANCE (NULL для файла)
			path,						// путь к файлу
			IMAGE_BITMAP,				// тип изображения
			g_i_BUTTON_IMAGE_SIZE, g_i_DOUBLE_BUTTON_IMAGE_SIZE,						// размер (0 = оригинальный)
			LR_LOADFROMFILE | LR_CREATEDIBSECTION         // флаги
		);

		HWND hButtonEquals = CreateWindowEx(
			NULL, "BUTTON", "=",
			WS_CHILD | WS_VISIBLE | BS_BITMAP,
			g_i_BUTTON_X_POSITION(4),
			g_i_BUTTON_Y_POSITION(2),
			g_i_BUTTON_SIZE, g_i_DOUBLE_BUTTON_SIZE,
			hwnd,
			(HMENU)(IDC_BUTTON_EQUAL),
			GetModuleHandle(NULL),
			NULL
		);

		SendMessage(hButtonEquals, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);



	}break;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///ОБРАБОТКА СООБЩЕНИЙ WM_COMMAND
	///
	case WM_COMMAND:
	{
		static DOUBLE	a = DBL_MIN, b = DBL_MIN;	
		static INT		operation = 0;
		static BOOL		input = FALSE;	
		static BOOL		input_operation = FALSE;	

		CHAR sz_digit[2] = {};
		CHAR sz_display[MAX_PATH] = {};
		HWND hEditDisplay = GetDlgItem(hwnd, IDC_DISPLAY);
		SendMessage(hEditDisplay, WM_GETTEXT, MAX_PATH, (LPARAM)sz_display);
		if (LOWORD(wParam) >= IDC_BUTTON_0 && LOWORD(wParam) <= IDC_BUTTON_9)
		{
			input_operation = FALSE;
			if (input == FALSE)ZeroMemory(sz_display, sizeof(sz_display));
			sz_digit[0] = LOWORD(wParam) - IDC_BUTTON_0 + '0';
			if (sz_display[0] == '0' && sz_display[1] != '.')
				strcpy(sz_display, sz_digit);
			else
				strcat(sz_display, sz_digit);
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
			input = TRUE;
			break;
		}
		if (LOWORD(wParam) == IDC_BUTTON_POINT)
		{
			input_operation = FALSE;
			if (strchr(sz_display, '.'))break;	
			strcat(sz_display, ".");
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
			input = TRUE;
			break;
		}
		if (LOWORD(wParam) == IDC_BUTTON_BSP)
		{
			sz_display[strlen(sz_display) - 1] = 0;	
			if (sz_display[0] == 0)sz_display[0] = '0';
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
		}
		if (LOWORD(wParam) == IDC_BUTTON_CLR)
		{
			a = DBL_MIN, b = DBL_MIN;	
			operation = 0;
			input = FALSE;
			input_operation = FALSE;
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)"0");
		}
		if (LOWORD(wParam) >= IDC_BUTTON_PLUS && LOWORD(wParam) <= IDC_BUTTON_SLASH)
		{
			if (input)
			{
				(a == DBL_MIN ? a : b) = atof(sz_display);
				input = false;
			}
			operation = LOWORD(wParam);
			input_operation = TRUE;
		}
		if (LOWORD(wParam) == IDC_BUTTON_EQUAL)
		{
			if (input)
			{
				(a == DBL_MIN ? a : b) = atof(sz_display);
				input = FALSE;
			}
			switch (operation)
			{
			case IDC_BUTTON_PLUS:	a += b;		break;
			case IDC_BUTTON_MINUS:	a -= b;		break;
			case IDC_BUTTON_ASTER:	a *= b;		break;
			case IDC_BUTTON_SLASH:	a /= b;		break;
			}
			input_operation = FALSE;
			if (a != DBL_MIN)
			{
				sprintf(sz_display, "%g", a);
				SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
			}
		}

		SetFocus(hwnd);
	}
	break;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Нажатие на кнопки клавиатуры
	///
	case WM_KEYDOWN:
	{
		CHAR sz_key[8] = {};
		sprintf(sz_key, "%i", wParam);

		if (GetKeyState(VK_SHIFT) < 0 && wParam == '8')
		{
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_ASTER), BM_SETSTATE, TRUE, 0);
		}
		else if (GetKeyState(VK_SHIFT) < 0 && wParam == VK_OEM_PLUS)
		{
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_PLUS), BM_SETSTATE, TRUE, 0);
		}
		else if (wParam == VK_OEM_PLUS || wParam == VK_RETURN)
		{
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_EQUAL), BM_SETSTATE, TRUE, 0);
		}
		else if (wParam >= '0' && wParam <= '9')
		{
			SendMessage(GetDlgItem(hwnd, wParam - '0' + IDC_BUTTON_0), BM_SETSTATE, TRUE, NULL);
		}
		else if (wParam >= VK_NUMPAD0 && wParam <= VK_NUMPAD9)
		{
			SendMessage(GetDlgItem(hwnd, wParam - VK_NUMPAD0 + IDC_BUTTON_0), BM_SETSTATE, TRUE, NULL);
		}
		switch (wParam)
		{
		case VK_ADD:		SendMessage(GetDlgItem(hwnd, IDC_BUTTON_PLUS), BM_SETSTATE, TRUE, 0); break;
		case VK_OEM_MINUS:
		case VK_SUBTRACT:	SendMessage(GetDlgItem(hwnd, IDC_BUTTON_MINUS), BM_SETSTATE, TRUE, 0); break;
		case VK_MULTIPLY:	SendMessage(GetDlgItem(hwnd, IDC_BUTTON_ASTER), BM_SETSTATE, TRUE, 0); break;
		case VK_OEM_2:
		case VK_DIVIDE:		SendMessage(GetDlgItem(hwnd, IDC_BUTTON_SLASH), BM_SETSTATE, TRUE, 0); break;
		case VK_OEM_PERIOD:
		case VK_DECIMAL:	SendMessage(GetDlgItem(hwnd, IDC_BUTTON_POINT), BM_SETSTATE, TRUE, 0); break;

		case VK_BACK:		SendMessage(GetDlgItem(hwnd, IDC_BUTTON_BSP), BM_SETSTATE, TRUE, 0); break;
		case VK_ESCAPE:		SendMessage(GetDlgItem(hwnd, IDC_BUTTON_CLR), BM_SETSTATE, TRUE, 0); break;
		}
	}break;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///Отжатие кнопок клавиатуры
	///
	case WM_KEYUP:
	{
		//https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-keydown
		if (GetKeyState(VK_SHIFT) < 0 && wParam == '8')
		{
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_ASTER), BM_SETSTATE, FALSE, 0);
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_ASTER), 0);
		}
		else if (GetKeyState(VK_SHIFT) < 0 && wParam == VK_OEM_PLUS)
		{
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_PLUS), BM_SETSTATE, FALSE, 0);
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_PLUS), 0);
		}
		else if (wParam == VK_OEM_PLUS || wParam == VK_RETURN)
		{
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_EQUAL), BM_SETSTATE, FALSE, 0);
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_EQUAL), 0);
		}
		else if (wParam >= '0' && wParam <= '9')
		{
			SendMessage(GetDlgItem(hwnd, wParam - '0' + IDC_BUTTON_0), BM_SETSTATE, FALSE, NULL);
			SendMessage(hwnd, WM_COMMAND, LOWORD(wParam - '0' + IDC_BUTTON_0), 0);
		}
		else if (wParam >= VK_NUMPAD0 && wParam <= VK_NUMPAD9)
		{
			SendMessage(GetDlgItem(hwnd, wParam - VK_NUMPAD0 + IDC_BUTTON_0), BM_SETSTATE, FALSE, NULL);
			SendMessage(hwnd, WM_COMMAND, LOWORD(wParam - VK_NUMPAD0 + IDC_BUTTON_0), 0);
		}
	LABEL:
		switch (wParam)
		{
			//case VK_OEM_PLUS:
		case VK_ADD:
			//int a = 2;
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_PLUS), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_PLUS), BM_SETSTATE, FALSE, 0);	break;
		case VK_OEM_MINUS:
		case VK_SUBTRACT:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_MINUS), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_MINUS), BM_SETSTATE, FALSE, 0); break;
		case VK_MULTIPLY:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_ASTER), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_ASTER), BM_SETSTATE, FALSE, 0); break;
		case VK_OEM_2:
		case VK_DIVIDE:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_SLASH), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_SLASH), BM_SETSTATE, FALSE, 0); break;
		case VK_OEM_PERIOD:
		case VK_DECIMAL:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_POINT), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_POINT), BM_SETSTATE, FALSE, 0); break;

		case VK_BACK:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_BSP), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_BSP), BM_SETSTATE, FALSE, 0); break;
		case VK_ESCAPE:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_CLR), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_CLR), BM_SETSTATE, FALSE, 0); break;
		}

	} break;


	case WM_DESTROY:
	{
		FreeConsole();
		PostQuitMessage(0);
	} break;
	case WM_CLOSE: { DestroyWindow(hwnd); } break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return FALSE;

}