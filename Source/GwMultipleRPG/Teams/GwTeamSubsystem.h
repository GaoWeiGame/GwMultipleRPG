// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GwTeamSubsystem.generated.h"


class AGwPlayerState;
class AGwTeamInfoBase;
class UGwTeamDisplayAsset;
class AGwTeamPrivateInfo;
class AGwTeamPublicInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGwTeamDisplayAssetChangedDelegate, const UGwTeamDisplayAsset*, DisplayAsset);

USTRUCT()
struct FGwTeamTrackingInfo
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<AGwTeamPublicInfo> PublicInfo = nullptr;

	UPROPERTY()
	TObjectPtr<AGwTeamPrivateInfo> PrivateInfo = nullptr;

	UPROPERTY()
	TObjectPtr<UGwTeamDisplayAsset> DisplayAsset = nullptr;

	UPROPERTY()
	FOnGwTeamDisplayAssetChangedDelegate OnTeamDisplayAssetChanged;

public:
	void SetTeamInfo(AGwTeamInfoBase* Info);
	void RemoveTeamInfo(AGwTeamInfoBase* Info);
};

UENUM(BlueprintType)
enum class EGwTeamComparison : uint8
{
	OnSameTeam,

	DifferentTeams,

	InvalidArgument
};


UCLASS()
class GWMULTIPLERPG_API UGwTeamSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	UGwTeamSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	int32 FindTeamFromObject(const UObject* TestObject) const;
	
	bool RegisterTeamInfo(AGwTeamInfoBase* TeamInfo);

	bool UnregisterTeamInfo(AGwTeamInfoBase* TeamInfo);
	
	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category=Teams, meta=(ExpandEnumAsExecs=ReturnValue))
	EGwTeamComparison CompareTeams(const UObject* A, const UObject* B, int32& TeamIdA, int32& TeamIdB) const;
	
	EGwTeamComparison CompareTeams(const UObject* A, const UObject* B) const;

	const AGwPlayerState* FindPlayerStateFromActor(const AActor* PossibleTeamActor) const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Teams)
	void AddTeamTagStack(int32 TeamId, FGameplayTag Tag, int32 StackCount);

	UFUNCTION(BlueprintCallable, Category=Teams)
	int32 GetTeamTagStackCount(int32 TeamId, FGameplayTag Tag) const;

	void NotifyTeamDisplayAssetModified(UGwTeamDisplayAsset* ModifiedAsset);

	FOnGwTeamDisplayAssetChangedDelegate& GetTeamDisplayAssetChangedDelegate(int32 TeamId);

	UFUNCTION(BlueprintCallable, Category=Teams)
	UGwTeamDisplayAsset* GetTeamDisplayAsset(int32 TeamId, int32 ViewerTeamId);
	
private:
	UPROPERTY()
	TMap<int32, FGwTeamTrackingInfo> TeamMap;

	FDelegateHandle CheatManagerRegistrationHandle;
};
