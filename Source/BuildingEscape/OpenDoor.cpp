// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	doorOwner = GetOwner();

	if (!pressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("Could not find pressure plate"));
	}
	
}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	//Poll the trigger volume every frame
	//if the opening actor is in the volume, open the door
	if (getTotalMassOfActorsOnPlate() > triggerMass) {
		onOpen.Broadcast();
	}
	else {
		onClose.Broadcast();
	}

}

float UOpenDoor::getTotalMassOfActorsOnPlate() {

	float totalMass = 0.f;

	///find all the overlapping actors and add their weights
	TArray<AActor*> overlappingActors;

	if (!pressurePlate) { return totalMass; }
	pressurePlate->GetOverlappingActors(overlappingActors); //gives us the actors on the pressure plate

	///iterate and add the weights
	for (const auto* actor : overlappingActors) {
		totalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return totalMass;

}

