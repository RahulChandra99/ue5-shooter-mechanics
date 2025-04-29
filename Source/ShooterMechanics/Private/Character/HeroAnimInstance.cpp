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

	LeanStrafeCalculation(DeltaSeconds);

}

void UHeroAnimInstance::LeanStrafeCalculation(float DeltaSeconds)
{
	// Offset Yaw for Strafing
	// Get the character's aiming rotation (where the player is looking)
	FRotator AimRotation = HeroCharacterRef->GetBaseAimRotation();

	// Get the rotation based on character's movement direction (velocity)
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(HeroCharacterRef->GetVelocity());

	// Find the *difference* between movement direction and aim direction
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);

	// Smoothly interpolate the DeltaRotation over time to the new DeltaRot
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaSeconds, 15.f);

	// Store the yaw offset, used for strafing animations (how much character is facing differently than moving)
	YawOffset = DeltaRotation.Yaw;


	// --- LEANING CALCULATIONS ---

	// Store rotation from last frame
	CharacterRotationLastFrame = CharacterRotation;

	// Update rotation for current frame
	CharacterRotation = HeroCharacterRef->GetActorRotation();

	// Find how much the character rotated between this frame and the last
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);

	// Calculate rotational speed (Yaw change per second)
	const float Target = Delta.Yaw / DeltaSeconds;

	// Smoothly interpolate the Lean value toward the Target rotational speed
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaSeconds, 6.f);

	// Clamp Lean value between -90 and 90 degrees to avoid extreme values
	Lean = FMath::Clamp(Interp, -90.f, 90.f);
}


