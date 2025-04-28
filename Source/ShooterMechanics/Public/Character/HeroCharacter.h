
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HeroCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class SHOOTERMECHANICS_API AHeroCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AHeroCharacter();
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta = (AllowPrivateAccess = true))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta = (AllowPrivateAccess = true))
	UCameraComponent* FollowCamera;

public:

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom;}
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera;}

};
