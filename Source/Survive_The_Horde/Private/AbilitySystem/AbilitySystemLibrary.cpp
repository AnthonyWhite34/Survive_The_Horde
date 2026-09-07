// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "Game/STHGameModeBase.h"
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

void UAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,
	ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	ASTHGameModeBase* STHGameMode =  Cast<ASTHGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (STHGameMode == nullptr) return;
	
	AActor* AvatarActor = ASC->GetAvatarActor();
	
	UCharacterClassInfo* CharacterClassInfo = STHGameMode->CharacterClassInfo;
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	
	FGameplayEffectContextHandle PrimaryAttributeContextHandle = ASC->MakeEffectContext();
	PrimaryAttributeContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());
	
	FGameplayEffectContextHandle SecondaryAttributeContextHandle = ASC->MakeEffectContext();
	SecondaryAttributeContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());
	
	FGameplayEffectContextHandle VitalAttributeContextHandle = ASC->MakeEffectContext();
	VitalAttributeContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}
