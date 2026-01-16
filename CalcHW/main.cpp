#undef UNICODE
#include <Windows.h>
#include <stdio.h>
#include <cstdlib> 
#include <cstdio>
#include <cmath>
#include "resource.h"


//именнованные константы препроцессора
#define ID_BUTTON_ZIRO 1010
#define ID_BUTTON_ONE 1011
#define ID_BUTTON_TWO 1012
#define ID_BUTTON_THREE 1013
#define ID_BUTTON_FOUR 1014
#define ID_BUTTON_FIVE 1015
#define ID_BUTTON_SIX 1016
#define ID_BUTTON_SEVEN 1017
#define ID_BUTTON_EITH 1018
#define ID_BUTTON_NINE 1019


#define ID_BUTTON_DOT 1020
#define IDC_BUTTON_PLUS 1021
#define IDC_BUTTON_MINUS 1022
#define ID_BUTTON_ENTER 1023
#define ID_BUTTON_MULTI 1024
#define ID_BUTTON_DIV 1025
#define ID_BUTTON_CLEAR 1026

#define ID_EDIT_CONTROL 1030
#define ID_STATIC_FILL 1031
#define ID_STATIC_FILL_BUFFER 1032



//размеры главного окна 
#define MAIN_WINDOW_BASE_WIDTH 303
#define MAIN_WINDOW_BASE_HEIGTH 375
#define MAIN_WINDOW_MIN_WIDTH 200
#define MAIN_WINDOW_MIN_HEIGTH 300
#define MAIN_WINDOW_MAX_WIDTH 375
#define MAIN_WINDOW_MAX_HEIGTH 420

//размеры элементов
#define BUTTON_UPPER_SHIFT 100
#define BUTTON_BASE_WIDTH 70
#define BUTTON_BASE_HEIGTH 50
#define BUTTON_MARGIN 5

#define STATIC_MARGIN 5
#define STATIC_BASE_WIDTH (MAIN_WINDOW_BASE_WIDTH - 2 * STATIC_MARGIN)
#define STATIC_BASE_HIEGTH 25



//константы
CONST INT QNT_BUTTON = 19;


struct ButtonMaker {
	INT id;
	LPCSTR typeControl;
	LPCSTR text;
	INT x, y;
	INT width, height;
};


CONST INT M = BUTTON_MARGIN;
CONST INT S = BUTTON_UPPER_SHIFT;
CONST INT W = BUTTON_BASE_WIDTH;
CONST INT H = BUTTON_BASE_HEIGTH;


CONST INT sM = STATIC_MARGIN;
CONST INT sW = STATIC_BASE_WIDTH;
CONST INT sH = STATIC_BASE_HIEGTH;


CONST ButtonMaker buttonList[QNT_BUTTON] = {

	//Line 0

			{(INT)ID_STATIC_FILL,			"STATIC", "", sM, sM, sW, sH},
			{(INT)ID_STATIC_FILL_BUFFER,	"STATIC", "", sM, (sH + 2 * sM ), sW / 4, sH},

	//Line 1
			{(INT)ID_BUTTON_CLEAR,	"BUTTON", "C", M, S, W, H},
			{(INT)ID_BUTTON_DIV,	"BUTTON", "/", (W + 2 * M), S, W, H},
			{(INT)ID_BUTTON_MULTI,	"BUTTON", "*", (2 * W + 3 * M), S, W, H},
			{(INT)IDC_BUTTON_MINUS,	"BUTTON", "-", (3 * W + 4 * M), S, W, H},

	//Line 2...................................................................

			{(INT)ID_BUTTON_SEVEN,	"BUTTON", "7", M, (S + H + M), W, H},
			{(INT)ID_BUTTON_EITH,	"BUTTON", "8", (W + 2 * M), (S + H + M), W, H},
			{(INT)ID_BUTTON_NINE,	"BUTTON", "9", (2 * W + 3 * M), (S + H + M), W, H},
			{(INT)IDC_BUTTON_PLUS,	"BUTTON", "+", (3 * W + 4 * M), (S + H + M), W, (2 * H + M)}, //двойная по высоте

	//Line 3..............................................................

			{(INT)ID_BUTTON_FOUR,	"BUTTON", "4", M, (S + 2 * H + 2 * M), W, H},
			{(INT)ID_BUTTON_FIVE,	"BUTTON", "5", (W + 2 * M), (S + 2 * H + 2 * M), W, H},
			{(INT)ID_BUTTON_SIX,	"BUTTON", "6", (2 * W + 3 * M), (S + 2 * H + 2 * M), W, H},


	//Line 4............................................................................

			{(INT)ID_BUTTON_ONE,	"BUTTON", "1", M, (S + 3 * H + 3 * M), W, H},
			{(INT)ID_BUTTON_TWO,	"BUTTON", "2", (W + 2 * M), (S + 3 * H + 3 * M), W, H},
			{(INT)ID_BUTTON_THREE,	"BUTTON", "3", (2 * W + 3 * M), (S + 3 * H + 3 * M), W, H},
			{(INT)ID_BUTTON_ENTER,	"BUTTON", "=", (3 * W + 4 * M), (S + 3 * H + 3 * M), W, (2 * H + M)}, //двойная по высоте


	//Line 5.............................................................................

			{(INT)ID_BUTTON_ZIRO,	"BUTTON", "0", M, (S + 4 * H + 4 * M), (2 * W + M), H}, //двойная в ширину
			{(INT)ID_BUTTON_DOT,	"BUTTON", ".", (2 * W + 3 * M), (S + 4 * H + 4 * M), W, H}

};





CONST CHAR g_sz_CLASS_NAME[] = "CalcHW"; //имя класса окна

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

VOID ButtonNUM(HWND hwndFillRecipient, INT num);
VOID ButtonOperation(HWND hwndStaticFill, HWND hwndStaticFillBuffer, CONST CHAR* signPrev);
VOID ButtonEnter(HWND hwndStaticFill, HWND hwndStaticFillBuffer);

FLOAT sstrToFloat(CHAR* sstr);
FLOAT operation(FLOAT num1, FLOAT num2, CHAR sign);

BOOL haveSign(CHAR sign);


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow) //точка запуска
{

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 1. Регистрация класса окна
	//

	WNDCLASSEX wClass; // объявили класс
	ZeroMemory(&wClass, sizeof(wClass)); //очистка памяти класса



	// 1. Устанавливаем размер структуры
	wClass.cbSize = sizeof(wClass); // cb - количестов байт (Размер экземпляра класса)

	// 2. Стиль окна
	wClass.style = NULL; //стиль окна

	// 3. Оконная процедура (обрботчик)
	wClass.lpfnWndProc = WndProc;

	// 4. Дополнительная память для класса и окон
	wClass.cbClsExtra = 0;
	wClass.cbWndExtra = 0;

	// 5. Дескриптор экземпляра приложения
	wClass.hInstance = hInstance;

	// 6. Иконки, курсор, фон
	wClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wClass.hbrBackground = CreateSolidBrush(RGB(128, 128, 128));

	// 7. Меню (если есть)
	wClass.lpszMenuName = NULL;

	// 8. Имя класса (уникальный идентификатор)
	wClass.lpszClassName = g_sz_CLASS_NAME;


	// 9. Иконка маленькая (для панели задач)
	wClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);


	if (!RegisterClassEx(&wClass)) //проверка прошла ли регистрация окна
	{
		MessageBox(NULL, "Class regestration failed", NULL, MB_OK | MB_ICONERROR);
		return 0;
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 2. Создание окна
	//


	// Получаем размеры экрана
	INT screenWidth = GetSystemMetrics(SM_CXSCREEN);
	INT screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Вычисляем позицию для центрирования
	int x = (screenWidth - MAIN_WINDOW_BASE_WIDTH) / 2;
	int y = (screenHeight - MAIN_WINDOW_BASE_HEIGTH) / 2;


	HWND hwnd = CreateWindowEx
	(
		NULL, //exStyles
		g_sz_CLASS_NAME, //ClassName
		"Калькулятор", //Window title
		WS_OVERLAPPEDWINDOW, //Стиль окна. Набор стилей всегда зависит от класса окна. 
		//Стиль главного окна всегда WS_OVERLAPPEDWINDOW
		x, y, //начальная позиция окна при запуске
		MAIN_WINDOW_BASE_WIDTH, MAIN_WINDOW_BASE_HEIGTH, //размер окна 
		NULL, //Parent window
		NULL, //hMenu - для главного окна этот параметр определяет главное меню.
		//Для дочернего окна (Control) - содержит ResorceID дочернего окна
		//Благодаря hMenu дочернее окно всегда можно найти при помощи функции GetDlgItem();
		hInstance, //Дескриптор экземпляра приложения
		NULL
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


	return 0;
}






LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static CONST INT SIZE = 256;



	switch (uMsg) {
	case WM_CREATE:
	{

		for (const auto& control : buttonList) //создание кнопок
		{
			if (strcmp(control.typeControl, "STATIC"))
			{
				CreateWindowEx(
					NULL, //Расширенные стили окна
					control.typeControl, //Класс окна (предопределенный или зарегистрированный)
					control.text,//Текст/заголовок окна
					WS_CHILD | WS_VISIBLE,// Основные стили окна
					control.x, control.y,// Позиция X, Позиция Y
					control.width, control.height,//Ширина, высота
					hwnd, // Родительское окно
					(HMENU)control.id, // Идентификатор или меню - просто ID
					GetModuleHandle(NULL),  // Дескриптор экземпляра приложения
					NULL);  // Дополнительные параметры создания
			}

			if (strcmp(control.typeControl, "BUTTON"))
			{

				CreateWindowEx(
					NULL, //Расширенные стили окна
					control.typeControl, //Класс окна (предопределенный или зарегистрированный)
					control.text,//Текст/заголовок окна
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,// Основные стили окна
					control.x, control.y,// Позиция X, Позиция Y
					control.width, control.height,//Ширина, высота
					hwnd, // Родительское окно
					(HMENU)control.id, // Идентификатор или меню - просто ID
					GetModuleHandle(NULL),  // Дескриптор экземпляра приложения
					NULL);  // Дополнительные параметры создания

			}

		}



	}break;
	case WM_COMMAND:
	{
		CHAR currentText[SIZE] = {};
		CHAR setText[SIZE] = {};

		UINT id = LOWORD(wParam);
		UINT codeNotification = HIWORD(wParam);
		HWND control = (HWND)lParam;
		HWND staticFill = GetDlgItem(hwnd, ID_STATIC_FILL);
		HWND staticFillBuffer = GetDlgItem(hwnd, ID_STATIC_FILL_BUFFER);

		switch (id)
		{
			//NUM BUTTON ................................................................................
			case ID_BUTTON_ZIRO:		ButtonNUM(staticFillBuffer, 0); break;
			case ID_BUTTON_ONE:			ButtonNUM(staticFillBuffer, 1); break;
			case ID_BUTTON_TWO:			ButtonNUM(staticFillBuffer, 2); break;
			case ID_BUTTON_THREE:		ButtonNUM(staticFillBuffer, 3); break;
			case ID_BUTTON_FOUR:		ButtonNUM(staticFillBuffer, 4); break;
			case ID_BUTTON_FIVE:		ButtonNUM(staticFillBuffer, 5); break;
			case ID_BUTTON_SIX:			ButtonNUM(staticFillBuffer, 6); break;
			case ID_BUTTON_SEVEN:		ButtonNUM(staticFillBuffer, 7); break;
			case ID_BUTTON_EITH:		ButtonNUM(staticFillBuffer, 8); break;
			case ID_BUTTON_NINE:		ButtonNUM(staticFillBuffer, 9); break;

			//OPERATION BUTTON........................................................................
			case IDC_BUTTON_PLUS:		ButtonOperation(staticFill, staticFillBuffer, "+"); break;
			case IDC_BUTTON_MINUS:		ButtonOperation(staticFill, staticFillBuffer, "-"); break;
			case ID_BUTTON_MULTI:		ButtonOperation(staticFill, staticFillBuffer, "*"); break;
			case ID_BUTTON_DIV:			ButtonOperation(staticFill, staticFillBuffer, "/"); break;

			case ID_BUTTON_ENTER:		ButtonEnter(staticFill, staticFillBuffer); break;

			case ID_BUTTON_DOT:
			{
				CHAR currentTextBuffer[SIZE] = {};
				SendMessage(staticFillBuffer, WM_GETTEXT, SIZE, (LPARAM)currentTextBuffer);

				char* result = strchr(currentTextBuffer, '.');
				if (result != nullptr) break; //если уже есть одна точка

				if (currentTextBuffer[0] != '\0')
				{
					sprintf_s(setText, SIZE, "%s%c", currentTextBuffer, '.');
					SendMessage(staticFillBuffer, WM_SETTEXT, 0, (LPARAM)setText);
				}

			} break;

			case ID_BUTTON_CLEAR:
			{
				SendMessage(staticFill, WM_SETTEXT, 0, (LPARAM)"");
				SendMessage(staticFillBuffer, WM_SETTEXT, 0, (LPARAM)"");
			} break;
		}


	}break;


	case WM_GETMINMAXINFO: //отправляет при  попытке изенения размера окна (установлен выбор растяжки рамки)
	{
		MINMAXINFO* pMMI = (MINMAXINFO*)lParam;
		// Устанавливаем минимальные размеры окна
		pMMI->ptMinTrackSize.x = MAIN_WINDOW_MIN_WIDTH;  // Минимальная ширина окна (включая рамки)
		pMMI->ptMinTrackSize.y = MAIN_WINDOW_MIN_HEIGTH;  // Минимальная высота окна (включая рамки)

		// 2. Максимальные размеры окна (окно нельзя растянуть больше)
		pMMI->ptMaxTrackSize.x = MAIN_WINDOW_MAX_WIDTH;  // Максимальная ширина главного окна
		pMMI->ptMaxTrackSize.y = MAIN_WINDOW_MAX_HEIGTH;  // Максимальная высота главного окна
	}
	break;

	case WM_SIZE:
	{
		int cutrrentMainWindowWidth = LOWORD(lParam); //ширина родительского окна
		int cutrrentMainWindowHeight = HIWORD(lParam); //высота родительского окна

		float scaleX = (float)cutrrentMainWindowWidth / MAIN_WINDOW_BASE_WIDTH;
		float scaleY = (float)cutrrentMainWindowHeight / MAIN_WINDOW_BASE_HEIGTH;

		//масиив HWND кнопок в порядке объявления с buttonList
		HWND buttonHWND_LIST[QNT_BUTTON] = { GetDlgItem(hwnd, ID_STATIC_FILL),
											 GetDlgItem(hwnd, ID_STATIC_FILL_BUFFER),
											 GetDlgItem(hwnd, ID_BUTTON_CLEAR), 
											 GetDlgItem(hwnd, ID_BUTTON_DIV),
											 GetDlgItem(hwnd, ID_BUTTON_MULTI),
											 GetDlgItem(hwnd, IDC_BUTTON_MINUS),
											 GetDlgItem(hwnd, ID_BUTTON_SEVEN),
											 GetDlgItem(hwnd, ID_BUTTON_EITH),
											 GetDlgItem(hwnd, ID_BUTTON_NINE),
											 GetDlgItem(hwnd, IDC_BUTTON_PLUS),
											 GetDlgItem(hwnd, ID_BUTTON_FOUR),
											 GetDlgItem(hwnd, ID_BUTTON_FIVE),
											 GetDlgItem(hwnd, ID_BUTTON_SIX),
											 GetDlgItem(hwnd, ID_BUTTON_ONE),
											 GetDlgItem(hwnd, ID_BUTTON_TWO),
											 GetDlgItem(hwnd, ID_BUTTON_THREE),
											 GetDlgItem(hwnd, ID_BUTTON_ENTER),
											 GetDlgItem(hwnd, ID_BUTTON_ZIRO),
											 GetDlgItem(hwnd, ID_BUTTON_DOT)
		};



		for (int i = 0; i < QNT_BUTTON; ++i)
		{

			if (buttonHWND_LIST[i] != NULL) 
			{
				int newX = buttonList[i].x * scaleX;
				int newY = buttonList[i].y * scaleY;
				int newWidth = buttonList[i].width * scaleX;
				int newHeight = buttonList[i].height * scaleY;

				MoveWindow(buttonHWND_LIST[i], newX, newY,newWidth, newHeight,TRUE); //устанавливаем новое положение и размеры элементов
			}
		}

	}
	break;

	case WM_DESTROY:
	{ PostQuitMessage(0); }
	break;

	case WM_CLOSE:
	{ DestroyWindow(hwnd); }
	break;

	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return FALSE;

}



VOID ButtonNUM(HWND hwndFillRecipient, INT num) //добавит в буферное окно число, если первое число не нуль
{
	CONST INT SIZE = 256;
	CHAR currentText[SIZE] = {};
	CHAR setText[SIZE] = {};

	SendMessage(hwndFillRecipient, WM_GETTEXT, SIZE, (LPARAM)currentText);

	if (currentText[0] != '0') //если в поле окна получателя есть цифра отличная от нуля протсо дописывам цифру
	{
		sprintf_s(setText, SIZE, "%s%d", currentText, num);
	}
	else 
	
	{ 
		char* result = strchr(currentText, '.');
		if (result != nullptr)
		{
			sprintf_s(setText, SIZE, "%s%d", currentText, num);
		}
		else
		{
			sprintf_s(setText, SIZE, "%d", num);
		}
	}

	SendMessage(hwndFillRecipient, WM_SETTEXT, 0, (LPARAM)setText);
}


VOID ButtonOperation(HWND hwndStaticFill, HWND hwndStaticFillBuffer, CONST CHAR* signNext)
{
	CONST INT SIZE = 256;
	CHAR currentTextStaticFill[SIZE] = {};
	CHAR currentTextBuffer[SIZE] = {};
	CHAR setText[SIZE] = {};

	SendMessage(hwndStaticFill, WM_GETTEXT, SIZE, (LPARAM)currentTextStaticFill);
	SendMessage(hwndStaticFillBuffer, WM_GETTEXT, SIZE, (LPARAM)currentTextBuffer);



	//если в статическом окне уже есть операнд
	if (currentTextStaticFill[0] != '\0')
	{
		FLOAT numStaticFill = sstrToFloat(currentTextStaticFill); //считываем число из статического поля

		//получаем знак операции 
		SIZE_T length = strlen(currentTextStaticFill);
		CHAR sign = currentTextStaticFill[length - 1];


		if (currentTextBuffer[0] != '\0') //буфер не пуст.......................................................
		{
			FLOAT numStaticFillBuffer = sstrToFloat(currentTextBuffer);

			//проверяем есть ли уже знак в статическом поле

			if (haveSign(sign) && length >=3)
			{

				if (numStaticFillBuffer == 0 && sign == '/') //проверка деления на нуль
				{
					SendMessage(hwndStaticFillBuffer, WM_SETTEXT, 0, (LPARAM)""); //очищаем поле буфера
					return;
				}

				FLOAT result = operation(numStaticFill, numStaticFillBuffer, sign); //результат операции
				float rounded = round(result);

				if (fabs(result - rounded) < 1e-9f) sprintf_s(setText, SIZE, "%d %s", (INT)result, signNext);
				else { sprintf_s(setText, SIZE, "%f %s", result, signNext); }


				SendMessage(hwndStaticFill, WM_SETTEXT, 0, (LPARAM)setText);
				SendMessage(hwndStaticFillBuffer, WM_SETTEXT, 0, (LPARAM)"");
			}
			else //в основном поле отсутствует знак
			{
				if (currentTextBuffer[0] != '0' || strchr(currentTextBuffer, '.') != nullptr)
				{
					sprintf_s(setText, SIZE, "%s %s", currentTextBuffer, signNext);
					SendMessage(hwndStaticFill, WM_SETTEXT, 0, (LPARAM)setText);
				}
				SendMessage(hwndStaticFillBuffer, WM_SETTEXT, 0, (LPARAM)"");

			}


		}
		else //буффер пуст (после =) записываем знак в поле........................................................
		{

			INT length = strlen(currentTextStaticFill);
			//проверяем есть ли уже знак в статическом поле
			if (haveSign(sign) && length >=3)
			{
				//меняем знак если уже был записан 

				currentTextStaticFill[length - 1] = signNext[0];
				SendMessage(hwndStaticFill, WM_SETTEXT, 0, (LPARAM)currentTextStaticFill);

			}
			else 
			{
				sprintf_s(setText, SIZE, "%s %s", currentTextStaticFill, signNext);
				SendMessage(hwndStaticFill, WM_SETTEXT, 0, (LPARAM)setText);
			}
		}
	}
	//если в статическом окне еще нет операндов
	else
	{
		//В буфере есть данные отличные от нуля 
		if (currentTextBuffer[0] != '\0')
		{
			if (currentTextBuffer[0] != '0' || strchr(currentTextBuffer, '.') != nullptr) 
			{
				sprintf_s(setText, SIZE, "%s %s", currentTextBuffer, signNext);
				SendMessage(hwndStaticFill, WM_SETTEXT, 0, (LPARAM)setText);
			}
		}

		SendMessage(hwndStaticFillBuffer, WM_SETTEXT, 0, (LPARAM)"");
	}
}



VOID ButtonEnter(HWND hwndStaticFill, HWND hwndStaticFillBuffer)
{

	CONST INT SIZE = 256;
	CHAR currentTextStaticFill[SIZE] = {};
	CHAR currentTextBuffer[SIZE] = {};
	CHAR setText[SIZE] = {};

	SendMessage(hwndStaticFill, WM_GETTEXT, SIZE, (LPARAM)currentTextStaticFill);
	SendMessage(hwndStaticFillBuffer, WM_GETTEXT, SIZE, (LPARAM)currentTextBuffer);

	if (currentTextBuffer[0] == '\0') return; //если буфер пуст


	if (currentTextStaticFill[0] == '\0') //если основное поле пустое, записываем в него данные из буфера
	{
		SendMessage(hwndStaticFill, WM_SETTEXT, 0, (LPARAM)currentTextBuffer);
		SendMessage(hwndStaticFillBuffer, WM_SETTEXT, 0, (LPARAM)"");
	}
	else //если основное поле имеет данные............................................................................
	{
		FLOAT numStaticFill = sstrToFloat(currentTextStaticFill); //считываем число из основного
		FLOAT numStaticFillBuffer = sstrToFloat(currentTextBuffer); //считываем число из буферного поля

		//получаем знак операции 
		SIZE_T length = strlen(currentTextStaticFill);
		CHAR sign = currentTextStaticFill[length - 1];


		if (numStaticFillBuffer == 0 && sign == '/') //защита от деления на 0
		{
			SendMessage(hwndStaticFillBuffer, WM_SETTEXT, 0, (LPARAM)"");
			return;
		}


		//проверяем содержит ли основное поле знак
		if (haveSign(sign) && length >= 3)
		{

			FLOAT result = operation(numStaticFill, numStaticFillBuffer, sign); //результат операции
			float rounded = round(result); 

			if (fabs(result - rounded) < 1e-9f) sprintf_s(setText, SIZE, "%d", (INT)result);
			else { sprintf_s(setText, SIZE, "%f", result); }


			SendMessage(hwndStaticFill, WM_SETTEXT, 0, (LPARAM)setText);
			SendMessage(hwndStaticFillBuffer, WM_SETTEXT, 0, (LPARAM)"");
		}
		else //основное поле не содержит знака
		{
			SendMessage(hwndStaticFill, WM_SETTEXT, 0, (LPARAM)currentTextBuffer);
			SendMessage(hwndStaticFillBuffer, WM_SETTEXT, 0, (LPARAM)"");
		}

	}
	
}



FLOAT sstrToFloat(CHAR* sstr)
{
	char* endptr;
	FLOAT num = std::strtof(sstr, &endptr);

	if (endptr == sstr)
	{
		throw EXCEPTION_FLT_INVALID_OPERATION;
	}

	return num;
}


FLOAT operation(FLOAT num1, FLOAT num2, CHAR sign)
{
	switch (sign)
	{
		case '+': return num1 + num2;
		case '-': return num1 - num2;
		case '*': return num1 * num2;
		case '/':
		{
			if (num2 != 0)
			{
				return num1 / num2;
			}
		}
			
	}
	return -1;
}

BOOL haveSign(CHAR sign)
{
	if (sign == '+' ||
		sign == '-' ||
		sign == '*' ||
		sign == '/') return true;
	else false;
}