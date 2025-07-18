// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleShooterPlayerController.h"
#include "Blueprint/UserWidget.h"

void ASimpleShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//Create HUD screen
	HUDScreen = CreateWidget(this, HUDWidget);
	HUDScreen->AddToViewport();
}

void ASimpleShooterPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);
	
	UUserWidget* EndGameScreen;

	if (bIsWinner)
	{
		//Create Win screen UI
		EndGameScreen = CreateWidget(this, WinScreenWidget);

		UE_LOG(LogTemp, Warning, TEXT("Player Won!!!"))
	}
	else
	{
		//Create lose screen UI
		EndGameScreen = CreateWidget(this, LoseScreenWidget);

		UE_LOG(LogTemp, Warning, TEXT("Player lost!!!"))
	}

	//Spawn the screen
	if (EndGameScreen)
	{
		EndGameScreen->AddToViewport();
	}

	//Remove the HUD
	if (HUDScreen)
	{
		HUDScreen->RemoveFromViewport();
	}

	//Timer to restart level
	GetWorldTimerManager().SetTimer(RestartTimerHandle, this, &APlayerController::RestartLevel, RestartTime, false);
}