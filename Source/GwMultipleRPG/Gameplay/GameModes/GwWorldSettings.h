// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "GwWorldSettings.generated.h"

class UGwExperienceDefinition;
/**
 * 
 */
UCLASS()
class GWMULTIPLERPG_API AGwWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
public:
	AGwWorldSettings(const FObjectInitializer& ObjectInitializer);

	FPrimaryAssetId GetDefaultGameplayExperience() const;

#if WITH_EDITOR
	virtual void CheckForErrors() override;
#endif

#if WITH_EDITORONLY_DATA
	// Is this level part of a front-end or other standalone experience?
	// When set, the net mode will be forced to Standalone when you hit Play in the editor
	UPROPERTY(EditDefaultsOnly, Category=PIE)
	bool ForceStandaloneNetMode = false;
#endif
	
protected:
	// The default experience to use when a server opens this map if it is not overridden by the user-facing experience
	UPROPERTY(EditDefaultsOnly, Category=GameMode)
	TSoftClassPtr<UGwExperienceDefinition> DefaultGameplayExperience;
};
