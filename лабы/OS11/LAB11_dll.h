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
		int     Capacity;               // емкость хранилища в количестве элементов 
		int     SecSnapshotInterval;    // переодичность сохранения в сек. 
		int     MaxKeyLength;           // максимальная длина ключа
		int     MaxPayloadLength;       // максимальная длина данных
		int		numberOfElements;		// текущее количество элементов в таблице
		wchar_t FileName[512];          // имя файла 
		char    LastErrorMessage[512];  // сообщение об последней ошибке или 0x00  
		char	HTUsersGroup[512];		// Группа OS-пользователей
		HANDLE  File;                   // File HANDLE != 0, если файл открыт
		HANDLE  FileMapping;            // Mapping File HANDLE != 0, если mapping создан 
		HANDLE	Mutex;
		HANDLE	Thread;
		LPVOID  Addr;                   // Addr != NULL, если mapview выполнен  
		time_t  lastsnaptime;           // дата последнего snap'a (time())  
	};
	struct Element   // элемент 
	{
		Element();
		Element(const void* key, int keylength);                                             // for Get
		Element(const void* key, int keylength, const void* payload, int  payloadlength);    // for Insert
		Element(Element * oldelement, const void* newpayload, int  newpayloadlength);         // for update
		const void* key;                 // значение ключа 
		int         keylength;           // рахмер ключа
		const void* payload;             // данные 
		int         payloadlength;       // размер данных
	};
	extern "C" LAB11_API HTHANDLE * Create(
		int	  Capacity,					   // емкость хранилища
		int   SecSnapshotInterval,		   // переодичность сохранения в сек.
		int   MaxKeyLength,                // максимальный размер ключа
		int   MaxPayloadLength,            // максимальный размер данных
		const wchar_t  FileName[512]       // имя файла 
	);
	//extern "C" LAB11_API HTHANDLE * Open(	// открыть HT, пользователь HTUsersGroup
	//	const char HTUser[256],				// HT-пользователь
	//	const char HTPassword[256],			// пароль
	//	const char FileName[512]			// имя файла
	//); // != NULL успешное завершение
	extern "C" LAB11_API HTHANDLE * Open(	// открыть HT, текущий пользователь HTUsersGroup
		const wchar_t FileName[512]
	);
	extern "C" LAB11_API BOOL Snap(
		const HTHANDLE* hthandle
	);
	extern "C" LAB11_API BOOL Close(
		const HTHANDLE* hthandle
	);
	extern "C" LAB11_API BOOL Insert      // добавить элемент в хранилище
	(
		HTHANDLE* hthandle,            // управление HT
		const Element* element              // элемент
	);	//  == TRUE успешное завершение 


	extern "C" LAB11_API BOOL Delete      // удалить элемент в хранилище
	(
		HTHANDLE* hthandle,            // управление HT (ключ)
		const Element* element              // элемент 
	);	//  == TRUE успешное завершение 

	extern "C" LAB11_API Element* Get     //  читать элемент в хранилище
	(
		const HTHANDLE* hthandle,            // управление HT
		const Element* element              // элемент 
	); 	//  != NULL успешное завершение 


	extern "C" LAB11_API BOOL Update     //  именить элемент в хранилище
	(
		HTHANDLE* hthandle,            // управление HT
		const Element* oldelement,          // старый элемент (ключ, размер ключа)
		const void* newpayload,          // новые данные  
		int             newpayloadlength     // размер новых данных
	); 	//  != NULL успешное завершение 

	extern "C" LAB11_API Element * CreateElementG(const void* key, int keylength); // for Get

	extern "C" LAB11_API Element * CreateElementI(const void* key, int keylength, const void* payload, int payloadlength);// for Insert

	extern "C" LAB11_API Element * CreateElementU(Element * oldelement, const void* newpayload, int newpayloadlength);//for Update

	extern "C" LAB11_API void print                               // распечатать элемент 
	(
		const Element* element              // элемент 
	);

}