// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GwTabListWidgetBase.h"
#include "GwMultipleRPG/UI/Foundation/GwButtonBase.h"
#include "GwTabButtonBase.generated.h"

class UCommonLazyImage;
/**
 * 
 */
UCLASS(Abstract, Blueprintable, meta = (DisableNativeTick))
class GWMULTIPLERPG_API UGwTabButtonBase : public UGwButtonBase, public IGwTabButtonInterface
{
	GENERATED_BODY()

public:
	void SetIconFromLazyObject(TSoftObjectPtr<UObject> LazyObject);
	void SetIconBrush(const FSlateBrush& Brush);

protected:
	UFUNCTION()
	virtual void SetTabLabelInfo_Implementation(const FGwTabDescriptor& TabDescriptor) override;

private:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UCommonLazyImage> LazyImage_Icon;
};
