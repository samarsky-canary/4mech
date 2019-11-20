#include "Memory.h"

void Memory::debug(const uint16_t& offset, const uint16_t& size)
{
	uint16_t end = offset + size;
	uint16_t begin = offset;
	for (; begin < end; begin++)
		for (int j = sizeof(uint16_t) * 8 - 1; j >= 0; j--)
			printf("%d", *(_memory + begin) & (1 << j) ? 1 : 0);

	std::cout << " ||";
	for (int i = 0; i < size/2; i++)
		printf("%X ", *(_memory + offset + i));	// в 16-ом виде

}

void Memory::push_data(const uint16_t& offset, const datas& data, const uint8_t& size)
{
	for (size_t i = 0; i < size; i++)
		* (_memory + offset + i) = data.WordMem[i];
}

datas Memory::load_data(const uint16_t& offset, const uint8_t& size)
{
	datas data;
	for (size_t i = 0; i < size; i++)
	{
		data.WordMem[i] = *(_memory + offset + i);
	}
	return data;
}


