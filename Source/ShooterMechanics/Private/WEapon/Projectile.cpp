// Developed by @Rahul Chandra


#include "WEapon/Projectile.h"

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if(Tracer)
	{
		TracerComponent = UGameplayStatics::SpawnEmitterAttached(
					Tracer,
					CollisionBox,
					FName(),
					GetActorLocation(),
					GetActorRotation(),
					EAttachLocation::KeepWorldPosition);
	}
	
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

