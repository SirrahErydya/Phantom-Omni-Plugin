/* Copyright (C) 2016 Fenja Kollasch
*
* This software may be distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#pragma once

#include "GameFramework/Pawn.h"
#include "PhantomController/Private/PhantomControllerPlugin.h"
#include "PhantomController/Private/PhantomControllerInputDevice.h"
#include "TestPawn.generated.h"

/**
 * An Unreal actor to implement a phantom omni controlled Pawn
 * You can attach a mesh to this Actor and see how it is moved and rotated by your phantoms
 */
UCLASS()
class MYPROJECT_API ATestPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATestPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// start rotation
	FRotator startRotation;

	// Start position of the device
	FMatrix startPhantom;

	// True, if the object is colliding with another one
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Phantom Collision")
	bool isColliding;

	// True, if a button is pressed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phantom Collision")
	bool buttonDown;

	// Saves the collision object
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phantom Collision")
	UPrimitiveComponent* collObject;

	// Saves the position where the collision takes place
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phantom Collision")
	FVector collPos;

	// Saves the applied force feedback
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phantom Collision")
	FVector forceFeedback;

	
	
};
