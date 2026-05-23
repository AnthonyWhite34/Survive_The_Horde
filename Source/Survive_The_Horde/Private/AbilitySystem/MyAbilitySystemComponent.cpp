// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MyAbilitySystemComponent.h"

#include "MyGameplayTags.h"

UMyAbilitySystemComponent::UMyAbilitySystemComponent()
{

}

void UMyAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UMyAbilitySystemComponent::EffectApplied);
	
	const FMyGameplayTags& GameplayTags = FMyGameplayTags::Get();
	GEngine->AddOnScreenDebugMessage(
		-1, 
		10.f, 
		FColor::Orange, 
		FString::Printf(TEXT("Tag: %s"), * GameplayTags.Attributes_Secondary_Armor.ToString())
		);
}
void UMyAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                              const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	//GEngine->AddOnScreenDebugMessage(1, 8.f, FColor::Blue, FString("Effect Applied!"));
	
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	
	EffectAssetTags.Broadcast(TagContainer);
	
}
