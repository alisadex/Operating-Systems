#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <tlhelp32.h> 
#include <conio.h> 
#include <locale>  
#include <psapi.h>
using namespace std;  

// Получить дескриптор, имя и полное имя
void getHandelNameFName()  { 
	HMODULE hModule = GetModuleHandle(NULL); 
	wchar_t* FName = new wchar_t[256]; 
	GetModuleFileName(hModule, FName, 256); 
	setlocale(LC_ALL, "rus"); 
	wcout.imbue(locale("rus_rus.1251"));
	wchar_t *name = wcsrchr(FName, L'\\');
	wchar_t *Name = new wchar_t[256]; 
	wcsncpy_s(Name, 256, name + 1, 256); 
	std::wcout << "Handle: " << hModule<< endl<< "Name: "<< Name << endl << "FName: " << FName << endl;

}  

// Получить имя и дескриптор, используя полное имя
void getHandleName()  {  
	setlocale(LC_ALL, "rus");  
	wcout.imbue(locale("rus_rus.1251"));
	wcout << "Введите FName: ";  
	wchar_t* FName = new wchar_t[256];  
	wcin >> FName;  
	wchar_t *name = wcsrchr(FName, L'\\');
	wchar_t *Name = new wchar_t[256]; 
	if (name != NULL)  {  
		wcsncpy_s(Name, 256, name + 1, 256); 
		HMODULE hModule = GetModuleHandle(FName);  
		wcout << "Handle: " << hModule << endl << "Name: " << Name << endl << "FName: " << FName << endl; 
	} 
}  

// Получить дескриптор и полное имя, используя имя
void getHandleFName()  {  
	setlocale(LC_ALL, "rus");
	wcout.imbue(locale("rus_rus.1251"));
	cout << "Введите Name: ";  
	wchar_t* Name = new wchar_t[256];  
	wcin >> Name;  
	wchar_t* FName = new wchar_t[256];  
	GetModuleFileName(GetModuleHandle(Name), FName, 256);  
	HMODULE hModule = GetModuleHandle(FName);  
	wcout << "Handle: " << hModule << endl << "Name: " << Name << endl << "FName: " << FName << endl;  
}  

// Получить полное имя и имя, используя дескриптор
void getNameFName()  {  
	setlocale(LC_ALL, "rus");
	cout << "Введите Handle: "; 
	wchar_t* temp = new wchar_t[10];  
	wcin >> temp;  
	wchar_t *endptr;  
	long num = wcstoul(temp, &endptr, 16); 
	HMODULE hModule = (HMODULE)num;  
	wchar_t* FName = new wchar_t[256]; 
	GetModuleFileName(hModule, FName, 256);  
	wchar_t *name = wcsrchr(FName, L'\\'); 
	wchar_t *Name = new wchar_t[256];  
	wcsncpy_s(Name, 256, name + 1, 256); 
	wcout << "Handle: " << hModule << endl << "Name: " << Name << endl << "FName: " << FName << endl;
	delete[] temp; 
}  

// Инфо о текущем процессе, псевдодескрипторе и дескрипторе. Создание копии текущего процесса и закрытие
void Handels()  {  
	setlocale(LC_ALL, "rus");  
	wcout.imbue(locale("rus_rus.1251"));  
	DWORD dCurrentId = GetCurrentProcessId(); 
	HANDLE hCurrentPseudo = GetCurrentProcess();
	HANDLE hCurrentIdDup; 
	DuplicateHandle(hCurrentPseudo, hCurrentPseudo, hCurrentPseudo,  &hCurrentIdDup, 0, FALSE, DUPLICATE_SAME_ACCESS);  
	HANDLE hCurrentIdOpen = OpenProcess(PROCESS_DUP_HANDLE, TRUE, dCurrentId);  
	BOOL fClosedDup = CloseHandle(hCurrentIdDup); 
	BOOL fClosedOpen = CloseHandle(hCurrentIdOpen);  
	wcout << "Идентификатор текущего процесса: " << dCurrentId << "\nПсевдодескриптор текущего процесса: " << hCurrentPseudo << "\nДескриптор, полученный функцией DuplicateHandle: " << hCurrentIdDup << "\nДескриптор, полученный функцией OpenProcess: " << hCurrentIdOpen << "\nЗакрываем дескриптор, полученный функцией OpenProcess: " <<  fClosedOpen << "\nЗакрываем дескриптор, полученный функцией DuplicateHandle: " << fClosedDup << endl;
}  

// Инфо о всех процессах и вывод атрибутов
void ShowProcesses()  { 
	setlocale(LC_ALL, "rus"); 
	wcout.imbue(locale("rus_rus.1251")); 
	HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);  
	wcout << "\nSnapshot ID = " << Snapshot;  
	PROCESSENTRY32 entry; 
	entry.dwSize = sizeof(PROCESSENTRY32);
	BOOL fRepeat = Process32First(Snapshot, &entry);  
	int i = 0;  
	while (fRepeat)  {  
		i++;  
		wcout << "\n\nth32ProcessID = " << entry.th32ProcessID;
		wcout << "\nth32DefaultHeapID = " << entry.th32DefaultHeapID;
		wcout << "\nth32ModuleID = " << entry.th32ModuleID;
		wcout << "\nth32ParentProcessID = " << entry.th32ParentProcessID;
		wcout << "\ncntUsage = " << entry.cntUsage;
		wcout << "\ncntThreads = " << entry.cntThreads;
		wcout << "\npcPriClassBase = " << entry.pcPriClassBase;
		wcout << "\ndwFlags = " << entry.dwFlags;
		wcout << "\nszExeFile = " << entry.szExeFile;
		fRepeat = Process32Next(Snapshot, &entry);  
	}  
	CloseHandle(Snapshot); 
}  

// Инфо о всех потоках системы текущего процесса и их атрибуты
void ShowThreads()  {  
	setlocale(LC_ALL, "rus"); 
	wcout.imbue(locale("rus_rus.1251")); 
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0); 
	wcout << "\nSnapshot ID = " << hSnapshot;
	THREADENTRY32 entry; 
	entry.dwSize = sizeof(THREADENTRY32); 
	BOOL fRepeat = Thread32First(hSnapshot, &entry);  
	DWORD dwCurentProcessId = GetCurrentProcessId();  
	while (fRepeat) { 
		if (entry.th32OwnerProcessID == dwCurentProcessId) { 
			wcout << "\n\nth32ThreadID = " << entry.th32ThreadID;
			wcout << "\nth32OwnerProcessID = " << entry.th32OwnerProcessID;
			wcout << "\ncntUsage = " << entry.cntUsage;
			wcout << "\ntpBasePri = " << entry.tpBasePri;
			wcout << "\ntpDeltaPri = " << entry.tpDeltaPri;
			wcout << "\ndwFlags = " << entry.dwFlags;
		}  
		fRepeat = Thread32Next(hSnapshot, &entry);
	} 
	CloseHandle(hSnapshot); 
}  

// Инфо о всех модулях системы и их атрибуты
void ShowModules() { 
	setlocale(LC_ALL, "rus");  
	wcout.imbue(locale("rus_rus.1251")); 
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0); 
	wcout << "\nSnapshot ID = " << hSnapshot;
	MODULEENTRY32 entry;  entry.dwSize = sizeof(MODULEENTRY32);
	BOOL fRepeat = Module32First(hSnapshot, &entry); 
	while (fRepeat) { 
		wcout << "\n\nth32ModuleID = " << entry.th32ModuleID;
		wcout << "\nth32ProcessID = " << entry.th32ProcessID;
		wcout << "\nGlblcntUsage = " << entry.GlblcntUsage;
		wcout << "\nProccntUsage = " << entry.ProccntUsage;
		wcout << "\nmodBaseAddr = " << entry.modBaseAddr;
		wcout << "\nmodBaseSize = " << entry.modBaseSize;
		wcout << "\nhModule = " << entry.hModule;
		wcout << "\nszModule = " << entry.szModule;
		wcout << "\nszExePath = " << entry.szExePath;
		fRepeat = Module32Next(hSnapshot, &entry);
	}  
	CloseHandle(hSnapshot);
}  

// Инфо о загруженных драйверов устройств
void ShowDrivers() {
	DWORD cbNeeded;
	HMODULE hModules[1024];
	
	// Получаем список загруженных модулей (включая драйверы) для текущего процесса
	EnumProcessModules(GetCurrentProcess(), hModules, sizeof(hModules), &cbNeeded);
	int numDrivers = cbNeeded / sizeof(HMODULE);

	for (int i = 0; i < numDrivers; i++) {
		TCHAR szDriverPath[MAX_PATH];
		if (GetModuleFileNameEx(GetCurrentProcess(), hModules[i], szDriverPath, MAX_PATH)) {
				std::wcout << L"Драйвер " << i + 1 << L": " << szDriverPath << std::endl;
		}
	}
}

int main() {
	setlocale(LC_ALL, "rus");  
	int a = -1;  
	while (a != 0) { 
		cout << "1. Получить Name, FName, Handle" << endl << "2. Получить FName и Handle по Name" << endl << "3. Получить Name и Handle по FName" << endl << "4. Получить Name и FName по Handle" << endl << "5. Получить идентификатор, псевдодескриптор, дескриптор и копию дескриптора текущего процесса" << endl << "6. Получить список всех процессов" << endl << "7. Получить список всех потоков" << endl << "8. Получить список всех модулей" << endl << "9. Получить список загруженных драйверов" << endl << "0. Выход" << endl << endl;
		cout << "> ";
		cin >> a; 
	switch (a) { 
		case 0: {
			break;  }  

		case 1: {  
			getHandelNameFName();  
			cout << endl;  break; }

		case 2: { 
			getHandleFName();  
			cout << endl;  break;  	}

		case 3: {  
			getHandleName(); 
			cout << endl;  break;	}
	
		case 4: { 
			getNameFName(); 
			cout << endl;  break; }
	
		case 5: { 
			Handels(); 
			cout << endl; break;	}
	
		case 6: {  
			cout << "_______________________________________________________________Процессы_________________________________________________" << endl;
			ShowProcesses();  
			cout << endl << endl;  break;  }
	
		case 7: { 
			cout << "____________________________________________________________Потоки______________________________________________________" << endl;
			ShowThreads();
			cout << endl << endl; break;  }
	
		case 8: { 
			cout << "__________________________________________________________Модули________________________________________________________" << endl;
			ShowModules();  
			cout << endl << endl;  break;	}

		case 9: {
			cout << "__________________________________________________________Драйверы______________________________________________________" << endl;
			ShowDrivers();
			cout << endl << endl;  break; }
		} 
	}
	return 0;
}
