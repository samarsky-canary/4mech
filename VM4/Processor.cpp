#include <iomanip>
#include  "Processor.h"
using namespace std;



Processor::Processor()
{
	enum
	{
		Escape = 0,
		Transmit = 1,

		IntAdd = 10,
		Intsub = 11,
		Intmul = 12,
		Intdiv = 13,

		FlAdd = 15,
		FlSub = 16,
		FlMul = 17,
		FlDiv = 18,

		Inputc = 20,
		Outputreg = 21,
		Outputmem = 22,
		Saveip = 30,

		jump_u = 40,
		jump_jz = 41,
		jump_jnz = 42,
		jump_jgz = 43,
		jump_jlz = 44,
		jump_subprog = 50,
		jump_return = 51,

		all_cmd = 128

	};
	_commands = new Command * [all_cmd];

	_commands[Escape] = nullptr;
	_commands[Transmit] = new Transmition();

	_commands[IntAdd] = new MathInt();
	_commands[Intsub] = new MathInt();
	_commands[Intmul] = new MathInt();
	_commands[Intdiv] = new MathInt();

	_commands[FlAdd] = new MathFloat();
	_commands[FlSub] = new MathFloat();
	_commands[FlMul] = new MathFloat();
	_commands[FlDiv] = new MathFloat();

	_commands[Inputc] = new Input();
	_commands[Outputreg] = new Output_reg();
	_commands[Outputmem] = new Output_mem();

	_commands[jump_u] = new Transition_unc();
	_commands[jump_jz] = new Transition_JZ();
	_commands[jump_jnz] = new Transition_JNZ();
	_commands[jump_jgz] = new Transition_JGZ();
	_commands[jump_jlz] = new Transition_JlZ();
	_commands[jump_subprog] = new Transition_tosub();
	_commands[jump_return] = new Transition_return();
}

bool Processor::run()
{
	do
	{
		datas command;
		command = _mem.load_data(_psw.IP, w);	// ¬ базовом случае подружаем только 1 слово, если необходимо догружаем остальную часть
		uint8_t cmd_size = get_size(command);
		command = _mem.load_data(_psw.IP, cmd_size);
		cmd.w3 = command.cmd3w;						// «аносим команду в регистр команд

		if (_commands[cmd.w.KOP])
			_commands[cmd.w.KOP]->operator()( *this);

		if (cmd.w.KOP < 40 || cmd.w.KOP > 46)
			setIP(_psw.IP += cmd_size);
	} while (cmd.w.KOP);
	return true; 
}

template<typename  T>
void cicle_chow(const size_t& _end, T* ptr)
{
	for (auto i = 0; i < _end; i++)
		std::cout << static_cast<T>(*(ptr + i)) << std::endl;
}

void Processor::debug()
{
	cout << "\n************************************************************************\n";
	uint8_t cmd_size = get_size(cmd.w);
	cmd_debug(cmd, cmd_size);
	std::cout << "PSW\n"
		<< (uint16_t)_psw.IP << " - IP" << std::endl
		<< (uint16_t)_psw.nf << " - neg flag" << std::endl
		<< (uint16_t)_psw.zf << " - zero flag" << std::endl;

	std::cout << "REGS\n";
	cout << " SHORT\t\t\t" << " long\t\t"  << "FLOAT\n";;
	for (auto i = 0; i < 8; i++)
	{
		cout << i + 1 << "R " << setw(15) << _poh.word[i];
		if (i < 4)
			cout << setw(15) << _poh.dword[i] << setw(15) << _poh.real[i];
		cout << endl;
	}
	cout << "\n************************************************************************\n";
}
