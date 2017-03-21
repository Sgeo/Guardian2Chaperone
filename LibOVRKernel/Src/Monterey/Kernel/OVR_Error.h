/************************************************************************************

Filename    :   OVR_Error.h
Content     :   Structs and functions for handling OVRErrors
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

#pragma once

#include "Kernel/OVR_System.h"
#include <string>


#ifndef OVR_RESULT_DEFINED
#define OVR_RESULT_DEFINED ///< Allows ovrResult to be independently defined.
    /// API call results are represented at the highest level by a single ovrResult.
    typedef int32_t ovrResult;
#endif


// Success is a value greater or equal to 0, while all error types are negative values.
#ifndef OVR_SUCCESS_DEFINED
#define OVR_SUCCESS_DEFINED ///< Allows ovrResult to be independently defined.
    typedef enum ovrSuccessType_
    {
        /// This is a general success result. Use OVR_SUCCESS to test for success.
        ovrSuccess = 0,
    } ovrSuccessType;
#endif


/// -----------------------------------------------------------------------------
/// ***** OVR_FILE / OVR_LINE
///
#if !defined(OVR_FILE)
    #if defined(OVR_BUILD_DEBUG)
        #define OVR_FILE __FILE__
        #define OVR_LINE __LINE__
    #else
        #define OVR_FILE nullptr
        #define OVR_LINE 0
    #endif
#endif

// LOG_VAARG_ATTRIBUTE macro, enforces printf-style formatting for message types
#ifdef __GNUC__
// LDC - TODO
#  define OVR_LOG_VAARG_ATTRIBUTE(a,b)
//#  define OVR_LOG_VAARG_ATTRIBUTE(a,b) __attribute__((format (printf, a, b)))
#else
#  define OVR_LOG_VAARG_ATTRIBUTE(a,b)
#endif



namespace OVR {


/// -----------------------------------------------------------------------------
/// ***** OVR_MAKE_ERROR, OVR_MAKE_ERROR_F, OVR_MAKE_SYS_ERROR, OVR_MAKE_SYS_ERROR_F
///
/// Declaration:
///      OVRError OVR_MAKE_ERROR(ovrResult code, const char* pDescription);
///      OVRError OVR_MAKE_ERROR_F(ovrResult code, const char* pFormat, ...);
///
///      OVRError OVR_MAKE_SYS_ERROR(ovrResult code, ovrSysErrorCode sysCode, const char* pDescription);
///      OVRError OVR_MAKE_SYS_ERROR_F(ovrResult code, ovrSysErrorCode sysCode, const char* pFormat, ...);
///
/// Example usage:
///      OVRError InitGraphics()
///      {
///          if(!GraphicsCardPresent())
///          {
///              return OVR_MAKE_ERROR(ovrError_GraphicsInit, "Failed to init graphics; graphics support absent.");
///          }
///
///          HRESULT hr = pDevice->CreateTexture2D(&dsDesc, nullptr, &Texture);
///          if(FAILED(hr))
///          {
///              return OVR_MAKE_SYS_ERROR_F(ovrError_GraphicsInit, hr, "Failed to create texture of size %u x %u", dsDesc.Width, dsDesc.Height);
///          }
///          or:
///              OVR_HR_CHECK_RET_ERROR(ovrError_GraphicsInit, hr, "Failed to create texture of size %u x %u", dsDesc.Width, dsDesc.Height);
///
///          return ovrSuccess; // Converts to an OVRError instance that has no error.
///      }
///
#define OVR_MAKE_ERROR(errorCode, pDescription) \
    OVR::MakeError((errorCode), OVR::ovrSysErrorCodeSuccess, OVR_FILE, OVR_LINE, true, true, nullptr, "%s", (pDescription))

// Note: The format string is the first part of the .../__VA_ARGS__ as per the C99-C++11 Standards.
#define OVR_MAKE_ERROR_F(errorCode, ...) \
    OVR::MakeError((errorCode), OVR::ovrSysErrorCodeSuccess, OVR_FILE, OVR_LINE, true, true, nullptr, __VA_ARGS__)

#define OVR_MAKE_SYS_ERROR(errorCode, sysErrorCode, pDescription) \
    OVR::MakeError((errorCode), (sysErrorCode), OVR_FILE, OVR_LINE, true, true, nullptr, "%s", (pDescription))

// Note: The format string is the first part of the .../__VA_ARGS__ as per the C99-C++11 Standards.
#define OVR_MAKE_SYS_ERROR_F(errorCode, sysErrorCode, ...) \
    OVR::MakeError((errorCode), (sysErrorCode), OVR_FILE, OVR_LINE, true, true, nullptr, __VA_ARGS__)

// Consider using OVR_MAKE_QUIET_ERROR() instead of OVR_MAKE_ERROR() where the error
// should not automatically log/assert.  If the error is normal (HMD unplugged) or
// repetitive then please use OVR_MAKE_QUIET_ERROR().

#define OVR_MAKE_QUIET_ERROR(errorCode, pDescription) \
    OVR::MakeError((errorCode), OVR::ovrSysErrorCodeSuccess, OVR_FILE, OVR_LINE, false, false, nullptr, "%s", (pDescription))

// Note: The format string is the first part of the .../__VA_ARGS__ as per the C99-C++11 Standards.
#define OVR_MAKE_QUIET_ERROR_F(errorCode, ...) \
    OVR::MakeError((errorCode), OVR::ovrSysErrorCodeSuccess, OVR_FILE, OVR_LINE, false, false, nullptr, __VA_ARGS__)

#define OVR_MAKE_QUIET_SYS_ERROR(errorCode, sysErrorCode, pDescription) \
    OVR::MakeError((errorCode), (sysErrorCode), OVR_FILE, OVR_LINE, false, false, nullptr, "%s", (pDescription))

// Note: The format string is the first part of the .../__VA_ARGS__ as per the C99-C++11 Standards.
#define OVR_MAKE_QUIET_SYS_ERROR_F(errorCode, sysErrorCode, ...) \
    OVR::MakeError((errorCode), (sysErrorCode), OVR_FILE, OVR_LINE, false, false, nullptr, __VA_ARGS__)

// Consider using OVR_MAKE_NOASSERT_ERROR() instead of OVR_MAKE_ERROR() where the error
// should not automatically log/assert.  If the error is normal (HMD unplugged) or
// repetitive then please use OVR_MAKE_NOASSERT_ERROR().

#define OVR_MAKE_NOASSERT_ERROR(errorCode, pDescription) \
    OVR::MakeError((errorCode), OVR::ovrSysErrorCodeSuccess, OVR_FILE, OVR_LINE, true, false, nullptr, "%s", (pDescription))

// Note: The format string is the first part of the .../__VA_ARGS__ as per the C99-C++11 Standards.
#define OVR_MAKE_NOASSERT_ERROR_F(errorCode, ...) \
    OVR::MakeError((errorCode), OVR::ovrSysErrorCodeSuccess, OVR_FILE, OVR_LINE, true, false, nullptr, __VA_ARGS__)

#define OVR_MAKE_NOASSERT_SYS_ERROR(errorCode, sysErrorCode, pDescription) \
    OVR::MakeError((errorCode), (sysErrorCode), OVR_FILE, OVR_LINE, true, false, nullptr, "%s", (pDescription))

// Note: The format string is the first part of the .../__VA_ARGS__ as per the C99-C++11 Standards.
#define OVR_MAKE_NOASSERT_SYS_ERROR_F(errorCode, sysErrorCode, ...) \
    OVR::MakeError((errorCode), (sysErrorCode), OVR_FILE, OVR_LINE, true, false, nullptr, __VA_ARGS__)


/// -----------------------------------------------------------------------------
/// ***** ovrSysErrorCode
///
/// Identifies a platform-specific error identifier.
/// For Windows this means an HRESULT or DWORD system error code from GetLastError.
/// For Unix this means errno.
///
typedef uint32_t ovrSysErrorCode;


/// -----------------------------------------------------------------------------
/// ***** ovrSysErrorCodeSuccess
///
/// Identifies a ovrSysErrorCode that's success.
///
const ovrSysErrorCode ovrSysErrorCodeSuccess = 0;


/// -----------------------------------------------------------------------------
/// ***** ovrSysErrorCodeNone
///
/// Identifies a ovrSysErrorCode that's un-set.
///
const ovrSysErrorCode ovrSysErrorCodeNone = 0;



/// -----------------------------------------------------------------------------
/// ***** OVRError
///
/// Represents an error and relevant information about it.
/// While you can create error instances directly via this class, it's better if
/// you create them via the OVR_MAKE_ERROR family of macros, or at least via the
/// MakeError function.
///
/// Relevant design analogues:
///     https://developer.apple.com/library/mac/documentation/Cocoa/Reference/Foundation/Classes/NSError_Class/
///     https://msdn.microsoft.com/en-us/library/windows/desktop/ms723041%28v=vs.85%29.aspx
///
class OVRError
{
    // Cannot convert boolean to OVRError - It must be done explicitly.
    OVRError(bool) { OVR_ASSERT(false); }

public:
    OVRError();
    OVRError(ovrResult code); // Intentionally not explicit.

    OVRError(const OVRError& OVRError);
    OVRError(OVRError&& OVRError);

    virtual ~OVRError();

    // Construct a success code.  Use Succeeded() to check for success.
    static OVRError Success() { return OVRError(); }

    OVRError& operator=(const OVRError& OVRError);
    OVRError& operator=(OVRError&& OVRError);

    // Use this to check if result is a success code
    bool Succeeded() const
    {
        return Code >= ovrSuccess;
    }
    bool Failed() const
    {
        return !Succeeded();
    }

    // Clears all members to a newly default-constructed state.
    void Reset();

    // Property accessors
    void      SetCode(ovrResult code);
    ovrResult GetCode() const;

    void            SetSysCode(ovrSysErrorCode sysCode);
    ovrSysErrorCode GetSysCode() const;

    void   SetDescription(const char* pDescription);
    std::string GetDescription() const;

    void   SetContext(const char* pContext);
    std::string GetContext() const;

    void        SetSource(const char* pSourceFilePath, int sourceFileLine);

protected:
    ovrResult         Code;                /// The main ovrResult, which is a high level error id.
    ovrSysErrorCode   SysCode;             /// May be ovrSysErrorCodeSuccess to indicate there isn't a relevant system error code.
    std::string       Description;         /// Unlocalized error description string.
    std::string       Context;             /// Context string. For example, for a file open failure this is the file path.
    std::string       SourceFilePath;      /// The __FILE__ where the error was first encountered.
    int               SourceFileLine;      /// The __LINE__ where the error was first encountered.
};


OVRError MakeError(ovrResult errorCode, ovrSysErrorCode sysCode, const char* pSourceFile,
    int sourceLine, bool logError, bool assertError, const char* pContext,
    const char* pDescriptionFormat, ...) OVR_LOG_VAARG_ATTRIBUTE(7, 8);


} // namespace OVR
