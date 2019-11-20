#pragma once
#include <iostream>

using address = uint16_t;

#pragma pack(push,1)
struct PSW
{				//16 + (2 + 6) + 8
	address IP;
	uint8_t zf : 1;
	uint8_t nf : 1;
	uint8_t es : 1; //аварийное состояние, если 1 - пк завершает работу
	uint8_t rez;
};
#pragma pack(pop)

#pragma pack(push,1)
struct comandW
{
	uint8_t KOP : 7;
	uint8_t s : 1;
	uint8_t dd : 2;
	uint8_t r1 : 3;
	uint8_t r2 : 3;
};
#pragma pack(pop)

#pragma pack(push,1)
struct comand2W
{
	uint8_t KOP : 7;
	uint8_t s : 1;
	uint8_t dd : 2;
	uint8_t r1 : 3;
	uint8_t r2 : 3;
	uint16_t o1;
};
#pragma pack(pop)

#pragma pack(push,1)
struct comand3W
{
	uint8_t KOP : 7;
	uint8_t s : 1;
	uint8_t dd : 2;
	uint8_t r1 : 3;
	uint8_t r2 : 3;
	uint16_t o1;
	uint16_t o2;
};
#pragma pack(pop)

// регистр команд
#pragma pack(push,1)
union comR
{
	comandW w;
	comand2W w2;
	comand3W w3;
};
#pragma pack(pop)

#pragma pack(push,1)
union POH
{
	uint16_t word[8];		// short signed
	int32_t dword[4];		// long signed
	float    real[4];		// float
};
#pragma pack(pop)

#pragma pack (push, 1)
union datas				// данные ВМ
{
	uint16_t WordMem[3];
	int32_t DwordMem;
	int16_t num16;
	comandW cmdw;
	comand2W cmd2w;
	comand3W cmd3w;
	float real;
};
#pragma pack(pop)