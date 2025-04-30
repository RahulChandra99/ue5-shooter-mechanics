// Developed by @Rahul Chandra

#pragma once

#include "CoreMinimal.h"
#include "WEapon/BaseWeapon.h"
#include "ProjectileWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERMECHANICS_API AProjectileWeapon : public ABaseWeapon
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> ProjectileClass;

public:
	virtual void Fire(const FVector& HitTarget) override;
	
};
