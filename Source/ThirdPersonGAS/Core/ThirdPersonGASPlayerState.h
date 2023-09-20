// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "ThirdPersonGASPlayerState.generated.h"

class UTBAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class THIRDPERSONGAS_API AThirdPersonGASPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=GAS, meta = (AllowPrivateAccess = "true"))
	UTBAbilitySystemComponent* AbilitySystemComponent;

public:
	AThirdPersonGASPlayerState();

	// IAbilitySystemComponent Interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
};