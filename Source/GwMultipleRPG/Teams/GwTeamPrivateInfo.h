// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GwTeamInfoBase.h"
#include "GwTeamPrivateInfo.generated.h"

UCLASS()
class GWMULTIPLERPG_API AGwTeamPrivateInfo : public AGwTeamInfoBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGwTeamPrivateInfo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
