// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "GwMultipleRPG/Input/GwInputConfig.h"

#include "GwHeroComponent.generated.h"


struct FInputMappingContextAndPriority;
class UInputMappingContext;
class UGwAbilitySet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInputMove);

UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class GWMULTIPLERPG_API UGwHeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	UGwHeroComponent(const FObjectInitializer& ObjectInitializer);

	virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);


	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual void CheckDefaultInitialization() override;

	
	bool IsReadyToBindInputs() const;

	void AddAdditionalInputConfig(const UGwInputConfig* InputConfig);

	void RemoveAdditionalInputConfig(const UGwInputConfig* InputConfig);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gw|Abilities")
	TArray<TObjectPtr<UGwAbilitySet>> AbilitySets;

	static const FName NAME_BindInputsNow;

	static const FName NAME_ActorFeatureName;

	UPROPERTY(BlueprintAssignable)
	FOnInputMove OnInputMove;
	
protected:
	UPROPERTY(EditAnywhere)
	TArray<FInputMappingContextAndPriority> DefaultInputMappings;

	bool bReadyToBindInputs;

	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);
	void Input_LookStick(const FInputActionValue& InputActionValue);
	void Input_Crouch(const FInputActionValue& InputActionValue);
	void Input_AutoRun(const FInputActionValue& InputActionValue);
};
