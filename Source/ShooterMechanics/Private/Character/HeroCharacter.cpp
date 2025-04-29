

#include "ShooterMechanics/Public/Character//HeroCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "WEapon/BaseWeapon.h"

AHeroCharacter::AHeroCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
}

void AHeroCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AHeroCharacter, OverlappingWeapon, COND_OwnerOnly)
}


void AHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward",this,&AHeroCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&AHeroCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn",this,&AHeroCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp",this,&AHeroCharacter::LookUp);

	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&ACharacter::Jump);
}

#pragma region LocomotionLogic

void AHeroCharacter::MoveForward(float Value)
{
	if(Controller!=nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));

		AddMovementInput(Direction, Value);
	}
}

void AHeroCharacter::MoveRight(float Value)
{
	if(Controller!=nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));

		AddMovementInput(Direction, Value);
	}
}

void AHeroCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void AHeroCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

#pragma endregion 


void AHeroCharacter::SetOverlappingWeapon(ABaseWeapon* Weapon)
{
	if(OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(false);// Hide old widget if we had one
	}
	OverlappingWeapon = Weapon;// Set the new weapon
	if (IsLocallyControlled())// Only on the local player
	{
		if(OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickupWidget(true);// Show the new weapon's widget
		}
	}
}

void AHeroCharacter::OnRep_OverlappingWeapon(ABaseWeapon* LastWeapon)
{
	if(OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(true);
	}
	if(LastWeapon)
	{
		LastWeapon->ShowPickupWidget(false);
	}
}







