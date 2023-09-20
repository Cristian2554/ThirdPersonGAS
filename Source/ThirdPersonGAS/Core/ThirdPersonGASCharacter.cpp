// Copyright Epic Games, Inc. All Rights Reserved.

#include "ThirdPersonGASCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ThirdPersonGASPlayerState.h"
#include "Components/TBAbilityInputBindingComponent.h"
#include "Components/TBAbilitySystemComponent.h"


//////////////////////////////////////////////////////////////////////////
// AThirdPersonGASCharacter

AThirdPersonGASCharacter::AThirdPersonGASCharacter()
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
	if(CameraBoom)
	{
		CameraBoom->SetupAttachment(RootComponent);
		CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
		CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	}

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	if (FollowCamera)
	{
		FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
		FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	}

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	bAlwaysRelevant = true;
	
	AbilityInputBindingComponent = CreateDefaultSubobject<UTBAbilityInputBindingComponent>(TEXT("Ability Input Binding Component"));
}

void AThirdPersonGASCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* playerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AThirdPersonGASCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (AThirdPersonGASPlayerState* playerState = GetPlayerState<AThirdPersonGASPlayerState>())
	{
		AbilitySystemComponent = Cast<UTBAbilitySystemComponent>(playerState->GetAbilitySystemComponent());
		playerState->GetAbilitySystemComponent()->InitAbilityActorInfo(playerState, this);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AThirdPersonGASCharacter::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* enhancedInputComponent = CastChecked<UEnhancedInputComponent>(playerInputComponent)) {
		//Moving
		enhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AThirdPersonGASCharacter::Move);

		//Looking
		enhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AThirdPersonGASCharacter::Look);

	}

}

void AThirdPersonGASCharacter::PossessedBy(AController* newController)
{
	Super::PossessedBy(newController);

	if (AThirdPersonGASPlayerState* playerState = GetPlayerState<AThirdPersonGASPlayerState>())
	{
		AbilitySystemComponent = Cast<UTBAbilitySystemComponent>(playerState->GetAbilitySystemComponent());
		playerState->GetAbilitySystemComponent()->InitAbilityActorInfo(playerState, this);
	}

	SetOwner(newController);
}

UAbilitySystemComponent* AThirdPersonGASCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void AThirdPersonGASCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D movementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation(0, rotation.Yaw, 0);

		// get forward vector
		const FVector forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector rightDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

		// add movement  
		AddMovementInput(forwardDirection, movementVector.Y);
		AddMovementInput(rightDirection, movementVector.X);
	}
}

void AThirdPersonGASCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D lookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(lookAxisVector.X);
		AddControllerPitchInput(lookAxisVector.Y);
	}
}




