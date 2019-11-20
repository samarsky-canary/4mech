// Пересылка - ДА
// Математика целая - ДА
// Математика дробная - ДА
// Ввод = ДА
// Вывод - ДА
// СОхранение IP
// Переход безусловный - да
// Переход условный -да

#include "Command.h"

using namespace std;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*												ПЕРЕСЫЛКА														*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///// При dd == 0
void Transmition::transmit_rr(Processor& p)
{
	if (p.cmd.w.s == 0)
		p.set_short_reg(p.cmd.w.r2, p.get_short_reg(p.cmd.w.r1));
	else
		p.set_long_reg(p.cmd.w.r2, p.get_long_reg(p.cmd.w.r1));
}

// При dd == 1
void Transmition::transmit_rm(Processor& p)
{
	if (p.cmd.w.s == 0)
		p._mem.push_data(p.cmd.w2.o1, p.get_short_reg(p.cmd.w2.r2));
	else
		p._mem.push_data(p.cmd.w2.o1, p.get_long_reg(p.cmd.w2.r2));
}

// При dd = 2
void Transmition::transmit_mr(Processor& p)
{
	if (p.cmd.w.s == 0)
		p.set_short_reg(p.cmd.w2.r2, p._mem.load_data<uint16_t>(p.cmd.w2.o1));
	else
		p.set_long_reg(p.cmd.w2.r2, p._mem.load_data<uint32_t>(p.cmd.w2.o1));
	p.debug();
}

// При dd == 3
void Transmition::transmit_mm(Processor& p)
{
	if (p.cmd.w.s == 0)
		p._mem.push_data(p.cmd.w3.o2, p._mem.load_data<uint16_t>(p.cmd.w3.o1));
	else
		p._mem.push_data(p.cmd.w3.o2, p._mem.load_data<uint32_t>(p.cmd.w3.o1));
}


void Transmition::operator()(Processor& p)
{
	/* Для длинных номера регистров - четные, иначе включаем аварийный флаг и завершаем работу*/
	const uint8_t size_data = (p.cmd.w.s == 0) ? SHORT : LONG;

	switch(p.cmd.w.dd)
	{
		case 0:
			transmit_rr(p);
			break;
		case 1:
			transmit_rm(p);
			break;
		case 2:
			transmit_mr(p);
			break;
		case 3:
			transmit_mm(p);
			break;
		default: 
			p.setEF(false);
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*												МАТЕМАТИКА														*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Размещение результата в памяти/ регистре
template <typename T>
T Math::accumulate(Processor& p, const T& left, const T& right)
{
	uint16_t KOP = p.cmd.w.KOP;
	switch (KOP)
	{
	case 10:
	case 15:
		return left + right;	//Для целой и дробной команды
	case 11:
	case 16:
		return left - right;
	case 12:
	case 17:
		return left * right;
	case 13:
	case 18:
		return left / right;
	default:
		return 0;
	}
};

template<typename T>
T Math::form_operands(Processor& p, std::function<T(const uint8_t&)> get_reg)
{
	T left;
	T right;
	switch (p.cmd.w.dd)
	{
	case 0:
		left = get_reg(p.cmd.w.r1);
		right = get_reg(p.cmd.w.r2);
		break;
	case 1:
		left = get_reg(p.cmd.w.r2);
		right = p._mem.load_data <T>(p.cmd.w2.o1);
		break;
	case 2:
		left = p._mem.load_data <T>(p.cmd.w2.o1);
		right = get_reg(p.cmd.w2.r2);
		break;
	case 3:
		left = p._mem.load_data <T>(p.cmd.w3.o1);
		right = p._mem.load_data <T>(p.cmd.w3.o2);
		break;
	default:
		p.setEF(false);
		return 0;
	}
	right = accumulate(p, left, right);

	return right;
}

template<typename T>
void Math::emplace(Processor& p, T result, std::function<void(const uint8_t&, const T& num)> set_reg)
{
	switch (p.cmd.w.dd)
	{
	case 0:
	case 2:
		set_reg(p.cmd.w.r2, result);
		break;
	case 1:
		p._mem.push_data(p.cmd.w2.o1, result);
		break;
	case 3:
		p._mem.push_data(p.cmd.w3.o2, result);
		break;
	default:
		break;
	}
}

template <typename T>
void Math::set_flags(T num, Processor& p)
{
	p._psw.nf = (num < 0);
	p._psw.zf = (fabs(num) < std::numeric_limits<double>::epsilon());
}

void MathInt::operator()(Processor& p)
{
	const uint8_t size_data = (p.cmd.w.s == 0) ? SHORT : LONG;

	if (size_data == SHORT)
	{
		int16_t right = form_operands<uint16_t>( p,[&](const uint8_t& param)->int16_t {return p.get_short_reg(param); });
		emplace<int16_t>(p, right, [&](const uint8_t& id, const int16_t& dat) { p.set_short_reg(id, dat); });
		set_flags(right, p);
	}
	else
	{
		int32_t right = form_operands<int32_t>(p,	[&](const uint8_t& param)->int32_t {return p.get_long_reg(param); });
		emplace<int32_t>(p, right, [&](const uint8_t& id, const int32_t& dat) { p.set_long_reg(id, dat); });
		set_flags(right, p);
	}
}


void MathFloat::operator()( Processor& p)
{
	float right = form_operands<float>(p, [&](const uint8_t& id)->float { return p.get_float_reg(id); });
	emplace<float>(p, right, [&](const uint8_t& id, const float& num) { return p.set_float_reg(id, num); });
	set_flags(right, p);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*												ВВОД/Вывод														*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/* Вывод из регистров*/
void Output_reg::operator()( Processor& p)
{
	cout << "R" << (int)p.cmd.w.r2 << " = ";
	switch (p.cmd.w.r1)
	{
	case 0:
		cout << static_cast<int16_t>(p.get_short_reg(p.cmd.w.r2));
		break;
	case 1:
		cout << static_cast<uint16_t>(p.get_short_reg(p.cmd.w.r2));
		break;
	case 2:
		cout << static_cast<int32_t>(p.get_long_reg(p.cmd.w.r2));
		break;
	case 3:
		cout << static_cast<uint32_t>(p.get_long_reg(p.cmd.w.r2));
		break;
	case 4:
		cout << p.get_float_reg(p.cmd.w.r2);
		break;
	default:
		break;
	}
}

/* Вывод из памяти*/
void Output_mem::operator()( Processor& p)
{
	cout << "mem[" << (int)p.cmd.w2.o1  << "] = ";
	switch (p.cmd.w.r1)
	{
	case 0:
		cout << p._mem.load_data<int16_t>(p.cmd.w2.o1);
		break;
	case 1:
		cout << p._mem.load_data<uint16_t>(p.cmd.w2.o1);
		break;
	case 2:
		cout << p._mem.load_data<int32_t>(p.cmd.w2.o1);
		break;
	case 3:
		cout << p._mem.load_data<uint32_t>(p.cmd.w2.o1);
		break;
	case 4:
		cout <<p._mem.load_data<float>(p.cmd.w2.o1);

		break;
	default:
		break;
	}
}

void Input::operator()( Processor& p)
{
	switch (p.cmd.w.r1) // Выбор типа по R1, запись из R2
	{
	case 0:
	{
		int16_t num;
		cin >> num;
		p.set_short_reg(p.cmd.w.r2, num);
	}
	break;
	case 1:
	{
		uint16_t num;
		cin >> num;
		p.set_short_reg(p.cmd.w.r2, num);
	}
	break;
	case 2:
	{
		int32_t num;
		cin >> num;
		p.set_long_reg(p.cmd.w.r2, num);
	}
	break;
	case 3:
	{
		uint32_t num;
		cin >> num;
		p.set_long_reg(p.cmd.w.r2, num);
	}
	break;
	case 4:
	{
		float num;
		cin >> num;
		p.set_float_reg(p.cmd.w.r2, num);
	}
	break;
	default:
		return;
	}
}

void SaveIP::operator()( Processor& p)
{
	p.set_short_reg(p.cmd.w.r2, p._psw.IP);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*												ПЕРЕХОДЫ														*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Transition::operator()(Processor& p)
{
	adress offset = 0;
	switch (p.cmd.w.dd)
	{
	case 0:
		offset = p.get_short_reg(p.cmd.w.r1);
		break;
	case 1:
		offset = p.get_short_reg(p.cmd.w.r2);
		break;
	case 2:
		offset = p.get_short_reg(p.cmd.w.r2) + p._mem.load_data<uint16_t>(p.cmd.w2.o1);
		break;
	case 3:
		offset = p._mem.load_data < uint16_t>(p.cmd.w3.o2);
		break;
	}
	if (p.cmd.w.s == 1)	// Если s == 1 -> переход относительный, иначе непосредственный
		offset += p._psw.IP;
	transit(p, offset);
}

void Transition_unc::transit(Processor& p, const uint16_t& address)
{
	p._psw.IP = address;
}

void Transition_JZ::transit(Processor& p, const uint16_t& address)
{
	if (p.getZF())
		p.setIP(address);
}

void Transition_JNZ::transit(Processor& p, const uint16_t& address)
{
	if (!p.getZF())
		p.setIP(address);
}

void Transition_JGZ::transit(Processor& p, const uint16_t& address)
{
	if (!p.getNF())
		p.setIP(address);
}

void Transition_JlZ::transit(Processor& p, const uint16_t& address)
{
	if (p.getNF())
		p.setIP(address);
}

void Transition_tosub::operator()( Processor& p)
{
	uint16_t offset;
	p.set_short_reg(p.cmd.w.r1, p._psw.IP);
	switch (p.cmd.w.dd)
	{
	case 0:
		offset = p.get_short_reg(p.cmd.w.r2);
		break;
	case 1:
		offset = p._mem.load_data<uint16_t>(p.cmd.w2.o1);
		break;
	default:
		return;
	}
	p._psw.IP = offset;
}

void Transition_return::operator()( Processor& p)
{
	p._psw.IP = p.get_short_reg(p.cmd.w.r2);
}
