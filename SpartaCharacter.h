// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpartaCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;
struct FInputActionValue;

UCLASS()
class PG0_API ASpartaCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ASpartaCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverHeadWidget;

	UFUNCTION(BlueprintPure, Category = "health")
	float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "health")
	void AddHealth(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Speed")
	void DecreaseSpeed(float DecreaseSpeed, float DecreaseTime);
	UFUNCTION(BlueprintCallable, Category = "Reverse")
	void InvertControl(float ReverseTime, bool ReverseInput);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "health")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "health")
	float Health;

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);

	void OnDeath();
	void UpdateOverheadHP();
	void RestoreSpeed();
	void UpdateSpeed();
	void RestoreControl();
private:	

	bool bIsSprint;
	FTimerHandle SpeedTimerHandle;

	bool bIsReverse;
	FTimerHandle ReverseTimerHandle;

	float NormalSpeed;
	float SprintSpeedMultiplier;
	float SprintSpeed;

};
