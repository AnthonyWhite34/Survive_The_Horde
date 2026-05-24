// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/MyAttributeSet.h"
#include "MyGameplayTags.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UMyAttributeSet* AS = CastChecked<UMyAttributeSet>(AttributeSet);
	
	check(AttributeInfo);
	FMyAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(FMyGameplayTags::Get().Attributes_Primary_Strength);
	
	Info.AttributeValue = AS->GetStrength();
	
	AttributeInfoDelegate.Broadcast(Info);
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{

}
