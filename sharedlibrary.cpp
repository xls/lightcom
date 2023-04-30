#include <iostream>
#include "lightcom.h"

// {7BA9D65A-D575-460A-8D95-3C3BDAAA5779}
const IID IID_ILightComClass =
{ 0x7ba9d65a, 0xd575, 0x460a, { 0x8d, 0x95, 0x3c, 0x3b, 0xda, 0xaa, 0x57, 0x79 } };

class ILightComClass : public IUnknown
{
public:
    virtual HRESULT __stdcall DoSomething(const wchar_t* name) = 0;
    virtual HRESULT __stdcall DoSomething2(const char* name) = 0;
};

class LightComClass : public ILightComClass
{
private:
    uint32_t _refcount;

public:
    LightComClass() : _refcount(0) { }
    virtual ~LightComClass() {}
    virtual HRESULT __stdcall QueryInterface(const IID* riid, void** ppv)
    {
        if (*riid == IID_IUnknown)
        {
            *ppv = static_cast<IUnknown*>(this);
        }
        else if (*riid == IID_ILightComClass)
        {
            *ppv = static_cast<ILightComClass*>(this);
        }
        else
        {
            *ppv = nullptr;
            return E_NOINTERFACE;
        }
        reinterpret_cast<IUnknown*>(*ppv)->AddRef();
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
            std::cout << "LightComClass::Finalized" << std::endl;
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
        reinterpret_cast<IUnknown*>(*ppv)->AddRef();
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
            std::cout << "FactoryClass::Finalized" << std::endl;
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
    *ppv = new FactoryClass(); 
    return E_OK;
}

/* for subsystem/window. */
int WinMain(char* pCmdLine, int nCmdShow)
{
    return 0;
}