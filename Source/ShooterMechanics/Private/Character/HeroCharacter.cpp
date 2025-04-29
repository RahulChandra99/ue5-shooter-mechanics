

#include "ShooterMechanics/Public/Character//HeroCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeroComponents/CombatComponent.h"
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

	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	Combat->SetIsReplicated(true);

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
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
	PlayerInputComponent->BindAction("Crouch",IE_Pressed,this,&AHeroCharacter::CrouchButtonPressed);
	PlayerInputComponent->BindAction("Equip",IE_Pressed,this,&AHeroCharacter::EquipButtonPressed);
	PlayerInputComponent->BindAction("Aim",IE_Pressed,this,&AHeroCharacter::AimButtonPressed);
	PlayerInputComponent->BindAction("Aim",IE_Released,this,&AHeroCharacter::AimButtonReleased);
	PlayerInputComponent->BindAction("Fire",IE_Pressed,this,&AHeroCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction("Fire",IE_Released,this,&AHeroCharacter::FireButtonReleased);
}

void AHeroCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if(Combat)
	{
		Combat->HeroCharacter = this;
	}
}

void AHeroCharacter::PlayFireMontage(bool bAiming)
{
	if(Combat == nullptr || Combat->EquippedWeapon == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && FireWeaponMontage)
	{
		AnimInstance->Montage_Play(FireWeaponMontage);
		FName SectionName;
		SectionName = bAiming ? FName("RifleAim") : FName("RifleHip");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void AHeroCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AHeroCharacter, OverlappingWeapon, COND_OwnerOnly)
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

void AHeroCharacter::CrouchButtonPressed()
{
	if(bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();	
	}
	
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

void AHeroCharacter::EquipButtonPressed()
{
	if(Combat)
	{
		if(HasAuthority())
		{
			Combat->EquipWeapon(OverlappingWeapon); //for server
		}
		else
		{
			ServerEquipButtonPressed(); // for clients
		}
	}
}



void AHeroCharacter::ServerEquipButtonPressed_Implementation()
{
	if(Combat)
	{
		Combat->EquipWeapon(OverlappingWeapon);
	}
	
}


bool AHeroCharacter::IsWeaponEquipped()
{
	return (Combat && Combat->EquippedWeapon);
}

bool AHeroCharacter::IsAiming()
{
	return(Combat && Combat->bAiming);
}

void AHeroCharacter::AimButtonPressed()
{
	if(Combat)
	{
		Combat->SetAiming(true);
	}
}

void AHeroCharacter::AimButtonReleased()
{
	if(Combat)
	{
		Combat->SetAiming(false);
	}
}

void AHeroCharacter::FireButtonPressed()
{
	if(Combat)
	{
		Combat->FireButtonPressed(true);
	}
}

void AHeroCharacter::FireButtonReleased()
{
	if(Combat)
	{
		Combat->FireButtonPressed(false);
	}
}







