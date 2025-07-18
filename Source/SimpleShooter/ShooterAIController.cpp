// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"

void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();
	
	Target = UGameplayStatics::GetPlayerPawn(this, 0);

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);	

		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
	}

}

void AShooterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

APawn* AShooterAIController::GetTarget() const
{
	return Target;
}

bool AShooterAIController::IsDead() const
{
	//Cast to shooter character
	if (AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(GetPawn()))
	{
		return ShooterCharacter->IsDead();
	}

	return true;
}
