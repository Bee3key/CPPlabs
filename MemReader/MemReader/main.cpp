#include <iostream>
#include "MemRead.h"

void main() 
{
	MemRead* mem = new MemRead("grimrock2.exe");
	mem->OpenProc();
	printf("PID: %X \n", mem->ShowPID());
	/*printf("String: %s \n", mem->ReadMem(0x00174B33, 32).toStringA() );
	printf("Test Float: %f \n", mem->ReadMem(0x02750E38, 4).toFloat());

	float health = 166.f;
	mem->WriteMem(&health, 0x02750E38, 4);

	printf("Test Float: %f \n", mem->ReadMem(0x02750E38, 4).toFloat());*/

	DWORD addr = mem->Alloc();
	printf("Allocated memory at 0x%X \n", addr);

//3180000
	mem->Free(0x3180000);
	system("PAUSE");

}