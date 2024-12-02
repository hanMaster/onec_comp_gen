#include "common.h"

void set_return_val_for_1c_as_utf16(tVariant* pvarRetValue, std::u16string val, IMemoryManager* m_iMemory)
{
	TV_VT(pvarRetValue) = VTYPE_PWSTR;

	size_t length = val.length() + 1;

	if (m_iMemory && m_iMemory->AllocMemory((void**)&(pvarRetValue->pwstrVal), length * sizeof(char16_t)))
	{
		memcpy(pvarRetValue->pwstrVal, val.c_str(), length * sizeof(char16_t));
		pvarRetValue->wstrLen = length - 1;
	}
}

std::u16string utf8_to_utf16(const std::string& utf8)
{
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
	return convert.from_bytes(utf8);
}

std::string utf16_to_utf8(const std::u16string& utf16)
{
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
	return convert.to_bytes(utf16);
}

//+++ получение параметров методов
std::string method_param_to_utf8(tVariant* paParams, int number_param) {
	char16_t* param_utf16_pointer = paParams[number_param].pwstrVal;
	std::string param_utf8 = utf16_to_utf8(param_utf16_pointer);
	
	return param_utf8;
}

std::u16string get_method_param_as_utf16(tVariant* paParams, int number_param) {
	
	std::u16string param_utf16(paParams[number_param].pwstrVal);
	return param_utf16;
}

std::string get_method_param_as_utf8(tVariant* paParams, int number_param) {
	char16_t* param_utf16_pointer = paParams[number_param].pwstrVal;
	std::string param_utf8 = utf16_to_utf8(param_utf16_pointer);
	return param_utf8;
}

float get_method_param_as_float(tVariant* paParams, int number_param) {
	double param_double = paParams[number_param].dblVal;
	return param_double;
}

int get_method_param_as_int(tVariant* paParams, int number_param) {
	double param_int = paParams[number_param].lVal;
	return param_int;
}

bool get_method_param_as_bool(tVariant* paParams, int number_param) {
	return paParams[number_param].bVal;
}

float get_method_param_as_number(tVariant* paParams, int number_param) {
	float val = get_method_param_as_float(paParams, number_param);
	if (val==0)
		val = get_method_param_as_int(paParams, number_param);
	return val;
}

//---