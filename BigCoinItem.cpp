#include "BigCoinItem.h"

ABigCoinItem::ABigCoinItem()
{

	PointValue = 50;
	ItemType = "BigCoin";
	ItemName = "BigCoin";

}

void ABigCoinItem::ActivateItem(AActor* Activator)
{

	Super::ActivateItem(Activator);

}