// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/MyAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Weapon = CreateDefaultSubobject<UStaticMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACharacterBase::ApplyDamage(float Damage, AActor* DamageCauser, const FVector& DamageLocation, const FVector& DamageImpulse)
{
	float ActualDamage = 0.0f;

	if (UMyAttributeSet* MyAttributeSet = Cast<UMyAttributeSet>(AttributeSet))
	{
		const float PreviousHealth = MyAttributeSet->GetHealth();
		const float NewHealth = FMath::Clamp(PreviousHealth - Damage, 0.0f, MyAttributeSet->GetMaxHealth());
		MyAttributeSet->SetHealth(NewHealth);
		ActualDamage = PreviousHealth - NewHealth;

		if (NewHealth <= 0.0f)
		{
			HandleDeath();
		}
	}
	else
	{
		FDamageEvent DamageEvent;
		ActualDamage = TakeDamage(Damage, DamageEvent, nullptr, DamageCauser);
	}

	if (ActualDamage > 0.0f)
	{
		GetCharacterMovement()->AddImpulse(DamageImpulse, true);
		OnDamageReceived(ActualDamage, DamageLocation, DamageImpulse.GetSafeNormal());
	}
}


void ACharacterBase::HandleDeath()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();
	GetMesh()->SetSimulatePhysics(true);
}

void ACharacterBase::ApplyHealing(float Healing, AActor* Healer)
{
	if (UMyAttributeSet* MyAttributeSet = Cast<UMyAttributeSet>(AttributeSet))
	{
		const float NewHealth = FMath::Clamp(MyAttributeSet->GetHealth() + Healing, 0.0f, MyAttributeSet->GetMaxHealth());
		MyAttributeSet->SetHealth(NewHealth);
	}
}

void ACharacterBase::NotifyDanger(const FVector& DangerLocation, AActor* DangerSource)
{
	// Hook for child classes that need danger/block/dodge reactions.
}

void ACharacterBase::OnDamageReceived(float Damage, const FVector& DamageLocation, const FVector& DamageDirection)
{
	// Hook for child classes that need damage effects.
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacterBase::InitAbilityActorInfo()
{
}

void ACharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	
	const FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ACharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
 	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
}