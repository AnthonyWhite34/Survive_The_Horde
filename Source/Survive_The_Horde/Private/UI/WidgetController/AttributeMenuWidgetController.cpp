// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/MyAttributeSet.h"
#include "MyGameplayTags.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UMyAttributeSet* AS = CastChecked<UMyAttributeSet>(AttributeSet);
	
	check(AttributeInfo);
	/*FMyAttributeInfo StrengthInfo = AttributeInfo->FindAttributeInfoForTag(FMyGameplayTags::Get().Attributes_Primary_Strength);
	StrengthInfo.AttributeValue = AS->GetStrength();
	AttributeInfoDelegate.Broadcast(StrengthInfo);
	
	FMyAttributeInfo IntelligenceInfo = AttributeInfo->FindAttributeInfoForTag(FMyGameplayTags::Get().Attributes_Primary_Intelligence);
	IntelligenceInfo.AttributeValue = AS->GetIntelligence();
	AttributeInfoDelegate.Broadcast(IntelligenceInfo);*/
	
	for (auto& Pair : AS->TagsToAttributes)
	{
		FMyAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
		Info.AttributeValue = Pair.Value().GetNumericValue(AS);
		AttributeInfoDelegate.Broadcast(Info);
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{

}
