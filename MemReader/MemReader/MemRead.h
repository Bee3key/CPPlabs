#pragma once

#include <windows.h>
#include <TlHelp32.h>

class MemRead
{
public:
	MemRead(char* procName, size_t bufferSize = 32); /*Class memory reader*/
	~MemRead();

	/*Functions*/

	/*Process handle*/
	void OpenProc(DWORD accessRights = PROCESS_ALL_ACCESS); /*Open Proccess*/
	void CloseProc(); /*Close Process*/
	DWORD ShowPID(); /*Show process*/

	/* Writing to process memory*/
	void WriteMem(void* ptr, DWORD to, size_t size );
	void WriteMem(void* ptr, DWORD to, size_t size, DWORD memProtect);

	/*Reading from process memory*/
	MemRead& ReadMem(DWORD from, size_t size);
	MemRead& ReadMem(DWORD from, size_t size, DWORD memProtect);
	DWORD ReadPointer(DWORD base, DWORD* offset, int count); /*Чтение указателя 
															 параметры: база указателя, смещения указателя, количество смещений */
	DWORD Alloc(DWORD memProtect = PAGE_EXECUTE_READWRITE);
	void Free(DWORD ptr);

	/*Teach our class to return data of all types */

	double toDouble();
	float  toFloat();

	UINT32 toUINT32();
	int    toINT();

	UINT64 toUINT64();
	INT64 toINT64();

	UINT16 toUINT16();
	short int toINT16();

	UINT8 toUINT8();
	char  toChar();

	char* toStringA();
	wchar_t* toStringW();

private:

	void   GetPID();				/*find porcess ID*/

	char*  m_pName;			    /*Process name*/
	HANDLE m_hProc;				    /*Process handle*/
	DWORD  m_pID;				    /*Process ID*/
	byte*  m_buffer;				/*Buffer*/
	size_t m_bufferSize;			/*Buffer Size*/


};

