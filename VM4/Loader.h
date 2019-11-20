#pragma once
#ifndef LOADERH_INCLUDED
#define LOADERH_INCLUDED
#include  <fstream>
#include  "Processor.h"

uint8_t get_size(const datas& cmd);
bool parse(std::fstream& file, Processor& p);
#endif
