// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "ShooterAIController.h"

void AKillEmAllGameMode::PawnKilled(APawn* Pawnkilled)
{
	Super::PawnKilled(Pawnkilled);	

	//Check if player died
	if (APlayerController* PlayerController = Cast<APlayerController>(Pawnkilled->GetController()))
	{
		//Player lost
		EndGame(false);
	}

	//Check if all AIs are dead
	for (AShooterAIController* AIControllers : TActorRange<AShooterAIController>(GetWorld()))
	{
		if (!AIControllers->IsDead()) return;
	}

	//Player won
	EndGame(true);

	UE_LOG(LogTemp, Warning, TEXT("A pawn was killed!!!"))
}

void AKillEmAllGameMode::EndGame(bool IsPlayerWinner)
{
	//Get all controllers in the level
	for (AController* Controllers : TActorRange<AController>(GetWorld()))
	{
		bool IsWinner = Controllers->IsPlayerController() == IsPlayerWinner;

		Controllers->GameHasEnded(Controllers->GetPawn(), IsWinner);
	} 
}
