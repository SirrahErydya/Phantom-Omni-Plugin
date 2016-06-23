#pragma once

#include "IInputDevice.h"

struct DeviceState {
	FPhantomControllerInputDevice* device;
};

HDCallbackCode HDCALLBACK UpdatePosition(void *pUserData);

class FPhantomControllerInputDevice : public IInputDevice
{
public:
	FPhantomControllerInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler);
	~FPhantomControllerInputDevice();

	/** Tick the interface (e.g. check for new controllers) */
	virtual void Tick(float DeltaTime) override;

	/** Poll for controller state and send events if needed */
	virtual void SendControllerEvents() override;

	/** Set which MessageHandler will get the events from SendControllerEvents. */
	virtual void SetMessageHandler(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler) override;

	/** Exec handler to allow console commands to be passed through for debugging */
	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;

	/** IForceFeedbackSystem pass through functions **/
	virtual void SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value) override;
	virtual void SetChannelValues(int32 ControllerId, const FForceFeedbackValues &values) override;

	/** Functions for Phantoms*/
	bool init();

	virtual void setWorkspace(FVector vOrigin, FVector vPositionModifier = FVector(1, 1, 1));

	virtual FVector getPosition(void);

	//gets Position of haptic environment
	virtual void retrievePosition(void);
	

	virtual FMatrix getRotation(void);

	// Set force
	virtual void applyForce(void);

	// Get ready state of device.
	virtual int isButtonDown();

	// Set force
	virtual void setForce(FVector force);


private:
	/* Message handler */
	TSharedRef<FGenericApplicationMessageHandler> MessageHandler;

	//Matrix Multiplication
	void mult(double src[3], double mat[16], double dst[3]);

	/* Phantom AAtrributes and Constants*/
	// Variables used only by servo thread
	int   m_buttonServo;
	
	double m_positionServo[3]; // hier Position eintragen

	double m_forceServo[3];

	FVector m_position;

	FMatrix m_orientation;

	// Transformation from Device coordinates to Application coordinates
	double m_transformMat[16];
	
	FQuat m_rotation;
	
	FVector m_vPositionModifier;

	// Handle to device
	HHD m_deviceHandle;

	// Scheduler
	HDSchedulerHandle m_UpdateCallback;

	// Variables used only by application thread

	// Device workspace dimensions
	double m_workspaceDims[6];

	double m_workspace[6];
	double m_center[3];

	FVector m_force;

	const char* m_strDeviceName;
};

