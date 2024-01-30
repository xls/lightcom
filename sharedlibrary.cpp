#include <iostream>
#include "lightcom.h"



#if defined(__clang__)
#define COMPILER "clang"
#elif defined(__GNUC__) || defined(__GNUG__)
#define COMPILER "gcc"
#elif defined(_MSC_VER)
#define COMPILER "msvc"
#else
#define COMPILER "noidea"
#endif


// {7BA9D65A-D575-460A-8D95-3C3BDAAA5779}
const IID IID_ILightComClass =
{ 0x7ba9d65a, 0xd575, 0x460a, { 0x8d, 0x95, 0x3c, 0x3b, 0xda, 0xaa, 0x57, 0x79 } };

struct LCOMDECL ILightComClass : IUnknown
{
    virtual HRESULT __stdcall DoSomething(const wchar_t* name) = 0;
    virtual HRESULT __stdcall DoSomething2(const char* name) = 0;
};

class LightComClass : public ILightComClass
{
private:
    uint32_t _refcount;

public:
    LightComClass() : _refcount(0) 
    { 
        std::cout << "LightComClass()" << std::endl;
    }
    virtual ~LightComClass() 
    {
        std::cout << "~LightComClass()" << std::endl;
    }
    virtual HRESULT __stdcall QueryInterface(const IID* riid, void** ppv)
    {
        if (*riid == IID_IUnknown)
        {
            *ppv = static_cast<IUnknown*>(this);
        }
        else if (*riid == IID_ILightComClass)
        {
            *ppv = reinterpret_cast<ILightComClass*>(this);
        }
        else
        {
            *ppv = nullptr;
            return E_NOINTERFACE;
        }
        static_cast<IUnknown*>(*ppv)->AddRef();
        return E_OK;
    }

    virtual uint32_t __stdcall AddRef()
    {
        return ++_refcount;
    }
 
  

    virtual uint32_t __stdcall Release()
    {
        auto refcount = --_refcount;
        if (refcount == 0)
        {
            std::cout << "LightComClass::Release()" << std::endl;
            delete this;
        }
        return refcount;
    }

    virtual HRESULT __stdcall DoSomething(const wchar_t* name)
    {
        std::wcout << name;
        return E_OK;
    }
    virtual HRESULT __stdcall DoSomething2(const char* name)
    {
        std::cout << name;
        return E_OK;
    }
    static HRESULT __stdcall Create(ILightComClass** ppLcc)
    {
        *ppLcc = new LightComClass();
        return E_OK;
    }
};

// {ACB8875E-1D4F-4982-97FE-3EE60DF4ED7C}
const IID IID_IFactoryClass = { 0xacb8875e, 0x1d4f, 0x4982, { 0x97, 0xfe, 0x3e, 0xe6, 0xd, 0xf4, 0xed, 0x7c } };

class IFactoryClass : public IUnknown
{
public:
    virtual HRESULT __stdcall Create(ILightComClass** ppLcc) = 0;
};

class FactoryClass : public IFactoryClass
{
private:
    uint32_t _refcount;

public:
    FactoryClass() : _refcount(0) { }
    virtual ~FactoryClass() {}
    virtual HRESULT __stdcall QueryInterface(const IID *riid, void **ppv)
    {
        if (*riid == IID_IUnknown)
        {
            *ppv = static_cast<IUnknown*>(this);
        }
        else if (*riid == IID_IFactoryClass)
        {
            *ppv = static_cast<IFactoryClass*>(this);
        }
        else
        {
            *ppv = nullptr;
            return E_NOINTERFACE;
        }
        static_cast<IUnknown*>(*ppv)->AddRef();
        return E_OK;
    }

    virtual uint32_t __stdcall AddRef()
    {
        return ++_refcount;
    }

    virtual uint32_t __stdcall Release()
    {
        auto refcount = --_refcount;
        if (refcount == 0)
        {
            std::cout << "FactoryClass::Released" << std::endl;
            delete this;
        }
        return refcount;
    }

    virtual HRESULT __stdcall Create(ILightComClass** ppLcc)
    {
        *ppLcc = new LightComClass();
        return E_OK;
    }
};

extern "C" HRESULT __declspec(dllexport) __stdcall CreateFactory(IFactoryClass **ppv)
{
    std::cout<< COMPILER << ": CreateFactory" << std::endl;
    
    *ppv = new FactoryClass(); 
    return E_OK;
}

/* gcc needs this for subsystem/window, ideas? */
int WinMain(char* pCmdLine, int nCmdShow)
{
    std::cout<< COMPILER << ": CreateFactory" << std::endl;
    return 0;
}