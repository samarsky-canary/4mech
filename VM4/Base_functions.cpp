#include "Base_functions.h"

using namespace  std;
uint8_t get_size(const datas& cmd)
{
	if (cmd.cmdw.KOP >= 40 && cmd.cmdw.KOP <= 44 && cmd.cmdw.dd == 1)
		return 1;

	switch (cmd.cmdw.dd)
	{
	case 0:
		return 1;
	case 1:
		return 2;
	case 2:
		return 2;
	case 3:
		return 3;
	default:
		return 0;
	}
}

uint8_t get_size(const comandW& cmd)
{
	if (cmd.dd == 1 || cmd.dd == 2)
		return 2;
	else if (cmd.dd == 3)
		return 3;
	return 1;
}

void cmd_debug(const comR& cmd, const uint8_t& cmd_size)
{
	cout << endl << (int)cmd.w3.KOP << " ";
	cout << (int)cmd.w3.s << " ";
	cout << (int)cmd.w3.dd << " ";
	cout << (int)cmd.w3.r1 << " ";
	cout << (int)cmd.w3.r2 << " ";
	if (cmd_size > 1)
	cout << (int)cmd.w3.o1 << " ";
	if (cmd_size > 2)
	cout << (int)cmd.w3.o2;
}

void line_debug(vector<string>& args)
{
	for (auto& i : args)
		cout << i << " ";
	cout << endl;
}

bool is_correct_data(const uint16_t& size_data, const adress& r1, const adress& r2)
{
	if(size_data == LONG && (r1 % 2 == 1 || r2 % 2 == 1))
	{
		return false;
	}
	return true;
}

void debug_cmd_bitout(datas& cmd, size_t lngth)
{
	for (auto i = 0; i < lngth; i++)
	{
		for (int j = sizeof(uint16_t) * 8 - 1; j >= 0; j--)
			printf("%d", cmd.WordMem[i] & (1 << j) ? 1 : 0);
		cout << ' ';
	}
	cout << endl;
}