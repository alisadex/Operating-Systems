#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <iostream>
#include <wdmguid.h>
#include <string>
#include <VersionHelpers.h>

#define  ENV_VAR_STRING_COUNT  (sizeof(envVarStrings)/sizeof(TCHAR*))
#define INFO_BUFFER_SIZE 32767

void main()
{
    //DWORD i;
    TCHAR infoBuf[INFO_BUFFER_SIZE];
    DWORD bufCharCount = INFO_BUFFER_SIZE;

    // Имя компутера 
    //bufCharCount = INFO_BUFFER_SIZE;
    GetComputerName(infoBuf, &bufCharCount);
    _tprintf(TEXT("\nComputer name:      %s"), infoBuf);

    // Имя челобэка
    //bufCharCount = INFO_BUFFER_SIZE;
    GetUserName(infoBuf, &bufCharCount);
    _tprintf(TEXT("\nUser name:          %s"), infoBuf);

    // Дайректория (системная)
    GetSystemDirectory(infoBuf, INFO_BUFFER_SIZE);
    _tprintf(TEXT("\nSystem Directory:   %s"), infoBuf);

    // Виндавс дайрэктория
    GetWindowsDirectory(infoBuf, INFO_BUFFER_SIZE);
    _tprintf(TEXT("\nWindows Directory:  %s\n"), infoBuf);

    // Дайрэктория временных
    GetTempPath(INFO_BUFFER_SIZE, infoBuf);
    _tprintf(TEXT("\Temp Path Directory:  %s\n\n"), infoBuf);


    //Версия операционной системы 
    OSVERSIONINFO osversion;
    memset(&osversion, 0, sizeof(OSVERSIONINFO));
    osversion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    if (!IsWindows10OrGreater()) { std::cout << "Windows 10/11" << std::endl; }
    else { std::cout << "Not win 10" << std::endl; }


    //// Систэмнэй мэтрик
    // Колво моников
    int monitorNumbers = GetSystemMetrics(SM_CMONITORS);
    std::cout << "\System Metrics: \nMonitor numbers: " << monitorNumbers << "\n";
    // Ширина экрана основного монитора отображения, в пикселях. 
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    std::cout << "Screen width: " << screenWidth << "\n";
    // Колво кнопок на мышаре
    int mouseButtons = GetSystemMetrics(SM_CMOUSEBUTTONS);
    std::cout << "The number of buttons on the mouse: " << mouseButtons << "\n";

    //// Системнэй парметоры
    // Скорость мышки
    unsigned int aMouseInfo;
    SystemParametersInfo(SPI_GETMOUSESPEED, 0, &aMouseInfo, 0);
    std::cout << "\n\System Parametrs: \nMouse speed: " << aMouseInfo;

    // Размер курсора
    int cursorWidth = 0;
    int cursorHeight = 0;

    SystemParametersInfo(SPI_GETMOUSE, 0, &cursorWidth, 0);
    SystemParametersInfo(SPI_GETMOUSE, 0, &cursorHeight, SPIF_SENDCHANGE);
    std::cout << "\nMouse cursor size:" << std::endl;
    std::cout << "Width: " << cursorWidth << " pixels" << std::endl;
    std::cout << "Height: " << cursorHeight << " pixels" << std::endl;

    // Системные цвета




    //Функции для работы со временем:
    SYSTEMTIME var1, var2;
    GetLocalTime(&var1);
    GetSystemTime(&var2);

    std::cout << "\nTime information:\nLocal Time: date " << var1.wDay << "." << var1.wMonth << "." << var1.wYear << " time " << var1.wHour << ":" << var1.wMinute

        << "\nSystem time: "

        << "date " << var2.wDay << "." << var2.wMonth << "." << var2.wYear << " time " << var2.wHour << ":" << var2.wMinute << std::endl;

    // Индивидуальные такси
    // GetClipCursor, GetKeyboardType, GetUserDefaultLCID, ShowCursor
    std::cout << "\n Individual task. Option #13";

    // Извлекает экранные координаты прямоугольной области, которой ограничен курсор.
    RECT mem;
    GetClipCursor(&mem);
    std::cout << "\n1.GetClipCursor = " << mem.left << "." << mem.bottom << "." << mem.right << "." << mem.top;

    // Тип клавы коки
    int keyboardType = GetKeyboardType(0);
    std::cout << "\n2.Keyboard type = " << keyboardType << std::endl;

    // Получение идентификатора языковой локали (LCID) пользователя
    LCID lcid = GetUserDefaultLCID();
    std::cout << "3.ID of the operating system's user-defined language = " << lcid << std::endl;

    // Показывает или скрывает курсор
    std::cout << "4.Cursor visible = " << ShowCursor(1) << std::endl;

    system("PAUSE");

}