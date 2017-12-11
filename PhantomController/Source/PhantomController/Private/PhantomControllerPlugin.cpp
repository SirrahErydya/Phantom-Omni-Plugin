/* Copyright (C) 2016 Fenja Kollasch
*
* This software may be distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "PhantomControllerPrivatePCH.h"
#include "PhantomControllerPlugin.h"


IMPLEMENT_MODULE(FPhantomControllerPlugin, PhantomController)


TSharedPtr<class IInputDevice> FPhantomControllerPlugin::CreateInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
{
	UE_LOG(LogTemp, Warning, TEXT("Created new input device!"));

	FPhantomControllerPlugin::PhantomInputDevice = MakeShareable(new FPhantomControllerInputDevice(InMessageHandler));
	return PhantomInputDevice;
}


void FPhantomControllerPlugin::StartupModule()
{
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
	// Custom module-specific init can go here.

	UE_LOG(LogTemp, Warning, TEXT("PhantomControllerPlugin initiated!"));

	// IMPORTANT: This line registers our input device module with the engine.
	//	      If we do not register the input device module with the engine,
	//	      the engine won't know about our existence. Which means 
	//	      CreateInputDevice never gets called, which means the engine
	//	      will never try to poll for events from our custom input device.
	IModularFeatures::Get().RegisterModularFeature(IInputDeviceModule::GetModularFeatureName(), this);
}


void FPhantomControllerPlugin::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UE_LOG(LogTemp, Warning, TEXT("PhantomControllerInputDevicePlugin shut down!"));

	// Unregister our input device module
	IModularFeatures::Get().UnregisterModularFeature(IInputDeviceModule::GetModularFeatureName(), this);
}
