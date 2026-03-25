// Fill out your copyright notice in the Description page of Project Settings.


#include "WidowmakerAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WidowMaker.h"

void UWidowmakerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    AWidowMaker* Widow = Cast<AWidowMaker>(TryGetPawnOwner());
    if (Widow)
    {
        UCharacterMovementComponent* Movement = Widow->GetCharacterMovement();
        if (Movement)
        {
            // 1. 기본 물리 데이터 업데이트
            GroundSpeed = Widow->GetVelocity().Size();
            bIsFalling = Movement->IsFalling();
            bIsAiming = Widow->GetIsAiming();

            // 2. 이동 여부 판단
            bShouldMove = (GroundSpeed > 3.0f) &&
                (Movement->GetCurrentAcceleration() != FVector::ZeroVector);
     
        }
    }

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(1, 0.0f, FColor::Yellow,
            FString::Printf(TEXT("IsFalling: %s, Speed: %f"), bIsFalling ? TEXT("True") : TEXT("False"), GroundSpeed));
    }
}