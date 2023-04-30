using System.Runtime.InteropServices;

[ComImport]
[Guid("ACB8875E-1D4F-4982-97FE-3EE60DF4ED7C")]
[InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
public interface IFactoryClass
{
    ILightComClass Create();
}

[ComImport]
[Guid("7BA9D65A-D575-460A-8D95-3C3BDAAA5779")]
[InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
public interface ILightComClass
{
    void DoSomething([In, MarshalAs(UnmanagedType.LPWStr)] string name);
    void DoSomething2([In, MarshalAs(UnmanagedType.LPStr)] string name);
}

class Program
{
    [DllImport("shared-vs22-clang-x86.dll", EntryPoint = "CreateFactory", CallingConvention = CallingConvention.StdCall)]
    extern static void CreateFactory(out IFactoryClass factory);

    static void Main(string[] args)
    {
        IFactoryClass factory;
        CreateFactory(out factory);
        ILightComClass light = factory.Create();
        light.DoSomething("Hello");
        light.DoSomething2(" World");
    }
}