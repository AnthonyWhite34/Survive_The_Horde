// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "MyInputConfig.h"
#include "MyInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVE_THE_HORDE_API UMyInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	//create a function that we can use on our Player controller so we can bind callbacks. and i want the callbacks to be paramaters to those callbacks
	template<class UserClass, typename PressedFuncType, typename ReleaseFuncType, typename HeldFuncType>
	void BindAbilityActions(const UMyInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleaseFuncType ReleasedFunc, HeldFuncType HeldFunc);
	
	
};

template <class UserClass, typename PressedFuncType, typename ReleaseFuncType, typename HeldFuncType>
void UMyInputComponent::BindAbilityActions(const UMyInputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleaseFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);
	
	for (const FMyInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			
			if (PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}
			
			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}
			
			if (HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
			
		}
	}
}
