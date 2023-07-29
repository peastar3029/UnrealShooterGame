#include "CharacterBase.h"
#include "Net/UnrealNetwork.h"

ACharacterBase::ACharacterBase()
{
	MaxHealth = 100;
	Health = MaxHealth;
}

void ACharacterBase::SetHealth(const float NewHealth)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		Health = FMath::Clamp(NewHealth, 0, MaxHealth);
		OnHealthUpdate();
	}
}

float ACharacterBase::TakeDamage(const float DamageTaken, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	SetHealth(Health - DamageTaken);
	return Super::TakeDamage(DamageTaken, DamageEvent, EventInstigator, DamageCauser);
}

void ACharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACharacterBase, Health);
}

void ACharacterBase::OnHealthUpdate()
{
	if (IsLocallyControlled())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("You now have %f health remaining."), Health));

		if (Health <= 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You have been killed!"));
		}
	}
	
	if (GetLocalRole() == ROLE_Authority)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), Health));
	}
}

void ACharacterBase::OnRep_Health()
{
	OnHealthUpdate();
}
