#uselib "HNBridge.dll"

#define global HDN_STATUS_FAILED "hdn://FAILED"
#define global HDN_STATUS_SUCCESS "hdn://SUCCESS"
#define global HDN_STATUS_NULL "hdn://NULL"
#define global HDN_STATUS_NOT_IMPLEMENTED "hdn://NOT_IMPLEMENTED"
#func global _GetHDNStatus "GetHDNStatus" var
#func global _GetLastException "GetLastException" var
#func global _GetLastReturnValue "GetLastReturnValue" var
#func global LoadAssembly "LoadAssembly" sptr
#func global UnloadAssembly "UnloadAssembly"
#func global ToInstance "ToInstance"
#func global CreateObjectFromClassName "CreateObjectFromClassName" sptr
#func global CreateObjectFromFullName "CreateObjectFromFullName" sptr, sptr, sptr, sptr
#func global CreateInstanceWithZeroArgs "CreateInstanceWithZeroArgs"
#func global CreateInstanceWithOneArg "CreateInstanceWithOneArg" sptr
#func global CreateInstanceWithTwoArgs "CreateInstanceWithTwoArgs" sptr, sptr
#func global CreateInstanceWithThreeArgs "CreateInstanceWithThreeArgs" sptr, sptr, sptr
#func global CreateInstanceWithFourArgs "CreateInstanceWithFourArgs" sptr, sptr, sptr, sptr
#func global CreateInstanceWithFiveArgs "CreateInstanceWithFiveArgs" sptr, sptr, sptr, sptr, sptr
#func global CreateInstanceWithSixArgs "CreateInstanceWithSixArgs" sptr, sptr, sptr, sptr, sptr, sptr
#func global CreateInstanceWithSevenArgs "CreateInstanceWithSevenArgs" sptr, sptr, sptr, sptr, sptr, sptr, sptr
#func global CreateInstanceWithEightArgs "CreateInstanceWithEightArgs" sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr
#func global CreateInstanceWithNineArgs "CreateInstanceWithNineArgs" sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr
#func global CreateInstanceWithTenArgs "CreateInstanceWithTenArgs" sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr
#func global CreateInstanceWithElevenArgs "CreateInstanceWithElevenArgs" sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr
#func global CreateInstanceWithTwelveArgs "CreateInstanceWithTwelveArgs" sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr
#func global CreateInstanceWithThirteenArgs "CreateInstanceWithThirteenArgs" sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr
#func global CreateInstanceWithFourteenArgs "CreateInstanceWithFourteenArgs" sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr
#func global CreateInstanceWith15Args "CreateInstanceWith15Args" sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr
#func global CreateInstanceWith16Args "CreateInstanceWith16Args" sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr
#func global InvokeMethodWithZeroArgs "InvokeMethodWithZeroArgs" sptr,
#func global InvokeMethodWithOneArg "InvokeMethodWithOneArg" sptr, sptr
#func global InvokeMethodWithTwoArgs "InvokeMethodWithTwoArgs" sptr, sptr, sptr
#func global InvokeMethodWithThreeArgs "InvokeMethodWithThreeArgs" sptr, sptr, sptr, sptr
#func global InvokeMethodWithFourArgs "InvokeMethodWithFourArgs" sptr, sptr, sptr, sptr, sptr
#func global InvokeMethodWithFiveArgs "InvokeMethodWithFiveArgs" sptr, sptr, sptr, sptr, sptr, sptr
#func global InvokeMethodWithSixArgs "InvokeMethodWithSixArgs" sptr, sptr, sptr, sptr, sptr, sptr, sptr
#func global InvokeMethodWithSevenArgs "InvokeMethodWithSevenArgs" sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr
#func global InvokeMethodWithEightArgs "InvokeMethodWithEightArgs" sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr
#func global InvokeMethodWithNineArgs "InvokeMethodWithNineArgs" sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr
#func global InvokeMethodWithTenArgs "InvokeMethodWithTenArgs" sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr
#func global InvokeMethodWithElevenArgs "InvokeMethodWithElevenArgs" sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr
#func global InvokeMethodWithTwelveArgs "InvokeMethodWithTwelveArgs" sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr
#func global InvokeMethodWithThirteenArgs "InvokeMethodWithThirteenArgs" sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr
#func global InvokeMethodWithFourteenArgs "InvokeMethodWithFourteenArgs" sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr
#func global InvokeMethodWith15Args "InvokeMethodWith15Args" sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr
#func global InvokeMethodWith16Args "InvokeMethodWith16Args" sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr, sptr
#func global TakeDownFromCurrent "TakeDown" sptr
#func global TakeUpFromBuffer "TakeUp" sptr
#func global CastToFromClassName "CastToFromClassName" sptr
#func global CastToFromClassNameWithLoadingAssembly "CastToFromClassNameWithLoadingAssembly" sptr, sptr
#func global CastToFromFullName "CastToFromFullName" sptr, sptr, sptr, sptr
#func global CastToFromFullNameWithLoadingAssembly "CastToFromFullNameWithLoadingAssembly" sptr, sptr, sptr, sptr, sptr
#func global SetField "SetField" sptr, sptr
#func global SetAFieldWithIndex "SetAFieldWithIndex" sptr, sptr, int
#func global _GetField "GetField" sptr, var
#func global _GetAField "GetAField" sptr, int, var
#func global AddDField "AddDField" sptr, sptr, sptr
#func global DelDField "DelDField" sptr, sptr
#func global _GetDField "GetDField" sptr, sptr, var
#func global AddLField "AddLField" sptr, sptr
#func global DelLField "DelLField" sptr, int
#func global _GetLField "GetLField" sptr, int, var
#func global SetProperty "SetProperty" sptr, sptr
#func global SetAProperyWithIndex "SetAProperyWithIndex" sptr, sptr, int
#func global _GetProperty "GetProperty" sptr, var
#func global _GetAProperty "GetAProperty" sptr, int, var
#func global AddDProperty "AddDProperty" sptr, sptr, sptr
#func global DelDProperty "DelDProperty" sptr, sptr
#func global _GetDProperty "GetDProperty" sptr, sptr, var
#func global AddLProperty "AddLProperty" sptr, sptr
#func global DelLProperty "DelLProperty" sptr, int
#func global _GetLProperty "GetLProperty" sptr, int, var

#module
#defcfunc GetHDNStatus
	hoge = "";
	_GetHDNStatus hoge
return hoge;

#defcfunc GetLastException
    hoge = "";
    _GetLastException hoge
return hoge;

#defcfunc GetLastReturnValue
    hoge = "";
    _GetLastReturnValue hoge
return hoge;

#defcfunc GetField str p1
    hoge = "";
    _GetField p1, hoge
return hoge;

#defcfunc GetAField str p1, int p2
	hoge = "";
	_GetAField p1, p2, hoge
return hoge;

#defcfunc GetDField str p1, str p2
	hoge = "";
	_GetDField p1, p2, hoge
return hoge

#defcfunc GetLField str p1, int p2
	hoge = ""
	_GetLField p1, p2, hoge
return hoge

#defcfunc GetProperty str p1
    hoge = "";
    _GetProperty p1, hoge
return hoge;

#defcfunc GetAProperty str p1, int p2
	hoge = "";
	_GetAProperty p1, p2, hoge
return hoge;

#defcfunc GetDProperty str p1, str p2
	hoge = ""
	_GetDProperty p1, p2, hoge
return hoge;

#defcfunc GetLProperty str p1, int p2
	hoge= ""
	_GetLProperty p1, p2, hoge
return hoge;

#global

