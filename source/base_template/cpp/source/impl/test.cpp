
#include "test.h"
#include "rust.h"

bool test(const long lMethodNum, tVariant* pvarRetValue, tVariant* paParams, const long lSizeArray, IMemoryManager* m_iMemory)
{
	//+++НачалоПримера
	std::string parm_for_rust = get_method_param_as_utf8(paParams, 0);
	float f = get_method_param_as_number(paParams, 1);
	bool b = get_method_param_as_bool(paParams, 2);
	//---

	//+++Получение параметров метода
	//ВставкаКодаПолученияПараметровМетода
	//---

	//+++Вызов метода Rust
	const char* res =  test__call_from_cpp(parm_for_rust.c_str(), f, b);
	std::u16string part = utf8_to_utf16(res);
	set_return_val_for_1c_as_utf16(pvarRetValue, part, m_iMemory);
	//---
	
	free_mem_after_cpp(res);//Освободить память выделенные в Rust, когда она больше не нужна на стороне cpp

	return true;

}