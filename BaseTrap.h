#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTrap.generated.h"

class UBoxComponent;

UCLASS()
class PG0_API ABaseTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseTrap();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trap|Component")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trap|Component")
	UBoxComponent* Collision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trap|Component")
	UStaticMeshComponent* StaticMesh;

	void DestroyItem();

};
