#include "CharacterBase.h"
#include "SimpleProjectile.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Net/UnrealNetwork.h"

ACharacterBase::ACharacterBase()
{
	MaxHealth = 100;
	Health = MaxHealth;
	ProjectileClass = ASimpleProjectile::StaticClass();
	FireRate = 0.25f;
	bIsFiringWeapon = false;
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

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACharacterBase, Health);
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			InputSystem->AddMappingContext(InputMappingContext, 0);
		}
	}
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(FireInputAction, ETriggerEvent::Triggered, this, &ACharacterBase::StartFire);
	}
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

void ACharacterBase::StartFire()
{
	if (!bIsFiringWeapon)
	{
		if (const UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(FiringTimer, this, &ACharacterBase::StopFire, FireRate, false);
			bIsFiringWeapon = true;
			HandleFire();
		}
	}
}

void ACharacterBase::StopFire()
{
	bIsFiringWeapon = false;
}

void ACharacterBase::HandleFire_Implementation()
{
	const FVector SpawnLocation = GetActorLocation() + GetActorRotation().Vector() * 100.0f + GetActorUpVector() * 50.0f;
	const FRotator SpawnRotation = GetActorRotation();
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = GetInstigator();
	SpawnParameters.Owner = this;

	GetWorld()->SpawnActor<ASimpleProjectile>(SpawnLocation, SpawnRotation, SpawnParameters);
}
