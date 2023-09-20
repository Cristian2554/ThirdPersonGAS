// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonGASPlayerState.h"
#include "Components/TBAbilitySystemComponent.h"

AThirdPersonGASPlayerState::AThirdPersonGASPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UTBAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	// Mixed mode means we only are replicated the GEs to ourself, not the GEs to simulated proxies. If another GDPlayerState (Hero) receives a GE,
	// we won't be told about it by the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Set PlayerState's NetUpdateFrequency to the same as the Character.
	// Default is very low for PlayerStates and introduces perceived lag in the ability system.
	// 100 is probably way too high for a shipping game, you can adjust to fit your needs.
	NetUpdateFrequency = 100.0f;
}

UAbilitySystemComponent* AThirdPersonGASPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
