// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UPROPERTY(BlueprintAssignable)
	FDoorEvent onOpen;

	UPROPERTY(BlueprintAssignable)
	FDoorEvent onClose;

private:
	UPROPERTY(EditAnywhere)
	float triggerMass = 30.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume *pressurePlate = nullptr;

	AActor *doorOwner = nullptr;

	float getTotalMassOfActorsOnPlate();
	
};
