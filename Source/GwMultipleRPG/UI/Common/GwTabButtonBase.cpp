// Copyright Epic Games, Inc. All Rights Reserved.

#include "GwTabButtonBase.h"

#include "CommonLazyImage.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GwTabButtonBase)

class UObject;
struct FSlateBrush;

void UGwTabButtonBase::SetIconFromLazyObject(TSoftObjectPtr<UObject> LazyObject)
{
	if (LazyImage_Icon)
	{
		LazyImage_Icon->SetBrushFromLazyDisplayAsset(LazyObject);
	}
}

void UGwTabButtonBase::SetIconBrush(const FSlateBrush& Brush)
{
	if (LazyImage_Icon)
	{
		LazyImage_Icon->SetBrush(Brush);
	}
}

void UGwTabButtonBase::SetTabLabelInfo_Implementation(const FGwTabDescriptor& TabLabelInfo)
{
	SetButtonText(TabLabelInfo.TabText);
	SetIconBrush(TabLabelInfo.IconBrush);
}

