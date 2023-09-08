// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ThirdPersonGASController.generated.h"

UCLASS()
class THIRDPERSONGAS_API AThirdPersonGASController : public APlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AThirdPersonGASController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void AcknowledgePossession(APawn* pawn) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
