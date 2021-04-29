// HNBridge.h

#pragma once

#include <msclr\marshal.h>
#include <msclr\marshal_cppstd.h>

using namespace msclr::interop;
using namespace System;
using namespace System::Collections::Generic;

namespace HNBridge {

    public ref class HspDotNetBridge
    {
    public:
        // HHBridge ステータス
        String^ GetHDNStatus();
        String^ GetLastReturnValue();
        String^ GetLastException();
        String^ GetLastException(String^ p1);					//> p1...Property Name

        // .NET 変数管理
        void TakeUp(String^ p1);								//> p1...Name
        void TakeDown(String^ p1);								//> p1...Name
        void Remove(String^ p1);								//> p1...Name
        void CastTo(String^ p1, String^ p2, String^ p3, String^ p4, String^ p5);
                                                                //> p1...Assembly Name, p2...Class Name(Contain Namespace),
                                                                //> p3...Version(DEF:4.0.0.0), p4...Token
                                                                //> p5...Assembly Path(If you must do "LoadAssembly")

        // インスタンス
        void ToInstance();
        void CreateClassObject(String^ p1, String^ p2, String^ p3, String^ p4);
        //> p1...Assembly Name, p2...Class Name(Contain Namespace),
        //> p3...Version(DEF:4.0.0.0), p4...Token
        void CreateInstance(... array<String^>^ p1);			//> p1...Arguments


        // .NET 通常変数変数アクセス
        void SetField(String^ p1, String^ p2);					//> p1...Field Name, p2...Value
        void GetField(String^ p1);								//> p1...Field Name

        // .NET 配列変数アクセス(Array)
        void SetAField(String^ p1, ... array<String^>^ p2);		//> p1...Field Name, p2...Value
        void SetAField(String^ p1, String^ p2, int p3);			//> p1...Field Name, p2...Value, p3...Index
        void GetAField(String^ p1, int p2);						//> p1...Field Name, p2...Index

        // .NET IList実装変数アクセス
        void SetLField(String^ p1, ... array<String^>^ p2);     //> p1...Field Name, p2...Value
        void AddLField(String^ p1, String^ p2);                 //> p1...Field Name, p2...Value
        void DelLField(String^ p1, int p2);                     //> p1...Field Name, p2...Index
        void GetLField(String^ p1, int p2);                     //> p1...Field Name, p2...Index

        // .NET IDictionary実装変数アクセス
        void SetDField(String^ p1, ...array<String^>^ p2);		//> p1...Field Name, p2...Value
        void AddDField(String^ p1, String^ p2, String^ p3);		//> p1...Field Name, p2...Key, p3...Value
        void DelDField(String^ p1, String^ p2);                 //> p1...Field Name, p2...Key
        void GetDField(String^ p1, String^ p2);					//> p1...Field Name, p2...Key


        // .NET 通常プロパティアクセス
        void SetProperty(String^ p1, String^ p2);				//> p1...Property Name, p2...Value
        void GetProperty(String^ p1);							//> p1...Property Name

        // .NET 配列プロパティアクセス
        void SetAProperty(String^ p1, ... array<String^>^ p2);	//> p1...Property Name, p2...Value
        void SetAProperty(String^ p1, String^ p2, int p3);		//> p1...Property Name, p2...Value, p3...Index
        void GetAProperty(String^ p1, int p2);					//> p1...Property Name, p2...Index

        // .NET IList実装プロパティアクセス
        void SetLProperty(String^ p1, ...array<String^>^ p2);   //> p1...Property Name, p2...Value
        void AddLProperty(String^ p1, String^ p2);              //> p1...Property Name, p2...Value
        void GetLProperty(String^ p1, int p2);                  //> p1...Property Name, p2...Index
        void DelLProperty(String^ p1, int p3);                  //> p1...Property Name, p2...Index

        // .NET IDictonary実装プロパティアクセス
        void SetDProperty(String^ p1, ...array<String^>^ p2);	//> p1...Property Name, p2...Value
        void AddDProperty(String^ p1, String^ p2, String^ p3);	//> p1...Property Name, p2...Key, p3...Value
        void GetDProperty(String^ p1, String^ p2);				//> p1...Property Name, p2...Key
        void DelDProperty(String^ p1, String^ p2);              //> p1...Property Name, p2...Key

        // .NET アセンブリ関連
        void LoadAssembly(String^ p1);							//> p1...Assembly Name
        void UnloadAssembly();


        // .NET メソッド関連
        void InvokeMethod(String^ p1, ... array<String^>^ p2);	//> p1...Method Name, p2...Values
        void InvokeMethodOnThread(String^ p1, ...array<String^>^ p2);
        //> p1...Method Name, p2...Values
    private:
        // 型変換
        Tuple<Type^, String^>^ GetPrimitivePrms(String^ prm);
        void SetPrimitivePrms(Tuple<Type^, String^>^ tuple, List<Object^>^ prms);
    };
}

public ref class GlobalAccess
{
public:
    static HNBridge::HspDotNetBridge^ hdnb;
private:
    static GlobalAccess();
};
static GlobalAccess::GlobalAccess()
{
    hdnb = gcnew HNBridge::HspDotNetBridge();
}

#define EXPORTS extern "C" __declspec (dllexport)

EXPORTS void GetHDNStatus(char* ptr)
{
    marshal_context^ context = gcnew marshal_context();
    strcpy(ptr, context->marshal_as<const char*>(GlobalAccess::hdnb->GetHDNStatus()));
}

EXPORTS void GetLastReturnValue(char* ptr)
{
    marshal_context^ context = gcnew marshal_context();
    strcpy(ptr, context->marshal_as<const char*>(GlobalAccess::hdnb->GetLastReturnValue()));
}

EXPORTS void GetLastException(char* ptr)
{
    marshal_context^ context = gcnew marshal_context();
    strcpy(ptr, context->marshal_as<const char*>(GlobalAccess::hdnb->GetLastException()));
}

/* Deprecated */
EXPORTS void GetLastException2(const char* p1, char* ptr)
{
    marshal_context^ context = gcnew marshal_context();
    strcpy(ptr, context->marshal_as<const char*>(GlobalAccess::hdnb->GetLastException(gcnew String(p1))));
}

EXPORTS void TakeUp(const char* p1)
{
    GlobalAccess::hdnb->TakeUp(gcnew String(p1));
}

EXPORTS void TakeDown(const char* p1)
{
    GlobalAccess::hdnb->TakeDown(gcnew String(p1));
}

/* Deprecated */
EXPORTS void Remove(const char* p1)
{
    GlobalAccess::hdnb->Remove(gcnew String(p1));
}

EXPORTS void CastToFromClassName(const char* p1)
{
    GlobalAccess::hdnb->CastTo("", gcnew String(p1), "", "", "");
}

EXPORTS void CastToFromClassNameWithLoadingAssembly(const char* p1, const char* p2)
{
    GlobalAccess::hdnb->CastTo("", gcnew String(p1), "", "", gcnew String(p2));
}

EXPORTS void CastToFromFullName(const char* p1, const char* p2, const char* p3, const char* p4)
{
    GlobalAccess::hdnb->CastTo(gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), "");
}

EXPORTS void CastToFromFullNameWithLoadingAssembly(const char* p1, const char* p2, const char* p3, const char* p4, const char* p5)
{
    GlobalAccess::hdnb->CastTo(gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5));
}

EXPORTS void ToInstance()
{
    GlobalAccess::hdnb->ToInstance();
}

EXPORTS void CreateObjectFromClassName(const char* p1)
{
    GlobalAccess::hdnb->CreateClassObject("", gcnew String(p1), "", "");
}

EXPORTS void CreateObjectFromFullName(const char* p1, const char* p2, const char* p3, const char* p4)
{
    GlobalAccess::hdnb->CreateClassObject(gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4));
}

EXPORTS void CreateInstanceWithZeroArgs()
{
    GlobalAccess::hdnb->CreateInstance();
}

EXPORTS void CreateInstanceWithOneArg(const char* p1)
{
    GlobalAccess::hdnb->CreateInstance(gcnew String(p1));
}

EXPORTS void CreateInstanceWithTwoArgs(const char* p1, const char* p2)
{
    GlobalAccess::hdnb->CreateInstance(gcnew String(p1), gcnew String(p2));
}

EXPORTS void CreateInstanceWithThreeArgs(const char* p1, const char* p2, const char* p3)
{
    GlobalAccess::hdnb->CreateInstance(gcnew String(p1), gcnew String(p2), gcnew String(p3));
}

EXPORTS void CreateInstanceWithFourArgs(const char* p1, const char* p2, const char* p3, const char* p4)
{
    GlobalAccess::hdnb->CreateInstance(gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4));
}

EXPORTS void CreateInstanceWithFiveArgs(const char* p1, const char* p2, const char* p3, const char* p4, const char* p5)
{
    GlobalAccess::hdnb->CreateInstance(gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5));
}

EXPORTS void CreateInstanceWithSixArgs(const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6)
{
    GlobalAccess::hdnb->CreateInstance(gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6));
}

EXPORTS void CreateInstanceWithSevenArgs(const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7)
{
    GlobalAccess::hdnb->CreateInstance(gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6), gcnew String(p7));
}

EXPORTS void CreateInstanceWithEightArgs(const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7, const char* p8)
{
    GlobalAccess::hdnb->CreateInstance(gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6), gcnew String(p7), gcnew String(p8));
}

EXPORTS void CreateInstanceWithNineArgs(const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7, const char* p8, const char* p9)
{
    GlobalAccess::hdnb->CreateInstance(gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6), gcnew String(p7), gcnew String(p8), gcnew String(p9));
}

EXPORTS void CreateInstanceWithTenArgs(const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7, const char* p8, const char* p9, const char* p10)
{
    GlobalAccess::hdnb->CreateInstance(gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6), gcnew String(p7), gcnew String(p8), gcnew String(p9), gcnew String(p10));
}

EXPORTS void CreateInstanceWithElevenArgs(const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7, const char* p8, const char* p9, const char* p10, const char* p11)
{
    GlobalAccess::hdnb->CreateInstance(gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6), gcnew String(p7), gcnew String(p8), gcnew String(p9), gcnew String(p10), gcnew String(p11));
}

EXPORTS void CreateInstanceWithTwelveArgs(const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7, const char* p8, const char* p9, const char* p10, const char* p11, const char* p12)
{
    GlobalAccess::hdnb->CreateInstance(gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6), gcnew String(p7), gcnew String(p8), gcnew String(p9), gcnew String(p10), gcnew String(p11), gcnew String(p12));
}

EXPORTS void CreateInstanceWithThirteenArgs(const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7, const char* p8, const char* p9, const char* p10, const char* p11, const char* p12, const char* p13)
{
    GlobalAccess::hdnb->CreateInstance(gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6), gcnew String(p7), gcnew String(p8), gcnew String(p9), gcnew String(p10), gcnew String(p11), gcnew String(p12), gcnew String(p13));
}

EXPORTS void CreateInstanceWithFourteenArgs(const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7, const char* p8, const char* p9, const char* p10, const char* p11, const char* p12, const char* p13, const char* p14)
{
    GlobalAccess::hdnb->CreateInstance(gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6), gcnew String(p7), gcnew String(p8), gcnew String(p9), gcnew String(p10), gcnew String(p11), gcnew String(p12), gcnew String(p13), gcnew String(p14));
}

EXPORTS void CreateInstanceWith15Args(const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7, const char* p8, const char* p9, const char* p10, const char* p11, const char* p12, const char* p13, const char* p14, const char* p15)
{
    GlobalAccess::hdnb->CreateInstance(gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6), gcnew String(p7), gcnew String(p8), gcnew String(p9), gcnew String(p10), gcnew String(p11), gcnew String(p12), gcnew String(p13), gcnew String(p14), gcnew String(p15));
}

EXPORTS void CreateInstanceWith16Args(const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7, const char* p8, const char* p9, const char* p10, const char* p11, const char* p12, const char* p13, const char* p14, const char* p15, const char* p16)
{
    GlobalAccess::hdnb->CreateInstance(gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6), gcnew String(p7), gcnew String(p8), gcnew String(p9), gcnew String(p10), gcnew String(p11), gcnew String(p12), gcnew String(p13), gcnew String(p14), gcnew String(p15), gcnew String(p16));
}

EXPORTS void SetField(const char* p1, const char* p2)
{
    GlobalAccess::hdnb->SetField(gcnew String(p1), gcnew String(p2));
}

EXPORTS void GetField(const char* p1, char* ptr)
{
    GlobalAccess::hdnb->GetField(gcnew String(p1));
    marshal_context^ context = gcnew marshal_context();
    strcpy(ptr, context->marshal_as<const char*>(GlobalAccess::hdnb->GetLastReturnValue()));
}

/* Not Implementation */
EXPORTS void SetAField(const char* p1, const char** p2)
{

}

EXPORTS void SetAFieldWithIndex(const char* p1, const char* p2, int p3)
{
    GlobalAccess::hdnb->SetAField(gcnew String(p1), gcnew String(p2), p3);
}

EXPORTS void GetAField(const char* p1, int p2, char* ptr)
{
    GlobalAccess::hdnb->GetAField(gcnew String(p1), p2);
    marshal_context^ context = gcnew marshal_context();
    strcpy(ptr, context->marshal_as<const char*>(GlobalAccess::hdnb->GetLastReturnValue()));
}

/* Not Implementation */
EXPORTS void SetDField(const char* p1, const char** p2)
{

}

/* Not Implementation */
EXPORTS void AddDField(const char* p1, const char* p2, const char* p3)
{
    GlobalAccess::hdnb->AddDField(gcnew String(p1), gcnew String(p2), gcnew String(p3));
}

EXPORTS void DelDField(const char* p1, const char* p2)
{
    GlobalAccess::hdnb->DelDField(gcnew String(p1), gcnew String(p2));
}

/* Not Implementation */
EXPORTS void GetDField(const char* p1, const char* p2, char* ptr)
{
    GlobalAccess::hdnb->GetDField(gcnew String(p1), gcnew String(p2));
    marshal_context^ context = gcnew marshal_context();
    strcpy(ptr, context->marshal_as<const char*>(GlobalAccess::hdnb->GetLastReturnValue()));
}

/* Not Implementation */
EXPORTS void SetLField(const char* p1, const char** p2)
{

}

EXPORTS void AddLField(const char* p1, const char* p2)
{
    GlobalAccess::hdnb->AddLField(gcnew String(p1), gcnew String(p2));
}

EXPORTS void DelLField(const char* p1, int p2)
{
    GlobalAccess::hdnb->DelLField(gcnew String(p1), p2);
}

EXPORTS void GetLField(const char* p1, int p2, char* ptr)
{
    GlobalAccess::hdnb->GetLField(gcnew String(p1), p2);
    marshal_context^ context = gcnew marshal_context();
    strcpy(ptr, context->marshal_as<const char*>(GlobalAccess::hdnb->GetLastReturnValue()));
}

EXPORTS void SetProperty(const char* p1, const char* p2)
{
    GlobalAccess::hdnb->SetProperty(gcnew String(p1), gcnew String(p2));
}

EXPORTS void GetProperty(const char* p1, char* ptr)
{
    GlobalAccess::hdnb->GetProperty(gcnew String(p1));
    marshal_context^ context = gcnew marshal_context();
    strcpy(ptr, context->marshal_as<const char*>(GlobalAccess::hdnb->GetLastReturnValue()));
}

/* Not Implementation */
EXPORTS void SetAProperty(const char* p1, const char** p2)
{

}

EXPORTS void SetAProperyWithIndex(const char* p1, const char* p2, int p3)
{
    GlobalAccess::hdnb->SetAProperty(gcnew String(p1), gcnew String(p2), p3);
}

EXPORTS void GetAProperty(const char* p1, int p2, char* ptr)
{
    GlobalAccess::hdnb->GetAProperty(gcnew String(p1), p2);
    marshal_context^ context = gcnew marshal_context();
    strcpy(ptr, context->marshal_as<const char*>(GlobalAccess::hdnb->GetLastReturnValue()));
}

/* Not Implementation */
EXPORTS void SetDProperty(const char* p1, const char** p2)
{

}

EXPORTS void AddDProperty(const char* p1, const char* p2, const char* p3)
{
    GlobalAccess::hdnb->AddDProperty(gcnew String(p1), gcnew String(p2), gcnew String(p3));
}

EXPORTS void DelDProperty(const char* p1, const char* p2)
{
    GlobalAccess::hdnb->DelDProperty(gcnew String(p1), gcnew String(p2));
}

EXPORTS void GetDProperty(const char* p1, const char* p2, char* ptr)
{
    GlobalAccess::hdnb->GetDProperty(gcnew String(p1), gcnew String(p2));
    marshal_context^ context = gcnew marshal_context();
    strcpy(ptr, context->marshal_as<const char*>(GlobalAccess::hdnb->GetLastReturnValue()));
}

/* Not Implementation */
EXPORTS void SetLProperty(const char* p1, const char** p2)
{
    
}

EXPORTS void AddLProperty(const char* p1, const char* p2)
{
    GlobalAccess::hdnb->AddLProperty(gcnew String(p1), gcnew String(p2));
}

EXPORTS void DelLProperty(const char* p1, int p2)
{
    GlobalAccess::hdnb->DelLProperty(gcnew String(p1), p2);
}

EXPORTS void GetLProperty(const char* p1, int p2, char* ptr)
{
    GlobalAccess::hdnb->GetLProperty(gcnew String(p1), p2);
    marshal_context^ context = gcnew marshal_context();
    strcpy(ptr, context->marshal_as<const char*>(GlobalAccess::hdnb->GetLastReturnValue()));
}

EXPORTS void LoadAssembly(const char* p1)
{
    GlobalAccess::hdnb->LoadAssembly(gcnew String(p1));
}

EXPORTS void UnloadAssembly()
{
    GlobalAccess::hdnb->UnloadAssembly();
}
EXPORTS void InvokeMethodWithZeroArgs(const char* p0)
{
    GlobalAccess::hdnb->InvokeMethod(gcnew String(p0));
}

EXPORTS void InvokeMethodWithOneArg(const char* p0, const char* p1)
{
    GlobalAccess::hdnb->InvokeMethod(gcnew String(p0), gcnew String(p1));
}

EXPORTS void InvokeMethodWithTwoArgs(const char* p0, const char* p1, const char* p2)
{
    GlobalAccess::hdnb->InvokeMethod(gcnew String(p0), gcnew String(p1), gcnew String(p2));
}

EXPORTS void InvokeMethodWithThreeArgs(const char* p0, const char* p1, const char* p2, const char* p3)
{
    GlobalAccess::hdnb->InvokeMethod(gcnew String(p0), gcnew String(p1), gcnew String(p2), gcnew String(p3));
}

EXPORTS void InvokeMethodWithFourArgs(const char* p0, const char* p1, const char* p2, const char* p3, const char* p4)
{
    GlobalAccess::hdnb->InvokeMethod(gcnew String(p0), gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4));
}

EXPORTS void InvokeMethodWithFiveArgs(const char* p0, const char* p1, const char* p2, const char* p3, const char* p4, const char* p5)
{
    GlobalAccess::hdnb->InvokeMethod(gcnew String(p0), gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5));
}

EXPORTS void InvokeMethodWithSixArgs(const char* p0, const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6)
{
    GlobalAccess::hdnb->InvokeMethod(gcnew String(p0), gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6));
}

EXPORTS void InvokeMethodWithSevenArgs(const char* p0, const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7)
{
    GlobalAccess::hdnb->InvokeMethod(gcnew String(p0), gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6), gcnew String(p7));
}

EXPORTS void InvokeMethodWithEightArgs(const char* p0, const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7, const char* p8)
{
    GlobalAccess::hdnb->InvokeMethod(gcnew String(p0), gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6), gcnew String(p7), gcnew String(p8));
}

EXPORTS void InvokeMethodWithNineArgs(const char* p0, const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7, const char* p8, const char* p9)
{
    GlobalAccess::hdnb->InvokeMethod(gcnew String(p0), gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6), gcnew String(p7), gcnew String(p8), gcnew String(p9));
}

EXPORTS void InvokeMethodWithTenArgs(const char* p0, const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7, const char* p8, const char* p9, const char* p10)
{
    GlobalAccess::hdnb->InvokeMethod(gcnew String(p0), gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6), gcnew String(p7), gcnew String(p8), gcnew String(p9), gcnew String(p10));
}

EXPORTS void InvokeMethodWithElevenArgs(const char* p0, const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7, const char* p8, const char* p9, const char* p10, const char* p11)
{
    GlobalAccess::hdnb->InvokeMethod(gcnew String(p0), gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6), gcnew String(p7), gcnew String(p8), gcnew String(p9), gcnew String(p10), gcnew String(p11));
}

EXPORTS void InvokeMethodWithTwelveArgs(const char* p0, const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7, const char* p8, const char* p9, const char* p10, const char* p11, const char* p12)
{
    GlobalAccess::hdnb->InvokeMethod(gcnew String(p0), gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6), gcnew String(p7), gcnew String(p8), gcnew String(p9), gcnew String(p10), gcnew String(p11), gcnew String(p12));
}

EXPORTS void InvokeMethodWithThirteenArgs(const char* p0, const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7, const char* p8, const char* p9, const char* p10, const char* p11, const char* p12, const char* p13)
{
    GlobalAccess::hdnb->InvokeMethod(gcnew String(p0), gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6), gcnew String(p7), gcnew String(p8), gcnew String(p9), gcnew String(p10), gcnew String(p11), gcnew String(p12), gcnew String(p13));
}

EXPORTS void InvokeMethodWithFourteenArgs(const char* p0, const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7, const char* p8, const char* p9, const char* p10, const char* p11, const char* p12, const char* p13, const char* p14)
{
    GlobalAccess::hdnb->InvokeMethod(gcnew String(p0), gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6), gcnew String(p7), gcnew String(p8), gcnew String(p9), gcnew String(p10), gcnew String(p11), gcnew String(p12), gcnew String(p13), gcnew String(p14));
}

EXPORTS void InvokeMethodWith15Args(const char* p0, const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7, const char* p8, const char* p9, const char* p10, const char* p11, const char* p12, const char* p13, const char* p14, const char* p15)
{
    GlobalAccess::hdnb->InvokeMethod(gcnew String(p0), gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6), gcnew String(p7), gcnew String(p8), gcnew String(p9), gcnew String(p10), gcnew String(p11), gcnew String(p12), gcnew String(p13), gcnew String(p14), gcnew String(p15));
}

EXPORTS void InvokeMethodWith16Args(const char* p0, const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7, const char* p8, const char* p9, const char* p10, const char* p11, const char* p12, const char* p13, const char* p14, const char* p15, const char* p16)
{
    GlobalAccess::hdnb->InvokeMethod(gcnew String(p0), gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6), gcnew String(p7), gcnew String(p8), gcnew String(p9), gcnew String(p10), gcnew String(p11), gcnew String(p12), gcnew String(p13), gcnew String(p14), gcnew String(p15), gcnew String(p16));
}

EXPORTS void InvokeMethodOnThreadWithZeroArgs(const char* p0)
{
    GlobalAccess::hdnb->InvokeMethodOnThread(gcnew String(p0));
}

EXPORTS void InvokeMethodOnThreadWithOneArg(const char* p0, const char* p1)
{
    GlobalAccess::hdnb->InvokeMethodOnThread(gcnew String(p0), gcnew String(p1));
}

EXPORTS void InvokeMethodOnThreadWithTwoArgs(const char* p0, const char* p1, const char* p2)
{
    GlobalAccess::hdnb->InvokeMethodOnThread(gcnew String(p0), gcnew String(p1), gcnew String(p2));
}

EXPORTS void InvokeMethodOnThreadWithThreeArgs(const char* p0, const char* p1, const char* p2, const char* p3)
{
    GlobalAccess::hdnb->InvokeMethodOnThread(gcnew String(p0), gcnew String(p1), gcnew String(p2), gcnew String(p3));
}

EXPORTS void InvokeMethodOnThreadWithFourArgs(const char* p0, const char* p1, const char* p2, const char* p3, const char* p4)
{
    GlobalAccess::hdnb->InvokeMethodOnThread(gcnew String(p0), gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4));
}

EXPORTS void InvokeMethodOnThreadWithFiveArgs(const char* p0, const char* p1, const char* p2, const char* p3, const char* p4, const char* p5)
{
    GlobalAccess::hdnb->InvokeMethodOnThread(gcnew String(p0), gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5));
}

EXPORTS void InvokeMethodOnThreadWithSixArgs(const char* p0, const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6)
{
    GlobalAccess::hdnb->InvokeMethodOnThread(gcnew String(p0), gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6));
}

EXPORTS void InvokeMethodOnThreadWithSevenArgs(const char* p0, const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7)
{
    GlobalAccess::hdnb->InvokeMethodOnThread(gcnew String(p0), gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6), gcnew String(p7));
}

EXPORTS void InvokeMethodOnThreadWithEightArgs(const char* p0, const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7, const char* p8)
{
    GlobalAccess::hdnb->InvokeMethodOnThread(gcnew String(p0), gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6), gcnew String(p7), gcnew String(p8));
}

EXPORTS void InvokeMethodOnThreadWithNineArgs(const char* p0, const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7, const char* p8, const char* p9)
{
    GlobalAccess::hdnb->InvokeMethodOnThread(gcnew String(p0), gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6), gcnew String(p7), gcnew String(p8), gcnew String(p9));
}

EXPORTS void InvokeMethodOnThreadWithTenArgs(const char* p0, const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7, const char* p8, const char* p9, const char* p10)
{
    GlobalAccess::hdnb->InvokeMethodOnThread(gcnew String(p0), gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6), gcnew String(p7), gcnew String(p8), gcnew String(p9), gcnew String(p10));
}

EXPORTS void InvokeMethodOnThreadWithElevenArgs(const char* p0, const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7, const char* p8, const char* p9, const char* p10, const char* p11)
{
    GlobalAccess::hdnb->InvokeMethodOnThread(gcnew String(p0), gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6), gcnew String(p7), gcnew String(p8), gcnew String(p9), gcnew String(p10), gcnew String(p11));
}

EXPORTS void InvokeMethodOnThreadWithTwelveArgs(const char* p0, const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7, const char* p8, const char* p9, const char* p10, const char* p11, const char* p12)
{
    GlobalAccess::hdnb->InvokeMethodOnThread(gcnew String(p0), gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6), gcnew String(p7), gcnew String(p8), gcnew String(p9), gcnew String(p10), gcnew String(p11), gcnew String(p12));
}

EXPORTS void InvokeMethodOnThreadWithThirteenArgs(const char* p0, const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7, const char* p8, const char* p9, const char* p10, const char* p11, const char* p12, const char* p13)
{
    GlobalAccess::hdnb->InvokeMethodOnThread(gcnew String(p0), gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6), gcnew String(p7), gcnew String(p8), gcnew String(p9), gcnew String(p10), gcnew String(p11), gcnew String(p12), gcnew String(p13));
}

EXPORTS void InvokeMethodOnThreadWithFourteenArgs(const char* p0, const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7, const char* p8, const char* p9, const char* p10, const char* p11, const char* p12, const char* p13, const char* p14)
{
    GlobalAccess::hdnb->InvokeMethodOnThread(gcnew String(p0), gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6), gcnew String(p7), gcnew String(p8), gcnew String(p9), gcnew String(p10), gcnew String(p11), gcnew String(p12), gcnew String(p13), gcnew String(p14));
}

EXPORTS void InvokeMethodOnThreadWith15Args(const char* p0, const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7, const char* p8, const char* p9, const char* p10, const char* p11, const char* p12, const char* p13, const char* p14, const char* p15)
{
    GlobalAccess::hdnb->InvokeMethodOnThread(gcnew String(p0), gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6), gcnew String(p7), gcnew String(p8), gcnew String(p9), gcnew String(p10), gcnew String(p11), gcnew String(p12), gcnew String(p13), gcnew String(p14), gcnew String(p15));
}

EXPORTS void InvokeMethodOnThreadWith16Args(const char* p0, const char* p1, const char* p2, const char* p3, const char* p4, const char* p5, const char* p6, const char* p7, const char* p8, const char* p9, const char* p10, const char* p11, const char* p12, const char* p13, const char* p14, const char* p15, const char* p16)
{
    GlobalAccess::hdnb->InvokeMethodOnThread(gcnew String(p0), gcnew String(p1), gcnew String(p2), gcnew String(p3), gcnew String(p4), gcnew String(p5), gcnew String(p6), gcnew String(p7), gcnew String(p8), gcnew String(p9), gcnew String(p10), gcnew String(p11), gcnew String(p12), gcnew String(p13), gcnew String(p14), gcnew String(p15), gcnew String(p16));
}