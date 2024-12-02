
#include "stdafx.h"


#if defined( __linux__ ) || defined(__APPLE__)
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <iconv.h>
#endif

#include <wchar.h>
#include "AddInNative.h"
#include <string>

#include <iostream>
#include <fstream>
#include <regex>
#include <codecvt>

#include "../lib/common.h"

#include "impl/test.h"
//includeВставкаЗаголовковМетодов


static wchar_t* g_PropNames[] = { L"TestProp" };
static wchar_t* g_PropNamesRu[] = { L"ТестовоеСвойство" };

static wchar_t* g_MethodNamesRu[] = { 
	L"Тест",
	/*ФайлCPPМетодыНаРусскомЯзыке*/};

static wchar_t* g_MethodNames[] = { 
	L"Test",
	/*ФайлCPPМетодыНаАнглийскомЯзыке*/ };

static const WCHAR_T g_kClassNames[] = u"CAddInNative"; //|OtherClass1|OtherClass2";

uint32_t convToShortWchar(WCHAR_T** Dest, const wchar_t* Source, size_t len = 0);
uint32_t convFromShortWchar(wchar_t** Dest, const WCHAR_T* Source, uint32_t len = 0);
uint32_t getLenShortWcharStr(const WCHAR_T* Source);
static AppCapabilities g_capabilities = eAppCapabilitiesInvalid;
static std::u16string s_names(g_kClassNames);



//---------------------------------------------------------------------------//
long GetClassObject(const WCHAR_T* wsName, IComponentBase** pInterface)
{
	if (!*pInterface)
	{
		*pInterface = new CAddInNative();
		return (long)*pInterface;
	}
	return 0;
}
//---------------------------------------------------------------------------//
AppCapabilities SetPlatformCapabilities(const AppCapabilities capabilities)
{
	g_capabilities = capabilities;
	return eAppCapabilitiesLast;
}
//---------------------------------------------------------------------------//
AttachType GetAttachType()
{
	return eCanAttachAny;
}
//---------------------------------------------------------------------------//
long DestroyObject(IComponentBase** pIntf)
{
	if (!*pIntf)
		return -1;

	delete* pIntf;
	*pIntf = 0;
	return 0;
}
//---------------------------------------------------------------------------//
const WCHAR_T* GetClassNames()
{
	return s_names.c_str();
}
//---------------------------------------------------------------------------//
//CAddInNative
CAddInNative::CAddInNative()
{
}
//---------------------------------------------------------------------------//
CAddInNative::~CAddInNative()
{
}
//---------------------------------------------------------------------------//
bool CAddInNative::Init(void* pConnection)
{
	m_iConnect = (IAddInDefBase*)pConnection;
	return m_iConnect != NULL;
}
//---------------------------------------------------------------------------//
long CAddInNative::GetInfo()
{
	return 2000;
}
//---------------------------------------------------------------------------//
void CAddInNative::Done()
{
}
/////////////////////////////////////////////////////////////////////////////
// ILanguageExtenderBase
//---------------------------------------------------------------------------//
bool CAddInNative::RegisterExtensionAs(WCHAR_T** wsExtensionName)
{

	wchar_t* wsExtension = L"My";
	int iActualSize = ::wcslen(wsExtension) + 1;

	if (m_iMemory)
	{
		if (m_iMemory->AllocMemory((void**)wsExtensionName, iActualSize * sizeof(WCHAR_T)))
			::convToShortWchar(wsExtensionName, wsExtension, iActualSize);
		return true;
	}

	return false;
}
//---------------------------------------------------------------------------//
bool CAddInNative::setMemManager(void* mem)
{
	m_iMemory = (IMemoryManager*)mem;
	return m_iMemory != 0;
}
//---------------------------------------------------------------------------//
long CAddInNative::GetNProps()
{
	return eLastProp;
}
//---------------------------------------------------------------------------//
long CAddInNative::FindProp(const WCHAR_T* wsPropName)
{
	long plPropNum = -1;
	wchar_t* propName = 0;

	::convFromShortWchar(&propName, wsPropName);
	plPropNum = findName(g_PropNames, propName, eLastProp);

	if (plPropNum == -1)
		plPropNum = findName(g_PropNamesRu, propName, eLastProp);

	delete[] propName;

	return plPropNum;
}
//---------------------------------------------------------------------------//
const WCHAR_T* CAddInNative::GetPropName(long lPropNum, long lPropAlias)
{
	if (lPropNum >= eLastProp)
		return NULL;

	wchar_t* wsCurrentName = NULL;
	WCHAR_T* wsPropName = NULL;
	size_t iActualSize = 0;

	switch (lPropAlias)
	{
	case 0: // First language
		wsCurrentName = (wchar_t*)g_PropNames[lPropNum];
		break;
	case 1: // Second language
		wsCurrentName = (wchar_t*)g_PropNamesRu[lPropNum];
		break;
	default:
		return 0;
	}

	iActualSize = wcslen(wsCurrentName) + 1;

	if (m_iMemory && wsCurrentName)
	{
		if (m_iMemory->AllocMemory((void**)&wsPropName, (unsigned)iActualSize * sizeof(WCHAR_T)))
			::convToShortWchar(&wsPropName, wsCurrentName, iActualSize);
	}

	return wsPropName;
}
//---------------------------------------------------------------------------//
bool CAddInNative::GetPropVal(const long lPropNum, tVariant* pvarPropVal)
{
	switch (lPropNum)
	{
	case TestProp:
	{
		if (m_iMemory)
		{
			TV_VT(pvarPropVal) = VTYPE_PWSTR;
			auto iActualSize = ::wcslen(pTestProp);

			if (m_iMemory->AllocMemory((void**)&(pvarPropVal->pwstrVal), iActualSize * sizeof(WCHAR_T)))
			{
				::convToShortWchar(&(pvarPropVal->pwstrVal), pTestProp, iActualSize);
				pvarPropVal->wstrLen = iActualSize;
			}

		}
		else
			TV_VT(pvarPropVal) = VTYPE_EMPTY;
	}
	break;
	default:
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------//
bool CAddInNative::SetPropVal(const long lPropNum, tVariant* varPropVal)
{
	switch (lPropNum)
	{
	case TestProp:
		::convFromShortWchar(&pTestProp, TV_WSTR(varPropVal));
		break;
	default:
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------//
bool CAddInNative::IsPropReadable(const long lPropNum)
{
	switch (lPropNum)
	{
	case TestProp:
		return true;
	default:
		return false;
	}

	return false;
}
//---------------------------------------------------------------------------//
bool CAddInNative::IsPropWritable(const long lPropNum)
{
	switch (lPropNum)
	{
	case TestProp:
		return true;
	default:
		return false;
	}

	return false;
}
//---------------------------------------------------------------------------//
long CAddInNative::GetNMethods()
{
	return eLastMethod;
}
//---------------------------------------------------------------------------//
long CAddInNative::FindMethod(const WCHAR_T* wsMethodName)
{
	long plMethodNum = -1;
	wchar_t* name = 0;

	::convFromShortWchar(&name, wsMethodName);

	plMethodNum = findName(g_MethodNames, name, eLastMethod);

	if (plMethodNum == -1)
		plMethodNum = findName(g_MethodNamesRu, name, eLastMethod);

	return plMethodNum;
}
//---------------------------------------------------------------------------//
const WCHAR_T* CAddInNative::GetMethodName(const long lMethodNum, const long lMethodAlias)
{
	if (lMethodNum >= eLastMethod)
		return NULL;

	wchar_t* wsCurrentName = NULL;
	WCHAR_T* wsMethodName = NULL;
	int iActualSize = 0;

	switch (lMethodAlias)
	{
	case 0: // First language
		wsCurrentName = g_MethodNames[lMethodNum];
		break;
	case 1: // Second language
		wsCurrentName = g_MethodNamesRu[lMethodNum];
		break;
	default:
		return 0;
	}

	iActualSize = wcslen(wsCurrentName) + 1;

	if (m_iMemory && wsCurrentName)
	{
		if (m_iMemory->AllocMemory((void**)&wsMethodName, iActualSize * sizeof(WCHAR_T)))
			::convToShortWchar(&wsMethodName, wsCurrentName, iActualSize);
	}

	return wsMethodName;
}
//---------------------------------------------------------------------------//
long CAddInNative::GetNParams(const long lMethodNum)
{
	switch (lMethodNum)
	{
	case Test:
		{
		return 3;
		}
	//GetNParamsДляВставки
	default:
		return 0;
	}

	return 0;
}
//---------------------------------------------------------------------------//
bool CAddInNative::GetParamDefValue(const long lMethodNum, const long lParamNum,
                        tVariant *pvarParamDefValue)
{
	return false;
}
//---------------------------------------------------------------------------//
bool CAddInNative::HasRetVal(const long lMethodNum)
{
	switch (lMethodNum)
	{
	case Test:
		return true;
	//HasRetValДляВставки
	default:
		return false;
	}

	return false;
}
//---------------------------------------------------------------------------//
bool CAddInNative::CallAsProc(const long lMethodNum,
	tVariant* paParams, const long lSizeArray)
{
	return true;
}
//---------------------------------------------------------------------------//
bool CAddInNative::CallAsFunc(const long lMethodNum, tVariant* pvarRetValue, tVariant* paParams, const long lSizeArray)
{

	switch (lMethodNum)
	{
	case Test: 
		return test(lMethodNum, pvarRetValue, paParams, lSizeArray, m_iMemory);
	//CallAsFuncДляВставки
	default:
		return false;
	}

	return false;
}

//---------------------------------------------------------------------------//
void CAddInNative::SetLocale(const WCHAR_T* loc)
{
}
//---------------------------------------------------------------------------//
void ADDIN_API CAddInNative::SetUserInterfaceLanguageCode(const WCHAR_T * lang)
{
}

//---------------------------------------------------------------------------//
uint32_t convToShortWchar(WCHAR_T** Dest, const wchar_t* Source, size_t len)
{
	if (!len)
		len = ::wcslen(Source) + 1;

	if (!*Dest)
		*Dest = new WCHAR_T[len];

	WCHAR_T* tmpShort = *Dest;
	wchar_t* tmpWChar = (wchar_t*)Source;
	uint32_t res = 0;

	::memset(*Dest, 0, len * sizeof(WCHAR_T));
#ifdef __linux__
	size_t succeed = (size_t)-1;
	size_t f = len * sizeof(wchar_t), t = len * sizeof(WCHAR_T);
	const char* fromCode = sizeof(wchar_t) == 2 ? "UTF-16" : "UTF-32";
	iconv_t cd = iconv_open("UTF-16LE", fromCode);
	if (cd != (iconv_t)-1)
	{
		succeed = iconv(cd, (char**)&tmpWChar, &f, (char**)&tmpShort, &t);
		iconv_close(cd);
		if (succeed != (size_t)-1)
			return (uint32_t)succeed;
	}
#endif //__linux__
	for (; len; --len, ++res, ++tmpWChar, ++tmpShort)
	{
		*tmpShort = (WCHAR_T)*tmpWChar;
	}

	return res;
}
//---------------------------------------------------------------------------//
uint32_t convFromShortWchar(wchar_t** Dest, const WCHAR_T* Source, uint32_t len)
{
	if (!len)
		len = getLenShortWcharStr(Source) + 1;

	if (!*Dest)
		*Dest = new wchar_t[len];

	wchar_t* tmpWChar = *Dest;
	WCHAR_T* tmpShort = (WCHAR_T*)Source;
	uint32_t res = 0;

	::memset(*Dest, 0, len * sizeof(wchar_t));
#if defined( __linux__ ) || defined(__APPLE__)
	size_t succeed = (size_t)-1;
	const char* fromCode = sizeof(wchar_t) == 2 ? "UTF-16" : "UTF-32";
	size_t f = len * sizeof(WCHAR_T), t = len * sizeof(wchar_t);
	iconv_t cd = iconv_open("UTF-32LE", fromCode);
	if (cd != (iconv_t)-1)
	{
		succeed = iconv(cd, (char**)&tmpShort, &f, (char**)&tmpWChar, &t);
		iconv_close(cd);
        if(succeed != (size_t)-1)
			return (uint32_t)succeed;
	}
#endif 
	for (; len; --len, ++res, ++tmpWChar, ++tmpShort)
	{
		*tmpWChar = (wchar_t)*tmpShort;
	}

	return res;
}
//---------------------------------------------------------------------------//
uint32_t getLenShortWcharStr(const WCHAR_T* Source)
{
	uint32_t res = 0;
    WCHAR_T *tmpShort = (WCHAR_T*)Source;

	while (*tmpShort++)
		++res;

	return res;
}
//---------------------------------------------------------------------------//


long CAddInNative::findName(wchar_t* names[], const wchar_t* name,
	const uint32_t size) const
{
	long ret = -1;
	for (uint32_t i = 0; i < size; i++)
	{
		if (!wcscmp(names[i], name))
		{
			ret = i;
			break;
		}
	}
	return ret;
}

//---------------------------------------------------------------------------//

