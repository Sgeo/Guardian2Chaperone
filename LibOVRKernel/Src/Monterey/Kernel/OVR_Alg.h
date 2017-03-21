/************************************************************************************

Filename    :   OVR_Alg.h
Content     :   Simple general purpose algorithms: Sort, Binary Search, etc.
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

#ifndef OVR_Alg_h
#define OVR_Alg_h

#include "OVR_Types.h"

namespace OVR { namespace Alg {



//-----------------------------------------------------------------------------------
// ***** Operator extensions

template <typename T> OVR_FORCE_INLINE void Swap(T &a, T &b) 
{  T temp(a); a = b; b = temp; }


// ***** min/max are not implemented in Visual Studio 6 standard STL

template <typename T> OVR_FORCE_INLINE const T Min(const T a, const T b)
{ return (a < b) ? a : b; }

template <typename T> OVR_FORCE_INLINE const T Max(const T a, const T b)
{ return (b < a) ? a : b; }

template <typename T> OVR_FORCE_INLINE const T Clamp(const T v, const T minVal, const T maxVal)
{ return Max<T>(minVal, Min<T>(v, maxVal)); }

template <typename T> OVR_FORCE_INLINE int     Chop(T f)
{ return (int)f; }

template <typename T> OVR_FORCE_INLINE T       Lerp(T a, T b, T f) 
{ return (b - a) * f + a; }

// Smooth transition between 0..1 that has 0 first derivative at 0 and 1.
template<typename T> OVR_FORCE_INLINE const T SmoothStep(const T s)
{ return s*s*(s*T(-2) + T(3)); }

// Same as SmoothStep but with 0 first and second derivatives at 0 and 1
template<typename T> OVR_FORCE_INLINE const T SmootherStep(const T s)
{ return s*s*s*(s*(s * T(6) - T(15)) + T(10)); }

// These functions stand to fix a stupid VC++ warning (with /Wp64 on):
// "warning C4267: 'argument' : conversion from 'size_t' to 'const unsigned', possible loss of data"
// Use these functions instead of gmin/gmax if the argument has size
// of the pointer to avoid the warning. Though, functionally they are
// absolutelly the same as regular gmin/gmax.
template <typename T>   OVR_FORCE_INLINE const T PMin(const T a, const T b)
{
    OVR_COMPILER_ASSERT(sizeof(T) == sizeof(size_t));
    return (a < b) ? a : b;
}
template <typename T>   OVR_FORCE_INLINE const T PMax(const T a, const T b)
{
    OVR_COMPILER_ASSERT(sizeof(T) == sizeof(size_t));
    return (b < a) ? a : b;
}


template <typename T>   OVR_FORCE_INLINE const T Abs(const T v)
{ return (v>=0) ? v : -v; }


}} // OVR::Alg

#endif
