#pragma once
#include  <iostream>
#include  <vector>
#include "names.h"
enum
{
	w = 1,
	dw = 2,
	tw = 3
};

enum sizes
{
	SHORT = 2,
	LONG = 4
};


using adress = uint16_t;


uint8_t get_size(const datas& cmd);
uint8_t get_size(const comandW& cmd);
void cmd_debug(const comR& cmd, const uint8_t& cmd_size);
void line_debug(std::vector<std::string>& args);
bool is_correct_data(const uint16_t& size_data, const adress& r1, const adress& r2);