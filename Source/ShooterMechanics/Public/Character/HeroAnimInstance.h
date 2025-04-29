// Developed by @Rahul Chandra

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HeroAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERMECHANICS_API UHeroAnimInstance : public UAnimInstance
{
	GENERATED_BODY()


public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:

	UPROPERTY(BlueprintReadOnly, Category="Character", meta=(AllowPrivateAccess = true))
	class AHeroCharacter* HeroCharacterRef;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = true))
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = true))
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = true))
	bool bIsAccelerating;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = true))
	bool bWeaponEquipped;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = true))
	bool bIsCrouched;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = true))
	bool bIsAiming;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = true))
	float YawOffset;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = true))
	float Lean;

	FRotator CharacterRotationLastFrame;
	FRotator CharacterRotation;
	FRotator DeltaRotation;

	void LeanStrafeCalculation(float DeltaSeconds);
	
};
