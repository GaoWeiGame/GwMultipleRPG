// Fill out your copyright notice in the Description page of Project Settings.


#include "GwAimSensitivityData.h"

const float UGwAimSensitivityData::SensitivtyEnumToFloat(const EGwGamepadSensitivity InSensitivity) const
{
	if (const float* Sens = SensitivityMap.Find(InSensitivity))
	{
		return *Sens;
	}

	return 1.0f;
}
