// Developed by @Rahul Chandra


#include "Character/HeroAnimInstance.h"

#include "Character/HeroCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UHeroAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	HeroCharacterRef = Cast<AHeroCharacter>(TryGetPawnOwner());
}

void UHeroAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(HeroCharacterRef == nullptr)
	{
		HeroCharacterRef = Cast<AHeroCharacter>(TryGetPawnOwner());
	}

	if(HeroCharacterRef == nullptr) return;

	//Speed
	FVector Velocity = HeroCharacterRef->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsInAir = HeroCharacterRef->GetCharacterMovement()->IsFalling();
	bIsAccelerating = HeroCharacterRef->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	bIsCrouched = HeroCharacterRef->bIsCrouched;
	bWeaponEquipped = HeroCharacterRef->IsWeaponEquipped();
}
