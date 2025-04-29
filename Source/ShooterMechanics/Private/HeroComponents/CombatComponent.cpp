// Developed by @Rahul Chandra


#include "HeroComponents/CombatComponent.h"

#include "Character/HeroCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "WEapon/BaseWeapon.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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
	EquippedWeapon->ShowPickupWidget(false);
}

