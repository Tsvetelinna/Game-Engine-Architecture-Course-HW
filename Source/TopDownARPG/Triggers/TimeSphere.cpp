// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeSphere.h"
#include "TopDownARPG.h"
#include "TopDownARPGCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/World.h"
#include "Projectiles/Projectile.h"

// Sets default values
ATimeSphere::ATimeSphere()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;


	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	RootComponent = SphereComponent;

	SphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ATimeSphere::OnOverlapBegin);
	SphereComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &ATimeSphere::OnOverlapEnd);

	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		UE_LOG(LogTopDownARPG, Error, TEXT("UAbility::Activate IsValid(World) == false"));
		return;
	}
	TimerManager = &World->GetTimerManager();
	TimerManager->SetTimer(ExpireTimerHandle, this, &ATimeSphere::OnExpireTimerExpired, ExpireTimeDilation, true, ExpireTime);
}

void ATimeSphere::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTopDownARPG, Log, TEXT("OnOverlapBegin"));
	
	AProjectile *Projectile = Cast<AProjectile>(OtherActor);
	if (IsValid(Projectile))
	{
		UE_LOG(LogTopDownARPG, Log, TEXT("Projectile entering"));
		Projectile->MovementComponent ->MaxSpeed /= slowAmount;
		UE_LOG(LogTopDownARPG, Log, TEXT("MaxSpeed %f"), Projectile->MovementComponent->MaxSpeed);

		return;
	}

	ATopDownARPGCharacter* Character = Cast<ATopDownARPGCharacter>(OtherActor);
	if (IsValid(Character))
	{
		UE_LOG(LogTopDownARPG, Log, TEXT("Character entering"));
		UCharacterMovementComponent* movement= Cast<UCharacterMovementComponent>(Character->GetMovementComponent());
		movement->MaxWalkSpeed /= slowAmount;
		UE_LOG(LogTopDownARPG, Log, TEXT("MaxWalkSpeed %f"), movement->MaxWalkSpeed);


		return;
	}
}


void ATimeSphere::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTopDownARPG, Log, TEXT("OnOverlapEnd"));


	ATopDownARPGCharacter* Character = Cast<ATopDownARPGCharacter>(OtherActor);
	if (IsValid(Character))
	{
		UE_LOG(LogTopDownARPG, Log, TEXT("Character exiting"));
		UCharacterMovementComponent* movement = Cast<UCharacterMovementComponent>(Character->GetMovementComponent());
		movement->MaxWalkSpeed *=  slowAmount;
		UE_LOG(LogTopDownARPG, Log, TEXT("MaxWalkSpeed %f"), movement->MaxWalkSpeed);
		
		return;
	}

	AProjectile *Projectile = Cast<AProjectile>(OtherActor);
	if (IsValid(Projectile))
	{
		UE_LOG(LogTopDownARPG, Log, TEXT("Projectile exiting"));
		UProjectileMovementComponent *mvmntComp = Projectile->MovementComponent;
		mvmntComp->MaxSpeed *= slowAmount;
		mvmntComp->Velocity *= slowAmount;
		mvmntComp->UpdateComponentVelocity();
		UE_LOG(LogTopDownARPG, Log, TEXT("MaxSpeed %f"), Projectile->MovementComponent->MaxSpeed);
		return;
	}
}

void ATimeSphere::OnExpireTimerExpired()
{
	if (Destroy() == false)
	{
		UE_LOG(LogTopDownARPG, Error, TEXT("TimeSphere is indestructable"));
	}
	else
	{
		UE_LOG(LogTopDownARPG, Log, TEXT("TimeSphere has expired"));
	}
}

// Called every frame
void ATimeSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

