#include <stdint.h>
#include <memory.h>



#ifdef USE_WIN_COM
    #include <windows.h>
    #include <guiddef.h>
#else

    #ifndef _HRESULT_DEFINED
    #define _HRESULT_DEFINED
    typedef long HRESULT;
    #endif // !_HLT_DEFINED


    #ifndef _DEFINED
    #define GUEFINED
    #if defined(__midl)
    typedef struct {
        unsigned long  Data1;
        unsigned short Data2;
        unsigned short Data3;
        byte           Data4[ 8 ];
    } GUID;
    #else
    typedef struct _GUID {
        unsigned long  Data1;
        unsigned short Data2;
        unsigned short Data3;
        unsigned char  Data4[ 8 ];
    } GUID;
    #endif
    #endif


    #ifndef __IID_DEFINED__
    #define __IID_DEFINED__
    typedef GUID IID;
    #endif // __IID_DEFINED__

    #ifndef E_NOINTERFACE
    #define E_NOINTERFACE       ((HRESULT)0x80004002L)
    #endif

    #ifndef E_OK
    #define E_OK                ((HRESULT)0x0L)
    #endif


    bool operator ==(const GUID& left, const GUID& right)
    {
        return memcmp(&left, &right, sizeof(GUID)) == 0;
    }

#endif // USE_WIN_COM

const IID IID_IUnknown = { 0x00000000, 0x0000, 0x0000, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 };

class __declspec(novtable) IUnknown
{
public:
    virtual HRESULT  __stdcall QueryInterface(const IID *riid, void **ppv) = 0;
    virtual uint32_t __stdcall AddRef() = 0;
    virtual uint32_t __stdcall Release() = 0;
};