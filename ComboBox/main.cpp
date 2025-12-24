#include <Windows.h>
#include <stdio.h>;
#include "resource.h"

CONST CHAR* g_sz_VALUES[] = { "This", "is", "my", "first", "combo", "Box" };

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lPARAM);
VOID concStr_c(CHAR* buff, CONST CHAR* str_1, CONST CHAR* str_2); //объединитель строк

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow) //точка запуска
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);
	return 0;
}


VOID concStr_c(CHAR* buff, CONST CHAR* str_1, CONST CHAR* str_2)
{
	INT SIZE = sizeof(buff) / sizeof(buff[0]);
	INT cntr = 0;
	while (str_1[cntr] != '\0')
	{
		buff[cntr] = str_1[cntr];
		cntr++;
	}

	if (cntr < SIZE - 2)
	{
		buff[cntr++] = '.';
	}

	if (cntr < SIZE - 2)
	{
		buff[cntr++] = ' ';
	}


	INT i = 0;
	while (str_2[i] != '\0' && cntr < SIZE)
	{
		if (cntr == SIZE - 1) //если в массиве остался один незаписанный элемент 
		{
			buff[cntr] = '\0'; // закрываем массив
			break; //прекращаем цикл
		}
		buff[cntr] = str_2[i++];
		cntr++;
	}

	if (cntr < SIZE - 1)
	{
		buff[cntr] = '\0';
	}
}



BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) //обработчик 
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

			HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1)); //получаем число под иконку
			SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon); //устанавливаем иконку 
		}
		break;

		case WM_COMMAND: //обработчик взаимодействий
			{
				UINT id = LOWORD(wParam);
				UINT codeNotification = HIWORD(wParam);
				HWND hControl = (HWND)lParam;

				switch (id)
				{
					case IDCANCEL: EndDialog(hwnd, 0); break;

					case IDOK:
					{

						HWND hCombo = GetDlgItem(hwnd, IDC_COMBO);
						INT index = SendMessage(hCombo, CB_GETCURSEL, 0, 0);//получаем индекс выбранного элемента в ComboBox
						if (index != CB_ERR) //CB_ERR - ничего не выбрано - 1;
						{
							CONST INT SIZE = 256;
							CHAR textBuf[SIZE] = {};
							CHAR Message[SIZE] = {};

							CONST INT safeSize = 32;
							CHAR indexStr[safeSize] = {};

							sprintf_s(indexStr, safeSize, "%d", (index + 1)); //преобразуем число в строковое представление
							SendMessage(hCombo, CB_GETLBTEXT, index, (LPARAM)textBuf); //переписываем выбранное сообщение в масисв
							concStr_c(Message, indexStr, textBuf); //объединяем (логика описана выше)


							MessageBox(NULL, Message, "Выбран элемент", MB_OK);
						}
						else
						{
							MessageBox(NULL, "Элемент не выбран!", "WARNING!", MB_OK);
						}


					} break;
				}
				
			}break;

		case WM_CLOSE: //обработка "крестика"
			{ 
				EndDialog(hwnd, 0); 
			}
			break;
	}
	return FALSE;
}