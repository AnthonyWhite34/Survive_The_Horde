// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/MyWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SURVIVE_THE_HORDE_API UAttributeMenuWidgetController : public UMyWidgetController
{
	GENERATED_BODY()
	
public:	
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
};
