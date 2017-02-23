// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	///look for attached physics handle
	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (physicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName());
	}

	///look for the input guy
	inputGuy = GetOwner()->FindComponentByClass<UInputComponent>();
	if (inputGuy) {
		///input guy is found
		///bind input axis
		inputGuy->BindAction("Grab", IE_Pressed, this, &UGrabber::grab);
		inputGuy->BindAction("Grab", IE_Released, this, &UGrabber::release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *GetOwner()->GetName());
	}
	
}

void UGrabber::grab() {
	///try to reach any actors with physics body collision channel set
	FHitResult hitResult = getFirstPhysicsBody();
	auto componentToGrab = hitResult.GetComponent();
	auto actorHit = hitResult.GetActor();

	if (!physicsHandle) { return; }

	///if we hit something, then attach a physics handle
	if (actorHit) {
		physicsHandle->GrabComponent(componentToGrab, NAME_None, componentToGrab->GetOwner()->GetActorLocation(), true);
	}
}

void UGrabber::release() {

	if (!physicsHandle) { return; }

	///release the physics handle
	physicsHandle->ReleaseComponent();

}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	/// get player view point
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerViewPointLocation, playerViewPointRotation);

	FVector lineTraceDirection = playerViewPointRotation.Vector() * reach;
	FVector lineTraceEnd = playerViewPointLocation + lineTraceDirection;

	if (!physicsHandle) { return; }

	///if physics handle is attached, move the object we're holding
	if (physicsHandle->GrabbedComponent) {
		physicsHandle->SetTargetLocation(lineTraceEnd);
	}

}

const FHitResult UGrabber::getFirstPhysicsBody() {

	/// get player view point
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerViewPointLocation, playerViewPointRotation);

	FVector lineTraceDirection = playerViewPointRotation.Vector() * reach;
	FVector lineTraceEnd = playerViewPointLocation + lineTraceDirection;

	/// set up query params
	FCollisionQueryParams traceParameters(FName(TEXT("")), false, GetOwner());

	/// ray-cast out to reach distance
	FHitResult hit;
	GetWorld()->LineTraceSingleByObjectType(hit, playerViewPointLocation, lineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), traceParameters);

	return hit;

}

