#pragma once
#define LAB11_API __declspec(dllexport)
#include <Windows.h>
#include <iostream>
#include <time.h>
namespace HT {
#define SECOND 1000
#define INCREM_F_SIZE 4*sizeof(int)
#define MUTEX_NAME L"mumutex"
#define MAPPING_NAME L"mapping"
	struct HTHANDLE {
		HTHANDLE();
		HTHANDLE(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const wchar_t FileName[512], int numberOfElements);
		int     Capacity;               // ������� ��������� � ���������� ��������� 
		int     SecSnapshotInterval;    // ������������� ���������� � ���. 
		int     MaxKeyLength;           // ������������ ����� �����
		int     MaxPayloadLength;       // ������������ ����� ������
		int		numberOfElements;		// ������� ���������� ��������� � �������
		wchar_t FileName[512];          // ��� ����� 
		char    LastErrorMessage[512];  // ��������� �� ��������� ������ ��� 0x00  
		char	HTUsersGroup[512];		// ������ OS-�������������
		HANDLE  File;                   // File HANDLE != 0, ���� ���� ������
		HANDLE  FileMapping;            // Mapping File HANDLE != 0, ���� mapping ������ 
		HANDLE	Mutex;
		HANDLE	Thread;
		LPVOID  Addr;                   // Addr != NULL, ���� mapview ��������  
		time_t  lastsnaptime;           // ���� ���������� snap'a (time())  
	};
	struct Element   // ������� 
	{
		Element();
		Element(const void* key, int keylength);                                             // for Get
		Element(const void* key, int keylength, const void* payload, int  payloadlength);    // for Insert
		Element(Element * oldelement, const void* newpayload, int  newpayloadlength);         // for update
		const void* key;                 // �������� ����� 
		int         keylength;           // ������ �����
		const void* payload;             // ������ 
		int         payloadlength;       // ������ ������
	};
	extern "C" LAB11_API HTHANDLE * Create(
		int	  Capacity,					   // ������� ���������
		int   SecSnapshotInterval,		   // ������������� ���������� � ���.
		int   MaxKeyLength,                // ������������ ������ �����
		int   MaxPayloadLength,            // ������������ ������ ������
		const wchar_t  FileName[512]       // ��� ����� 
	);
	//extern "C" LAB11_API HTHANDLE * Open(	// ������� HT, ������������ HTUsersGroup
	//	const char HTUser[256],				// HT-������������
	//	const char HTPassword[256],			// ������
	//	const char FileName[512]			// ��� �����
	//); // != NULL �������� ����������
	extern "C" LAB11_API HTHANDLE * Open(	// ������� HT, ������� ������������ HTUsersGroup
		const wchar_t FileName[512]
	);
	extern "C" LAB11_API BOOL Snap(
		const HTHANDLE* hthandle
	);
	extern "C" LAB11_API BOOL Close(
		const HTHANDLE* hthandle
	);
	extern "C" LAB11_API BOOL Insert      // �������� ������� � ���������
	(
		HTHANDLE* hthandle,            // ���������� HT
		const Element* element              // �������
	);	//  == TRUE �������� ���������� 


	extern "C" LAB11_API BOOL Delete      // ������� ������� � ���������
	(
		HTHANDLE* hthandle,            // ���������� HT (����)
		const Element* element              // ������� 
	);	//  == TRUE �������� ���������� 

	extern "C" LAB11_API Element* Get     //  ������ ������� � ���������
	(
		const HTHANDLE* hthandle,            // ���������� HT
		const Element* element              // ������� 
	); 	//  != NULL �������� ���������� 


	extern "C" LAB11_API BOOL Update     //  ������� ������� � ���������
	(
		HTHANDLE* hthandle,            // ���������� HT
		const Element* oldelement,          // ������ ������� (����, ������ �����)
		const void* newpayload,          // ����� ������  
		int             newpayloadlength     // ������ ����� ������
	); 	//  != NULL �������� ���������� 

	extern "C" LAB11_API Element * CreateElementG(const void* key, int keylength); // for Get

	extern "C" LAB11_API Element * CreateElementI(const void* key, int keylength, const void* payload, int payloadlength);// for Insert

	extern "C" LAB11_API Element * CreateElementU(Element * oldelement, const void* newpayload, int newpayloadlength);//for Update

	extern "C" LAB11_API void print                               // ����������� ������� 
	(
		const Element* element              // ������� 
	);

}