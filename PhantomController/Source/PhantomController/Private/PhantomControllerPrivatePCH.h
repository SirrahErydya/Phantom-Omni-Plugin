/* Copyright (C) 2016 Fenja Kollasch
*
* This software may be distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

// You should place include statements to your module's private header files here.  You only need to
// add includes for headers that are used in most of your module's source files though.

//Unreal Stuff
#include "Core.h"
#include "CoreUObject.h"

//Freaking important: If you build for Win32 include these headers FIRST!
//For Win64 this is fine
#include <stdio.h>
#include <assert.h>

#if defined(WIN32)
# include <windows.h>
# include <conio.h>
#else
# include "conio.h"
# include <string.h>
#endif

#include "../../ThirdParty/PhantomSDK/include/HD/hd.h"
#include "../../ThirdParty/PhantomSDK/include/HL/hl.h"

//#include "../../ThirdParty/PhantomSDK/include/HDU/hdu.h"
//#include "../../ThirdParty/PhantomSDK/include/HDU/hduError.h"
//#include "../../ThirdParty/PhantomSDK/include/HDU/hduMath.h"
//#include "../../ThirdParty/PhantomSDK/include/HDU/hduMatrix.h"
//#include "../../ThirdParty/PhantomSDK/include/HDU/hduHapticDevice.h"



//Controller Interface
#include "../Public/IPhantomControllerPlugin.h"


 


