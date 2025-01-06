// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GwCharacterMovementComponent.generated.h"


UCLASS(Config = Game)
class GWMULTIPLERPG_API UGwCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UGwCharacterMovementComponent();


};
