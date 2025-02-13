#include "SpartaCharacter.h"
#include "SpartaGameState.h"
#include "EnhancedInputComponent.h"
#include "SpartaPlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"

ASpartaCharacter::ASpartaCharacter()
{

	PrimaryActorTick.bCanEverTick = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArim"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	OverHeadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverHeadWidget"));
	OverHeadWidget->SetupAttachment(GetMesh());
	OverHeadWidget->SetWidgetSpace(EWidgetSpace::Screen);

	NormalSpeed = 600.0f;
	SprintSpeedMultiplier = 1.5f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	MaxHealth = 100;
	Health = MaxHealth;

	bIsReverse = true;

}

void ASpartaCharacter::BeginPlay()
{

	Super::BeginPlay();
	UpdateOverheadHP();

}

void ASpartaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{

		if (ASpartaPlayerController* PlayerController = Cast<ASpartaPlayerController>(GetController()))
		{

			if (PlayerController->MoveAction)
			{

				EnhancedInput->BindAction(

					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::Move

				);

			}

			if (PlayerController->JumpAction)
			{

				EnhancedInput->BindAction(

					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::StartJump

				);

				EnhancedInput->BindAction(

					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&ASpartaCharacter::StopJump

					);

			}
			if (PlayerController->LookAction)
			{

				EnhancedInput->BindAction(

					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::Look

				);

			}
			if (PlayerController->SprintAction)
			{

				EnhancedInput->BindAction(

					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::StartSprint

				);

				EnhancedInput->BindAction(

					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&ASpartaCharacter::StopSprint

				);

			}

		}

	}

}

void ASpartaCharacter::RestoreControl()
{

	bIsReverse = true;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Control is Restore!!!")));

}

void ASpartaCharacter::InvertControl(float ReverseTime, bool ReverseInput)
{


	bIsReverse = ReverseInput;

	GetWorld()->GetTimerManager().SetTimer(
		ReverseTimerHandle,
		this,
		&ASpartaCharacter::RestoreControl,
		ReverseTime,
		false
	);

}

void ASpartaCharacter::Move(const FInputActionValue& value)
{
	
	if (!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();

	if (!bIsReverse)
	{
		if (!FMath::IsNearlyZero(MoveInput.X))
		{

			AddMovementInput(GetActorForwardVector(), -MoveInput.X);

		}
		if (!FMath::IsNearlyZero(MoveInput.Y))
		{

			AddMovementInput(GetActorRightVector(), -MoveInput.Y);

		}
	}
	else
	{
		if (!FMath::IsNearlyZero(MoveInput.X))
		{

			AddMovementInput(GetActorForwardVector(), MoveInput.X);

		}
		if (!FMath::IsNearlyZero(MoveInput.Y))
		{

			AddMovementInput(GetActorRightVector(), MoveInput.Y);

		}
	}
}

void ASpartaCharacter::StartJump(const FInputActionValue& value)
{

	if (value.Get<bool>())
	{

		Jump();

	}

}

void ASpartaCharacter::StopJump(const FInputActionValue& value)
{

	if (!value.Get<bool>())
	{

		StopJumping();

	}

}

void ASpartaCharacter::Look(const FInputActionValue& value)
{

	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);

}

void ASpartaCharacter::StartSprint(const FInputActionValue& value)
{

	bIsSprint = true;
	UpdateSpeed();

}

void ASpartaCharacter::StopSprint(const FInputActionValue& value)
{

	bIsSprint = false;
	UpdateSpeed();

}

float ASpartaCharacter::GetHealth() const
{

	return Health;

}

void ASpartaCharacter::AddHealth(float Amount)
{

	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
	UpdateOverheadHP();

}

void ASpartaCharacter::UpdateSpeed()
{

	if (bIsSprint)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed * SprintSpeedMultiplier;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed * SprintSpeedMultiplier;
	}

}

void ASpartaCharacter::DecreaseSpeed(float DecreaseSpeed, float DecreaseTime)
{
	
	SprintSpeedMultiplier *= DecreaseSpeed;

	UpdateSpeed();

	GetWorld()->GetTimerManager().ClearTimer(SpeedTimerHandle);

	GetWorld()->GetTimerManager().SetTimer(
		SpeedTimerHandle,
		this,
		&ASpartaCharacter::RestoreSpeed,
		DecreaseTime,
		false
	);

}

void ASpartaCharacter::RestoreSpeed()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Speed is Restore!!!")));
	SprintSpeedMultiplier = 1.5f;
	UpdateSpeed();
}

float ASpartaCharacter::TakeDamage(
	float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser)
{

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	UpdateOverheadHP();

	if (Health <= 0.0f)
	{

		OnDeath();

	}

	return ActualDamage;

}

void ASpartaCharacter::OnDeath()
{
	ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
	if (SpartaGameState)
	{
		
		SpartaGameState->OnGameOver();

	}

}

void ASpartaCharacter::UpdateOverheadHP()
{

	if (!OverHeadWidget) return;

	UUserWidget* OverheadWidgetInstance = OverHeadWidget->GetUserWidgetObject();
	if (!OverheadWidgetInstance) return;

	if (UTextBlock* HPText = Cast<UTextBlock>(OverheadWidgetInstance->GetWidgetFromName(TEXT("OverHeadHP"))))
	{
		HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f /%.0f"), Health, MaxHealth)));
	}

}