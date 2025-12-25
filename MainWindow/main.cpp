#undef UNICODE
#include <Windows.h>
#include <stdio.h>;

CONST CHAR g_sz_CLASS_NAME[] = "Main Window PV_522"; //имя класса окна

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


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
	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;

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

	HWND hwnd = CreateWindowEx
	(
		NULL, //exStyles
		g_sz_CLASS_NAME, //ClassName
		g_sz_CLASS_NAME, //Window title
		WS_OVERLAPPEDWINDOW, //Стиль окна. Набор стилей всегда зависит от класса окна. 
		//Стиль главного окна всегда WS_OVERLAPPEDWINDOW
		CW_USEDEFAULT, CW_USEDEFAULT, //начальная позиция окна при запуске
		CW_USEDEFAULT, CW_USEDEFAULT, //размер окна 
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



LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg,	WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
		case WM_CREATE: 
			{}break;
		case WM_COMMAND: 
			{}break;
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


