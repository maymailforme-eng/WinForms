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



	switch (uMsg)//сравниваем сообщение-ответ полученное от окна hwnd (в данном сучае основное диалоговое)
	{
		//инициализация окна диалога .....................................................................................
		case WM_INITDIALOG: //Инициализация окна дифлога. Секция отрабатывает один раз, при запуске окна.
		{
			HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN); //код HWND окна ввода логина
			HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1)); //загружаем иконку
			SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon); //устанавливаем иконку
			SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)TEXT("Введите имя пользователя")); //дефолтная запись в Edit control
		}
		break;



		//обработка нажатия кнопок и т.д............................................................................
		case WM_COMMAND: //В этой секции обрабатываються нажатия клавишь и другия события.
		{
			CONST INT SIZE = 256; //размер для чаровских массивов
			const char textDef[25] = { "Введите имя пользователя" };

			UINT id = LOWORD(wParam); //колбэк запишет id отправляющего сообщение окна в LOWORD(wParam) (может повтараться)
			UINT code = HIWORD(wParam); //колбэк запишет шифр события (что произошло) в HIWORD(wParam)
			HWND hControl = (HWND)lParam; //в lParam колбэк запишет HWND окна отправившего сообщения (уникальный код) 

			switch (code) //шагаем по событиям 
			{
				case EN_SETFOCUS: //кликнули в поле
				{
					if (id == IDC_EDIT_LOGIN) //если сообщение отправило поле логина
					{
						CHAR currentText[SIZE] = {};
						HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN); //код HWND окна ввода логина

						SendMessage(hEditLogin, WM_GETTEXT, SIZE, (LPARAM)currentText); //считываем текст в поле

						if (lstrcmp(currentText, TEXT(textDef)) == 0)
						{
							SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)TEXT(""));
						}
					}
				}
				break;

				case EN_KILLFOCUS: //текст покинул поле ввода
				{
					if (id == IDC_EDIT_LOGIN)
					{
						CHAR currentText[SIZE] = {};
						HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN); //код HWND окна ввода логина

						SendMessage(hEditLogin, WM_GETTEXT, SIZE, (LPARAM)currentText); //считываем текст в поле
						if (lstrcmp(currentText, TEXT("")) == 0)
						{
							SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)TEXT(textDef));
						}
					}
				
				}break;

				case BN_CLICKED: //нажатие кнопки
				{
					if (id == IDC_BUTTON_COPY) //кнопка  COPY
					{
						CHAR currentText[SIZE] = {};
						HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN); //код HWND окна ввода логина
						HWND hEditPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD); // код HWND окна ввода пароля
						SendMessage(hEditLogin, WM_GETTEXT, SIZE, (LPARAM)currentText);

						if ((lstrcmp(currentText, TEXT(textDef)) != 0))
						{
							SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)currentText);
						}
					}
					else if (id == IDOK) //кнопка ОК
					{
						MessageBox(hwnd, "Была нажата кнопка", "info", MB_OK | MB_ICONINFORMATION); break;
					}
					else if (id == IDCANCEL) //кнопка Cancel
					{
						EndDialog(hwnd, 0);
					}
					
				}
				break;
			}
		}
		break;


	//логика нажатия на крестик .............................................................................................
	case WM_CLOSE:
		EndDialog(hwnd, 0); // позволяет закрыть на крестик окно
		break; // отрабатывает при закрытии окна.
	}

	return FALSE;
}


