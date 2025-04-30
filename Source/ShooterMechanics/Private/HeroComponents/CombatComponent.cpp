// Developed by @Rahul Chandra


#include "HeroComponents/CombatComponent.h"

#include "Character/HeroCharacter.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "WEapon/BaseWeapon.h"

#define TRACE_LENGTH 80000.f

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	BaseWalkSpeed = 600.f;
	BaseAimWalkSpeed = 450.f;

}


void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if(HeroCharacter)
	{
		HeroCharacter->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	}
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHitResult HitResult;
	TraceUnderCrosshairs(HitResult);
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent,EquippedWeapon);
	DOREPLIFETIME(UCombatComponent,bAiming);
}

void UCombatComponent::EquipWeapon(ABaseWeapon* WeaponToEquip)
{
	if(HeroCharacter == nullptr || WeaponToEquip == nullptr) return;

	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	
	const USkeletalMeshSocket* HandSocket = HeroCharacter->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if(HandSocket)
	{
		HandSocket->AttachActor(EquippedWeapon, HeroCharacter->GetMesh());
	}
	EquippedWeapon->SetOwner(HeroCharacter);
	HeroCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	HeroCharacter->bUseControllerRotationYaw = true;
	
}

void UCombatComponent::OnRep_EquippedWeapon()
{
	if(EquippedWeapon && HeroCharacter)
	{
		HeroCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
		HeroCharacter->bUseControllerRotationYaw = true;
	}
}

void UCombatComponent::SetAiming(bool bIsAiming)
{
	bAiming = bIsAiming;	//Set locally first; Fast feedback, no input lag
	ServerSetAiming(bIsAiming);	//Ask server to set	; Server needs to know the truth

	if(HeroCharacter)
	{
		HeroCharacter->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? BaseAimWalkSpeed : BaseWalkSpeed;
	}
}



void UCombatComponent::ServerSetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;

	if(HeroCharacter)
	{
		HeroCharacter->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? BaseAimWalkSpeed : BaseWalkSpeed;
	}
}



void UCombatComponent::FireButtonPressed(bool bPressed)
{
	bFireButtonPressed = bPressed;

	if(bFireButtonPressed)
		ServerFire();
	
}



void UCombatComponent::ServerFire_Implementation()
{
	MultiCastFire();
}


void UCombatComponent::MultiCastFire_Implementation()
{
	if(EquippedWeapon == nullptr) return;
	
	if(HeroCharacter)
	{
		HeroCharacter->PlayFireMontage(bAiming);
		EquippedWeapon->Fire(HitTarget);
	}
}

void UCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;
	if(GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y/2.f);

	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
		);

	if(bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;
		FVector End = Start + CrosshairWorldDirection * TRACE_LENGTH;

		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,
			Start,
			End,
			ECC_Visibility
		);

		if(!TraceHitResult.bBlockingHit)
		{
			TraceHitResult.ImpactPoint = End;
			HitTarget = End;
		}
		else
		{
			HitTarget = TraceHitResult.ImpactPoint;
			DrawDebugSphere(GetWorld(), TraceHitResult.ImpactPoint,12.f,12,FColor::Red,false,-1);
		}
	}
}