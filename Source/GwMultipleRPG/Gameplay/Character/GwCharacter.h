// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "AttributeSet.h"
#include "Abilities/GameplayAbility.h"
#include "GwMultipleRPG/AbilitySystem/GwAbilitySystemComponent.h"
#include "GwMultipleRPG/Teams/GwTeamAgentInterface.h"
#include "GwCharacter.generated.h"

class UGwHealthComponent;
class UGwEquipmentManagerComponent;
class UGwPawnExtensionComponent;
class UGwHeroComponent;

UCLASS()
class GWMULTIPLERPG_API AGwCharacter : public ACharacter, public IAbilitySystemInterface, public IGwTeamAgentInterface
{
	GENERATED_BODY()

public:

	AGwCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintPure, Category = "Gw|Pawn")
	UGwAbilitySystemComponent* GetGwAbilitySystemComponent() const ;

	void ToggleCrouch();
	
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnGwTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;

	virtual void NotifyControllerChanged() override;

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	virtual void OnRep_PlayerState() override;
	virtual void OnRep_Controller() override;

	virtual void OnAbilitySystemInitialized();
	virtual void OnAbilitySystemUninitialized();
	
	void InitializeGameplayTags();

	void SetMovementModeTag(EMovementMode MovementMode, uint8 CustomMovementMode, bool bTagEnabled);

	UPROPERTY()
	TObjectPtr<UGwAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	virtual void InitAbilityActorInfo();

	UFUNCTION()
	virtual void OnDeathStarted(AActor* OwningActor);

	UFUNCTION()
	virtual void OnDeathFinished(AActor* OwningActor);

	void DisableMovementAndCollision();
	void DestroyDueToDeath();
	void UninitAndDestroy();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnDeathFinished"))
	void K2_OnDeathFinished();

	virtual FGenericTeamId DetermineNewTeamAfterPossessionEnds(FGenericTeamId OldTeamID) const
	{
		return FGenericTeamId::NoTeam;
	}
	
private:
	UPROPERTY(VisibleAnywhere, Category=Camera)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category=Camera)
	class UCameraComponent* ThirdPersonCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gw|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGwHeroComponent> HeroComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gw|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGwPawnExtensionComponent> PawnExtComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gw|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGwEquipmentManagerComponent> EquipmentManagerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gw|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGwHealthComponent> HealthComponent;

	UFUNCTION()
	void OnRep_MyTeamID(FGenericTeamId OldTeamID);
	
	UPROPERTY(ReplicatedUsing = OnRep_MyTeamID)
	FGenericTeamId MyTeamID;

	UPROPERTY()
	FOnGwTeamIndexChangedDelegate OnTeamChangedDelegate;

	UFUNCTION()
	void OnControllerChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);
};
