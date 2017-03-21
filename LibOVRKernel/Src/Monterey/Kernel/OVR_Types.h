/************************************************************************************

Filename    :   OVR_Types.h
Content     :   Standard library defines and simple types
Created     :   September 19, 2012
Notes       : 

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

#ifndef OVR_Types_h
#define OVR_Types_h


//-----------------------------------------------------------------------------------
// ****** Operating system identification
//
// Try to use the most generic version of these defines as possible in order to achieve
// the simplest portable code. For example, instead of using #if (defined(OVR_OS_IPHONE) || defined(OVR_OS_MAC)),
// consider using #if defined(OVR_OS_APPLE).
//
// Type definitions exist for the following operating systems: (OVR_OS_x)
//
//    WIN32      - Win32 and Win64 (Windows XP and later) Does not include Microsoft phone and console platforms, despite that Microsoft's _WIN32 may be defined by the compiler for them.
//    WIN64      - Win64 (Windows XP and later)
//    MAC        - Mac OS X (may be defined in addition to BSD)
//    LINUX      - Linux
//    BSD        - BSD Unix
//    ANDROID    - Android (may be defined in addition to LINUX)
//    IPHONE     - iPhone
//    MS_MOBILE  - Microsoft mobile OS.
//
//  Meta platforms
//    MS        - Any OS by Microsoft (e.g. Win32, Win64, phone, console)
//    APPLE     - Any OS by Apple (e.g. iOS, OS X)
//    UNIX      - Linux, BSD, Mac OS X.
//    MOBILE    - iOS, Android, Microsoft phone
//    CONSOLE   - Console platforms.
//

#if (defined(__APPLE__) && (defined(__GNUC__) ||\
     defined(__xlC__) || defined(__xlc__))) || defined(__MACOS__)
#  if (defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) || defined(__IPHONE_OS_VERSION_MIN_REQUIRED))
#      if !defined(OVR_OS_IPHONE)
#        define OVR_OS_IPHONE
#      endif
#  else
#    if !defined(OVR_OS_MAC)
#      define OVR_OS_MAC
#    endif
#    if !defined(OVR_OS_DARWIN)
#      define OVR_OS_DARWIN
#    endif
#    if !defined(OVR_OS_BSD)
#      define OVR_OS_BSD
#    endif
#  endif
#elif (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))
#  if !defined(OVR_OS_WIN64)
#      define OVR_OS_WIN64
#  endif
#  if !defined(OVR_OS_WIN32)
#      define OVR_OS_WIN32  //Can be a 32 bit Windows build or a WOW64 support for Win32.  In this case WOW64 support for Win32.
#  endif
#elif (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#  if !defined(OVR_OS_WIN32)
#      define OVR_OS_WIN32  //Can be a 32 bit Windows build or a WOW64 support for Win32.  In this case WOW64 support for Win32.
#  endif
#elif defined(ANDROID) || defined(__ANDROID__)
#  if !defined(OVR_OS_ANDROID)
#      define OVR_OS_ANDROID
#  endif
#  if !defined(OVR_OS_LINUX)
#      define OVR_OS_LINUX
#  endif
#elif defined(__linux__) || defined(__linux)
#  if !defined(OVR_OS_LINUX)
#      define OVR_OS_LINUX
#  endif
#elif defined(_BSD_) || defined(__FreeBSD__)
#  if !defined(OVR_OS_BSD)
#      define OVR_OS_BSD
#  endif
#else
#  if !defined(OVR_OS_OTHER)
#      define OVR_OS_OTHER
#  endif
#endif

#if !defined(OVR_OS_MS_MOBILE)
#   if (defined(_M_ARM) || defined(_M_IX86) || defined(_M_AMD64)) && !defined(OVR_OS_WIN32) && !defined(OVR_OS_CONSOLE)
#       define OVR_OS_MS_MOBILE
#   endif
#endif

#if !defined(OVR_OS_MS)
#   if defined(OVR_OS_WIN32) || defined(OVR_OS_WIN64) || defined(OVR_OS_MS_MOBILE)
#       define OVR_OS_MS
#   endif
#endif

#if !defined(OVR_OS_APPLE)
#   if defined(OVR_OS_MAC) || defined(OVR_OS_IPHONE)
#       define OVR_OS_APPLE
#   endif
#endif

#if !defined(OVR_OS_UNIX)
#   if defined(OVR_OS_ANDROID) || defined(OVR_OS_BSD) || defined(OVR_OS_LINUX) || defined(OVR_OS_MAC)
#       define OVR_OS_UNIX
#   endif
#endif

#if !defined(OVR_OS_MOBILE)
#   if defined(OVR_OS_ANDROID) || defined(OVR_OS_IPHONE) || defined(OVR_OS_MS_MOBILE)
#       define OVR_OS_MOBILE
#   endif
#endif


//-----------------------------------------------------------------------------------
// ***** Type definitions for Common Systems

namespace OVR {


#if defined(OVR_OS_WIN32)

#elif defined(OVR_OS_MAC) || defined(OVR_OS_IPHONE) || defined(OVR_CC_GNU)

#else

//#include <sys/types.h>
#include <cstdint>

#endif


//-----------------------------------------------------------------------------------
// ***** Macro Definitions
//
// We define the following:
//
//  OVR_CDECL           - Use cdecl calling convention (C argument order)
//

#if defined(OVR_OS_MS)

#else

// Calling convention - goes after function return type but before function name
#define OVR_CDECL

#endif // defined(OVR_OS_WIN32)

} // namespace OVR


// ------------------------------------------------------------------------
// ***** OVR_FORCE_INLINE
//
// Force inline substitute - goes before function declaration
// Example usage:
//     OVR_FORCE_INLINE void Test();

#if !defined(OVR_FORCE_INLINE)
    #if defined(OVR_CC_MSVC)
        #define OVR_FORCE_INLINE  __forceinline
    #elif defined(OVR_CC_GNU)
        #define OVR_FORCE_INLINE  __attribute__((always_inline)) inline
    #else
        #define OVR_FORCE_INLINE  inline
    #endif  // OVR_CC_MSVC
#endif


//-----------------------------------------------------------------------------------
// ***** OVR_DEBUG_BREAK,
//       OVR_ASSERT,
//
// Macros have effect only in debug builds.
//
// Example OVR_DEBUG_BREAK usage (note the lack of parentheses):
//     #define MY_ASSERT(expression) do { if (!(expression)) { OVR_DEBUG_BREAK; } } while(0)
//
//
// Example OVR_ASSERT usage:
//     OVR_ASSERT(count < 100);
//
#ifndef OVR_BUILD_DEBUG

#  define OVR_DEBUG_BREAK  ((void)0)

#ifndef OVR_FAIL_M
#define OVR_FAIL_M(message)  ((void)0)
#endif
#ifndef OVR_FAIL
#define OVR_FAIL()           ((void)0)
#endif

#  define OVR_ASSERT(p)    ((void)0)

#else 

// Microsoft Win32 specific debugging support
#if defined(OVR_OS_WIN32)
#  ifdef OVR_CPU_X86
#    if defined(__cplusplus_cli)
#      define OVR_DEBUG_BREAK   do { __debugbreak(); } while(0)
#    elif defined(OVR_CC_GNU)
#      define OVR_DEBUG_BREAK   do { OVR_ASM("int $3\n\t"); } while(0)
#    else
#      define OVR_DEBUG_BREAK   do { OVR_ASM int 3 } while (0)
#    endif
#  else
#    define OVR_DEBUG_BREAK     do { __debugbreak(); } while(0)
#  endif
// Android specific debugging support
#elif defined(OVR_OS_ANDROID)
#  include <android/log.h>
#  define OVR_EXPAND1( s )      #s
#  define OVR_EXPAND( s ) OVR_EXPAND1( s )
#  define OVR_DEBUG_BREAK       do { __builtin_trap(); } while(0)
#  define OVR_ASSERT(p)         do { if (!(p)) { __android_log_write( ANDROID_LOG_WARN, "OVR", "ASSERT@ " __FILE__ "(" OVR_EXPAND( __LINE__ ) "): " #p ); OVR_DEBUG_BREAK; } } while(0)
// Unix specific debugging support
#elif defined(OVR_CPU_X86) || defined(OVR_CPU_X86_64)
#  define OVR_DEBUG_BREAK       do { OVR_ASM("int $3\n\t"); } while(0)
#else
#  define OVR_DEBUG_BREAK       do { *((int *) 0) = 1; } while(0)
#endif

#if !defined( OVR_ASSERT ) // Android currently defines its own version of OVR_ASSERT() with logging
// This will cause compiler breakpoint
#define OVR_ASSERT(p)           do { if (!(p))  { OVR_DEBUG_BREAK; } } while(0)
#endif

#ifndef OVR_FAIL_M
#define OVR_FAIL_M(message)                                                                            \
                {                                                                                                  \
                    OVR_DEBUG_BREAK;                                                                           \
                }
#endif

#ifndef OVR_FAIL
#define OVR_FAIL()  \
                OVR_FAIL_M("Assertion failure")  
#endif


#endif // OVR_BUILD_DEBUG


// ------------------------------------------------------------------------
// ***** OVR_COMPILER_ASSERT
//
// Compile-time assert; produces compiler error if condition is false.
// The expression must be a compile-time constant expression.
// This macro is deprecated in favor of static_assert, which provides better
// compiler output and works in a broader range of contexts.
// 
// Example usage:
//     OVR_COMPILER_ASSERT(sizeof(int32_t == 4));

#if !defined(OVR_COMPILER_ASSERT)
#define OVR_COMPILER_ASSERT(expression)        static_assert(expression, #expression)
#define OVR_COMPILER_ASSERT_M(expression, msg) static_assert(expression, msg)
#endif


#endif  // OVR_Types_h
