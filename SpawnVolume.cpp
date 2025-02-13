#include "SpawnVolume.h"
#include "Components/BoxComponent.h"

ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);

}

AActor* ASpawnVolume::SpawnRandomItem()
{

	if (FItemSpawnRow* SelectedRow = GetRandomItem())
	{

		if (UClass* ActualClass = SelectedRow->ItemClass.Get())
		{

			return SpawnItem(ActualClass);

		}

	}

	return nullptr;
}

FItemSpawnRow* ASpawnVolume::GetRandomItem() const
{

	if (!ItemDataTable) return nullptr;

	TArray<FItemSpawnRow*> Allrows;
	static const FString ContextString(TEXT("ItemSpawnContext"));

	ItemDataTable->GetAllRows(ContextString,Allrows);

	if (Allrows.IsEmpty()) return nullptr;

	float TotalChance = 0.0f;
	for (const FItemSpawnRow* Row : Allrows)
	{

		if (Row)
		{

			TotalChance += Row->Spawnchance;

		}

	}

	const float RandValue = FMath::FRandRange(0.0f, TotalChance);
	float AccumulateChance = 0.0f;

	for (FItemSpawnRow* Row : Allrows)
	{

		AccumulateChance += Row->Spawnchance;
		if (RandValue <= AccumulateChance)
		{

			return Row;

		}
	}

	return nullptr;
}

FVector ASpawnVolume::GetRandomPointInVolume() const
{

	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	FVector BoxOrigin = SpawningBox->GetComponentLocation();

	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X), 
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
	);

}

AActor* ASpawnVolume::SpawnItem(TSubclassOf<AActor> ItemClass)
{

	if (!ItemClass) return nullptr;

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
		ItemClass,
		GetRandomPointInVolume(),
		FRotator::ZeroRotator
	);
	return SpawnedActor;
}