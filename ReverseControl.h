#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "ReverseControl.generated.h"


UCLASS()
class PG0_API AReverseControl : public ABaseItem
{
	GENERATED_BODY()
	
public:
	AReverseControl();

	virtual void ActivateItem(AActor* Activator);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reverse")
	float ReverseTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reverse")
	bool ReverseInput;

};
