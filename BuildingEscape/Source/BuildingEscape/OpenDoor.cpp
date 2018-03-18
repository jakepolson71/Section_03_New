// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Array.h"
#include "Rotator.h"
#include "Engine/World.h"
#include "Components.h"
#include "GameFramework/Actor.h"

#define OUT


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	if (Owner == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing the Owner component"), *GetOwner()->GetName());
	}


	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing the Pressure Plate"), *GetOwner()->GetName());
	}

}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// Poll Trigger Volume

	if (GetTotalMassOfActorsOnPlate() > TriggerMass)
	{
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}

}



float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;
		
	// Find all the overlapping actors
	TArray<AActor*> OverlappingActors;

	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);


	// Iterate through them adding their masses


	for (const auto Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s is on Pressure Plate"), *Actor->GetName());
	}

	return TotalMass;
}
