#pragma once
#ifndef PROCESSOR_H_INCLUDED
#define PROCESSOR_H_INCLUDED
#include  "Memory.h"
#include  "Command.h"
#include "Base_functions.h"


class Processor
{
public:
	Processor();
	Processor(Processor& another) = delete;
	Processor& operator=(Processor& another) = delete;

	void debug();
	bool run();

	void setIP(const uint16_t& IP) { _psw.IP = IP; }

	void setNF(const bool& condition) { _psw.nf = condition; }
	bool getNF() { return _psw.nf; }

	void setZF(const bool& condition) { _psw.zf = condition; }
	bool getZF() { return _psw.zf; }

	void setEF(const uint8_t& condition) { (condition) ? _psw.es = 1 : _psw.es = 0; }	// Состояние флага зависит от передаваемого состояния от команд
	bool getEF() { return _psw.es; }
	/* Get set for registers*/
	uint16_t get_short_reg(const uint8_t& id) const { return _poh.word[id]; }
	void	 set_short_reg(const uint8_t& id, const uint16_t& value) { _poh.word[id] = value; }
	 
	uint32_t get_long_reg(const uint8_t& id) const { return _poh.dword[id ]; }
	void	 set_long_reg(const uint8_t& id, const uint32_t& value) { _poh.dword[id] = value; }

	float	 get_float_reg(const uint8_t& id) const { return _poh.real[id]; }
	void	 set_float_reg(const uint8_t& id, const float& value) { _poh.real[id] = value; }

	Memory _mem;
	PSW _psw;
	comR cmd; // Регистр команд

private:
	POH _poh;
	class Command** _commands;
};

#endif
