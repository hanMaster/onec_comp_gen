#include "common.h"
#include <stdexcept>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#else
#include <iconv.h>
#endif

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
#ifdef _WIN32
    if (utf8.empty()) {
        return std::u16string();
    }
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &utf8[0], (int)utf8.size(), NULL, 0);
    std::u16string utf16(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &utf8[0], (int)utf8.size(), (LPWSTR)&utf16[0], size_needed);
    return utf16;
#else
    if (utf8.empty()) {
        return u"";
    }

    iconv_t cd = iconv_open("UTF-16LE", "UTF-8");
    if (cd == (iconv_t)-1) {
        throw std::runtime_error("iconv_open failed");
    }

    char* in_buf = const_cast<char*>(utf8.c_str());
    size_t in_bytes_left = utf8.size();
    
    // Allocate a buffer for the output. It might be larger than needed.
    size_t out_buf_size = utf8.length() * sizeof(char16_t) + sizeof(char16_t);
    std::vector<char16_t> out_buf(out_buf_size);
    char* out_ptr = reinterpret_cast<char*>(out_buf.data());
    size_t out_bytes_left = out_buf_size * sizeof(char16_t);

    if (iconv(cd, &in_buf, &in_bytes_left, &out_ptr, &out_bytes_left) == (size_t)-1) {
        iconv_close(cd);
        throw std::runtime_error("iconv failed");
    }

    iconv_close(cd);
    
    return std::u16string(out_buf.data());
#endif
}

std::string utf16_to_utf8(const std::u16string& utf16)
{
#ifdef _WIN32
    if (utf16.empty()) {
        return std::string();
    }
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)utf16.c_str(), (int)utf16.size(), NULL, 0, NULL, NULL);
    std::string utf8(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)utf16.c_str(), (int)utf16.size(), &utf8[0], size_needed, NULL, NULL);
    return utf8;
#else
    if (utf16.empty()) {
        return "";
    }

    iconv_t cd = iconv_open("UTF-8", "UTF-16LE");
    if (cd == (iconv_t)-1) {
        throw std::runtime_error("iconv_open failed");
    }

    char* in_buf = const_cast<char*>(reinterpret_cast<const char*>(utf16.c_str()));
    size_t in_bytes_left = utf16.size() * sizeof(char16_t);

    size_t out_buf_size = utf16.length() * 4 + 1;
    std::vector<char> out_buf(out_buf_size);
    char* out_ptr = out_buf.data();
    size_t out_bytes_left = out_buf_size;

    if (iconv(cd, &in_buf, &in_bytes_left, &out_ptr, &out_bytes_left) == (size_t)-1) {
        iconv_close(cd);
        throw std::runtime_error("iconv failed");
    }

    iconv_close(cd);

    return std::string(out_buf.data());
#endif
}

//+++   
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