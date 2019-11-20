#include <sstream>
#include "Loader.h"
#include  "Base_functions.h"
using namespace std;

datas load_command(stringstream& str, uint8_t& cmd_size, Processor& p)
{
	datas cmd;
	uint16_t temp;
	str >> temp;	cmd.cmdw.KOP = temp;
	str >> temp;	cmd.cmdw.s = temp;
	str >> temp;	cmd.cmdw.dd = temp;
	str >> temp;	cmd.cmdw.r1 = temp;
	str >> temp;	cmd.cmdw.r2 = temp;

	cmd_size = get_size(cmd);
	switch (cmd_size)
	{
	case 1:
		break;
	case 2:
		str >> cmd.cmd2w.o1;
		break;
	case 3:
		str >> cmd.cmd2w.o1;
		str >> cmd.cmd3w.o2;
		break;
	default:
		break;
	}
	return cmd;
}

bool parse(fstream& file, Processor& p)
{
	datas data;
	string temp, prefix;
	uint16_t offset = 0, counter = 1;

	while (!file.eof())
	{
		getline(file, temp);
		stringstream command_line (temp);

		command_line >> prefix;
		uint8_t cmd_size = 0;
		switch(prefix[0])
		{
			case('a') :											/* load address*/
				command_line >> offset;
			break;

			case('i'):											/*set IP*/
				command_line >> p._psw.IP;
			break;

			case('s'):											/* load short signed 16bit num */
				command_line >> data.num16;
				cmd_size = 1;
			break;

			case('u'):											/* load short unsigned 16bit num */
				command_line >> data.WordMem[0];
				cmd_size = 1;

			break;

			case('l'):											/* load long signed 32bit num */
				command_line >> data.DwordMem;
				cmd_size = 2;
			break;

			case('f'):												/* load float 32bit num */
				command_line >> data.real;
				cmd_size = 2;
			break;

			case('c'):												/* load command*/
				data = load_command(command_line,cmd_size, p);
			break;

			case(';'):
				break;
			default:
				cout << "unknown prefix in Line"  << counter << endl;
				return false;

		}
		p._mem.push_data(offset, data, cmd_size);
		offset += cmd_size;
		counter++;
	}
	file.close();
	return true;
}