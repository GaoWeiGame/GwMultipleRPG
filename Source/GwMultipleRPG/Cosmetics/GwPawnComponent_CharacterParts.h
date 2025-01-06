// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/PawnComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "GwPawnComponent_CharacterParts.generated.h"

class UGwPawnComponent_CharacterParts;
struct FGwCharacterPartList;

USTRUCT()
struct FGwAppliedCharacterPartEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()
private:
	friend FGwCharacterPartList;
	friend UGwPawnComponent_CharacterParts;
	UPROPERTY(NotReplicated)
	TObjectPtr<UChildActorComponent> SpawnedComponent = nullptr;
};

USTRUCT(BlueprintType)
struct FGwCharacterPartList : public FFastArraySerializer
{
	GENERATED_BODY()

	FGameplayTagContainer CollectCombinedTags() const;

private:
	UPROPERTY()
	TArray<FGwAppliedCharacterPartEntry> Entries;
};

UCLASS(meta=(BlueprintSpawnableComponent))
class GWMULTIPLERPG_API UGwPawnComponent_CharacterParts : public UPawnComponent
{
	GENERATED_BODY()

public:
	UGwPawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintPure=false, BlueprintCosmetic, Category=Cosmetics)
	FGameplayTagContainer GetCombinedTags(FGameplayTag RequiredPrefix) const;

private:
	UPROPERTY(Replicated, Transient)
	FGwCharacterPartList CharacterPartList;
};
