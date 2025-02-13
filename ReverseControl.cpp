#include "ReverseControl.h"
#include "SpartaCharacter.h"

AReverseControl::AReverseControl()
{
	ItemType = "Reverse";
	ItemName = "ReverseItem";
	ReverseTime = 3.0f;
	ReverseInput = false;
}

void AReverseControl::ActivateItem(AActor* Activator)
{

	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player"))
	{

		if (ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(Activator))
		{

			PlayerCharacter->InvertControl(ReverseTime, ReverseInput);

		}
		DestroyItem();
	}

}