#include <Windows.h>

// точка входа в программу, аналог main в консольном 
INT WINAPI WinMain(HINSTANCE hInstance, // Дескриптор (handle) текущего экземпляра приложения
					HINSTANCE hPrevInst, // Дескриптор предыдущего экземпляра (всегда NULL в Win32)
					LPSTR lpCmdLine,  // Командная строка (как char*)
					INT nCmdShow)  // Флаг того, как показывать окно (свернуто, развернуто и т.д.)

{
	MessageBox(NULL, 
				"Hello Windows!", 
				"Message Box", 
				MB_YESNOCANCEL |  MB_ICONINFORMATION | MB_HELP | MB_DEFBUTTON1 | MB_SYSTEMMODAL);
	return 0;
}