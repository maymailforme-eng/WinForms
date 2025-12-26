#undef UNICODE
#include <Windows.h>
#include "resource.h"

CONST CHAR* g_sz_VALUES[] = { "This", "is", "my", "first", "List", "Box" };
CONST CHAR* g_sz_DEFAULT_TEXT = "¬ведите текст";
CONST CHAR* g_sz_ADD = "ADD"; //режим добавлени€
CONST CHAR* g_sz_CHANGE = "CHANGE"; //режим изменени€

HWND g_hListBox = NULL;


BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lPARAM);
BOOL CALLBACK InputDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lPARAM);


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow) //точка запуска
{


	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);


	return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//функци€ обработчик ќ—Ќќ¬Ќќ√ќ окна 
BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_INITDIALOG: //»Ќ»÷»јЋ»«ј÷»я
		{
			HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1)); 											
			SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon); 

			HWND hList = GetDlgItem(hwnd, IDC_LIST);
			g_hListBox = hList;

			for (int i = 0; i < sizeof(g_sz_VALUES) / sizeof(g_sz_VALUES[0]); i++) //запись значение в ListBox
			{
				SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)g_sz_VALUES[i]);
			}



		
		} 
		
		break;
		case WM_COMMAND: //ќЅ–ќЅќ“ ј  Ќќѕќ 
		{
			UINT id = LOWORD(wParam);
			UINT codeNotefication = HIWORD(wParam);
			HWND control = (HWND)lParam;


			switch (id)
			{


//стандартные кнопки ..................................................................................

				case IDOK: //ќ 
				{
					MessageBox(hwnd, "Ѕыла нажата кнопка \"OK\"", "Info", MB_OK | MB_ICONINFORMATION);
				} 
				break;

				case IDCANCEL: //Cansle
				{
					EndDialog(hwnd, 0);
				} 
				break;


//дполнительные кнопки ........................................................................................

				case IDC_BUTTON1: //ƒќЅј¬»“№
				{

					HINSTANCE hInst = GetModuleHandle(NULL); //получаем ƒескриптор приложени€
					DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hwnd, (DLGPROC)InputDlgProc, (LPARAM)g_sz_ADD);
				}
				break;

				case IDC_BUTTON_DEL: //”ƒјЋ»“№ 
				{
					INT Index = SendMessage(g_hListBox, LB_GETCURSEL, 0, 0); //считываем выбранную строку
					if (Index != LB_ERR)
					{
						SendMessage(g_hListBox, LB_DELETESTRING, (WPARAM)Index, 0); //удала€ем выбранную строку
					}
					else
					{
						MessageBox(hwnd, "Ќе выбран элемент дл€ удалени€!", "Info", MB_OK | MB_ICONINFORMATION);
					}
				} 
				break;

				case IDC_LIST: //сообщени€ от ListBox
				{
					if (codeNotefication == LBN_DBLCLK) //двойной клик
					{

						INT Index = SendMessage(g_hListBox, LB_GETCURSEL, 0, 0); //считываем индекс элемента
						SetWindowLongPtr(g_hListBox, GWLP_USERDATA, (LPARAM)Index); //записываем в UserData индекс выбранного элемента

						HINSTANCE hInst = GetModuleHandle(NULL); //получаем ƒескриптор приложени€
						DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hwnd, (DLGPROC)InputDlgProc, (LPARAM)g_sz_CHANGE);
					}

				}

			}
		

		
		} break;
		case WM_CLOSE: //«ј –џ“»≈
		{
			EndDialog(hwnd, 0);
		}
		break;
	}


	return FALSE;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//функци€ обработчик окна ƒќЅј¬»“№ | »«ћ≈Ќ»“№
BOOL CALLBACK InputDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CONST INT SIZE = 256;

	switch (uMsg)
	{
	case WM_INITDIALOG: //»Ќ»÷»јЋ»«ј÷»я
	{
		if (g_hListBox == NULL) return FALSE; //защита 

		SetWindowLongPtr(hwnd, GWLP_USERDATA, lParam); //записываем в UserData режим
		CONST CHAR* mode = (const char*)GetWindowLongPtr(hwnd, GWLP_USERDATA); //извлекаем из UserData режим

		HWND hEdit = GetDlgItem(hwnd, IDC_EDIT1);

		if (strcmp(mode, g_sz_ADD) == 0)//режим добавлени€..........................................................
		{
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)TEXT(g_sz_DEFAULT_TEXT));
		}

		else if (strcmp(mode, g_sz_CHANGE) == 0)//режим изменени€...................................................
		{
			SetWindowText(hwnd, "»зменить элемент"); //мен€ем название окна
			INT Index = (INT)GetWindowLongPtr(g_hListBox, GWLP_USERDATA); //считываем индекс элемента из UserData
			if (Index != LB_ERR)
			{
				CHAR currentText[SIZE] = {};
				SendMessage(g_hListBox, LB_GETTEXT, Index, (LPARAM)currentText); //считываем выбранный техт ListBox
				SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)currentText); //устанавливаем текст в EditControl
			}
		}

	}
	break;

	case WM_COMMAND://ќЅ–ќЅќ“ ј  Ќќѕќ 
	{
		UINT id = LOWORD(wParam);
		UINT codeNotification = HIWORD(wParam);
		HWND control = (HWND)lParam;
		CONST CHAR* mode = (const char*)GetWindowLongPtr(hwnd, GWLP_USERDATA); //извлекаем из UserData режим

		//стандартные кнопки .................................................................................

		switch (id)
		{
		case IDOK: //ќ 
		{

			CONST INT SIZE = 256;
			CHAR currentText[SIZE] = {};
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT1); //HWND edit control
			SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)currentText); //считываем текст в поле

			if (strcmp(mode, g_sz_ADD) == 0) //режим добавлени€............................................................
			{
				if (lstrcmp(currentText, g_sz_DEFAULT_TEXT) == 0)
				{
					MessageBox(hwnd, "“екст не был введен", "Info", MB_OK | MB_ICONINFORMATION);
				}
				else
				{
					SendMessage(g_hListBox, LB_ADDSTRING, 0, (LPARAM)currentText); //переносим текст в ListBox
					EndDialog(hwnd, 0);
				}
			}
			else if (strcmp(mode, g_sz_CHANGE) == 0)//режим изменени€............................................
			{
				CHAR OldText[SIZE] = {};
				INT Index = (INT)GetWindowLongPtr(g_hListBox, GWLP_USERDATA);
				SendMessage(g_hListBox, LB_GETTEXT, Index, (LPARAM)OldText); //считываем выбранный техт ListBox


				if (lstrcmp(currentText, "") == 0)
				{
					INT Index = (INT)GetWindowLongPtr(g_hListBox, GWLP_USERDATA); //считываем индекс элемента из UserData
					SendMessage(g_hListBox, LB_DELETESTRING, (WPARAM)Index, 0); //удала€ем выбранную строку
					EndDialog(hwnd, 0);
				}
				else if (lstrcmp(currentText, OldText) == 0)
				{
					EndDialog(hwnd, 0);
				}
				else
				{
					INT Index = (INT)GetWindowLongPtr(g_hListBox, GWLP_USERDATA); //считываем индекс элемента из UserData
					SendMessage(g_hListBox, LB_DELETESTRING, (WPARAM)Index, 0); //удала€ем выбранную строку
					SendMessage(g_hListBox, LB_INSERTSTRING, (WPARAM)Index, (LPARAM)currentText); //создаем на его  месте новую
					EndDialog(hwnd, 0);
				}
			}
			

		}
		break;

		case IDCANCEL: //Cansle
		{
			EndDialog(hwnd, 0);
		}
		break;

		//Edit control ...............................................................................................

		case IDC_EDIT1: //поле ввода EditControl
		{
			CONST INT SIZE = 256;
			CHAR currentText[SIZE] = {};
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT1);
			SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)currentText); //считываем текст в поле

			if (strcmp(mode, g_sz_ADD) == 0) //режим добавлени€...................................................................................
			{
				if (codeNotification == EN_SETFOCUS) //был установлен фокус в поле
				{
					if (lstrcmp(currentText, g_sz_DEFAULT_TEXT) == 0)
					{
						SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)TEXT(""));
					}
				}
				else if (codeNotification == EN_KILLFOCUS) //куссор убран из пол€ ввода
				{
					if (lstrcmp(currentText, TEXT("")) == 0)
					{
						SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)g_sz_DEFAULT_TEXT);
					}
				}
			}
		}
		break;
		}

	}
	break;

	case WM_CLOSE://«ј –џ“»≈
	{
		EndDialog(hwnd, 0);
	}
	break;


	}

	

	return FALSE;


}