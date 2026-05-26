// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/MyInputConfig.h"

const UInputAction* UMyInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FMyInputAction& Input : AbilityInputActions)
	{
		//Input.InputAction && Action.InputTag == InputTag
		if (Input.InputTag.MatchesTagExact(InputTag))
		{
			return Input.InputAction;
		}
	}
	
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Cant find AbilityInputAction for InputTag[%s], on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}
