// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayer.h"
#include <Runtime\Engine\Public\DrawDebugHelpers.h>

// Sets default values
ABasePlayer::ABasePlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	TriggerCapsule->InitCapsuleSize(55.f, 96.0f);;
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);

	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &ABasePlayer::OnOverlapBegin);
	TriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &ABasePlayer::OnOverlapEnd);

	isAroundCar = false;
	isDisableCamera = false;
	isDisableWalk = false;
}

// Called when the game starts or when spawned
void ABasePlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("PlayerMoveForward", this, &ABasePlayer::MoveForward);
	PlayerInputComponent->BindAxis("PlayerMoveRight", this, &ABasePlayer::MoveRight);

	PlayerInputComponent->BindAxis("PlayerTurn", this, &ABasePlayer::AddControllerYawInput);
	PlayerInputComponent->BindAxis("PlayerLookUp", this, &ABasePlayer::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PlayerSitCar", IE_Pressed, this, &ABasePlayer::OnPressSitInCar);
	//PlayerInputComponent->BindAction("interact", IE_Pressed, this, &ABasePlayer::Interact);
}

void ABasePlayer::MoveForward(float Value)
{
	if (!isDisableWalk) {
		// Find out which way is "forward" and record that the player wants to move that way.
		FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}

}

void ABasePlayer::MoveRight(float Value)
{
	if (!isDisableWalk) {
		// Find out which way is "right" and record that the player wants to move that way.
		FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}

}

void ABasePlayer::OnPressSitInCar() {
	if (isAroundCar) {
		// handle possession
		if (!SavedController)
		{
			// save the controller
			SavedController = GetController();
		}

		// unpossess first ( helps with multiplayer )
		SavedController->UnPossess();

		// disable current player state management
		isDisableWalk = true;
		// disable movement mode
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

		// possess our new actor
		SavedController->Possess(Cast<APawn>(aroundedCar));
	}
}

void ABasePlayer::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// check if Actors do not equal nullptr
	if (OtherActor && OtherActor != this)
	{
		AatgArenaPawn* PossessableCharacter = Cast<AatgArenaPawn>(OtherActor);
		if (PossessableCharacter) {
			UE_LOG(LogTemp, Warning, TEXT("OVERLAP"));
			isAroundCar = true;
			aroundedCar = PossessableCharacter;
		}
	}
}

void ABasePlayer::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		AatgArenaPawn* PossessableCharacter = Cast<AatgArenaPawn>(OtherActor);
		if (PossessableCharacter) {
			UE_LOG(LogTemp, Warning, TEXT("OVERLAP END"));
			isAroundCar = false;
			aroundedCar = NULL;
		}
	}
}

void ABasePlayer::Interact()
{
	FVector Start;
	FVector End;

	FVector PlayerEyesLoc;
	FRotator PlayerEyesRot;

	GetActorEyesViewPoint(PlayerEyesLoc, PlayerEyesRot);

	Start = PlayerEyesLoc;
	End = PlayerEyesLoc + (PlayerEyesRot.Vector() * 200.f);

	FCollisionQueryParams TraceParams(FName(TEXT("InteractTrace")), true, this);

	FHitResult InteractHit = FHitResult(ForceInit);

	bool bIsHit = GetWorld()->LineTraceSingleByChannel(InteractHit, Start, End, ECC_GameTraceChannel3, TraceParams);

	if (bIsHit && InteractHit.GetActor() != this)
	{
		// Log(ELogLevel::WARNING, InteractHit.Actor->GetName());
		// start to end, green, will lines always stay on, depth priority, thickness of line
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 5.f, ECC_WorldStatic, 1.f);

		AatgArenaPawn* PossessableCharacter = Cast<AatgArenaPawn>(InteractHit.GetActor());
		if (PossessableCharacter)
		{
			// handle possession
			if (!SavedController)
			{
				// save the controller
				SavedController = GetController();
			}

			// unpossess first ( helps with multiplayer )
			SavedController->UnPossess();

			// disable current player state management
			isDisableWalk = true;
			// disable movement mode
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

			// possess our new actor
			SavedController->Possess(Cast<APawn>(InteractHit.GetActor()));
			// enable movement back on the possessed actor
			//PossessableCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Purple, false, 5.f, ECC_WorldStatic, 1.f);
	}
}