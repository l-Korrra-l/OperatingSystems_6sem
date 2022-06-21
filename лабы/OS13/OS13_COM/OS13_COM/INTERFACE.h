#pragma once
#include <objbase.h>
#include "OS11_HTAPI.h"

#define FNAME L"Kravts13.CA.COM"
#define VINDX L"Kravts13.CA.1"
#define PRGID L"Kravts13.CA"

// {F2530037-63FD-45FB-9EA0-43D237C8E6F0}
static const GUID CLSID_CA =
{ 0xf2530037, 0x63fd, 0x45fb, { 0x9e, 0xa0, 0x43, 0xd2, 0x37, 0xc8, 0xe6, 0xf0 } };


// {2DD760A3-CB4E-4964-80A0-E0DF715C7184}
static const GUID IID_IOS13 =
{ 0x2dd760a3, 0xcb4e, 0x4964, { 0x80, 0xa0, 0xe0, 0xdf, 0x71, 0x5c, 0x71, 0x84 } };

interface IOS13 :IUnknown {
    STDMETHOD(Add(const double, const double, double*)) PURE;
    
	STDMETHOD(Sub(const double, const double, double*)) PURE;

	STDMETHOD(Create   //  ������� HT             
	(
		int	  Capacity,					   // ������� ���������
		int   SecSnapshotInterval,		   // ������������� ���������� � ���.
		int   MaxKeyLength,                // ������������ ������ �����
		int   MaxPayloadLength,            // ������������ ������ ������
		const wchar_t  FileName[512],          // ��� ����� 
		HT::HTHANDLE** result
	)) PURE; 	// != NULL �������� ����������  

	STDMETHOD(Open     //  ������� HT             
	(
		const wchar_t    FileName[512],         // ��� ����� 
		HT::HTHANDLE** result
	)) PURE; 	// != NULL �������� ����������  

	STDMETHOD(Close     //  ������� HT             
	(
		HT::HTHANDLE** hthandle           // ���������� HT (File, FileMapping)
	)) PURE; 	// != NULL �������� ���������� 

	STDMETHOD(OpenExist     //  ������� HT             
	(
		const wchar_t    FileName[512],         // ��� ����� 
		HT::HTHANDLE** result
	)) PURE; 	// != NULL �������� ���������� 

	STDMETHOD(Snap     //  ������� HT             
	(
		HT::HTHANDLE** hthandle
	)) PURE; 	// != NULL �������� ���������� 

	STDMETHOD(Insert     //  ������� HT             
	(
		HT::HTHANDLE** hthandle,            // ���������� HT
		HT::Element** element              // �������
	)) PURE; 	// != NULL �������� ����������

	STDMETHOD(Get     //  ������� HT             
	(
		HT::Element** result,
		HT::HTHANDLE** hthandle,            // ���������� HT
		HT::Element** element              // ������� 
	)) PURE; 	// != NULL �������� ����������

	STDMETHOD(Delete     //  ������� HT             
	(
		HT::HTHANDLE** hthandle,            // ���������� HT (����)
		HT::Element** element              // ������� 
	)) PURE; 	// != NULL �������� ����������

	STDMETHOD(Update     //  ������� HT             
	(
		HT::HTHANDLE** hthandle,            // ���������� HT
		HT::Element** oldelement,          // ������ ������� (����, ������ �����)
		void** newpayload,          // ����� ������  
		int*   newpayloadlength     // ������ ����� ������
	)) PURE; 	// != NULL �������� ����������

	STDMETHOD(GetLastErrorProg     //  ������� HT             
	(
		std::string& error,
		HT::HTHANDLE** ht                         // ���������� HT
	)) PURE; 	// != NULL �������� ����������

	STDMETHOD(CreateElement     //  ������� HT             
	(
		std::string* key,
		HT::Element** el                         // ���������� HT
	)) PURE; 	// != NULL �������� ����������

	STDMETHOD(CreateElement     //  ������� HT             
	(
		std::string* key,
		std::string* payload,
		HT::Element** el                         // ���������� HT
	)) PURE; 	// != NULL �������� ����������

	STDMETHOD(print     //  ������� HT             
	(
		HT::Element** element              // ������� 
	)) PURE; 	// != NULL �������� ����������
};

