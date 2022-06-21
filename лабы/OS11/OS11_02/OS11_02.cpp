#include <Windows.h>
#include <iostream>
#include <time.h>
#include "C:\\Users\\User\\Documents\\Study_6sem\\ОСиСП\\HT.h"
#define OPEN (HT::HTHANDLE*(*)(const wchar_t*))
#define GET (HT::Element*(*)(const HT::HTHANDLE*, const HT::Element*))
#define CREATE_ELEMENT (HT::Element*(*)(const void*, int, const void*, int))
#define INSERT (BOOL(*)(HT::HTHANDLE*, HT::Element*))
int wmain(int argc, wchar_t* argv[]) {
	HMODULE hmdll;
	try {
		if (argc != 2) throw " write only a filename";
		hmdll = LoadLibrary(L"C:\\Users\\User\\Documents\\Study_6sem\\ОСиСП\\Lab11\\x64\\Debug\\Lab11.dll");
		if (!hmdll) throw " load library failed";
		auto Open = OPEN GetProcAddress(hmdll, "Open");
		if (!Open) throw " get Open failed";
		auto Insert = INSERT GetProcAddress(hmdll, "Insert");
		if (!Insert) throw " get Insert failed";
		auto Get = GET GetProcAddress(hmdll, "Get");
		if (!Get) throw "get Get failed";
		auto CreateElement = CREATE_ELEMENT GetProcAddress(hmdll, "CreateElementI");
		if (!CreateElement) throw " get CreateElement failed";
		HT::HTHANDLE* ht = Open(argv[1]);
		HT::Element* elem;
		if (!ht) throw " open file failed";
		if (ht->File != NULL) {
			throw " open file before adding data";
		}
		char* ch = new char[1];
		char* ch2 = new char[1];
		*ch2 = '0';
		srand(time(NULL));
		while (true) {
			elem = CreateElement(ch, 1, ch2, 1);
			*ch = '0' + rand()%50;
			if (!Insert(ht, elem) || !Get(ht, elem))
				std::cout << "Add failure element: " << *ch << " " << 1 << " " << 0 << " " << 1 << std::endl;
			else
				std::cout << "Add successful element: " << *ch << " " << 1 << " " << 0 << " " << 1<< std::endl;
			Sleep(1000);
		}
		delete ch, ch2;
		if (!FreeLibrary(hmdll)) throw " free library failed";
	}
	catch (const char* ch) {
		std::cout << ch;
	}
}