#include <iostream>
#include <Windows.h>
#include "C:\\Users\\User\\Documents\\Study_6sem\\ОСиСП\\HT.h"
#define CORR_NUM_PARMS 2
#define GET_OPEN_PARAMS (HT::HTHANDLE*(*)(const wchar_t[]))
#define GET_CLOSE_PARAMS (HT::HTHANDLE*(*)(const HT::HTHANDLE*))
int wmain(int argc, wchar_t* argv[]) {
	try {
		if (argc < CORR_NUM_PARMS) throw "write filename in params";
		HMODULE hmdll = LoadLibrary(L"C:\\Users\\User\\Documents\\Study_6sem\\ОСиСП\\Lab11\\x64\\Debug\\Lab11.dll");
		if (!hmdll) throw "load library failed";
		auto Open = GET_OPEN_PARAMS GetProcAddress(hmdll, "Open");
		auto Close = GET_CLOSE_PARAMS GetProcAddress(hmdll, "Close");
		if (!Open) throw "getting open failed";
		if (!Close) throw "getting close failed";
		HT::HTHANDLE* ht = Open(argv[1]);
		if (!ht) throw "open file failed";
		std::wcout << "HT-Storage Created filename = " << argv[1] << ", snapshotinterval = "
			<< ht->SecSnapshotInterval << ", capacity = " << ht->Capacity << " , maxkeylength = "
			<< ht->MaxKeyLength << ", maxdatalength = " << ht->MaxPayloadLength << std::endl;
		char ch = ' ';
		std::cin >> ch;
		if (Close(ht))
			std::cout << "Start closed";
		if (!FreeLibrary(hmdll)) throw "free library failed";
	}
	catch (const char* error) {
		std::cout << "Error: " << error << std::endl;
	}
}