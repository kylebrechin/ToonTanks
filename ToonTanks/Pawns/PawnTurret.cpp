// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTurret.h"
#include "Kismet/GameplayStatics.h"
#include "PawnTank.h"

APawnTurret::APawnTurret()
{
    
}


void APawnTurret::BeginPlay()
{
    Super::BeginPlay();
    GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle,this, &APawnTurret::CheckFireCondition, FireRate, true);

    PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this,0));
}

// Called every frame
void APawnTurret::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(!PlayerPawn)
    {
        return;
    }

    if(bTargetInRange)
    {
        RotateTurret(PlayerPawn->GetActorLocation());
    }

}

void APawnTurret::CheckFireCondition()
{
    //UE_LOG(LogTemp, Warning, TEXT("Turret: Fire Condition Check"))

    // if player == null || is DEAD then bail
    if(!PlayerPawn) { return; }

    if(CheckTargetInRange())
    {
        bTargetInRange = true;
        //UE_LOG(LogTemp, Warning, TEXT("Turret: Target in Range"));
        // If player IS in range THEN FIRE
        if(ReturnDistanceToPlayer() <= FireRange)
        {
            // Call Fire() from base class
            Fire();
        }
    } else
    {
        bTargetInRange = false;
        //UE_LOG(LogTemp, Error, TEXT("Turret: No Target Found"));

    }
}

bool APawnTurret::CheckTargetInRange()
{
    if(!PlayerPawn) { return false; }

    return (FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation()) <= ViewRange);
}


float APawnTurret::ReturnDistanceToPlayer()
{
    if(!PlayerPawn) { return 0.0f; }

    return FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation());
}

void APawnTurret::HandleDestruction()
{
    Super::HandleDestruction();
    Destroy();
}

