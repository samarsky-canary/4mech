#pragma once
#include <iostream>
#include  "names.h"
class Memory
{
public:
	Memory(){
		_memory = new uint16_t[65536];
	}
	~Memory(){
		delete[] _memory;
	}

	void debug(const uint16_t& offset, const uint16_t& size);
	void push_data(const uint16_t& offset, const datas& data, const uint8_t& size);

	template <typename T>
	void push_data(const uint16_t& offset, T data)
	{
		datas num;
		(sizeof(T) == 2) ? num.WordMem[0] = data : num.DwordMem = data;

		for (size_t i = 0; i < sizeof(T) / 2; i++)
			* (_memory + offset + i) = num.WordMem[i];
	}
	template<>
	void push_data<float>(const uint16_t& offset, float data)
	{
		datas num;
		num.real = data;
		for (size_t i = 0; i < sizeof(float) / 2; i++)
			* (_memory + offset + i) = num.WordMem[i];
	}

	


	datas load_data(const uint16_t& offset, const uint8_t& size);

	template <typename T>
	T load_data(const uint16_t& offset)
	{
		datas num;
		for (size_t i = 0; i < sizeof(T) / 2; i++)
			num.WordMem[i] = *(_memory + offset + i);

		return (sizeof(T) == 2) ? static_cast<T>(num.WordMem[0]) : static_cast<T>(num.DwordMem);
	}

	template <>
	float load_data<float>(const uint16_t& offset)
	{
		datas num;
		for (size_t i = 0; i < sizeof(float) / 2; i++)
			num.WordMem[i] = *(_memory + offset + i);
		return num.real;
	}

private:
	uint16_t *_memory;
};

