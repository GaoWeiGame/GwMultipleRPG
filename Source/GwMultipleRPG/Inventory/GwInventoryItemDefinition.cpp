// Fill out your copyright notice in the Description page of Project Settings.


#include "GwInventoryItemDefinition.h"

#include "GwInventoryItemFragment.h"

const UGwInventoryItemFragment* UGwInventoryItemDefinition::FindFragmentByClass(TSubclassOf<UGwInventoryItemFragment> FragmentClass) const
{
	if (FragmentClass != nullptr)
	{
		for (UGwInventoryItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}

	return nullptr;
}
