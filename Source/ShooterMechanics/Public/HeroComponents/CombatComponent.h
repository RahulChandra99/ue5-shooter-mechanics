// Developed by @Rahul Chandra

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTERMECHANICS_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	friend class AHeroCharacter;
	
	void EquipWeapon(class ABaseWeapon* WeaponToEquip);
protected:
	virtual void BeginPlay() override;
	void SetAiming(bool bIsAiming);

	UFUNCTION(Server,Reliable)
	void ServerSetAiming(bool bIsAiming);

	UFUNCTION()
	void OnRep_EquippedWeapon();

	UFUNCTION()
	void FireButtonPressed(bool bPressed);

	UFUNCTION(Server, Reliable)
	void ServerFire();

	UFUNCTION(NetMulticast, Reliable)
	void MultiCastFire();

	UFUNCTION()
	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

private:

	UPROPERTY()
	AHeroCharacter* HeroCharacter;
	
	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	class ABaseWeapon* EquippedWeapon;

	UPROPERTY(Replicated)
	bool bAiming;

	UPROPERTY()
	bool bFireButtonPressed;
	
	UPROPERTY(EditAnywhere, Category="Movement")
	float BaseWalkSpeed;

	UPROPERTY(EditAnywhere, Category="Movement")
	float BaseAimWalkSpeed;

	FVector HitTarget;
};
