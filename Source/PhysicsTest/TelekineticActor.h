// // Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TelekineticActor.generated.h"

UENUM(BlueprintType)
enum class ETelekineticState : uint8
{
	Undefined = 0,
	Hovering = 1,
	Falling = 2,
	Pushed = 4 
};

UCLASS()
class PHYSICSTEST_API ATelekineticActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		ETelekineticState TelekineticState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Params, meta = (AllowPrivateAccess = "true"))
		float liftHeight = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Params, meta = (AllowPrivateAccess = "true"))
		float hoveringNoise = 100.f;

public:	
	// Sets default values for this actor's properties
	ATelekineticActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void LiftOff();
	void Pushed();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
