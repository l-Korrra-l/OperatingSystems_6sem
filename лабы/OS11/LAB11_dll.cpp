#include "pch.h"
#include "LAB11_dll.h"
#include <iostream>
#pragma warning (disable:4996)
namespace HT {

	HTHANDLE::HTHANDLE() :Capacity(0), SecSnapshotInterval(0), MaxKeyLength(0), MaxPayloadLength(0), FileName(L""), numberOfElements(0) {};
	HTHANDLE::HTHANDLE(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const wchar_t FileName[512], int numberOfElements) {
		this->Capacity = Capacity;
		this->SecSnapshotInterval = SecSnapshotInterval;
		this->MaxKeyLength = MaxKeyLength;
		this->MaxPayloadLength = MaxPayloadLength;
		this->numberOfElements = numberOfElements;
		wcscpy_s(this->FileName, FileName);
	};
	Element::Element() :key(new void*), keylength(0), payload(new void*), payloadlength(0) { }
	Element::Element(const void* key, int keylength) : key(key), keylength(keylength), payload(new void*), payloadlength(0) {}
	Element::Element(const void* key, int keylength, const void* payload, int payloadlength) : key(key), keylength(keylength), payload(payload), payloadlength(payloadlength) {}
	Element::Element(Element* oldelement, const void* newpayload, int newpayloadlength) : key(oldelement->key), keylength(oldelement->keylength), payload(newpayload), payloadlength(newpayloadlength) {}
	Element* CreateElementG(const void* key, int keylength) {
		return new Element(key, keylength);
	}
	Element* CreateElementI(const void* key, int keylength, const void* payload, int payloadlength) {
		return new Element(key, keylength, payload, payloadlength);
	}
	Element* CreateElementU(Element* oldElement, const void* newpayload, int newpayloadlength) {
		return new Element(oldElement, newpayload, newpayloadlength);
	}
	HTHANDLE* getSizeInfoView(LPVOID lp) {
		HTHANDLE* ht = new HTHANDLE();
		if (!lp) {
			strcpy_s(ht->LastErrorMessage, "Error:");
			return NULL;
		}
		try {
			memcpy(&ht->Capacity, lp, sizeof(int));
			memcpy(&ht->SecSnapshotInterval, (char*)lp + sizeof(int), sizeof(int));
			memcpy(&ht->MaxKeyLength, (char*)lp + 2 * sizeof(int), sizeof(int));
			memcpy(&ht->MaxPayloadLength, (char*)lp + 3 * sizeof(int), sizeof(int));
			memcpy(&ht->numberOfElements, (char*)lp + 4 * sizeof(int), sizeof(int));
		}
		catch (const char* err) {
			strcpy_s(ht->LastErrorMessage, err);
			return NULL;
		}
		return ht;
	}
	HTHANDLE* getSizeInfo(HANDLE File) {
		HTHANDLE* ht = new HTHANDLE();
		HANDLE hm = CreateFileMapping(File, NULL, PAGE_READWRITE, 0, 5 * sizeof(int), NULL);
		if (!hm) {
			strcpy_s(ht->LastErrorMessage, "Error:");
			return NULL;
		}
		LPVOID lp = MapViewOfFile(hm, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if (!lp) {
			strcpy_s(ht->LastErrorMessage, "Error:");
			return NULL;
		}
		try {
			memcpy(&ht->Capacity, lp, sizeof(int));
			memcpy(&ht->SecSnapshotInterval, (char*)lp + sizeof(int), sizeof(int));
			memcpy(&ht->MaxKeyLength, (char*)lp + 2 * sizeof(int), sizeof(int));
			memcpy(&ht->MaxPayloadLength, (char*)lp + 3 * sizeof(int), sizeof(int));
			memcpy(&ht->numberOfElements, (char*)lp + 4 * sizeof(int), sizeof(int));
		}
		catch (const char* err) {
			strcpy_s(ht->LastErrorMessage, err);
			if (!UnmapViewOfFile(lp)) throw "unmapping view failed";
			if (!CloseHandle(hm)) throw "close file mapping failed";
			return NULL;
		}
		if (!UnmapViewOfFile(lp)) throw "unmapping view failed";
		if (!CloseHandle(hm)) throw "close file mapping failed";
		return ht;
	}
	BOOL insertSizeInfoToFile(HTHANDLE* hthandle) {
		try {
			memcpy(hthandle->Addr, &hthandle->Capacity, sizeof(int));
			memcpy((char*)hthandle->Addr + sizeof(int), &hthandle->SecSnapshotInterval, sizeof(int));
			memcpy((char*)hthandle->Addr + 2 * sizeof(int), &hthandle->MaxKeyLength, sizeof(int));
			memcpy((char*)hthandle->Addr + 3 * sizeof(int), &hthandle->MaxPayloadLength, sizeof(int));
			memcpy((char*)hthandle->Addr + 4 * sizeof(int), &hthandle->numberOfElements, sizeof(int));
			hthandle->Addr = (char*)hthandle->Addr + 4 * sizeof(int);
		}
		catch (const char* err) {
			strcpy_s(hthandle->LastErrorMessage, err);
			return FALSE;
		}
		return TRUE;
	}
	BOOL updateElementHT(HTHANDLE* hthandle, const void* newpayload, int newpayloadint, int N) {
		try {
			void* part = new (hthandle->Addr)char + (hthandle->MaxKeyLength + hthandle->MaxPayloadLength + 2 * sizeof(int)) * N + hthandle->MaxKeyLength + sizeof(int);
			memset(part, 0, hthandle->MaxPayloadLength);
			memset((char*)part + hthandle->MaxPayloadLength, 0, sizeof(int));
			memcpy(part, newpayload, newpayloadint);
			memcpy((char*)part + hthandle->MaxPayloadLength, &newpayloadint, sizeof(int));
			return TRUE;
		}
		catch (const char* err) {
			strcpy(hthandle->LastErrorMessage, err);
			return FALSE;
		}
	}
	BOOL deleteElementHT(HTHANDLE* hthandle, int N) {
		try {
			void* part = new (hthandle->Addr)char + (hthandle->MaxKeyLength + hthandle->MaxPayloadLength + 2 * sizeof(int)) * N;
			memset(part, '\0', hthandle->MaxKeyLength);
			memset((char*)part + hthandle->MaxKeyLength, 0, sizeof(int));
			memset((char*)part + hthandle->MaxKeyLength + sizeof(int), '\0', hthandle->MaxPayloadLength);
			memset((char*)part + hthandle->MaxKeyLength + sizeof(int) + hthandle->MaxPayloadLength, 0, sizeof(int));
			return TRUE;
		}
		catch (const char* err) {
			strcpy(hthandle->LastErrorMessage, err);
			return FALSE;
		}
	}
	BOOL checkElementOfHT(const HTHANDLE* hthandle, const Element* element, int N) {
		void* part = new (hthandle->Addr)char + (hthandle->MaxKeyLength + hthandle->MaxPayloadLength + 2 * sizeof(int)) * N;
		return memcmp(part, element->key, element->keylength) == 0 ? TRUE : FALSE;
	}
	BOOL checkParmLength(const HTHANDLE* hthandle, const Element* element) {
		if (hthandle->MaxKeyLength < element->keylength || hthandle->MaxPayloadLength < element->payloadlength || hthandle->numberOfElements == hthandle->Capacity)
			return FALSE;
		return TRUE;
	}
	Element* getElementByIndex(const HTHANDLE* hthandle, int N) {
		void* part = new (hthandle->Addr)char + (hthandle->MaxKeyLength + hthandle->MaxPayloadLength + 2 * sizeof(int)) * N;
		Element* element = new Element();
		element->key = part;
		element->keylength = *(int*)((char*)part + hthandle->MaxKeyLength);
		if (element->keylength <= 0 || element->keylength > hthandle->MaxKeyLength) {
			delete element;
			strcpy((char*)hthandle->LastErrorMessage, "Error: value didnt find");
			return NULL;
		}
		element->payload = (char*)part + hthandle->MaxKeyLength + sizeof(int);
		element->payloadlength = *(int*)((char*)part + hthandle->MaxKeyLength + sizeof(int) + hthandle->MaxPayloadLength);
		ReleaseMutex(hthandle->Mutex);
		return element;
	}
	void inputToHT(const HTHANDLE* hthandle, const Element* element, int N) {
		void* part = new (hthandle->Addr)char + (hthandle->MaxKeyLength + hthandle->MaxPayloadLength + 2 * sizeof(int)) * N;
		memcpy((char*)part, (char*)element->key, element->keylength);
		memcpy(((char*)part + hthandle->MaxKeyLength), &element->keylength, sizeof(int));
		memcpy(((char*)part + hthandle->MaxKeyLength + sizeof(int)), (char*)element->payload, element->payloadlength);
		memcpy(((char*)part + hthandle->MaxKeyLength + sizeof(int) + hthandle->MaxPayloadLength), &element->payloadlength, sizeof(int));
	}
	wchar_t* getNewSnapName() {
		srand(time(NULL));
		int nameLength = rand() % 20 + 15;
		wchar_t* name = new wchar_t[nameLength];
		for (int i = 0; i < nameLength; i++)
			name[i] = 66 + rand() % 25;
		name[nameLength] = '\0';
		return wcscat(name, L".ht");
	}
	int GetHash(const Element* elem, int size, int k) {
		char* arrKeyBytes = new char[elem->keylength];
		memcpy(arrKeyBytes, elem->key, elem->keylength);
		int sumBytes = 0;
		for (int i = 0; i < elem->keylength; i++)
			sumBytes += arrKeyBytes[i];
		double key = 3 * ((0.617012781010 * sumBytes) - int((0.617012781010 * sumBytes)));
		delete[] arrKeyBytes;
		return (k + sumBytes) % size;
	}
	int GetNextHash(int hash, int size, int k) {
		return (hash + 5 * k + 3 * k * k) % size;
	}
	DWORD WINAPI SnapCycle(HTHANDLE* ht) {
		while (true) {
			if (ht) {
				Snap(ht);
				Sleep(ht->SecSnapshotInterval * SECOND);
				ht->lastsnaptime = time(NULL);
			}
			else
				return NULL;
		}
		return TRUE;
	}
	char* GetLastError(HTHANDLE* hthandle) {
		return hthandle->LastErrorMessage;
	}

	HTHANDLE* Create(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const wchar_t FileName[512]) {
		HANDLE th = CreateFile(FileName, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (th == INVALID_HANDLE_VALUE) return NULL;
		DWORD HTsize = sizeof(HTHANDLE) + Capacity * (MaxKeyLength + MaxPayloadLength + sizeof(Element)) + INCREM_F_SIZE;
		HANDLE hm = CreateFileMapping(th, NULL, PAGE_READWRITE, 0, HTsize, NULL);
		if (!hm) return NULL;
		LPVOID lp = MapViewOfFile(hm, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if (!lp) return NULL;
		HTHANDLE* ht = new HTHANDLE(Capacity, SecSnapshotInterval, MaxKeyLength, MaxPayloadLength, FileName, 0);
		ht->File = th;
		ht->FileMapping = hm;
		ht->Addr = lp;
		if (!insertSizeInfoToFile(ht)) return NULL;
		if (!UnmapViewOfFile(lp)) return NULL; if(!CloseHandle(hm)) return NULL; if(!CloseHandle(th)) return NULL;
		return ht;
	};
	HTHANDLE* Open(const wchar_t FileName[512]) {
		HANDLE th = CreateFile(FileName, GENERIC_WRITE | GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		HTHANDLE* ht = new HTHANDLE();
		if (th == INVALID_HANDLE_VALUE) {
			HANDLE hm = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, MAPPING_NAME);
			if (!hm) return NULL;
			LPVOID lp = MapViewOfFile(hm, FILE_MAP_ALL_ACCESS, 0, 0, 0);
			if (!lp) return NULL;
			ht = getSizeInfoView(lp);
			HANDLE Mutex = OpenMutex(SYNCHRONIZE, FALSE, MUTEX_NAME);
			if (!Mutex) return NULL;
			ht->File = NULL;
			wcscpy_s(ht->FileName, FileName);
			ht->FileMapping = hm;
			ht->Addr = lp;
			ht->Mutex = Mutex;
			ht->lastsnaptime = time(NULL);
			DWORD thread = NULL;
		}
		else {
			ht = getSizeInfo(th);
			DWORD HTsize = sizeof(HTHANDLE) + ht->Capacity * (ht->MaxKeyLength + ht->MaxPayloadLength + sizeof(Element)) + INCREM_F_SIZE;
			HANDLE hm = CreateFileMapping(th, NULL, PAGE_READWRITE, 0, HTsize, MAPPING_NAME);
			if (!hm) return NULL;
			LPVOID lp = MapViewOfFile(hm, FILE_MAP_ALL_ACCESS, 0, 0, 0);
			if (!lp) return NULL;
			HANDLE mutex = CreateMutex(NULL, FALSE, MUTEX_NAME);
			if (!mutex) return NULL;
			wcscpy_s(ht->FileName, FileName);
			ht->File = th;
			ht->FileMapping = hm;
			ht->Addr = lp;
			ht->Mutex = mutex;
			ht->lastsnaptime = time(NULL);
			DWORD thread = NULL;
			if (!(ht->Thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SnapCycle, ht, 0, &thread)))
				return NULL;
		}
		return ht;
	}
	BOOL Insert(HTHANDLE* hthandle, const Element* element) {
		if (!checkParmLength(hthandle, element)) {
			strcpy(hthandle->LastErrorMessage, "invalid length of params");
			return FALSE;
		}
		bool success = false;
		if (Get(hthandle, element) != NULL) {
			strcpy(hthandle->LastErrorMessage, "Error Insert: can't insert values with same keys ");
			return FALSE;
		}
		void* vv = nullptr;
		WaitForSingleObject(hthandle->Mutex, INFINITY);
		for (int i = 0, j = GetHash(element, hthandle->Capacity, 0); j + i != hthandle->Capacity && !success; ++i) {
			void* part = new (hthandle->Addr)char + (hthandle->MaxKeyLength + hthandle->MaxPayloadLength + 2 * sizeof(int)) * (j + i);
				inputToHT(hthandle, element, j + i);
				success = true; hthandle->numberOfElements++;
		}
		ReleaseMutex(hthandle->Mutex);
		if (success)
			return TRUE;
		else {
			strcpy(hthandle->LastErrorMessage, "Error Insert: can't insert the value");
			return FALSE;
		}
	}
	Element* Get(const HTHANDLE* hthandle, const Element* element) {
		Element* rt = NULL;
		if (!checkParmLength(hthandle, element)) {
			strcpy((char*)hthandle->LastErrorMessage, "invalid length of params");
			return NULL;
		}
		for (int i = 0, j = GetHash(element, hthandle->Capacity, 0); j + i != hthandle->Capacity; i++)
			if (checkElementOfHT(hthandle, element, j + i))
				rt = getElementByIndex(hthandle, j + i);
		ReleaseMutex(hthandle->Mutex);
		return rt;
	}
	BOOL Delete(HTHANDLE* hthandle, const Element* element) {//mutex
		BOOL rt = FALSE;
		if (!checkParmLength(hthandle, element)) {
			strcpy(hthandle->LastErrorMessage, "invalid length of params");
			return NULL;
		}
		WaitForSingleObject(hthandle->Mutex, INFINITY);
		for (int i = 0, j = GetHash(element, hthandle->Capacity, 0); j + i != hthandle->Capacity; i++)
			if (checkElementOfHT(hthandle, element, j + i))
				rt = deleteElementHT(hthandle, j + i);
		ReleaseMutex(hthandle->Mutex);
		return rt;
	}
	BOOL Update(HTHANDLE* hthandle, const Element* element, const void* newpayload, int newpayloadint) {
		if (hthandle->MaxPayloadLength < newpayloadint) {
			strcpy(hthandle->LastErrorMessage, "invalid length of params");
			return FALSE;
		}
		WaitForSingleObject(hthandle->Mutex, INFINITY);
		for (int i = 0, j = GetHash(element, hthandle->Capacity, 0); j + i != hthandle->Capacity; i++) {
			if (checkElementOfHT(hthandle, element, j + i)) {
				return updateElementHT(hthandle, newpayload, newpayloadint, j + i);
				ReleaseMutex(hthandle->Mutex);
			}
		}
		ReleaseMutex(hthandle->Mutex);
		return FALSE;
	}
	BOOL Snap(const HTHANDLE* hthandle) {//mutex
		WaitForSingleObject(hthandle->Mutex, INFINITY);
		const wchar_t* filename = getNewSnapName();
		HANDLE ht = CreateFile(filename, GENERIC_WRITE | GENERIC_READ, NULL, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (ht == INVALID_HANDLE_VALUE) {
			strcpy((char*)hthandle->LastErrorMessage, "Error: cant create file snap");
			return FALSE;
		}
		DWORD HTSize = sizeof(HTHANDLE) + hthandle->Capacity * (hthandle->MaxKeyLength + hthandle->MaxPayloadLength + sizeof(Element));
		HANDLE mapping = CreateFileMapping(ht, NULL, PAGE_READWRITE, 0, HTSize, NULL);
		if (!mapping) {
			strcpy((char*)hthandle->LastErrorMessage, "Error: cant create filemapping snap");
			return FALSE;
		}
		LPVOID lp = MapViewOfFile(mapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if (!lp) {
			strcpy((char*)hthandle->LastErrorMessage, "Error: cant create view of file snap");
			return FALSE;
		}
		memcpy(lp, hthandle->Addr, HTSize);//проверить в свою ли память влазит
		ReleaseMutex(hthandle->Mutex);
		if (!UnmapViewOfFile(lp))
			return FALSE;
		if (!CloseHandle(mapping))
			return FALSE;
		if (!CloseHandle(ht))
			return FALSE;
		return TRUE;
	}
	BOOL Close(const HTHANDLE* hthandle) {
		WaitForSingleObject(hthandle->Thread, INFINITY);
		if (!TerminateThread(hthandle->Thread, 0)) {
			strcpy((char*)hthandle->LastErrorMessage, "Error: terminated thread");
			return FALSE;
		}
		if (!Snap(hthandle))
			return FALSE;
		if (!UnmapViewOfFile(hthandle->Addr)) {
			strcpy((char*)hthandle->LastErrorMessage, "Error: unmapping view failed");
			return FALSE;
		}
		if (!CloseHandle(hthandle->FileMapping)) {
			strcpy((char*)hthandle->LastErrorMessage, "Error: close file mapping failed");
			return FALSE;
		}
		if (!CloseHandle(hthandle->File)) {
			strcpy((char*)hthandle->LastErrorMessage, "Error: close file failed");
			return FALSE;
		}
		if (!CloseHandle(hthandle->Mutex)) {
			strcpy((char*)hthandle->LastErrorMessage, "Error: close Mutex failed");
			return FALSE;
		}
		delete hthandle;
		return TRUE;
	}
	void print(const Element* element) { std::cout << (char*)(*element).key << ' ' << (char*)(*element).payload << '\n'; }
}
