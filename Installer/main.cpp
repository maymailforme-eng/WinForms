#define _CRT_SECURE_NO_WARNINGS
#undef UNICODE
#include <windows.h>
#include <shlobj.h>
//#include <commdlg.h>
#include <stdio.h>




// Функция выбора папки вернет 
BOOL BrowseForFolderA(HWND hwndParent, char* selectedPath);



void CopyAllFilesFromFolderA(const char* sourceFolder, const char* destFolder);




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 

{
    CONST INT SIZE = 256; 

    CHAR pathInastal[SIZE] = {};
    CHAR fullPathInstall[SIZE] = {};

    CHAR pathSourceEXE[SIZE]        = { "C:\\Users\\ASUS\\Desktop\\Репозиторий\\WinForms\\x64\\Debug\\CalckInClass.exe" };
    CHAR pathSourceDLL_1[SIZE]      = {"C:\\Users\\ASUS\\Desktop\\Репозиторий\\WinForms\\x64\\Debug\\square_blue.dll"};
    CHAR pathSourceDLL_2[SIZE]      = {"C:\\Users\\ASUS\\Desktop\\Репозиторий\\WinForms\\x64\\Debug\\metal_mistral.dll"};

    CHAR pathSourceBMP_BLUE[SIZE]   = { "C:\\Users\\ASUS\\Desktop\\Репозиторий\\WinForms\\x64\\Debug\\ButtonsBMP\\SquareBlue" };
    CHAR pathSourceBMP_METAL[SIZE]  = { "C:\\Users\\ASUS\\Desktop\\Репозиторий\\WinForms\\x64\\Debug\\ButtonsBMP\\MetalMistral" };

    CHAR pathSourceBMP_FONTS[SIZE]  = { "C:\\Users\\ASUS\\Desktop\\Репозиторий\\WinForms\\CalckInClass\\Fonts" };





    //выбираем папку устнавки
    BrowseForFolderA(NULL, pathInastal);


    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //Копирование файлов .exe .dll

    /*
    BOOL CopyFileA ([in] LPCSTR lpExistingFileName,  // Исходный файл (ANSI) Путь + имя-файла.расширение
                    [in] LPCSTR lpNewFileName,       // Целевой файл  (ANSI) 
                    [in] BOOL   bFailIfExists        // Флаг поведения (TRUE - перезапишет сушествующий файл в папке назнаяения)
    );
    
    */

    BOOL flagCorrectInstal = true;

    sprintf(fullPathInstall, "%s%s", pathInastal, "\\CalckInClass.exe");
    if (!CopyFileA(pathSourceEXE, fullPathInstall, FALSE))  flagCorrectInstal = false;


    sprintf(fullPathInstall, "%s%s", pathInastal, "\\square_blue.dll");
    if (!CopyFileA(pathSourceDLL_1, fullPathInstall, FALSE))  flagCorrectInstal = false;


    sprintf(fullPathInstall, "%s%s", pathInastal, "\\metal_mistral.dll");
    if (!CopyFileA(pathSourceDLL_2, fullPathInstall, FALSE))  flagCorrectInstal = false;



    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //КОИПРОВАНИЕ ИЗОБРАЖЕНИЙ

    
    sprintf(fullPathInstall, "%s%s", pathInastal, "\\ButtonsBMP");
    CreateDirectoryA(fullPathInstall, NULL); //создаем папку ButtonsBMP 


    sprintf(fullPathInstall, "%s%s", pathInastal, "\\ButtonsBMP\\SquareBlue");
    CreateDirectoryA(fullPathInstall, NULL);                        //создаем папку SquareBlue 
    CopyAllFilesFromFolderA(pathSourceBMP_BLUE, fullPathInstall);   //копируем в нее изображения

    sprintf(fullPathInstall, "%s%s", pathInastal, "\\ButtonsBMP\\MetalMistral");
    CreateDirectoryA(fullPathInstall, NULL);                        //создаем папку MetalMistral 
    CopyAllFilesFromFolderA(pathSourceBMP_METAL, fullPathInstall);  //копируем в нее изображения


    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //КОИПРОВАНИЕ ШРИФТОВ
    sprintf(fullPathInstall, "%s%s", pathInastal, "\\FONTS");
    CreateDirectoryA(fullPathInstall, NULL); //создаем папку FONTS
    CopyAllFilesFromFolderA(pathSourceBMP_FONTS, fullPathInstall);  //копируем в шрифты

    if (flagCorrectInstal)
    {
        MessageBox(NULL, "Калькулятор установлен", "Installer", MB_OK);
    }
    else
    {
        MessageBox(NULL, "В ходе установки возникли ошибки", "Installer", MB_OK);
    }

 

    return !flagCorrectInstal ? 0 : 1;
}



BOOL BrowseForFolderA(HWND hwndParent, char* selectedPath)
{
    //стандартная структура для выбора папки 
       /*
    typedef struct _browseinfoa {
                                    HWND        hwndOwner;      // Родительское окно
                                    PCIDLIST_ABSOLUTE pidlRoot; // Начальная папка
                                    LPSTR       pszDisplayName; // Буфер для имени
                                    LPCSTR      lpszTitle;      // Заголовок окна
                                    UINT        ulFlags;        // Флаги поведения
                                    BFFCALLBACK lpfn;           // Callback-функция
                                    LPARAM      lParam;         // Параметр callback
                                    int         iImage;         // Индекс иконки
                                 } BROWSEINFOA, *PBROWSEINFOA, *LPBROWSEINFOA;




    */


    BROWSEINFOA bi = { 0 };
    char displayName[MAX_PATH] = { 0 };

    // Заполняем структуру
    bi.hwndOwner = hwndParent;
    bi.lpszTitle = "Выберите папку установки калькулятора:";
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    bi.pszDisplayName = displayName;




    //SHBrowseForFolderA - это ANSI версия функции Windows API, которая отображает диалоговое окно для выбора папки
    //принимает в качестве параметра указатель на структуру BROWSEINFOA
    LPITEMIDLIST pidl = SHBrowseForFolderA(&bi);

    if (pidl != NULL)
    {
        // SHGetPathFromIDListA - это стандартная функция Windows API, 
        // которая конвертирует ITEMIDLIST в обычный строковый путь.
        if (SHGetPathFromIDListA(pidl, selectedPath)) {
            // Освобождаем память
            CoTaskMemFree(pidl);
            return TRUE;
        }
        CoTaskMemFree(pidl);
    }

    return FALSE; // Пользователь отменил
}



//рекурсивное копирование для картинок
void CopyAllFilesFromFolderA(const char* sourceFolder, const char* destFolder) {
    char searchPath[MAX_PATH];

    //WIN32_FIND_DATAA - это ANSI-версия структуры, которая используется функциями FindFirstFileA() и FindNextFileA() 
    // для получения информации о найденных файлах и папках.
    WIN32_FIND_DATAA findData;
    HANDLE hFind;

    // Создаем путь для поиска: "папка\*.*"
    snprintf(searchPath, MAX_PATH, "%s\\*.*", sourceFolder);

    // Начинаем поиск
    // стандартная функция Windows API для поиска файлов и каталогов (ANSI версия).
    /*HANDLE FindFirstFileA(
                            [in]  LPCSTR lpFileName,   // Путь и маска поиска
                            [out] LPWIN32_FIND_DATAA lpFindFileData // Буфер для данных о файле
                            );*/

    hFind = FindFirstFileA(searchPath, &findData);

    if (hFind == INVALID_HANDLE_VALUE) //если файлы не найдены вернет INVALID_HANDLE_VALUE
    {
        return;
    }

    // Создаем целевую папку
    CreateDirectoryA(destFolder, NULL);

    int fileCount = 0;

    do {

        // Формируем полные пути
        char sourceFile[MAX_PATH];
        char destFile[MAX_PATH];

        snprintf(sourceFile, MAX_PATH, "%s\\%s", sourceFolder, findData.cFileName);
        snprintf(destFile, MAX_PATH, "%s\\%s", destFolder, findData.cFileName);

        // Копируем файл
        CopyFileA(sourceFile, destFile, FALSE);

    } while (FindNextFileA(hFind, &findData));
    //это стандартная функция Windows API, которая продолжает поиск файлов, начатый функцией FindFirstFileA


    FindClose(hFind);
}