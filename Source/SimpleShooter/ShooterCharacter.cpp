// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//Spawn gun
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	//Hide the default character gun
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	//Attach to character's hand
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("weapon_socket"));
	Gun->SetOwner(this);

	//Sets health to character
	Health = MaxHealth;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//Take damage override
float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToAppy = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToAppy = FMath::Min(Health, DamageToAppy);
	Health -= DamageToAppy;

	if (IsDead())
	{
		//Warn the gamemode that a pawn was killed
		if (ASimpleShooterGameModeBase* Gamemode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>())
		{
			Gamemode->PawnKilled(this);
		}

		//Disable capsule collsion
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//Remove controller
		DetachFromControllerPendingDestroy();
	}

	UE_LOG(LogTemp, Warning, TEXT("Health Left: %f"), Health)

	return DamageToAppy;
}

//Return true if pawn is dead
bool AShooterCharacter::IsDead() const
{
	if (Health <= 0)
	{
		return true;
	}
	else return false;
}

//Return the percentage of health left
float AShooterCharacter::GetHealtPercent() const
{
	return (Health / MaxHealth);
}

void AShooterCharacter::Shoot()
{
	Gun->PullTrigger();
}
