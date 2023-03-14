// // Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.


#include "TelekineticActor.h"

// Sets default values
ATelekineticActor::ATelekineticActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TelekineticState = ETelekineticState::Undefined;
}

// Called when the game starts or when spawned
void ATelekineticActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATelekineticActor::LiftOff()
{
	FVector start = GetRootComponent()->GetComponentLocation();
	FVector end = start + (start.Z * 10.f);


}

void ATelekineticActor::Pushed()
{
}

// Called every frame
void ATelekineticActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

