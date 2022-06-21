#include <iostream>
#include "C:\\Users\\User\\Documents\\Study_6sem\\ОСиСП\\HT.h"
#include <ctype.h>
#include <Windows.h>
#pragma warning (disable:4996)
#define CORR_NUM_PARMS 6
#define GET_PROC_PARAMS (HT::HTHANDLE*(*)(int, int, int, int, const wchar_t[]))
int wmain(int argc, wchar_t* argv[]) {
	try {
		if (argc != CORR_NUM_PARMS) throw "write htname, snapshotinterval, capacity, maxkeylength, maxdatalength with file";
		HMODULE hmdll = LoadLibrary(L"C:\\Users\\User\\Documents\\Study_6sem\\ОСиСП\\Lab11\\x64\\Debug\\Lab11.dll");
		if (!hmdll)	throw "load library failed";
		auto Create = GET_PROC_PARAMS GetProcAddress(hmdll, "Create");
		if (!Create) throw "getting create failed";		
		HT::HTHANDLE* ht = Create(_wtoi(argv[3]), _wtoi(argv[2]), _wtoi(argv[4]), _wtoi(argv[5]), argv[1]);
		if (!ht) throw "create file failed";
		std::wcout << "HT-Storage Created filename = " << argv[1] << ", snapshotinterval = "
			<< _wtoi(argv[2]) << ", capacity = " << _wtoi(argv[3]) << " , maxkeylength = "
			<< _wtoi(argv[4]) << ", maxdatalength = " << _wtoi(argv[5]) << std::endl;
		if (!FreeLibrary(hmdll)) throw "free library failed";
	}
	catch (const char* ch) {
		std::cout << "Error: " << ch << std::endl;
	}
}