#include <Windows.h>
#include "resource.h"

CONST CHAR* g_sz_VALUES[] = { "This", "is", "my", "first", "combo", "Box" };

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow) //точка запуска
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lPARAM) //обработчик 
{
	switch (uMsg)
	{
		case WM_INITDIALOG: //инициализатор 
		{
			HWND hCombo = GetDlgItem(hwnd, IDC_COMBO);
			for (int i = 0; i < sizeof(g_sz_VALUES) / sizeof(g_sz_VALUES[0]); i++) //запись значение в ComboBox
			{
				SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)g_sz_VALUES[i]);
			}
		}
		break;

		case WM_COMMAND: //обработчик взаимодействий
			{
				CONST INT SIZE = 256;
				const char textBuf[SIZE] = {};
				UINT id = LOWORD(wParam);
				UINT codeNotification = HIWORD(wParam);
				//HWND hControl = 
				
			}break;

		case WM_CLOSE: //обработка "крестика"
			{ 
				EndDialog(hwnd, 0); 
			}
			break;
	}
	return FALSE;
}