// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAssetManager.h"
#include "MyGameplayTags.h"
#include "AbilitySystem/MyAbilitySystemComponent.h"

UMyAssetManager& UMyAssetManager::Get()
{
	check(GEngine);
	UMyAssetManager* MyAssetManager = Cast<UMyAssetManager>(GEngine->AssetManager);
	return *MyAssetManager;
}

void UMyAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	
	FMyGameplayTags::InitializeNativeGameplayTags();
	
	//This is required to use TargetData
	UAbilitySystemGlobals::Get().InitGlobalData();
