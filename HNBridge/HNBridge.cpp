// ����� ���C�� DLL �t�@�C���ł��B

#include "stdafx.h"

#include "HNBridge.h"

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::ComponentModel;
using namespace System::Reflection;

using namespace HNBridge;

#define HNBRIDGE_STATUS_SUCCESS "hdn://SUCCESS"
#define HNBRIDGE_STATUS_FAILED "hdn://FAILED"
#define HNBRIDGE_STATUS_NULL "hdn://NULL"
#define HNBRIDGE_STATUS_NOT_IMPLEMENTED "hdn://NOT_IMPLEMENTED"

ref class ClassObject
{
public:
    property Type^ Class;
    property Object^ Instance;
    property Assembly^ Assembly;
};

ref class HNBStack
{
    static Dictionary<String^, ClassObject^>^ _ClassObjects;
    static ClassObject^ _CurrentClsObj;
public:
    static property Dictionary<String^, ClassObject^>^ ClassObjects
    {
        Dictionary<String^, ClassObject^>^ get()
        {
            if (_ClassObjects == nullptr)
            {
                _ClassObjects = gcnew Dictionary<String^, ClassObject^>();
            }
            return _ClassObjects;
        }
    }

    static property ClassObject^ CurrentClsObj
    {
        ClassObject^ get()
        {
            if (_CurrentClsObj == nullptr)
            {
                _CurrentClsObj = gcnew ClassObject();
            }
            return _CurrentClsObj;
        }
        void set(ClassObject^ obj)
        {
            _CurrentClsObj = obj;
        }
    }
    static property String^ LastStatus;
    static property Exception^ Exception;
    //�Ō�Ɏ��s��������̖߂�l
    static property Object^ LastObject;

};

/*-------------------------------------------
* HspDotNetBridge::GetHDNStatus()
* �Ō�Ɏ��s��������̏�Ԃ�Ԃ��܂��B
*-------------------------------------------*/
String^ HspDotNetBridge::GetHDNStatus()
{
    if (String::IsNullOrEmpty(HNBStack::LastStatus))
    {
        return HNBRIDGE_STATUS_NULL;
    }
    return HNBStack::LastStatus;
}

/*-------------------------------------------
* HspDotNetBridge::GetLastReturnValue()
* �Ō�Ɏ��s��������̕Ԃ�l��Ԃ��܂��B
* �Ԃ�l�����݂��Ȃ�����̏ꍇ�́AHNBRIDGE_STATUS_NULL���Ԃ�܂��B
*-------------------------------------------*/
String^ HspDotNetBridge::GetLastReturnValue()
{
    if (HNBStack::LastObject == nullptr)
    {
        return HNBRIDGE_STATUS_NULL;
    }
    return HNBStack::LastObject->ToString();
}

/*-------------------------------------------
* HspDotNetBridge::GetLastException()
* �Ō�Ɏ��s��������̗�O��Ԃ��܂��B
* ��O����x���������Ă��Ȃ��ꍇ�́AHNBRIDGE_STATUS_NULL���Ԃ�܂��B
*-------------------------------------------*/
String^ HspDotNetBridge::GetLastException()
{
    if (HNBStack::Exception == nullptr)
    {
        return HNBRIDGE_STATUS_NULL;
    }
    return HNBStack::Exception->ToString();
}

String^ HspDotNetBridge::GetLastException(String^ name)
{
    return HNBRIDGE_STATUS_NOT_IMPLEMENTED;
}

/*-------------------------------------------
* HspDotNetBridge::TakeUp(String p1)
* ���ݎg�p���̃N���X�I�u�W�F�N�g���X�^�b�N��ɖ��O�����ĕۑ����܂��B
*-------------------------------------------*/
void HspDotNetBridge::TakeUp(String^ name)
{
    if (HNBStack::ClassObjects->ContainsKey(name))
    {
        HNBStack::CurrentClsObj = HNBStack::ClassObjects[name];
        HNBStack::LastStatus = HNBRIDGE_STATUS_SUCCESS;
    }
    else {
        HNBStack::LastStatus = HNBRIDGE_STATUS_FAILED;
    }
}

/*-------------------------------------------
* HspDotNetBridge::TakeDown(String p1)
* �X�^�b�N��ɕۑ������N���X�I�u�W�F�N�g���g�p�\��Ԃɂ��܂��B
*-------------------------------------------*/
void HspDotNetBridge::TakeDown(String^ name)
{
    if (HNBStack::ClassObjects->ContainsKey(name))
    {
        HNBStack::ClassObjects->Remove(name);
    }
    HNBStack::ClassObjects->Add(name, HNBStack::CurrentClsObj);
    HNBStack::CurrentClsObj = nullptr;
    HNBStack::LastStatus = HNBRIDGE_STATUS_SUCCESS;
}

/*-------------------------------------------
* HspDotNetBridge:Remove(String p1)
* �X�^�b�N��ɕۑ������N���X�I�u�W�F�N�g���폜���܂��B
* ��DLLEXPORT����Ȃ��A������p���\�b�h�ł��B
*-------------------------------------------*/
void HspDotNetBridge::Remove(String^ name)
{
    if (HNBStack::ClassObjects->ContainsKey(name))
    {
        HNBStack::ClassObjects->Remove(name);
    }
}

/*-------------------------------------------
* HspDotNetBridge:CastTo(String p1)
* �Ō�Ɏ��s��������̃I�u�W�F�N�g���L���X�g���܂��B
*-------------------------------------------*/
void HspDotNetBridge::CastTo(String^ assembly, String^ classname, String^ version, String^ token, String^ path)
{
    try
    {
        Type^ type = nullptr;
        Assembly^ _assembly;
        if (String::IsNullOrEmpty(assembly) || String::IsNullOrEmpty(version) || String::IsNullOrEmpty(token))
        {
            //mscorlib.dll
            if (String::IsNullOrEmpty(path))
            {
                type = Type::GetType(classname);
            }
            //out
            else
            {
                _assembly = Assembly::LoadFrom(path);
                type = _assembly->GetType(classname);
            }
        }
        else
        {
            if (String::IsNullOrEmpty(path))
            {
                type = Type::GetType(String::Format("{0}, {1}, Version={2}, Culture=neutral, PublicKeyToken={3}", classname, assembly, version, token));
            }
            else
            {
                _assembly = Assembly::LoadFrom(path);
                type = _assembly->GetType(String::Format("{0}, {1}, Version={2}, Culture=neutral, PublicKeyToken={3}", classname, assembly, version, token));
            }
        }
        HNBStack::LastObject = Convert::ChangeType(HNBStack::LastObject, type);
        HNBStack::LastStatus = HNBRIDGE_STATUS_SUCCESS;
        return;
    }
    catch (Exception^ e)
    {
        HNBStack::Exception = e;
    }
    HNBStack::LastStatus = HNBRIDGE_STATUS_FAILED;
}


/*-------------------------------------------
* HspDotNetBridge::ToInstance()
* �Ō�Ɏ��s��������̕Ԃ�l���C���X�^���X�Ƃ��Đݒ肵�܂��B
*-------------------------------------------*/
void HspDotNetBridge::ToInstance()
{
    HNBStack::CurrentClsObj->Instance = HNBStack::LastObject;
    HNBStack::LastStatus = HNBRIDGE_STATUS_SUCCESS;
}

/*-------------------------------------------
* HspDotNetBridge::ToInstanceCreateClassObject(String p1, String p2, String p3, String p4)
* �Ō�Ɏ��s��������̕Ԃ�l���C���X�^���X�Ƃ��Đݒ肵�܂��B
*-------------------------------------------*/
void HspDotNetBridge::CreateClassObject(String^ assembly, String^ classname, String^ version, String^ token)
{
    try
    {
        if (String::IsNullOrEmpty(assembly) || String::IsNullOrEmpty(version) || String::IsNullOrEmpty(token))
        {
            //mscorlib.dll
            if (HNBStack::CurrentClsObj->Assembly == nullptr)
            {
                HNBStack::CurrentClsObj->Class = Type::GetType(classname);
            }
            //out
            else
            {
                HNBStack::CurrentClsObj->Class = HNBStack::CurrentClsObj->Assembly->GetType(classname);
            }
        }
        else
        {
            if (HNBStack::CurrentClsObj->Assembly == nullptr)
            {
                HNBStack::CurrentClsObj->Class = Type::GetType(String::Format("{0}, {1}, Version={2}, Culture=neutral, PublicKeyToken={3}", classname, assembly, version, token));
            }
            else
            {
                HNBStack::CurrentClsObj->Class = HNBStack::CurrentClsObj->Assembly->GetType(String::Format("{0}, {1}, Version={2}, Culture=neutral, PublicKeyToken={3}", classname, assembly, version, token));
            }
        }
        HNBStack::LastStatus = HNBRIDGE_STATUS_SUCCESS;
        return;
    }
    catch (Exception^ e)
    {
        HNBStack::Exception = e;
    }
    HNBStack::LastStatus = HNBRIDGE_STATUS_FAILED;
}

/*-------------------------------------------
* HspDotNetBridge::CreateInstance(... array<String^>^ p1)
* ���ݎ��s���̃N���X�I�u�W�F�N�g�𗘗p���āA�C���X�^���X���쐬���܂��B
*-------------------------------------------*/
void HspDotNetBridge::CreateInstance(... array<String^>^ parameters)
{
    try
    {
        if (HNBStack::CurrentClsObj->Class == nullptr)
        {
            HNBStack::LastStatus = HNBRIDGE_STATUS_NULL;
            return;
        }
        if (parameters->Length == 0)
        {
            HNBStack::CurrentClsObj->Instance = Activator::CreateInstance(HNBStack::CurrentClsObj->Class);
        }
        else
        {
            List<Object^>^ prms = gcnew List<Object^>();
            for each (String^ var in parameters)
            {
                this->SetPrimitivePrms(this->GetPrimitivePrms(var), prms);
            }
            HNBStack::CurrentClsObj->Instance = Activator::CreateInstance(HNBStack::CurrentClsObj->Class, prms->ToArray());
        }
        HNBStack::LastStatus = HNBRIDGE_STATUS_SUCCESS;
        return;
    }
    catch (Exception^ e)
    {
        HNBStack::Exception = e;
    }
    HNBStack::LastStatus = HNBRIDGE_STATUS_FAILED;
}

/*-------------------------------------------
* HspDotNetBridge::SetField(String p1, String p2)
* ���ݎ��s���̃N���X�I�u�W�F�N�g�ƃC���X�^���X�𗘗p���āA�t�B�[���h�ɒl��ݒ肵�܂��B
* �C���X�^���X���쐬����Ă��Ȃ��ꍇ�́A�ÓI�A�N�Z�X���s���܂��B
*-------------------------------------------*/
void HspDotNetBridge::SetField(String^ fieldname, String^ value)
{
    try
    {
        if (HNBStack::CurrentClsObj->Class == nullptr)
        {
            HNBStack::LastStatus = HNBRIDGE_STATUS_NULL;
            return;
        }
        FieldInfo^ info = HNBStack::CurrentClsObj->Class->GetField(fieldname);
        List<Object^>^ list = gcnew List<Object^>();
        this->SetPrimitivePrms(this->GetPrimitivePrms(value), list);
        info->SetValue(HNBStack::CurrentClsObj->Instance, list->ToArray()[0]);
        HNBStack::LastStatus = HNBRIDGE_STATUS_SUCCESS;
        return;
    }
    catch (Exception^ e)
    {
        HNBStack::Exception = e;
    }
    HNBStack::LastStatus = HNBRIDGE_STATUS_FAILED;
}

/*-------------------------------------------
* HspDotNetBridge::GetField(String p1)
* ���ݎ��s���̃N���X�I�u�W�F�N�g�ƃC���X�^���X�𗘗p���āA�t�B�[���h�̒l���擾���܂��B
* �C���X�^���X���쐬����Ă��Ȃ��ꍇ�́A�ÓI�A�N�Z�X���s���܂��B
* �t�B�[���h�̖߂�l��GetLastReturnValue()�Ŏ擾�ł��܂��B
*-------------------------------------------*/
void HspDotNetBridge::GetField(String^ fieldname)
{
    try
    {
        if (HNBStack::CurrentClsObj->Class == nullptr)
        {
            HNBStack::LastStatus = HNBRIDGE_STATUS_NULL;
            return;
        }
        FieldInfo^ info = HNBStack::CurrentClsObj->Class->GetField(fieldname);
        Object^ obj = info->GetValue(HNBStack::CurrentClsObj->Instance);
        if (obj != nullptr)
        {
            HNBStack::LastObject = obj;
            HNBStack::LastStatus = HNBRIDGE_STATUS_SUCCESS;
            return;
        }

    }
    catch (Exception^ e)
    {
        HNBStack::Exception = e;
    }
    HNBStack::LastStatus = HNBRIDGE_STATUS_FAILED;
}

/*-------------------------------------------
* HspDotNetBridge::SetAField(String p1, ... array<String^>^ values)
* ���ݎ��s���̃N���X�I�u�W�F�N�g�ƃC���X�^���X�𗘗p���āA�t�B�[���h�ɔz��l��ݒ肵�܂��B
* �C���X�^���X���쐬����Ă��Ȃ��ꍇ�́A�ÓI�A�N�Z�X���s���܂��B
*-------------------------------------------*/
void HspDotNetBridge::SetAField(String^ fieldname, ... array<String^>^ values)
{
    HNBStack::LastStatus = HNBRIDGE_STATUS_NOT_IMPLEMENTED;
}

/*-------------------------------------------
* HspDotNetBridge::SetAField(String p1, String p2, int p3)
* ���ݎ��s���̃N���X�I�u�W�F�N�g�ƃC���X�^���X�𗘗p���āA�t�B�[���h�ɔz��l��ݒ肵�܂��B
* �C���X�^���X���쐬����Ă��Ȃ��ꍇ�́A�ÓI�A�N�Z�X���s���܂��B
*-------------------------------------------*/
void HspDotNetBridge::SetAField(String^ fieldname, String^ value, int index)
{
    try
    {
        if (HNBStack::CurrentClsObj->Class == nullptr)
        {
            HNBStack::LastStatus = HNBRIDGE_STATUS_NULL;
            return;
        }
        FieldInfo^ info = HNBStack::CurrentClsObj->Class->GetField(fieldname);
        List<Object^>^ list = gcnew List<Object^>();
        this->SetPrimitivePrms(this->GetPrimitivePrms(value), list);
        Object^ obj = info->GetValue(HNBStack::CurrentClsObj->Instance);

        List<Object^>^ buffer = gcnew List<Object^>();
        int i = 0;
        for each (Object^ var in (Array^)obj)
        {
            if (i++ == index)
            {
                buffer->Add(list[0]);
            }
            buffer->Add(var);
        }
        info->SetValue(HNBStack::CurrentClsObj->Instance, buffer->ToArray());
        HNBStack::LastStatus = HNBRIDGE_STATUS_SUCCESS;
        return;
    }
    catch (Exception^ e)
    {
        HNBStack::Exception = e;
    }
    HNBStack::LastStatus = HNBRIDGE_STATUS_FAILED;
}

/*-------------------------------------------
* HspDotNetBridge::SetAField(String p1, int p2)
* ���ݎ��s���̃N���X�I�u�W�F�N�g�ƃC���X�^���X�𗘗p���āA�t�B�[���h�̔z��l���擾���܂��B
* �C���X�^���X���쐬����Ă��Ȃ��ꍇ�́A�ÓI�A�N�Z�X���s���܂��B
*-------------------------------------------*/
void HspDotNetBridge::GetAField(String^ fieldname, int index)
{
    try
    {
        if (HNBStack::CurrentClsObj->Class == nullptr)
        {
            HNBStack::LastStatus = HNBRIDGE_STATUS_NULL;
            return;
        }
        FieldInfo^ info = HNBStack::CurrentClsObj->Class->GetField(fieldname);
        Object^ obj = info->GetValue(HNBStack::CurrentClsObj->Instance);
        if (obj == nullptr)
        {
            HNBStack::LastStatus = HNBRIDGE_STATUS_FAILED;
            return;
        }
        if (index >= ((Array^)obj)->Length)
        {
            HNBStack::LastStatus = HNBRIDGE_STATUS_FAILED;
            HNBStack::Exception = gcnew IndexOutOfRangeException(index.ToString());
            return;
        }
        int i = 0;
        for each (Object^ var in (Array^)obj)
        {
            if (i++ == index)
            {
                HNBStack::LastObject = var;
                break;
            }
        }
        HNBStack::LastStatus = HNBRIDGE_STATUS_SUCCESS;
        return;
    }
    catch (Exception^ e)
    {
        HNBStack::Exception = e;
    }
    HNBStack::LastStatus = HNBRIDGE_STATUS_FAILED;
}

/*-------------------------------------------
* HspDotNetBridge::SetDField(String p1, ...array<String> values)
* ���ݎ��s���̃N���X�I�u�W�F�N�g�ƃC���X�^���X�𗘗p���āA�t�B�[���h�̃R���N�V�����ɐV�K�ɒl��ݒ肵�܂��B
* �ǉ����s���ꍇ�́AAddDField(String p1, String p2, String p3)�𗘗p���Ă��������B
* �C���X�^���X���쐬����Ă��Ȃ��ꍇ�́A�ÓI�A�N�Z�X���s���܂��B
*-------------------------------------------*/
void HspDotNetBridge::SetDField(String^ fieldname, ... array<String^>^ values)
{
    HNBStack::LastStatus = HNBRIDGE_STATUS_NOT_IMPLEMENTED;
}

/*-------------------------------------------
* HspDotNetBridge::SetDField(String p1, String p2, String p3)
* ���ݎ��s���̃N���X�I�u�W�F�N�g�ƃC���X�^���X�𗘗p���āA�t�B�[���h�̃R���N�V�����ɒl��ǉ����܂��B
* �C���X�^���X���쐬����Ă��Ȃ��ꍇ�́A�ÓI�A�N�Z�X���s���܂��B
*-------------------------------------------*/
void HspDotNetBridge::AddDField(String^ fieldname, String^ key, String^ value)
{
    try
    {
        if (HNBStack::CurrentClsObj->Class == nullptr)
        {
            HNBStack::LastStatus = HNBRIDGE_STATUS_NULL;
            return;
        }
        FieldInfo^ info = HNBStack::CurrentClsObj->Class->GetField(fieldname);
        List<Object^>^ list = gcnew List<Object^>();
        this->SetPrimitivePrms(this->GetPrimitivePrms(key), list);
        this->SetPrimitivePrms(this->GetPrimitivePrms(value), list);
        Object^ obj = info->GetValue(HNBStack::CurrentClsObj->Instance);

        System::Collections::IDictionary^ buffer = (System::Collections::IDictionary^)obj;
        buffer->Add(list[0], list[1]);
        //�K�v�Ȃ��C������
        info->SetValue(HNBStack::CurrentClsObj->Instance, buffer);
        HNBStack::LastStatus = HNBRIDGE_STATUS_SUCCESS;
        return;
    }
    catch (Exception^ e)
    {
        HNBStack::Exception = e;
    }
    HNBStack::LastStatus = HNBRIDGE_STATUS_FAILED;
}

/*-------------------------------------------
* HspDotNetBridge::DelDField(String p1, String p2)
* ���ݎ��s���̃N���X�I�u�W�F�N�g�ƃC���X�^���X�𗘗p���āA�t�B�[���h�̃R���N�V�����̎w�肳�ꂽ�L�[�̒l���폜���܂��B
* �C���X�^���X���쐬����Ă��Ȃ��ꍇ�́A�ÓI�A�N�Z�X���s���܂��B
*-------------------------------------------*/
void HspDotNetBridge::DelDField(String^ fieldname, String^ key)
{
    try
    {
        if (HNBStack::CurrentClsObj->Class == nullptr)
        {
            HNBStack::LastStatus = HNBRIDGE_STATUS_NULL;
            return;
        }
        FieldInfo^ info = HNBStack::CurrentClsObj->Class->GetField(fieldname);
        List<Object^>^ list = gcnew List<Object^>();
        this->SetPrimitivePrms(this->GetPrimitivePrms(key), list);
        Object^ obj = info->GetValue(HNBStack::CurrentClsObj->Instance);

        System::Collections::IDictionary^ buffer = (System::Collections::IDictionary^)obj;
        buffer->Remove(list[0]);
        //�K�v�Ȃ��C������
        info->SetValue(HNBStack::CurrentClsObj->Instance, buffer);
        HNBStack::LastStatus = HNBRIDGE_STATUS_SUCCESS;
        return;
    }
    catch (Exception^ e)
    {
        HNBStack::Exception = e;
    }
    HNBStack::LastStatus = HNBRIDGE_STATUS_FAILED;
}

/*-------------------------------------------
* HspDotNetBridge::GetCField(String p1, String p2)
* ���ݎ��s���̃N���X�I�u�W�F�N�g�ƃC���X�^���X�𗘗p���āA�t�B�[���h�̃R���N�V�����̎w�肳�ꂽ�L�[�ɊY������l���擾���܂��B
* �C���X�^���X���쐬����Ă��Ȃ��ꍇ�́A�ÓI�A�N�Z�X���s���܂��B
*-------------------------------------------*/
void HspDotNetBridge::GetDField(String^ fieldname, String^ key)
{
    try
    {
        if (HNBStack::CurrentClsObj->Class == nullptr)
        {
            HNBStack::LastStatus = HNBRIDGE_STATUS_NULL;
            return;
        }
        FieldInfo^ info = HNBStack::CurrentClsObj->Class->GetField(fieldname);
        List<Object^>^ list = gcnew List<Object^>();
        this->SetPrimitivePrms(this->GetPrimitivePrms(key), list);
        Object^ obj = info->GetValue(HNBStack::CurrentClsObj->Instance);

        System::Collections::IDictionary^ buffer = (System::Collections::IDictionary^)obj;
        HNBStack::LastObject = buffer[list[0]];
        HNBStack::LastStatus = HNBRIDGE_STATUS_SUCCESS;
        return;
    }
    catch (Exception^ e)
    {
        HNBStack::Exception = e;
    }
    HNBStack::LastStatus = HNBRIDGE_STATUS_FAILED;
}

/*-------------------------------------------
* HspDotNetBridge::SetLField(String p1, ...array<String> values)
* ���ݎ��s���̃N���X�I�u�W�F�N�g�ƃC���X�^���X�𗘗p���āA�t�B�[���h�̃��X�g�ɐV�K�ɒl��ݒ肵�܂��B
* �ǉ����s���ꍇ�́AAddLField(String p1, String p2)�𗘗p���Ă��������B
* �C���X�^���X���쐬����Ă��Ȃ��ꍇ�́A�ÓI�A�N�Z�X���s���܂��B
*-------------------------------------------*/
void HspDotNetBridge::SetLField(String^ p1, ...array<String^>^ values)
{
    HNBStack::LastStatus = HNBRIDGE_STATUS_NOT_IMPLEMENTED;
}

/*-------------------------------------------
* HspDotNetBridge::AddLField(String p1, String p2)
* ���ݎ��s���̃N���X�I�u�W�F�N�g�ƃC���X�^���X�𗘗p���āA�t�B�[���h�̃��X�g�ɒl��ǉ����܂��B
* �C���X�^���X���쐬����Ă��Ȃ��ꍇ�́A�ÓI�A�N�Z�X���s���܂��B
*-------------------------------------------*/
void HspDotNetBridge::AddLField(String^ fieldname, String^ value)
{
    try
    {
        if (HNBStack::CurrentClsObj->Class == nullptr)
        {
            HNBStack::LastStatus = HNBRIDGE_STATUS_NULL;
            return;
        }
        FieldInfo^ info = HNBStack::CurrentClsObj->Class->GetField(fieldname);
        List<Object^>^ list = gcnew List<Object^>();
        this->SetPrimitivePrms(this->GetPrimitivePrms(value), list);
        Object^ obj = info->GetValue(HNBStack::CurrentClsObj->Instance);

        System::Collections::IList^ buffer = (System::Collections::IList^)obj;
        buffer->Add(list[0]);
        //����Ȃ��C������
        info->SetValue(HNBStack::CurrentClsObj->Instance, buffer);
        HNBStack::LastStatus = HNBRIDGE_STATUS_SUCCESS;
        return;
    }
    catch (Exception^ e)
    {
        HNBStack::Exception = e;
    }
    HNBStack::LastStatus = HNBRIDGE_STATUS_FAILED;
}

/*-------------------------------------------
* HspDotNetBridge::DelLField(String p1, int p2)
* ���ݎ��s���̃N���X�I�u�W�F�N�g�ƃC���X�^���X�𗘗p���āA�t�B�[���h�̃��X�g�̎w�肳�ꂽ�L�[�̒l���폜���܂��B
* �C���X�^���X���쐬����Ă��Ȃ��ꍇ�́A�ÓI�A�N�Z�X���s���܂��B
*-------------------------------------------*/
void HspDotNetBridge::DelLField(String^ fieldname, int index)
{
    try
    {
        if (HNBStack::CurrentClsObj->Class == nullptr)
        {
            HNBStack::LastStatus = HNBRIDGE_STATUS_NULL;
            return;
        }
        FieldInfo^ info = HNBStack::CurrentClsObj->Class->GetField(fieldname);
        Object^ obj = info->GetValue(HNBStack::CurrentClsObj->Instance);

        System::Collections::IList^ buffer = (System::Collections::IList^)obj;
        buffer->RemoveAt(index);
        //����Ȃ��C������
        info->SetValue(HNBStack::CurrentClsObj->Instance, buffer);
        HNBStack::LastStatus = HNBRIDGE_STATUS_SUCCESS;
        return;
    }
    catch (Exception^ e)
    {
        HNBStack::Exception = e;
    }
    HNBStack::LastStatus = HNBRIDGE_STATUS_FAILED;
}

/*-------------------------------------------
* HspDotNetBridge::GetCField(String p1, String p2)
* ���ݎ��s���̃N���X�I�u�W�F�N�g�ƃC���X�^���X�𗘗p���āA�t�B�[���h�̃��X�g�̎w�肳�ꂽ�C���f�b�N�X�ɊY������l���擾���܂��B
* �C���X�^���X���쐬����Ă��Ȃ��ꍇ�́A�ÓI�A�N�Z�X���s���܂��B
*-------------------------------------------*/
void HspDotNetBridge::GetLField(String^ fieldname, int index)
{
    try
    {
        if (HNBStack::CurrentClsObj->Class == nullptr)
        {
            HNBStack::LastStatus = HNBRIDGE_STATUS_NULL;
            return;
        }
        FieldInfo^ info = HNBStack::CurrentClsObj->Class->GetField(fieldname);
        Object^ obj = info->GetValue(HNBStack::CurrentClsObj->Instance);

        System::Collections::IList^ buffer = (System::Collections::IList^)obj;
        HNBStack::LastObject = buffer[index];
        HNBStack::LastStatus = HNBRIDGE_STATUS_SUCCESS;
        return;
    }
    catch (Exception^ e)
    {
        HNBStack::Exception = e;
    }
    HNBStack::LastStatus = HNBRIDGE_STATUS_FAILED;
}

/*-------------------------------------------
* HspDotNetBridge::SetProperty(String p1, String p2)
* ���ݎ��s���̃N���X�I�u�W�F�N�g�ƃC���X�^���X�𗘗p���āA�v���p�e�B�ɒl��ݒ肵�܂��B
* �C���X�^���X���쐬����Ă��Ȃ��ꍇ�́A�ÓI�A�N�Z�X���s���܂��B
*-------------------------------------------*/
void HspDotNetBridge::SetProperty(String^ propname, String^ value)
{
    try
    {
        if (HNBStack::CurrentClsObj->Class == nullptr)
        {
            HNBStack::LastStatus = HNBRIDGE_STATUS_NULL;
            return;
        }
        PropertyInfo^ info = HNBStack::CurrentClsObj->Class->GetProperty(propname);
        List<Object^>^ list = gcnew List<Object^>();
        this->SetPrimitivePrms(this->GetPrimitivePrms(value), list);
        info->SetValue(HNBStack::CurrentClsObj->Instance, list->ToArray()[0]);
        HNBStack::LastStatus = HNBRIDGE_STATUS_SUCCESS;
        return;
    }
    catch (Exception^ e)
    {
        HNBStack::Exception = e;
    }
    HNBStack::LastStatus = HNBRIDGE_STATUS_FAILED;
}

/*-------------------------------------------
* HspDotNetBridge::GetProperty(String p1)
* ���ݎ��s���̃N���X�I�u�W�F�N�g�ƃC���X�^���X�𗘗p���āA�v���p�e�B�̒l���擾���܂��B
* �C���X�^���X���쐬����Ă��Ȃ��ꍇ�́A�ÓI�A�N�Z�X���s���܂��B
*-------------------------------------------*/
void HspDotNetBridge::GetProperty(String^ propname)
{
    try
    {
        if (HNBStack::CurrentClsObj->Class == nullptr)
        {
            HNBStack::LastStatus = HNBRIDGE_STATUS_NULL;
            return;
        }
        PropertyInfo^ info = HNBStack::CurrentClsObj->Class->GetProperty(propname);
        Object^ obj = info->GetValue(HNBStack::CurrentClsObj->Instance);
        if (obj != nullptr)
        {
            HNBStack::LastObject = obj;
            HNBStack::LastStatus = HNBRIDGE_STATUS_SUCCESS;
            return;
        }
    }
    catch (Exception^ e)
    {
        HNBStack::Exception = e;
    }
    HNBStack::LastStatus = HNBRIDGE_STATUS_FAILED;
}

/*-------------------------------------------
* HspDotNetBridge::SetAProperty(String p1, ...array<String> p2)
* ���ݎ��s���̃N���X�I�u�W�F�N�g�ƃC���X�^���X�𗘗p���āA�v���p�e�B�ɔz��l��ݒ肵�܂��B
* �C���X�^���X���쐬����Ă��Ȃ��ꍇ�́A�ÓI�A�N�Z�X���s���܂��B
*-------------------------------------------*/
void HspDotNetBridge::SetAProperty(String^ propname, ... array<String^>^ value)
{
    HNBStack::LastStatus = HNBRIDGE_STATUS_NOT_IMPLEMENTED;
}

/*-------------------------------------------
* HspDotNetBridge::SetAProperty(String p1, String p2, int p3)
* ���ݎ��s���̃N���X�I�u�W�F�N�g�ƃC���X�^���X�𗘗p���āA�v���p�e�B�ɔz��l��ݒ肵�܂��B
* �C���X�^���X���쐬����Ă��Ȃ��ꍇ�́A�ÓI�A�N�Z�X���s���܂��B
*-------------------------------------------*/
void HspDotNetBridge::SetAProperty(String^ propname, String^ value, int index)
{
    try
    {
        if (HNBStack::CurrentClsObj->Class == nullptr)
        {
            HNBStack::LastStatus = HNBRIDGE_STATUS_NULL;
            return;
        }
        PropertyInfo^ info = HNBStack::CurrentClsObj->Class->GetProperty(propname);
        List<Object^>^ list = gcnew List<Object^>();
        this->SetPrimitivePrms(this->GetPrimitivePrms(value), list);
        Object^ obj = info->GetValue(HNBStack::CurrentClsObj->Instance);
        List<Object^>^ buffer = gcnew List<Object^>();
        int i = 0;
        for each (Object^ var in (Array^)obj)
        {
            if (i++ == index)
            {
                buffer->Add(list[0]);
            }
            buffer->Add(var);
        }
        info->SetValue(HNBStack::CurrentClsObj->Instance, buffer->ToArray());
        HNBStack::LastStatus = HNBRIDGE_STATUS_SUCCESS;
        return;
    }
    catch (Exception^ e)
    {
        HNBStack::Exception = e;
    }
    HNBStack::LastStatus = HNBRIDGE_STATUS_FAILED;
}

/*-------------------------------------------
* HspDotNetBridge::GetAProperty(String p1, , int p2)
* ���ݎ��s���̃N���X�I�u�W�F�N�g�ƃC���X�^���X�𗘗p���āA�v���p�e�B�̔z��l���擾���܂��B
* �C���X�^���X���쐬����Ă��Ȃ��ꍇ�́A�ÓI�A�N�Z�X���s���܂��B
*-------------------------------------------*/
void HspDotNetBridge::GetAProperty(String^ propname, int index)
{
    try
    {
        if (HNBStack::CurrentClsObj->Class == nullptr)
        {
            HNBStack::LastStatus = HNBRIDGE_STATUS_NULL;
            return;
        }
        PropertyInfo^ info = HNBStack::CurrentClsObj->Class->GetProperty(propname);
        Object^ obj = info->GetValue(HNBStack::CurrentClsObj->Instance);
        if (obj == nullptr)
        {
            HNBStack::LastStatus = HNBRIDGE_STATUS_FAILED;
            return;
        }
        if (index >= ((Array^)obj)->Length)
        {
            HNBStack::LastStatus = HNBRIDGE_STATUS_FAILED;
            HNBStack::Exception = gcnew IndexOutOfRangeException(index.ToString());
            return;
        }
        int i = 0;
        for each (Object^ var in (Array^)obj)
        {
            if (i++ == index)
            {
                HNBStack::LastObject = var;
                break;
            }
        }
        HNBStack::LastStatus = HNBRIDGE_STATUS_SUCCESS;
        return;
    }
    catch (Exception^ e)
    {
        HNBStack::Exception = e;
    }
    HNBStack::LastStatus = HNBRIDGE_STATUS_FAILED;
}

/*-------------------------------------------
* HspDotNetBridge::SetDProperty(String p1, ...array<String> values)
* ���ݎ��s���̃N���X�I�u�W�F�N�g�ƃC���X�^���X�𗘗p���āA�v���p�e�B�̃R���N�V�����ɐV�K�ɒl��ݒ肵�܂��B
* �ǉ����s���ꍇ�́AAddDProperty(String p1, String p2, String p3)�𗘗p���Ă��������B
* �C���X�^���X���쐬����Ă��Ȃ��ꍇ�́A�ÓI�A�N�Z�X���s���܂��B
*-------------------------------------------*/
void HspDotNetBridge::SetDProperty(String^ fieldname, ... array<String^>^ values)
{
    HNBStack::LastStatus = HNBRIDGE_STATUS_NOT_IMPLEMENTED;
}

/*-------------------------------------------
* HspDotNetBridge::SetDProperty(String p1, String p2, String p3)
* ���ݎ��s���̃N���X�I�u�W�F�N�g�ƃC���X�^���X�𗘗p���āA�v���p�e�B�̃R���N�V�����ɒl��ǉ����܂��B
* �C���X�^���X���쐬����Ă��Ȃ��ꍇ�́A�ÓI�A�N�Z�X���s���܂��B
*-------------------------------------------*/
void HspDotNetBridge::AddDProperty(String^ fieldname, String^ key, String^ value)
{
    try
    {
        if (HNBStack::CurrentClsObj->Class == nullptr)
        {
            HNBStack::LastStatus = HNBRIDGE_STATUS_NULL;
            return;
        }
        PropertyInfo^ info = HNBStack::CurrentClsObj->Class->GetProperty(fieldname);
        List<Object^>^ list = gcnew List<Object^>();
        this->SetPrimitivePrms(this->GetPrimitivePrms(key), list);
        this->SetPrimitivePrms(this->GetPrimitivePrms(value), list);
        Object^ obj = info->GetValue(HNBStack::CurrentClsObj->Instance);

        System::Collections::IDictionary^ buffer = (System::Collections::IDictionary^)obj;
        buffer->Add(list[0], list[1]);
        //�K�v�Ȃ��C������
        info->SetValue(HNBStack::CurrentClsObj->Instance, buffer);
        HNBStack::LastStatus = HNBRIDGE_STATUS_SUCCESS;
        return;
    }
    catch (Exception^ e)
    {
        HNBStack::Exception = e;
    }
    HNBStack::LastStatus = HNBRIDGE_STATUS_FAILED;
}

/*-------------------------------------------
* HspDotNetBridge::DelDProperty(String p1, String p2)
* ���ݎ��s���̃N���X�I�u�W�F�N�g�ƃC���X�^���X�𗘗p���āA�t�B�[���h�̃R���N�V�����̎w�肳�ꂽ�L�[�̒l���폜���܂��B
* �C���X�^���X���쐬����Ă��Ȃ��ꍇ�́A�ÓI�A�N�Z�X���s���܂��B
*-------------------------------------------*/
void HspDotNetBridge::DelDProperty(String^ fieldname, String^ key)
{
    try
    {
        if (HNBStack::CurrentClsObj->Class == nullptr)
        {
            HNBStack::LastStatus = HNBRIDGE_STATUS_NULL;
            return;
        }
        PropertyInfo^ info = HNBStack::CurrentClsObj->Class->GetProperty(fieldname);
        List<Object^>^ list = gcnew List<Object^>();
        this->SetPrimitivePrms(this->GetPrimitivePrms(key), list);
        Object^ obj = info->GetValue(HNBStack::CurrentClsObj->Instance);

        System::Collections::IDictionary^ buffer = (System::Collections::IDictionary^)obj;
        buffer->Remove(list[0]);
        //�K�v�Ȃ��C������
        info->SetValue(HNBStack::CurrentClsObj->Instance, buffer);
        HNBStack::LastStatus = HNBRIDGE_STATUS_SUCCESS;
        return;
    }
    catch (Exception^ e)
    {
        HNBStack::Exception = e;
    }
    HNBStack::LastStatus = HNBRIDGE_STATUS_FAILED;
}

/*-------------------------------------------
* HspDotNetBridge::GetDProperty(String p1, String p2)
* ���ݎ��s���̃N���X�I�u�W�F�N�g�ƃC���X�^���X�𗘗p���āA�v���p�e�B�̃R���N�V�����̒l��ݒ肵�܂��B
* �C���X�^���X���쐬����Ă��Ȃ��ꍇ�́A�ÓI�A�N�Z�X���s���܂��B
*-------------------------------------------*/
void HspDotNetBridge::GetDProperty(String^ fieldname, String^ key)
{
    try
    {
        if (HNBStack::CurrentClsObj->Class == nullptr)
        {
            HNBStack::LastStatus = HNBRIDGE_STATUS_NULL;
            return;
        }
        PropertyInfo^ info = HNBStack::CurrentClsObj->Class->GetProperty(fieldname);
        List<Object^>^ list = gcnew List<Object^>();
        this->SetPrimitivePrms(this->GetPrimitivePrms(key), list);
        Object^ obj = info->GetValue(HNBStack::CurrentClsObj->Instance);

        System::Collections::IDictionary^ buffer = (System::Collections::IDictionary^)obj;
        HNBStack::LastObject = buffer[list[0]];
        HNBStack::LastStatus = HNBRIDGE_STATUS_SUCCESS;
        return;
    }
    catch (Exception^ e)
    {
        HNBStack::Exception = e;
    }
    HNBStack::LastStatus = HNBRIDGE_STATUS_FAILED;
}

/*-------------------------------------------
* HspDotNetBridge::SetLProperty(String p1, ...array<String> values)
* ���ݎ��s���̃N���X�I�u�W�F�N�g�ƃC���X�^���X�𗘗p���āA�v���p�e�B�̃��X�g�ɐV�K�ɒl��ݒ肵�܂��B
* �ǉ����s���ꍇ�́AAddLProperty(String p1, String p2)�𗘗p���Ă��������B
* �C���X�^���X���쐬����Ă��Ȃ��ꍇ�́A�ÓI�A�N�Z�X���s���܂��B
*-------------------------------------------*/
void HspDotNetBridge::SetLProperty(String^ p1, ...array<String^>^ values)
{
    HNBStack::LastStatus = HNBRIDGE_STATUS_NOT_IMPLEMENTED;
}

/*-------------------------------------------
* HspDotNetBridge::AddLProperty(String p1, String p2)
* ���ݎ��s���̃N���X�I�u�W�F�N�g�ƃC���X�^���X�𗘗p���āA�v���p�e�B�̃��X�g�ɒl��ǉ����܂��B
* �C���X�^���X���쐬����Ă��Ȃ��ꍇ�́A�ÓI�A�N�Z�X���s���܂��B
*-------------------------------------------*/
void HspDotNetBridge::AddLProperty(String^ fieldname, String^ value)
{
    try
    {
        if (HNBStack::CurrentClsObj->Class == nullptr)
        {
            HNBStack::LastStatus = HNBRIDGE_STATUS_NULL;
            return;
        }
        PropertyInfo^ info = HNBStack::CurrentClsObj->Class->GetProperty(fieldname);
        List<Object^>^ list = gcnew List<Object^>();
        this->SetPrimitivePrms(this->GetPrimitivePrms(value), list);
        Object^ obj = info->GetValue(HNBStack::CurrentClsObj->Instance);

        System::Collections::IList^ buffer = (System::Collections::IList^)obj;
        buffer->Add(list[0]);
        //����Ȃ��C������
        info->SetValue(HNBStack::CurrentClsObj->Instance, buffer);
        HNBStack::LastStatus = HNBRIDGE_STATUS_SUCCESS;
        return;
    }
    catch (Exception^ e)
    {
        HNBStack::Exception = e;
    }
    HNBStack::LastStatus = HNBRIDGE_STATUS_FAILED;
}

/*-------------------------------------------
* HspDotNetBridge::DelLProperty(String p1, int p2)
* ���ݎ��s���̃N���X�I�u�W�F�N�g�ƃC���X�^���X�𗘗p���āA�v���p�e�B�̃��X�g�̎w�肳�ꂽ�L�[�̒l���폜���܂��B
* �C���X�^���X���쐬����Ă��Ȃ��ꍇ�́A�ÓI�A�N�Z�X���s���܂��B
*-------------------------------------------*/
void HspDotNetBridge::DelLProperty(String^ fieldname, int index)
{
    try
    {
        if (HNBStack::CurrentClsObj->Class == nullptr)
        {
            HNBStack::LastStatus = HNBRIDGE_STATUS_NULL;
            return;
        }
        PropertyInfo^ info = HNBStack::CurrentClsObj->Class->GetProperty(fieldname);
        Object^ obj = info->GetValue(HNBStack::CurrentClsObj->Instance);

        System::Collections::IList^ buffer = (System::Collections::IList^)obj;
        buffer->RemoveAt(index);
        //����Ȃ��C������
        info->SetValue(HNBStack::CurrentClsObj->Instance, buffer);
        HNBStack::LastStatus = HNBRIDGE_STATUS_SUCCESS;
        return;
    }
    catch (Exception^ e)
    {
        HNBStack::Exception = e;
    }
    HNBStack::LastStatus = HNBRIDGE_STATUS_FAILED;
}

/*-------------------------------------------
* HspDotNetBridge::GetCProperty(String p1, String p2)
* ���ݎ��s���̃N���X�I�u�W�F�N�g�ƃC���X�^���X�𗘗p���āA�v���p�e�B�̃��X�g�̎w�肳�ꂽ�C���f�b�N�X�ɊY������l���擾���܂��B
* �C���X�^���X���쐬����Ă��Ȃ��ꍇ�́A�ÓI�A�N�Z�X���s���܂��B
*-------------------------------------------*/
void HspDotNetBridge::GetLProperty(String^ fieldname, int index)
{
    try
    {
        if (HNBStack::CurrentClsObj->Class == nullptr)
        {
            HNBStack::LastStatus = HNBRIDGE_STATUS_NULL;
            return;
        }
        PropertyInfo^ info = HNBStack::CurrentClsObj->Class->GetProperty(fieldname);
        Object^ obj = info->GetValue(HNBStack::CurrentClsObj->Instance);

        System::Collections::IList^ buffer = (System::Collections::IList^)obj;
        HNBStack::LastObject = buffer[index];
        HNBStack::LastStatus = HNBRIDGE_STATUS_SUCCESS;
        return;
    }
    catch (Exception^ e)
    {
        HNBStack::Exception = e;
    }
    HNBStack::LastStatus = HNBRIDGE_STATUS_FAILED;
}

/*-------------------------------------------
* HspDotNetBridge::LoadAssembly(String p1)
* �w�肳�ꂽ�p�X��.DLL��ǂݍ��݁A���ݎ��s���̃N���X�I�u�W�F�N�g�Ɋi�[���܂��B
*-------------------------------------------*/
void HspDotNetBridge::LoadAssembly(String^ path)
{
    try
    {
        HNBStack::CurrentClsObj->Assembly = Assembly::LoadFrom(path);
        HNBStack::LastStatus = HNBRIDGE_STATUS_SUCCESS;
        return;
    }
    catch (Exception^ e)
    {
        HNBStack::Exception = e;
    }
    HNBStack::LastStatus = HNBRIDGE_STATUS_FAILED;
}

/*-------------------------------------------
* HspDotNetBridge::UnLoadAssembly()
* �w�肳�ꂽ�p�X��.DLL��ǂݍ��݁A���ݎ��s���̃N���X�I�u�W�F�N�g�Ɋi�[���܂��B
*-------------------------------------------*/
void HspDotNetBridge::UnloadAssembly()
{
    HNBStack::CurrentClsObj->Assembly = nullptr;
    HNBStack::LastStatus = HNBRIDGE_STATUS_SUCCESS;
}

/*-------------------------------------------
* HspDotNetBridge::InvokeMethod(String p1, ...array<String> args)
* ���ݎ��s���̃N���X�I�u�W�F�N�g�ƃC���X�^���X�𗘗p���āA���\�b�h�����s���܂��B
* �C���X�^���X���쐬����Ă��Ȃ��ꍇ�́A�ÓI�A�N�Z�X���s���܂��B
*-------------------------------------------*/
void HspDotNetBridge::InvokeMethod(String^ methodname, ... array<String^>^ arguments)
{
    try
    {
        if (HNBStack::CurrentClsObj->Class == nullptr)
        {
            HNBStack::LastStatus = HNBRIDGE_STATUS_NULL;
            return;
        }
        if (arguments->Length == 0)
        {
            MethodInfo^ info = HNBStack::CurrentClsObj->Class->GetMethod(methodname, gcnew array<Type^>(0));
            Object^ obj = info->Invoke(HNBStack::CurrentClsObj->Instance, nullptr);
            if (obj != nullptr)
            {
                HNBStack::LastObject = obj;
                HNBStack::LastStatus = HNBRIDGE_STATUS_SUCCESS;
                return;
            }
        }
        else
        {
            List<Type^>^ types = gcnew List<Type^>();
            List<Object^>^ prms = gcnew List<Object^>();
            for each (String^ var in arguments)
            {
                Tuple<Type^, String^>^ tuple = this->GetPrimitivePrms(var);
                types->Add(tuple->Item1);
                this->SetPrimitivePrms(tuple, prms);
            }
            MethodInfo^ info = HNBStack::CurrentClsObj->Class->GetMethod(methodname, types->ToArray());
            Object^ obj = info->Invoke(HNBStack::CurrentClsObj->Instance, prms->ToArray());
            if (obj != nullptr)
            {
                HNBStack::LastObject = obj;
                HNBStack::LastStatus = HNBRIDGE_STATUS_SUCCESS;
                return;
            }
        }
        HNBStack::LastStatus = HNBRIDGE_STATUS_NULL;
        return;
    }
    catch (Exception^ e)
    {
        HNBStack::Exception = e;
    }
    HNBStack::LastStatus = HNBRIDGE_STATUS_FAILED;
}

/*-------------------------------------------
* HspDotNetBridge::InvokeMethodOnThread(String p1, ...array<String> args)
* ���ݎ��s���̃N���X�I�u�W�F�N�g�ƃC���X�^���X�𗘗p���āA�X���b�h�𗧂��グ�Ă��̏�Ń��\�b�h�����s���܂��B
* �C���X�^���X���쐬����Ă��Ȃ��ꍇ�́A�ÓI�A�N�Z�X���s���܂��B
*-------------------------------------------*/
void HspDotNetBridge::InvokeMethodOnThread(String^ methodname, ... array<String^>^ arguments)
{
    HNBStack::LastStatus = HNBRIDGE_STATUS_NOT_IMPLEMENTED;
}

// PRIVATE
Tuple<Type^, String^>^ HspDotNetBridge::GetPrimitivePrms(String^ prm)
{
    if (prm->StartsWith("byte://") || prm->StartsWith("Byte://"))
    {
        return Tuple::Create(Byte::typeid, prm->Substring(7));
    }
    if (prm->StartsWith("sbyte://") || prm->StartsWith("SByte://"))
    {
        return Tuple::Create(SByte::typeid, prm->Substring(8));
    }
    if (prm->StartsWith("int://") || prm->StartsWith("Int32://"))
    {
        return Tuple::Create(int::typeid, prm->StartsWith("int://") ? prm->Substring(6) : prm->Substring(8));
    }
    if (prm->StartsWith("uint://") || prm->StartsWith("UInt32://"))
    {
        return Tuple::Create(UInt32::typeid, prm->StartsWith("uint://") ? prm->Substring(7) : prm->Substring(9));
    }
    if (prm->StartsWith("short://") || prm->StartsWith("Int16://"))
    {
        return Tuple::Create(short::typeid, prm->Substring(8));
    }
    if (prm->StartsWith("ushort://") || prm->StartsWith("UInt16://"))
    {
        return Tuple::Create(UInt16::typeid, prm->Substring(9));
    }
    if (prm->StartsWith("long://") || prm->StartsWith("Int64://"))
    {
        return Tuple::Create(long::typeid, prm->StartsWith("long://") ? prm->Substring(7) : prm->Substring(8));
    }
    if (prm->StartsWith("float://") || prm->StartsWith("Single://"))
    {
        return Tuple::Create(float::typeid, prm->StartsWith("float://") ? prm->Substring(8) : prm->Substring(9));
    }
    if (prm->StartsWith("double://") || prm->StartsWith("Double://"))
    {
        return Tuple::Create(double::typeid, prm->Substring(9));
    }
    if (prm->StartsWith("char://") || prm->StartsWith("Char://"))
    {
        return Tuple::Create(char::typeid, prm->Substring(7));
    }
    if (prm->StartsWith("bool://") || prm->StartsWith("Boolean://"))
    {
        return Tuple::Create(bool::typeid, prm->StartsWith("bool://") ? prm->Substring(7) : prm->Substring(10));
    }
    if (prm->StartsWith("string://") || prm->StartsWith("String://"))
    {
        return Tuple::Create(String::typeid, prm->Substring(9));
    }
    if (prm->StartsWith("decimal://") || prm->StartsWith("Decimal://"))
    {
        return Tuple::Create(Decimal::typeid, prm->Substring(10));
    }
    if (prm->StartsWith("class://"))
    {
        ClassObject^ obj;
        if (!HNBStack::ClassObjects->TryGetValue(prm->Substring(8), obj))
        {
            return Tuple::Create(String::typeid, prm);
        }
        //SPC: Tuple.Create(Type, "class://");
        return Tuple::Create(obj->Class, prm);
    }
    return Tuple::Create(String::typeid, prm);
}

void HspDotNetBridge::SetPrimitivePrms(Tuple<Type^, String^>^ tuple, List<Object^>^ prms)
{
    String^ name = tuple->Item1->Name;
    if (String::Compare(name, "Byte") == 0)
    {
        prms->Add(Byte::Parse(tuple->Item2));
        return;
    }
    if (String::Compare(name, "SByte") == 0)
    {
        prms->Add(SByte::Parse(tuple->Item2));
        return;
    }
    if (String::Compare(name, "Int32") == 0)
    {
        prms->Add(Int32::Parse(tuple->Item2));
        return;
    }
    if (String::Compare(name, "Int16") == 0)
    {
        prms->Add(Int16::Parse(tuple->Item2));
        return;
    }
    if (String::Compare(name, "UInt16") == 0)
    {
        prms->Add(UInt16::Parse(tuple->Item2));
        return;
    }
    if (String::Compare(name, "Int64") == 0)
    {
        prms->Add(Int64::Parse(tuple->Item2));
        return;
    }
    if (String::Compare(name, "UInt64") == 0)
    {
        prms->Add(UInt64::Parse(tuple->Item2));
        return;
    }
    if (String::Compare(name, "Single") == 0)
    {
        prms->Add(Single::Parse(tuple->Item2));
        return;
    }
    if (String::Compare(name, "Double") == 0)
    {
        prms->Add(Double::Parse(tuple->Item2));
        return;
    }
    if (String::Compare(name, "Char") == 0)
    {
        prms->Add(Char::Parse(tuple->Item2));
        return;
    }
    if (String::Compare(name, "Boolean") == 0)
    {
        prms->Add(Boolean::Parse(tuple->Item2));
        return;
    }
    if (String::Compare(name, "String") == 0)
    {
        prms->Add(tuple->Item2);
        return;
    }
    if (String::Compare(name, "Decimal") == 0)
    {
        prms->Add(Decimal::Parse(tuple->Item2));
        return;
    }
    ClassObject^ obj;
    if (HNBStack::ClassObjects->TryGetValue(tuple->Item2->Substring(8), obj))
    {
        prms->Add(obj->Instance);
    }
}