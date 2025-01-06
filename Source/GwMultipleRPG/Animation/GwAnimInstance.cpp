// Fill out your copyright notice in the Description page of Project Settings.


#include "GwAnimInstance.h"
#include "GwMultipleRPG/Gameplay/Character/GwCharacter.h"

UGwAnimInstance::UGwAnimInstance(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

void UGwAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* ASC)
{
	check(ASC);

	GameplayTagPropertyMap.Initialize(this, ASC);
}

float UGwAnimInstance::GetSpeed()
{
	Speed = 0.f;
	AGwCharacter* GwCharacter = Cast<AGwCharacter>(TryGetPawnOwner());
	if (GwCharacter)
	{
		Speed = GwCharacter->GetVelocity().Length();
	}
	return Speed;
}
