#pragma once

#include "debug.h"
#define validate(...) debug::validate_impl(__FILE__, __LINE__, #__VA_ARGS__, std::cout, std::clog, __VA_ARGS__)
