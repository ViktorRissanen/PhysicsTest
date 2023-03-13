// Copyright Epic Games, Inc. All Rights Reserved.

#include "PhysicsTestCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DrawDebugHelpers.h"


//////////////////////////////////////////////////////////////////////////
// APhysicsTestCharacter

APhysicsTestCharacter::APhysicsTestCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void APhysicsTestCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void APhysicsTestCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APhysicsTestCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APhysicsTestCharacter::Look);

		//Ability
		EnhancedInputComponent->BindAction(AbilityAction, ETriggerEvent::Triggered, this, &APhysicsTestCharacter::TelekinesisCircle);

	}

}

void APhysicsTestCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APhysicsTestCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APhysicsTestCharacter::TelekinesisCircle()
{
	FVector start = FollowCamera->GetSocketLocation(USpringArmComponent::SocketName);
	FVector forward = FollowCamera->GetForwardVector();

	// offset start position to avoid colliding with self
	start = FVector(start.X + (forward.X * 500.f), start.Y + (forward.Y * 500.f), start.Z + (forward.Z * 500.f));

	FVector end = start + (forward * 10000.f);
	FHitResult hit;

	if (!GetWorld())
		return;
	// raycast and debugdraw line
	bool actorHit = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Pawn, FCollisionQueryParams(), FCollisionResponseParams());
	DrawDebugLine(GetWorld(), start, end, FColor::Green, false, 0.f, 0.f, 5.f);

	//auto field = GetWorld()->PhysicsField.GetClass()->CreateAbstractDefaultSubobject<UField>(TEXT("PhysicsField"), true);
	
	if (!actorHit && !hit.GetActor())
		return;

	FCollisionShape sphere = FCollisionShape::MakeSphere(CircleRadius);
	TArray<FHitResult> hits;

	GetWorld()->SweepMultiByChannel(hits, );
	DrawDebugCircle(GetWorld(), hit.Location, 500.f, 40.f, FColor::Red, false, 0.f, 0, 3.f, FVector(1.f, 0.f, 0.f), FVector(0.f, 1.f, 0.f));

	// add onscreen debug message of actor hit
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, hit.GetActor()->GetFName().ToString());
}




