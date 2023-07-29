#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

UCLASS()
class UNREALSHOOTERGAME_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterBase();

	UFUNCTION(BlueprintPure, Category = "Stats")
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintPure, Category = "Stats")
	float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetHealth(const float NewHealth);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	virtual float TakeDamage(float DamageTaken, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
protected:
	UPROPERTY(ReplicatedUsing = OnRep_Health)
	float Health;
	
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float MaxHealth;
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void OnHealthUpdate();
	
	UFUNCTION()
	void OnRep_Health();
};
