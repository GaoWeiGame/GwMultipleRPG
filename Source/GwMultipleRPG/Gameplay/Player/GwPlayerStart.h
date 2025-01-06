// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerStart.h"
#include "GwPlayerStart.generated.h"

enum class EGwPlayerStartLocationOccupancy
{
	Empty,
	Partial,
	Full
};

UCLASS()
class GWMULTIPLERPG_API AGwPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	AGwPlayerStart(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	const FGameplayTagContainer& GetGameplayTags() { return StartPointTags; }

	EGwPlayerStartLocationOccupancy GetLocationOccupancy(AController* const ControllerPawnToFit) const;
	bool TryClaim(AController* OccupyingController);
	bool IsClaimed() const;

protected:
	void CheckUnclaimed();

	UPROPERTY(Transient)
	TObjectPtr<AController> ClaimingController = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Player Start Claiming")
	float ExpirationCheckInterval = 1.f;

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer StartPointTags;

	FTimerHandle ExpirationTimerHandle;
};