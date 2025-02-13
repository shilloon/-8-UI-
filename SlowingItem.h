#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "SlowingItem.generated.h"


UCLASS()
class PG0_API ASlowingItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	ASlowingItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slow")
	float DecreaseMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slow")
	float DecreaseTime;

	virtual void ActivateItem(AActor* Activator) override;

};
