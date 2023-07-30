#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class ASimpleProjectile;
class UInputAction;
class UInputMappingContext;

UCLASS()
class UNREALSHOOTERGAME_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterBase();
	
protected:
	FTimerHandle FiringTimer;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health)
	float Health;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	TSubclassOf<ASimpleProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float FireRate;

	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	bool bIsFiringWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* FireInputAction;

public:
	float GetHealth() const { return Health; }
	float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetHealth(const float NewHealth);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	virtual float TakeDamage(float DamageTaken, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void OnHealthUpdate();
	
	UFUNCTION()
	void OnRep_Health();
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StartFire();
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopFire();
	
	UFUNCTION(Server, Reliable)
	void HandleFire();
};
