/************************************************************************************

Filename    :   OVR_System.cpp
Content     :   General kernel initialization/cleanup, including that
                of the memory allocator.
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

#include "OVR_System.h"

namespace OVR {

//-----------------------------------------------------------------------------
// System

// Initializes System core, installing allocator.
void System::Init()
{
}

void System::Destroy()
{
}

// Returns 'true' if system was properly initialized.
bool System::IsInitialized()
{
    return true;
}

// Dump any leaked memory
void System::CheckForAllocatorLeaks()
{
}

} // namespace OVR
