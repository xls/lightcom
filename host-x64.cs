using System.Runtime.InteropServices;
using System;

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
    [DllImport("shared-msvc17-mtr-x64.dll", EntryPoint = "CreateFactory", CallingConvention = CallingConvention.StdCall)]
    extern static void CreateFactory(out IFactoryClass factory);

    [DllImport("shared-msvc17-mtd-x64.dll", EntryPoint = "CreateFactory", CallingConvention = CallingConvention.StdCall)]
    extern static void CreateFactoryd(out IFactoryClass factory);

    [DllImport("shared-vs22-clang-x64.dll", EntryPoint = "CreateFactory", CallingConvention = CallingConvention.StdCall)]
    extern static void CreateFactory_clang(out IFactoryClass factory);
	
	[DllImport("shared-gcc-r-x64.dll", EntryPoint = "CreateFactory", CallingConvention = CallingConvention.StdCall)]
    extern static void CreateFactory_gccr(out IFactoryClass factory);
	
	[DllImport("shared-gcc-d-x64.dll", EntryPoint = "CreateFactory", CallingConvention = CallingConvention.StdCall)]
    extern static void CreateFactory_gccd(out IFactoryClass factory);

    static void Main(string[] args)
    {
		IFactoryClass factory;
		
		switch((args.Length > 0)?args[0]:"msvc")
		{
			case "gccd":
				CreateFactory_gccd(out factory);
			break;
			
			case "gcc":
			case "gccr":
				CreateFactory_gccr(out factory);
			break;
		
			case "clang":
				CreateFactory_clang(out factory);
			break;

            case "msvcd":
                CreateFactoryd(out factory);
                break;

            case "msvc":
			default:
				CreateFactory(out factory);
			break;
		}
        
        if(factory != null)
		{
			ILightComClass light = factory.Create();
            
            light.DoSomething("Hello");
			light.DoSomething2(" World\n");
		}
    }
}