// Developed by @Rahul Chandra


#include "Character/HeroAnimInstance.h"

#include "Character/HeroCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

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
	bIsAiming = HeroCharacterRef->IsAiming();

	//Offset Yaw for Strafing
	FRotator AimRotation = HeroCharacterRef->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(HeroCharacterRef->GetVelocity());
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaSeconds, 15.f);
	YawOffset = DeltaRotation.Yaw;

	
	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = HeroCharacterRef->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = Delta.Yaw/ DeltaSeconds;
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaSeconds, 6.f);
	Lean = FMath::Clamp(Interp, -90.f,90.f);
}
