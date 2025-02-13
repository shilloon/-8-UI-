#include "SlowingItem.h"
#include "SpartaCharacter.h"

ASlowingItem::ASlowingItem()
{

	DecreaseTime = 5.0f;
	DecreaseMove = 0.5f;
	ItemType = "Slow";
	ItemName = "SlowItem";
}

void ASlowingItem::ActivateItem(AActor* Activator)
{

	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player"))
	{

		if (ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(Activator))
		{

			PlayerCharacter->DecreaseSpeed(DecreaseMove, DecreaseTime);

		}
		DestroyItem();
	}

}