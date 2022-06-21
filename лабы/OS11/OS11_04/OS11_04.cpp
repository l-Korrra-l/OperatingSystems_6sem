#include <Windows.h>
#include <iostream>
#include <time.h>
#include "C:\\Users\\User\\Documents\\Study_6sem\\ОСиСП\\HT.h"
#define OPEN (HT::HTHANDLE*(*)(const wchar_t*))
#define CREATE_ELEMENT (HT::Element*(*)(const void*, int, const void*, int))
#define GET_ELEMENT (HT::Element*(*)(const HT::HTHANDLE*, const HT::Element*))
#define UPDATE (BOOL(*)(HT::HTHANDLE*, const HT::Element*, const void*, int))
int wmain(int argc, wchar_t* argv[]) {
	HMODULE hmdll;
	try {
		if (argc != 2) throw " write only a filename";
		hmdll = LoadLibrary(L"C:\\Users\\User\\Documents\\Study_6sem\\ОСиСП\\Lab11\\x64\\Debug\\Lab11.dll");
		if (!hmdll) throw " load library failed";
		auto Open = OPEN GetProcAddress(hmdll, "Open");
		if (!Open) throw " get Open failed";
		auto Update = UPDATE GetProcAddress(hmdll, "Update");
		if (!Update) throw " get Update failed";
		auto CreateElement = CREATE_ELEMENT GetProcAddress(hmdll, "CreateElementI");
		if (!CreateElement) throw " get CreateElement failed";
		auto GetElement = GET_ELEMENT GetProcAddress(hmdll, "Get");
		if (!GetElement) "throw get Get failed";
		HT::HTHANDLE* ht = Open(argv[1]);
		if (!ht) throw " open file failed";
		if (ht->File != NULL) {
			throw " open file before udpating";
		}
		HT::Element* element;
		char* ch = new char[1];
		char* ch2 = new char[1];
		*ch2 = '7';
		srand(time(NULL));
		while (true) {
			*ch = '0' + rand() % 50;
			if (!(element = GetElement(ht, CreateElement(ch, 1, ch2, 1)))) {
				std::cout << "Get element: " << *ch << " " << 1 << " " << 0 << " " << 1 << " failure" << std::endl;
				Sleep(1000);
				continue;
			}
			if (!Update(ht, element, ch2, 1))
				std::cout << "Update element: " << (char*)(element->key) << " " << element->keylength << " " << (char*)(element->payload) << " " << element->payloadlength << " failure" << std::endl;
			else
				std::cout << "Update element: " << (char*)(element->key) << " " << element->keylength << " " << (char*)(element->payload) << " " << element->payloadlength << " successful" << std::endl;
			Sleep(1000);
		}
		delete ch, element;
		if (!FreeLibrary(hmdll)) throw " free library failed";
	}
	catch (const char* ch) {
		std::cout << ch;
	}
}