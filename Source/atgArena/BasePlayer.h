// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "atgArenaPawn.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BasePlayer.generated.h"

UCLASS()
class ATGARENA_API ABasePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasePlayer();

	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
	class UCapsuleComponent* TriggerCapsule;

	UPROPERTY(VisibleAnywhere)
	class AatgArenaPawn* aroundedCar;

	UPROPERTY(VisibleAnywhere)
	bool isAroundCar;

	UPROPERTY(VisibleAnywhere)
	class AController* SavedController;

	UPROPERTY(VisibleAnywhere)
		bool isDisableWalk;

	UPROPERTY(VisibleAnywhere)
		bool isDisableCamera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// declare overlap end function
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void MoveForward(float Value);

	// Handles input for moving right and left.
	UFUNCTION()
		void MoveRight(float Value);

	UFUNCTION()
		void OnPressSitInCar();

	UFUNCTION()
		void Interact();
};
