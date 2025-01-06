// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "GwPlayerSpawningManagerComponent.generated.h"


class APlayerStart;
class AGwPlayerStart;

UCLASS()
class GWMULTIPLERPG_API UGwPlayerSpawningManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	UGwPlayerSpawningManagerComponent(const FObjectInitializer& ObjectInitializer);

	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void OnFinishRestartPlayer(AController* Player, const FRotator& StartRotation) { }

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName=OnFinishRestartPlayer))
	void K2_OnFinishRestartPlayer(AController* Player, const FRotator& StartRotation);

	virtual AActor* OnChoosePlayerStart(AController* Player, TArray<AGwPlayerStart*>& PlayerStarts) { return nullptr; }
	APlayerStart* GetFirstRandomUnoccupiedPlayerStart(AController* Controller, const TArray<AGwPlayerStart*>& FoundStartPoints) const;

private:
	friend class AGwGameMode;

	void OnLevelAdded(ULevel* InLevel, UWorld* InWorld);
	void HandleOnActorSpawned(AActor* SpawnedActor);
	AActor* ChoosePlayerStart(AController* Player);
	bool ControllerCanRestart(AController* Player);
	void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation);

#if WITH_EDITOR
	APlayerStart* FindPlayFromHereStart(AController* Player);
#endif
	
	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<AGwPlayerStart>> CachedPlayerStarts;
};
