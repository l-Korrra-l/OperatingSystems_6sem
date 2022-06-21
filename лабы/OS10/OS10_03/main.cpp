#pragma comment(lib, "C:/Users/User/Documents/Study_6sem/�����/lab1-file-mapping/x64/Debug/OS10_HTAPI.lib")
#include "../OS10_HTAPI/pch.h"
#include "../OS10_HTAPI/HT.h"

using namespace std;

int main()
{
	ht::HtHandle* ht1 = nullptr;
	ht::HtHandle* ht2 = nullptr;
	try
	{
		cout << "---- 1 ----" << endl;
		ht1 = ht::create(1000, 4, 10, 256, L"./files/HTspace.ht");
		if (ht1)
			cout << "-- create: success" << endl;
		else
			throw "-- create: error";

		cout << "ht1: " << ht1->addr << endl;

		if (ht::insert(ht1, new ht::Element("key", 4, "payload", 8)))
			cout << "-- insert: success" << endl;
		else
			throw "-- insert: error";

		cout << "\n---- 2 ----" << endl;
		ht2 = ht::open(L"./files/HTspace.ht");
		if (ht2)
			cout << "-- create: success" << endl;
		else
			throw "-- create: error";

		cout << "ht2: " << ht2->addr << endl;

		ht::Element* hte = ht::get(ht1, new ht::Element("key", 4));
		if (hte)
			cout << "-- get: success" << endl;
		else
			throw "-- get: error";

		ht::print(hte);

		if (ht1 != nullptr)
			if (ht::close(ht1))
				cout << "-- close: success (ht1)" << endl;
			else
				throw "-- close: error (ht1)";

		if (ht2 != nullptr)
			if (ht::close(ht2))
				cout << "-- close: success (ht2)" << endl;
			else
				throw "-- close: error (ht2)";
	}
	catch (const char* msg)
	{
		cout << msg << endl;

		if (ht1 != nullptr)
			cout << ht::getLastError(ht1) << endl;
	}
}