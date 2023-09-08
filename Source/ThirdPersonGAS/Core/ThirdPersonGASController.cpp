// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonGASController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

// Sets default values
AThirdPersonGASController::AThirdPersonGASController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AThirdPersonGASController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AThirdPersonGASController::AcknowledgePossession(APawn* pawn)
{
	Super::AcknowledgePossession(pawn);

	if (const IAbilitySystemInterface* pawnAbilityInterface = Cast<IAbilitySystemInterface>(pawn))
	{
		pawnAbilityInterface->GetAbilitySystemComponent()->InitAbilityActorInfo(pawn, pawn);
	}
}

// Called every frame
void AThirdPersonGASController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

