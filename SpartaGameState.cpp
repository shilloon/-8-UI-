#include "SpartaGameState.h"
#include "SpartaGameInstance.h"
#include "SpartaPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "CoinItem.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

ASpartaGameState::ASpartaGameState()
{

	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	CurrentLevelIndex = 0;
	MaxLevels = 3;
	CurrentWaveIndex = 0;
	MaxWavesPerLevel = 3;
	LevelDuration = 30.f;
	ItemToSpawn = 20;
}

void ASpartaGameState::BeginPlay()
{

	Super::BeginPlay();

	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ASpartaGameState::UpdateHUD,
		0.1f,
		true
	);

}

int32 ASpartaGameState::GetScore() const
{

	return Score;

}

void ASpartaGameState::AddScore(int32 Amount)
{

	if (UGameInstance* GameInstance = GetGameInstance())
	{

		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{

			SpartaGameInstance->AddToScore(Amount);

		}

	}

}

void ASpartaGameState::StartWave()
{

	if (UGameInstance* GameInstance = GetGameInstance())
	{

		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{

			CurrentLevelIndex = SpartaGameInstance->CurrentLevelIndex;

		}

	}

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("%dLevel %dWave start!"), CurrentLevelIndex + 1, CurrentWaveIndex + 1));

	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	/*
	switch (CurrentWaveIndex)
	{

		case 2:

			break;
		case 3:

			break;
		default:
			break;

	}
	*/

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	for (int i = 0; i < ItemToSpawn; i++)
	{

		if (FoundVolumes.Num() > 0)
		{

			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{

				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{

					SpawnedCoinCount++;

				}

			}

		}

	}
	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&ASpartaGameState::OnWaveTimeUp,
		LevelDuration,
		false
	);

}

void ASpartaGameState::StartLevel()
{
	CurrentWaveIndex = 0;
	StartWave();
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{

		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			
			SpartaPlayerController->ShowGameHUD();

		}

	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{

		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{

			CurrentLevelIndex = SpartaGameInstance->CurrentLevelIndex;

		}

	}

}

void ASpartaGameState::OnCoinCollected()
{

	CollectedCoinCount++;
	UE_LOG(LogTemp, Warning, TEXT("Coin Collected: %d / %d"), CollectedCoinCount, SpawnedCoinCount);

	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{

		EndWave();

	}

}

void ASpartaGameState::OnWaveTimeUp()
{

	EndWave();

}

void ASpartaGameState::EndWave()
{

	ItemToSpawn += (CurrentWaveIndex * 0.5);
	LevelDuration -= 10.f;
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	CurrentWaveIndex++;

	if (CurrentWaveIndex < MaxWavesPerLevel)
	{

		StartWave();

	}
	else
	{

		EndLevel();

	}
}

void ASpartaGameState::EndLevel()
{
	
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	CurrentLevelIndex++;
	if (UGameInstance* GameInstance = GetGameInstance())
	{

		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{

			AddScore(Score);
			SpartaGameInstance->CurrentLevelIndex = CurrentLevelIndex;

		}

	}

	if (CurrentLevelIndex > MaxLevels)
	{

		OnGameOver();
		return;
	}

	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{

		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);

	}
	else
	{

		OnGameOver();

	}
}

void ASpartaGameState::OnGameOver()
{

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{

		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			SpartaPlayerController->SetPause(true);
			SpartaPlayerController->ShowMainMenu(true);

		}

	}

}

void ASpartaGameState::UpdateHUD()
{

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{

		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{

			if (UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget())
			{

				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{

					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));

				}
				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{

					if (UGameInstance* GameInstance = GetGameInstance())
					{
						USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
						if (SpartaGameInstance)
						{

							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), SpartaGameInstance->TotalScore)));

						}
					}

				}
				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{

					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d-%d"), CurrentLevelIndex + 1, CurrentWaveIndex + 1)));

				}
			}
		}

	}

}