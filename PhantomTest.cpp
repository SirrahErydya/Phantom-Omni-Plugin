/*
* Copyright (C) 2016 Fenja Kollasch
*
* This software may be distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "MyProject.h"
#include "TestPawn.h"


// Sets default values
ATestPawn::ATestPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	isColliding = false;
}

// Called when the game starts or when spawned
void ATestPawn::BeginPlay()
{
	Super::BeginPlay();
	if (IPhantomControllerPlugin::IsAvailable()) {
		FPhantomControllerInputDevice& device = *IPhantomControllerPlugin::Get().PhantomInputDevice.Get();

		startRotation = GetActorRotation();
		startPhantom = device.getRotation();
		device.setWorkspace(GetActorLocation(), FVector(1, 1, 1));

	}
	
}

// Called every frame
void ATestPawn::Tick( float DeltaTime )
{
	Super::Tick(DeltaTime);
	if (IPhantomControllerPlugin::IsAvailable()) {
		FPhantomControllerInputDevice& device = *IPhantomControllerPlugin::Get().PhantomInputDevice.Get();

		// Scale the retrieved position a little bit up, so you could even see the difference ;)
		FVector myPosition = device.getPosition();
		SetActorLocation(myPosition);

		FRotator rotator = FRotator(FQuat(device.getRotation()));
		FRotator start = FRotator(FQuat(startPhantom));

		//Relativate Position
		float angleX = rotator.Pitch - start.Pitch;
		float angleY = rotator.Yaw - start.Yaw;
		float angleZ = rotator.Roll - start.Roll;

		// Commented out since the rotation was disturbing during usage
		//UE_LOG(LogTemp, Warning, TEXT("Rot X: %f, Rot Y: %f, Rot Z: %f"), rotator.Pitch, rotator.Roll, rotator.Yaw);
		//UE_LOG(LogTemp, Warning, TEXT("Angle X: %f, Angle Y: %f, Angle Z: %f"), angleX, angleY, angleZ);
		//SetActorRotation(FRotator(angleX + startRotation.Pitch, angleY + startRotation.Yaw, angleZ + startRotation.Roll));


		buttonDown = device.isButtonDown() == 1;

		device.isColliding = isColliding;

		//Set up force Feedback
		// Outcommented since the force feedback is handled over BluePrints
		/*UE_LOG(LogTemp, Warning, TEXT("Force Setup"));
		if (collPos.X < collObject->RelativeLocation.X) {
			forceFeedback.X = - FMath::Abs(forceFeedback.X);
			UE_LOG(LogTemp, Warning, TEXT("Correcting X"));
		}
		else {
			forceFeedback.X = FMath::Abs(forceFeedback.X);
		}

		if (collPos.Y < collObject->RelativeLocation.Y) {
			forceFeedback.Y = -FMath::Abs(forceFeedback.Y);
			UE_LOG(LogTemp, Warning, TEXT("Correcting Y"));
		}
		else {
			forceFeedback.Y = FMath::Abs(forceFeedback.Y);
		}

		if (collPos.Z < collObject->RelativeLocation.Z) {
			forceFeedback.Z = -FMath::Abs(forceFeedback.Z);
			UE_LOG(LogTemp, Warning, TEXT("Correcting Z"));
		}
		else {
			forceFeedback.Z = FMath::Abs(forceFeedback.Z);
		}*/

		if (device.isButtonDown() && isColliding) {
			device.setForce(forceFeedback);
		}

	}
	else {
		SetActorLocation(GetActorLocation() + FVector(1, 0, 0));
	}


}

// Called to bind functionality to input
void ATestPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}
