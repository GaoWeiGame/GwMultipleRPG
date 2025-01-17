﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "GwTabListWidgetBase.generated.h"


USTRUCT(BlueprintType)
struct FGwTabDescriptor
{
	GENERATED_BODY()

public:
	FGwTabDescriptor()
	: bHidden(false)
	, CreatedTabContentWidget(nullptr)
	{ }

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName TabId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText TabText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush IconBrush;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bHidden;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UCommonButtonBase> TabButtonType;

	//TODO NDarnell - This should become a TSoftClassPtr<>, the underlying common tab list needs to be able to handle lazy tab content construction.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UCommonUserWidget> TabContentType;

	UPROPERTY(Transient)
	TObjectPtr<UWidget> CreatedTabContentWidget;
};


UINTERFACE(BlueprintType)
class UGwTabButtonInterface : public UInterface
{
	GENERATED_BODY()
};

class IGwTabButtonInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Tab Button")
	void SetTabLabelInfo(const FGwTabDescriptor& TabDescriptor);
};


UCLASS(Blueprintable, BlueprintType, Abstract, meta = (DisableNativeTick))
class GWMULTIPLERPG_API UGwTabListWidgetBase : public UCommonTabListWidgetBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Tab List")
	bool RegisterDynamicTab(const FGwTabDescriptor& TabDescriptor);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Tab List")
	bool GetPreregisteredTabInfo(const FName TabNameId, FGwTabDescriptor& OutTabInfo);

	/** Delegate broadcast when a new tab is created. Allows hook ups after creation. */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTabContentCreated, FName, TabId, UCommonUserWidget*, TabWidget);
	DECLARE_EVENT_TwoParams(UGwTabListWidgetBase, FOnTabContentCreatedNative, FName /* TabId */, UCommonUserWidget* /* TabWidget */);

	/** Broadcasts when a new tab is created. */
	UPROPERTY(BlueprintAssignable, Category = "Tab List")
	FOnTabContentCreated OnTabContentCreated;
	FOnTabContentCreatedNative OnTabContentCreatedNative;
	
protected:
	// UUserWidget interface
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// End UUserWidget

	virtual void HandlePreLinkedSwitcherChanged() override;
	virtual void HandlePostLinkedSwitcherChanged() override;
	
	virtual void HandleTabCreation_Implementation(FName TabId, UCommonButtonBase* TabButton) override;


private:

	void SetupTabs();

	UPROPERTY()
	TMap<FName, FGwTabDescriptor> PendingTabLabelInfoMap;

	UPROPERTY(EditAnywhere, meta=(TitleProperty="TabId"))
	TArray<FGwTabDescriptor> PreregisteredTabInfoArray;
};
