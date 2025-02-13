#include "SmallCoinItem.h"

ASmallCoinItem::ASmallCoinItem()
{

	PointValue = 10;
	ItemType = "SmallCoin";
	ItemName = "SmallCoin";

}

void ASmallCoinItem::ActivateItem(AActor* Activator)
{

	Super::ActivateItem(Activator);

}