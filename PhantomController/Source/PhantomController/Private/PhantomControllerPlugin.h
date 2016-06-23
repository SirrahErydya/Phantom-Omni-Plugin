#pragma once
#include "PhantomControllerPrivatePCH.h"
#include "PhantomControllerInputDevice.h"

class FPhantomControllerPlugin : public IPhantomControllerPlugin
{
public:
	/** Implements the rest of the IInputDeviceModule interface **/

	/** Creates a new instance of the IInputDevice associated with this IInputDeviceModule **/
	virtual TSharedPtr<class IInputDevice> CreateInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler);

	/** Called right after the module DLL has been loaded and the module object has been created **/
	virtual void StartupModule() override;

	/** Called before the module is unloaded, right before the module object is destroyed. **/
	virtual void ShutdownModule() override;


};

