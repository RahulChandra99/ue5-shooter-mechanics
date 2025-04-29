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

private:

	UPROPERTY()
	AHeroCharacter* HeroCharacter;
	
	UPROPERTY(Replicated)
	class ABaseWeapon* EquippedWeapon;

		
};
