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

	openingActor = GetWorld()->GetFirstPlayerController()->GetPawn();
	doorOwner = GetOwner();
	
}

void UOpenDoor::openDoor() {

	doorOwner->SetActorRotation(FRotator(0.f, 170.f, 0.f));
}

void UOpenDoor::closeDoor() {

	doorOwner->SetActorRotation(FRotator(0.f, 90.f, 0.f));
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	//Poll the trigger volume every frame
	//if the opening actor is in the volume, open the door
	if (pressurePlate->IsOverlappingActor(openingActor)) {
		openDoor();
		lastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	if ((GetWorld()->GetTimeSeconds() - lastDoorOpenTime) > doorCloseDelay) {
		closeDoor();
	}

}

