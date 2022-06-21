#include <Windows.h>
#include <iostream>
#include <time.h>
#include "C:\\Users\\User\\Documents\\Study_6sem\\ОСиСП\\HT.h"
#define OPEN (HT::HTHANDLE*(*)(const wchar_t*))
#define CREATE_ELEMENT (HT::Element*(*)(const void*, int))
#define DELETE (BOOL(*)(HT::HTHANDLE*, const HT::Element*))
int wmain(int argc, wchar_t* argv[]) {
	HMODULE hmdll;
	try {
		if (argc != 2) throw " write only a filename";
		hmdll = LoadLibrary(L"C:\\Users\\User\\Documents\\Study_6sem\\ОСиСП\\Lab11\\x64\\Debug\\Lab11.dll");
		if (!hmdll) throw " load library failed";
		auto Open = OPEN GetProcAddress(hmdll, "Open");
		if (!Open) throw " get Open failed";
		auto Delete = DELETE GetProcAddress(hmdll, "Delete");
		if (!Delete) throw " get Delete failed";
		auto CreateElement = CREATE_ELEMENT GetProcAddress(hmdll, "CreateElementG");
		if (!CreateElement) throw " get CreateElement failed";
		HT::HTHANDLE* ht = Open(argv[1]);
		if (!ht) throw " open file failed";
		if (ht->File != NULL) {
			throw " open file before delete data";
		}
		char* ch = new char[1];
		srand(time(NULL));
		while (true) {
			*ch = '0' + rand() % 50;
			if (!Delete(ht, CreateElement(ch, 1)))
				std::cout << "Delete element: " << *ch << " " << 1 << " " << 0 << " " << 1 << " failure" << std::endl;
			else
				std::cout << "Delete element: " << *ch << " " << 1 << " " << 0 << " " << 1 << " successful" << std::endl;
			Sleep(1000);
		}
		delete ch;
		if (!FreeLibrary(hmdll)) throw " free library failed";
	}
	catch (const char* ch) {
		std::cout << ch;
	}
}