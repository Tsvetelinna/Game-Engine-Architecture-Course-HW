// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeSphere.generated.h"

UCLASS()
class TOPDOWNARPG_API ATimeSphere : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimeSphere();
	
	UPROPERTY(EditAnywhere, Category = Gameplay)
	int slowAmount = 3;

	UPROPERTY(EditAnywhere)
	float ExpireTime = 7.f;
	UPROPERTY(EditAnywhere)
	float ExpireTimeDilation = 1.f;

protected:
	UPROPERTY(VisibleAnywhere, Category = Gameplay)
	class USphereComponent* SphereComponent;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	FTimerManager* TimerManager = nullptr;
	FTimerHandle ExpireTimerHandle;

	void OnExpireTimerExpired();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
