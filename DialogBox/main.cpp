#include <Windows.h>
#include "resource.h"

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// точка входа в программу, аналог main в консольном 
INT WINAPI WinMain(HINSTANCE hInstance, // Дескриптор (handle) текущего экземпляра приложения
	HINSTANCE hPrevInst, // Дескриптор предыдущего экземпляра (всегда NULL в Win32)
	LPSTR lpCmdLine,  // Командная строка (как char*)
	INT nCmdShow)  // Флаг того, как показывать окно (свернуто, развернуто и т.д.)

{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);
	return 0;
}


BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG: break; //Инициализация окна дифлога. Секция отрабатывает один раз, при запуске окна.
	case WM_COMMAND: //В этой секции обрабатываються нажатия клавишь и другия события.
		switch (LOWORD(wParam))
		{
		case IDOK: MessageBox(hwnd, "Была нажата кнопка", "info", MB_OK | MB_ICONINFORMATION); break;
		case IDCANCEL: EndDialog(hwnd, 0); break;

		}
		
		break; 
	case WM_CLOSE: 
		EndDialog(hwnd, 0); // позволяет закрыть на крестик окно
		break; // отрабатывает при закрытии окна.
	}

	return FALSE;
}