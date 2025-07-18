// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "kismet/GameplayStatics.h"
#include "ShooterCharacter.h"
#include "Engine/DamageEvents.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

void AGun::PullTrigger()
{
	FHitResult HitResult;

	FVector InvertedShootDirection;

	//Particle and sound
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

	UE_LOG(LogTemp, Warning, TEXT("Gun Shot!"));

	//Line trace
	if (GunTrace(HitResult, InvertedShootDirection))
	{
		//Spawn impact particle and sound
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Impact, HitResult.Location, InvertedShootDirection.Rotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, HitResult.Location, 0.25);

		//Actor that got hit
		AActor* ActorHit = HitResult.GetActor();

		if (ActorHit)
		{
			//Damage event
			FPointDamageEvent DamageEvent(Damage, HitResult, InvertedShootDirection, nullptr);

			ActorHit->TakeDamage(Damage, DamageEvent, GetOwnerController(), this);
		}
	}
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
		
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Linetrace for the gun
bool AGun::GunTrace(FHitResult& OutHitResult, FVector& OutShotDirection)
{
	FVector CameraLocation;
	FRotator CameraRotator;

	//Get Player camera location and rotator
	if (GetOwnerController())
	{
		GetOwnerController()->GetPlayerViewPoint(CameraLocation, CameraRotator);
	}

	FVector End = CameraLocation + CameraRotator.Vector() * MaxRange;

	//Collsion parameters
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	//Inverted shot direction
	OutShotDirection = -CameraRotator.Vector();

	return GetWorld()->LineTraceSingleByChannel(OutHitResult, CameraLocation, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

//Returns the owner controller
AController* AGun::GetOwnerController() const
{
	//Get the pawn owner
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return nullptr;

	return OwnerPawn->GetController();
}

