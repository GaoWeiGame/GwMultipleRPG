// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ModularGameState.h"
#include "GameFramework/GameState.h"
#include "GwGameState.generated.h"

struct FGwVerbMessage;
class UGwAbilitySystemComponent;
class UGwExperienceManagerComponent;
/**
 * 
 */
UCLASS(Config = Game)
class GWMULTIPLERPG_API AGwGameState : public AModularGameStateBase,public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AGwGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable, Category = "Gw|GameState")
	void MulticastMessageToClients(const FGwVerbMessage Message);
	
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Gw|GameState")
	void MulticastReliableMessageToClients(const FGwVerbMessage Message);

	
private:
	UPROPERTY()
	TObjectPtr<UGwExperienceManagerComponent> ExperienceManagerComponent;

	UPROPERTY(VisibleAnywhere, Category = "Gw|GameState")
	TObjectPtr<UGwAbilitySystemComponent> AbilitySystemComponent;
};
