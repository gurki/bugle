#pragma once

#define MC_INFO __FILE__, __func__, __LINE__
#define MC_INFO_NAMES file, func, line
#define MC_INFO_DEFINE const char* file, const char* func, const int line
#define MC_INFO_DEFINE_DEFAULT const char* file = nullptr, const char* func = nullptr, const int line = -1
