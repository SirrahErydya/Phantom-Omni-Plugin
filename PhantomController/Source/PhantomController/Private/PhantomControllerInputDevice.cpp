
#include "PhantomControllerPrivatePCH.h"
#include "PhantomControllerInputDevice.h"
#include "IInputInterface.h"


FPhantomControllerInputDevice::FPhantomControllerInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler) :
	MessageHandler(InMessageHandler)
{
	// Initiate your device here
	init();
	
}

FPhantomControllerInputDevice::~FPhantomControllerInputDevice()
{
	// Close your device here
	UE_LOG(LogTemp, Warning, TEXT("Stopping Scheduler"));
	hdUnschedule(m_UpdateCallback);
	hdStopScheduler();
	hdDisableDevice(hdGetCurrentDevice());
}


void FPhantomControllerInputDevice::Tick(float DeltaTime)
{
	// Nothing necessary to do (boilerplate code to complete the interface)
}

void FPhantomControllerInputDevice::SendControllerEvents()
{
	// Poll your device here and fire off events related to its current state
	// FPhantomControllerInputDevice::retrievePosition();
	

	if (FPhantomControllerInputDevice::isButtonDown() == 1) {
		UE_LOG(LogTemp, Warning, TEXT("Device is here!"));
	}

}


void FPhantomControllerInputDevice::SetMessageHandler(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler)
{
	MessageHandler = InMessageHandler;
}


bool FPhantomControllerInputDevice::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	// Nothing necessary to do (boilerplate code to complete the interface)
	return false;
}


void FPhantomControllerInputDevice::SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value)
{
	// Nothing necessary to do (boilerplate code to complete the interface)
}


void FPhantomControllerInputDevice::SetChannelValues(int32 ControllerId, const FForceFeedbackValues &values)
{
	// Nothing necessary to do (boilerplate code to complete the interface)
}


/* Phantom Omni specific Methods*/


bool FPhantomControllerInputDevice::init()
{
	m_strDeviceName = HD_DEFAULT_DEVICE;

	HDErrorInfo error;
	m_deviceHandle = hdInitDevice(m_strDeviceName);
	if (HD_DEVICE_ERROR(error = hdGetError()))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to initialize device."));
		getchar();
		return false;
	}

	if (HD_SUCCESS != hdGetError().errorCode)
	{
		UE_LOG(LogTemp, Error, TEXT("Error initializing haptic device."));
		getchar();
		return false;
	}


	hdEnable(HD_FORCE_OUTPUT);

	
	hdScheduleAsynchronous(UpdatePosition, this, HD_MAX_SCHEDULER_PRIORITY);

	/* Start the scheduler to get the servo loop going. */	
	hdStartScheduler();
	UE_LOG(LogTemp, Warning, TEXT("Starting Scheduler"));
	if (HD_DEVICE_ERROR(error = hdGetError()))
	{
		UE_LOG(LogTemp, Error, TEXT("Error while creating Scheduler"));
		getchar();
		exit(-1);
	}

	

	//hdStopScheduler();
	//hdUnschedule(m_UpdateCallback);

	return true;
}

void FPhantomControllerInputDevice::mult(double src[3], double mat[16], double dst[3]) 
{
	dst[0] = mat[0] * src[0]
		+ mat[4] * src[1]
		+ mat[8] * src[2]
		+ mat[12];

	dst[1] = mat[1] * src[0]
		+ mat[5] * src[1]
		+ mat[9] * src[2]
		+ mat[13];
	dst[2] = mat[2] * src[0]
		+ mat[6] * src[1]
		+ mat[10] * src[2]
		+ mat[14];

}


// Creates your working environment
// If you want to transform an Actor with the Phantoms, give the Actor Location as origin and modify the position depending of the rotation
void FPhantomControllerInputDevice::setWorkspace(FVector vOrigin, FVector vPositionModifier)
{
	double appWorkspaceDim[6] = { vOrigin[0] - 5, vOrigin[1] - 5, vOrigin[2] - 2, vOrigin[0] + 5, vOrigin[1] + 5, vOrigin[2] + 5 };
	hdMakeCurrentDevice(m_deviceHandle);

	hdGetDoublev(HD_MAX_WORKSPACE_DIMENSIONS, m_workspace);

	HDErrorInfo error;
	if (HD_DEVICE_ERROR(error = hdGetError()))
	{
		UE_LOG(LogTemp, Error, TEXT("Error during set workspace callback"));
	}

	// center of the workspaces in real space
	m_center[0] = (m_workspace[3] - m_workspace[0]) * 0.5;
	m_center[1] = (m_workspace[4] - m_workspace[1]) * 0.5;
	m_center[2] = (m_workspace[5] - m_workspace[2]) * 0.5;

	UE_LOG(LogTemp, Warning, TEXT("Dimension Phantom Workspace: ( %f, %f, %f )\n"), (m_workspace[3] - m_workspace[0]), (m_workspace[4] - m_workspace[1]), (m_workspace[5] - m_workspace[2]));
	UE_LOG(LogTemp, Warning, TEXT("Center Phantom Workspace: ( %f, %f, %f )\n"), m_center[0], m_center[1], m_center[2]);

	
	m_transformMat[0] = (10.0) / (m_workspace[3] - m_workspace[0]);
	m_transformMat[1] = 0;
	m_transformMat[2] = 0;
	m_transformMat[3] = 0;

	m_transformMat[4] = 0;
	m_transformMat[5] = (10.0) / (m_workspace[4] - m_workspace[1]);
	m_transformMat[6] = 0;
	m_transformMat[7] = 0;

	m_transformMat[8] = 0;
	m_transformMat[9] = 0;
	m_transformMat[10] = (10.0) / (m_workspace[4] - m_workspace[1]);
	m_transformMat[11] = 0;

	m_transformMat[12] = vOrigin[0];
	m_transformMat[13] = vOrigin[1];
	m_transformMat[14] = vOrigin[2];
	m_transformMat[15] = 1.0; 


	printf("WorkspaceDims: ( %f, %f, %f, %f, %f, %f )\n", appWorkspaceDim[0], appWorkspaceDim[1], appWorkspaceDim[2], appWorkspaceDim[3], appWorkspaceDim[4], appWorkspaceDim[5]);

	printf("%s FMatrix\n", m_strDeviceName);

	printf("( %f, %f, %f, %f,\n", m_transformMat[0], m_transformMat[1], m_transformMat[2], m_transformMat[3]);
	printf("%f, %f, %f, %f,\n", m_transformMat[4], m_transformMat[5], m_transformMat[6], m_transformMat[7]);
	printf("%f, %f, %f, %f,\n", m_transformMat[8], m_transformMat[9], m_transformMat[10], m_transformMat[11]);
	printf("%f, %f, %f, %f );\n", m_transformMat[12], m_transformMat[13], m_transformMat[14], m_transformMat[15]); 

	m_vPositionModifier = vPositionModifier;

	if (vOrigin[0] < 0.0)
		m_rotation = FQuat(FVector(0, 0, 1), 3.14159 / 2.0);
	else
		m_rotation = FQuat(FVector(0, 0, 1), -3.14159 / 2.0);
	m_orientation = FRotationMatrix(m_rotation.Rotator()); 
}

FVector FPhantomControllerInputDevice::getPosition(void)
{
	return m_position;
}

//Retrieves the position of the device and transforms it to FVector
void FPhantomControllerInputDevice::retrievePosition(void)
{
	hdMakeCurrentDevice(m_deviceHandle);
	hdBeginFrame(m_deviceHandle);

	if (hdIsEnabled(HD_FORCE_OUTPUT))
	{
		double positionServo[3];
		positionServo[0] = 0;
		positionServo[1] = 0;
		positionServo[2] = 0;

		hdMakeCurrentDevice(m_deviceHandle);
		hdGetDoublev(HD_CURRENT_POSITION, positionServo);

		HDErrorInfo error;
		if (HD_DEVICE_ERROR(error = hdGetError()))
		{
			UE_LOG(LogTemp, Error, TEXT("Error during scheduler callback"));
		}

		// Phantoms and Unreal got different coordinate Systems, so we need to translate here
		// Careful! This is especiallyfor the ivor operating needle and its rotation
		// Actually the posServo should be (x, z, y)!
		m_positionServo[0] = positionServo[2];
		m_positionServo[1] = - positionServo[0];
		m_positionServo[2] = positionServo[1];

		m_positionServo[0] *= m_vPositionModifier[0];
		m_positionServo[1] *= m_vPositionModifier[1];
		m_positionServo[2] *= m_vPositionModifier[2];

		double positionPhantom[3];
		m_positionServo[0] -= m_center[0];
		m_positionServo[1] -= m_center[1];
		m_positionServo[2] -= m_center[2];

		mult(m_positionServo, m_transformMat, positionPhantom);

		m_positionServo[0] = positionPhantom[0];
		m_positionServo[1] = positionPhantom[1];
		m_positionServo[2] = positionPhantom[2];

		//UE_LOG(LogTemp, Warning, TEXT("Pos Matrix: %f %f %f"), m_positionServo[0], m_positionServo[1], m_positionServo[2]);
		//Transformation to Unreal coordinates
		m_position = FVector(m_positionServo[0], m_positionServo[1], m_positionServo[2]);
	}

	hdEndFrame(m_deviceHandle);

}

// Update Callback Function
HDCallbackCode HDCALLBACK UpdatePosition(void *pUserData) {

	FPhantomControllerInputDevice *device = static_cast<FPhantomControllerInputDevice *>(pUserData);
	device->retrievePosition();
	//device->transform();
	return HD_CALLBACK_CONTINUE;
}

FMatrix FPhantomControllerInputDevice::getRotation(void)
{
	
	double v[16];
	hdMakeCurrentDevice(m_deviceHandle);
	hdGetDoublev(HD_CURRENT_TRANSFORM, v);


	for (int i = 0; i<3; i++)
		for (int j = 0; j<3; j++)
			m_orientation.M[i][j] = v[j * 4 + i];


	m_orientation.TransposeAdjoint();

	return m_orientation;
}

void FPhantomControllerInputDevice::setForce(FVector force) {
	UE_LOG(LogTemp, Warning, TEXT("Applying force"));
	m_force = force;
	m_forceServo[0] = m_force.X;
	m_forceServo[1] = m_force.Y;
	m_forceServo[2] = m_force.Z;
	FPhantomControllerInputDevice::applyForce();
}

void FPhantomControllerInputDevice::applyForce(void)
{
	hdMakeCurrentDevice(m_deviceHandle);
	hdBeginFrame(m_deviceHandle);
	double dTemp[3] = { m_forceServo[0] * m_vPositionModifier[0], m_forceServo[1] * m_vPositionModifier[1], m_forceServo[2] * m_vPositionModifier[2] };
	hdSetDoublev(HD_CURRENT_FORCE, dTemp);
	//UE_LOG(LogTemp, Warning, TEXT("Current Force: %f %f %f"), dTemp[0], dTemp[1], dTemp[2]);
	hdEndFrame(m_deviceHandle);
}

// Interface function to get button state.  Only one button is used
// in this application.
int FPhantomControllerInputDevice::isButtonDown()
{
	int currentButtons;
	hdMakeCurrentDevice(m_deviceHandle);
	hdGetIntegerv(HD_CURRENT_BUTTONS, &currentButtons);

	if ((currentButtons & HD_DEVICE_BUTTON_1) != 0 ||
		(currentButtons & HD_DEVICE_BUTTON_2) != 0)
		return 1;
	else
		return 0;
}

