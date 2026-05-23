// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/*
 * Survive the Horde Gameplay Tags
 *
 * 
 */

struct FMyGameplayTags
{
public: 
	static const FMyGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();
	
	FGameplayTag Attributes_Secondary_Armor;
	
protected:
	
private:
	static FMyGameplayTags GameplayTags;
	
};
