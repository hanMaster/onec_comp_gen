#pragma once
#include "../include/types.h"
#include "../include/IMemoryManager.h"
#include <string>
#include <codecvt>

void set_return_val_for_1c_as_utf16(tVariant* pvarRetValue, std::u16string val, IMemoryManager* m_iMemory);

std::u16string utf8_to_utf16(const std::string& utf8);
std::string utf16_to_utf8(const std::u16string& utf16);

//+++ получение параметров методов
std::string method_param_to_utf8(tVariant* paParams, int number_param);

std::u16string get_method_param_as_utf16(tVariant* paParams, int number_param);

std::string get_method_param_as_utf8(tVariant* paParams, int number_param);

float get_method_param_as_float(tVariant* paParams, int number_param);

bool get_method_param_as_bool(tVariant* paParams, int number_param);

int get_method_param_as_int(tVariant* paParams, int number_param);

float get_method_param_as_number(tVariant* paParams, int number_param);
//---