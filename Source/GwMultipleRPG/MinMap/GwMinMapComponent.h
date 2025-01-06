// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "W_GWMinMap.h"
#include "Components/GameStateComponent.h"
#include "GwMinMapComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGwUpdateMinMapPlayer);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGwRemoveMinMapPlayer, APawn*, RemovePawn);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GWMULTIPLERPG_API UGwMinMapComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	UGwMinMapComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintAuthorityOnly)
	void PlayerLogin(APawn* Pawn);


	UFUNCTION(BlueprintPure)
	TArray<APawn*> GetAllPawn();

	UFUNCTION(BlueprintCallable)
	void RemovePawn(APawn* Pawn);

	UFUNCTION()
	void OnRep_AllPawn(const TArray<APawn*>& OldData);

	UPROPERTY(BlueprintAssignable)
	FGwUpdateMinMapPlayer OnGwUpdateMinMapPlayer;
	
	UPROPERTY(BlueprintAssignable)
	FGwRemoveMinMapPlayer OnGwRemoveMinMapPlayer;
	
	//通过actor的世界位置，计算出在地图上的相对位置
	UFUNCTION(BlueprintCallable)
	FVector2D UpdateActorPosOnMap(AActor* InActor);

	FVector2D LocationToMapPos(FVector InLocation, FVector2D InWidgetSize);

	FVector2D ConvertLocationToMapLocation(FVector InLocation);

	UFUNCTION(BlueprintCallable)
	void SetGWMinMap(UW_GWMinMap* MinMap);

	UFUNCTION(BlueprintCallable)
	void UpdateAllPlayerPos();

private:
	float GetMapWidth() const;
	float GetMapHeight() const;

	UPROPERTY()
	UW_GWMinMap* GwMinMap;

	UPROPERTY(ReplicatedUsing=OnRep_AllPawn)
	TArray<APawn*> AllPawns;

	FVector2D LowerRightPos;
	FVector2D UpperLeftPos;
};
