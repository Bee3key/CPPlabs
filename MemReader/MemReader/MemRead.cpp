#include "MemRead.h"


MemRead::MemRead(char* procName, size_t bufferSize) :m_pID(0), m_bufferSize(bufferSize)
{
	int len = strlen(procName) + 1;
	m_pName = new char[len]; /*allocation memory for process*/
	strcpy_s(m_pName, len, procName);
	m_buffer = new byte[bufferSize];

}


MemRead::~MemRead()
{
	delete[] m_pName;
	delete[] m_buffer;
}


void MemRead::GetPID()  /*Find target process in stack*/
{
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); /*Takes a snapshot of the specified processes*/
	
	/*Structure of process list initialization*/
	PROCESSENTRY32 pInfo;  
	pInfo.dwSize = sizeof (PROCESSENTRY32); /*Must be set up or fails*/


	if (Process32First(snapshot, &pInfo))
	{
		while (Process32Next(snapshot, &pInfo))
		{
			if (_stricmp(m_pName, pInfo.szExeFile) == 0) /* _strpcmp for good*/
			{
				m_pID = pInfo.th32ProcessID;			/*If target process in the list then get it PID*/
				CloseHandle(snapshot);
				return;
			}
		}
	}
	CloseHandle(snapshot); /* In case seach failes */
	m_pID = 0;
	return;
}

/*Lets open found process*/

void MemRead::OpenProc(DWORD accessRights)
{
	GetPID();
	m_hProc = OpenProcess(accessRights,false, m_pID);
}

void MemRead::CloseProc()
{
	CloseHandle(m_hProc);
}

DWORD MemRead::ShowPID()
{
	return m_pID;
}

void MemRead::WriteMem(void* ptr, DWORD to, size_t size) {	WriteProcessMemory(m_hProc, (LPVOID)to, ptr, size, NULL); }
void MemRead::WriteMem(void* ptr, DWORD to, size_t size, DWORD memProtect)
{
	DWORD lastProtect = 0; /*last memory protection setting*/

	VirtualProtectEx(m_hProc, (LPVOID)to, size, memProtect, &lastProtect);  /*Change protection of region to new setup
																			TODO add checking for size param*/
	WriteProcessMemory(m_hProc, (LPVOID)to, ptr, size, NULL);

	VirtualProtectEx(m_hProc, (LPVOID)to, size, lastProtect, &lastProtect); /* Restor previos protection param*/

}

MemRead& MemRead::ReadMem(DWORD from, size_t size) 
{
	//if (size > m_bufferSize) return;   /* TODO: Checking size*/
	memset(m_buffer, 0, m_bufferSize); /* Fill buffer with nulls*/
	ReadProcessMemory(m_hProc, (LPVOID)from, m_buffer, size, NULL);
	return *this;
}
MemRead& MemRead::ReadMem(DWORD from, size_t size, DWORD memProtect)
{
	DWORD lastProtect = 0;
	VirtualProtectEx(m_hProc, (LPVOID)from, size, memProtect, &lastProtect);

	//if (size > m_bufferSize) return;   /*TODO:  Checking size*/
	memset(m_buffer, 0, m_bufferSize); /* Fill buffer with nulls*/

	ReadProcessMemory(m_hProc, (LPVOID)from, m_buffer, size, NULL);
	VirtualProtectEx(m_hProc, (LPVOID)from, size, lastProtect, &lastProtect);
	return *this;
}

double MemRead::toDouble()
{
	double var = 0;
	memcpy(&var, m_buffer, sizeof(double));
	return var;
}
float  MemRead::toFloat()
{
	float var = 0;
	memcpy(&var, m_buffer, sizeof(float));
	return var;
}

UINT32 MemRead::toUINT32()
{
	UINT32 var = 0;
	memcpy(&var, m_buffer, sizeof(UINT32));
	return var;
}
int    MemRead::toINT()
{
	int var = 0;
	memcpy(&var, m_buffer, sizeof(int));
	return var;
}

UINT64 MemRead::toUINT64()
{
	UINT64 var = 0;
	memcpy(&var, m_buffer, sizeof(UINT64));
	return var;
}

INT64 MemRead::toINT64()
{
	INT64 var = 0;
	memcpy(&var, m_buffer, sizeof(INT64));
	return var;
}

UINT16 MemRead::toUINT16()
{
	UINT16 var = 0;
	memcpy(&var, m_buffer, sizeof(UINT16));
	return var;
}
short int MemRead::toINT16()
{
	short int var = 0;
	memcpy(&var, m_buffer, sizeof(short int));
	return var;
}

UINT8 MemRead::toUINT8()
{
	UINT8 var = 0;
	memcpy(&var, m_buffer, sizeof(UINT8));
	return var;
}
char  MemRead::toChar()
{
	char var = 0;
	memcpy(&var, m_buffer, sizeof(char));
	return var;
}

char* MemRead::toStringA()
{	
	return (char*)m_buffer; /* for memmory economy*/
}
wchar_t* MemRead::toStringW()
{
	return (wchar_t*)m_buffer;
}
/*
DWORD MemRead::ReadPointer(DWORD base, DWORD* offset, int count)
{
	for (int i = 0; i < count - 1; i++)
	{
		base = ReadMem(base + offset[i], 4).toUINT16;  // читаем по 4 байта так как адреса занимают 32 битные 4 байта, и приводим к UINT32
	}
	return base + offset[count - 1]; count - 1 - для того что бы возвращать адрес, а не значение
}   
*/
DWORD MemRead::Alloc(DWORD memProtect) { return (DWORD)VirtualAllocEx(m_hProc, NULL, 0x1000, MEM_COMMIT | MEM_RESERVE, memProtect);}
void MemRead::Free(DWORD ptr) { VirtualFreeEx(m_hProc, (LPVOID) ptr, 0, MEM_RELEASE); }