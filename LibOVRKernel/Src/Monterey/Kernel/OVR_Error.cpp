/************************************************************************************

PublicHeader:   None
Filename    :   OVR_Error.cpp
Content     :   Structs and functions for handling OVRErrorInfos
Created     :   February 15, 2015
Copyright   :   Copyright 2014-2016 Oculus VR, LLC All Rights reserved.

Licensed under the Oculus VR Rift SDK License Version 3.3 (the "License");
you may not use the Oculus VR Rift SDK except in compliance with the License,
which is provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

You may obtain a copy of the License at

http://www.oculusvr.com/licenses/LICENSE-3.3

Unless required by applicable law or agreed to in writing, the Oculus VR SDK
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

************************************************************************************/

#include "OVR_Error.h"
#include "OVR_Types.h"

#include <android/log.h>

#define APP_NAME "LibOVRKernel"

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, APP_NAME, __VA_ARGS__);
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, APP_NAME, __VA_ARGS__);
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, APP_NAME, __VA_ARGS__);
#ifdef DEBUG_MODE
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, APP_NAME, __VA_ARGS__);
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, APP_NAME, __VA_ARGS__);
#else
#define LOGD(...)
#define LOGV(...)
#endif


namespace OVR {

static void OVRRemoveTrailingNewlines(std::string& s)
{
    while(!s.empty() && ((s.back() == '\n') || (s.back() == '\r')))
        s.pop_back();
}


OVRError::OVRError()
{
    Reset();
}

OVRError::OVRError(ovrResult code)
    : OVRError()
{
    Code = code;
}

OVRError::OVRError(const OVRError& ovrError)
{
    operator=(ovrError);
}


OVRError::OVRError(OVRError&& ovrError)
{
    operator=(std::move(ovrError));
}

OVRError::~OVRError()
{
    // Empty
}

OVRError& OVRError::operator=(const OVRError& ovrError)
{
    Code             = ovrError.Code;

    return *this;
}

OVRError& OVRError::operator=(OVRError&& ovrError)
{
    Code             = ovrError.Code;

    return *this;
}

void OVRError::Reset()
{
    Code = ovrSuccess;
}

void OVRError::SetCode(ovrResult code)
{
    Code = code;
}

ovrResult OVRError::GetCode() const
{
    return Code;
}

void OVRError::SetSysCode(ovrSysErrorCode sysCode)
{
    SysCode = sysCode;
}


ovrSysErrorCode OVRError::GetSysCode() const
{
    return SysCode;
}


void OVRError::SetDescription(const char* pDescription)
{
    if (pDescription)
    {
        Description = pDescription;
        OVRRemoveTrailingNewlines(Description); // Users sometimes send text with trailing newlines, which have no purpose in the error report.
    }
    else
        Description.clear();
}


std::string OVRError::GetDescription() const
{
    return Description;
}


void OVRError::SetContext(const char* pContext)
{
    if (pContext)
    {
        Context = pContext;
        OVRRemoveTrailingNewlines(Context);
    }
    else
        Context.clear();
}


std::string OVRError::GetContext() const
{
    return Context;
}


void OVRError::SetSource(const char* pSourceFilePath, int sourceFileLine)
{
    if (pSourceFilePath)
        SourceFilePath = pSourceFilePath;
    else
        SourceFilePath.clear();
    SourceFileLine = sourceFileLine;
}


// LDC - Taken from Kernel/OVR_String_FormatUtil.cpp
std::string FormatV(const char* format, va_list argList)
{
    char    buffer[512];
    char*   bufferUsed = buffer;
    char*   bufferAllocated = NULL;

    va_list argListSaved;
    va_copy(argListSaved, argList);

    int requiredStrlen = vsnprintf(bufferUsed, sizeof(buffer), format, argListSaved); // The large majority of the time this will succeed.

    if (requiredStrlen >= (int)sizeof(buffer)) // If the initial capacity wasn't enough...
    {
        bufferAllocated = new char[requiredStrlen + 1];
        bufferUsed = bufferAllocated;
        if (bufferAllocated)
        {
            va_end(argListSaved);
            va_copy(argListSaved, argList);
            requiredStrlen = vsnprintf(bufferAllocated, (requiredStrlen + 1), format, argListSaved);
        }
    }

    if (requiredStrlen < 0) // If there was a printf format error...
    {
        bufferUsed = NULL;
    }

    va_end(argListSaved);

    std::string result;

    if (bufferUsed)
        result = std::string(bufferUsed);

    if (bufferAllocated)
        delete[] bufferAllocated;

    return result;
}

OVRError MakeError(ovrResult errorCode, ovrSysErrorCode sysCode, const char* pSourceFile,
                    int sourceLine, bool logError, bool assertError, const char* pContext, const char* pDescriptionFormat, ...)
{
    OVRError ovrError(errorCode);

    ovrError.SetSysCode(sysCode);

    va_list argList;
    va_start(argList, pDescriptionFormat);
    std::string result = FormatV(pDescriptionFormat, argList);
    va_end(argList);
    ovrError.SetDescription(result.c_str());

    ovrError.SetContext(pContext);

    ovrError.SetSource(pSourceFile, sourceLine);

    // If logging the error:
    if (logError)
    {
        LOGE(ovrError.GetDescription().c_str());
    }

    // If asserting the error:
    if (assertError)
    {
        // Assert in debug mode to alert unit tester/developer of the error as it occurs.
        OVR_FAIL_M(ovrError.GetDescription().c_str());
    }

    return ovrError;
}

}  // namespace OVR
