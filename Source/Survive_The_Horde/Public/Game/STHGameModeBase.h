// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "STHGameModeBase.generated.h"

/**
 * 
 */
class UCharacterClassInfo;

UCLASS()
class SURVIVE_THE_HORDE_API ASTHGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defualts")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
	
};
