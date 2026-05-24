// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MyPlayerState.h"
#include "UI/WidgetController/MyWidgetController.h"
#include "UI/STH_HUD.h"

UOverlayWidgetController* UAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	//get widget controller and return to the caller
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ASTH_HUD* STH_HUD = Cast<ASTH_HUD>(PlayerController->GetHUD()))
		{
			AMyPlayerState* PS = PlayerController->GetPlayerState<AMyPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PS, ASC, AS);
			return STH_HUD->GetOverlayWidgetController(WidgetControllerParams);
			
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	//get widget controller and return to the caller
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ASTH_HUD* STH_HUD = Cast<ASTH_HUD>(PlayerController->GetHUD()))
		{
			AMyPlayerState* PS = PlayerController->GetPlayerState<AMyPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PS, ASC, AS);
			return STH_HUD->GetAttributeMenuWidgetController(WidgetControllerParams);
			
		}
	}
	return nullptr;
}
