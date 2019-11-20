#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED
#include <functional>
#include "Processor.h"

class Processor;

class Command
{
public:
	virtual ~Command() = default;
	virtual void operator() (Processor& p) = 0;
};

class Transmition : public Command
{
	void transmit_rr(Processor& p);
	void transmit_rm(Processor& p);
	void transmit_mr(Processor& p);
	void transmit_mm(Processor& p);
public:
	void operator() (Processor& p) override;
};

class Math : public Command
{
public:
	template <typename T>
	T accumulate(Processor& p, const T& left, const T& right);

	template<typename T>
	T form_operands(Processor& p, std::function<T(const uint8_t&)> get_reg);

	template<typename T>
	void emplace(Processor& p, T result, std::function<void(const uint8_t&, const T& num)> set_reg);

	template <typename T>
	void set_flags(T num, Processor& p);
};

 class  MathInt final : public Math 
{
public:
	void operator() (Processor& p) override;
};


class MathFloat final : public Math
{
public:
	void operator() (Processor& p) override;
};

class Output_reg  final : public Command
{
public:
	void operator() (Processor& p) override;
};

class Output_mem final : public Command
{
public:
	void operator() (Processor& p) override;
};

class Input final : public Command
{
public:
	void operator() (Processor& p) override;
};

class SaveIP final : public Command
{
public:
	void operator() (Processor& p) override;
};

class Transition : public Command
{
	virtual void transit(Processor &p, const uint16_t& address = 0) = 0;
public:
	void operator() (Processor& p) override;
};

class Transition_unc final : public Transition
{
	void transit(Processor& p, const uint16_t& address) override;
};

class Transition_JZ final : public Transition
{
	void transit(Processor& p, const uint16_t& address) override;
};

class Transition_JNZ final : public Transition
{
	void transit(Processor& p, const uint16_t& address) override;
};

class Transition_JGZ final : public Transition
{
	void transit(Processor& p, const uint16_t& address) override;
};

class Transition_JlZ final : public Transition
{
	void transit(Processor& p, const uint16_t& address) override;
};

class Transition_tosub final : public Command
{
public:
	void operator() (Processor& p) override;
};
class Transition_return final : public Command
{
public:
	void operator() (Processor& p) override;
};
#endif