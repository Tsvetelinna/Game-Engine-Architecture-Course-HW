// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeSlowAbility.h"
#include "TopDownARPG.h"
#include "Engine/World.h"
#include "TopDownARPGCharacter.h"
#include "TopDownARPGPlayerController.h"
#include "Triggers/TimeSphere.h"
#include "Math/Vector.h"

void UTimeSlowAbility::Activate(AActor* Source)
{
	if (!bIsOffCooldown)
	{
		return;
	}
	Super::Activate(Source);

	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		UE_LOG(LogTopDownARPG, Error, TEXT("UBoltAbility::Activate IsValid(World) == false"));
		return;
	}
	FHitResult HitResult;


	ATopDownARPGCharacter* Character = Cast<ATopDownARPGCharacter>(Source);
	ATopDownARPGPlayerController* Controller = Cast<ATopDownARPGPlayerController>(Character->GetController());
	
	FHitResult Hit;
	Controller->GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	UE_LOG(LogTopDownARPG, Log, TEXT("Distance: %f"), FVector::Dist(Hit.Location, Source->GetActorLocation()) );
	
	if (FVector::Dist(Hit.Location, Source->GetActorLocation()) <= MaxCastDistance)
	{
		UE_LOG(LogTopDownARPG, Log, TEXT("TimeSlowActivated on: x:%d, y:%d, z%d"), Hit.Location.X, Hit.Location.Y, Hit.Location.Z);
		
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = Source;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AActor* TimeSphere = World->SpawnActor<AActor>(TimeSlowSphereClass, Hit.Location, Source->GetActorRotation(), SpawnParameters);
		if (IsValid(TimeSphere) == false)
		{
			UE_LOG(LogTopDownARPG, Error, TEXT("TIME SLOW ::Activate IsValid == false"));
			return;
		}
	}
}