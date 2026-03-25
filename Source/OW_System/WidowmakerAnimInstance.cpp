// Fill out your copyright notice in the Description page of Project Settings.


#include "WidowmakerAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WidowMaker.h"

void UWidowmakerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    // 중복되는 로직을 정리하여 최적화한 코드입니다.
    AWidowMaker* Widow = Cast<AWidowMaker>(TryGetPawnOwner());

    if (Widow)
    {
        // 1. 속도 계산
        GroundSpeed = Widow->GetVelocity().Size();

        // 2. 이동 여부 판단 (CharacterMovement 활용)
        bShouldMove = (GroundSpeed > 3.0f) &&
            (Widow->GetCharacterMovement()->GetCurrentAcceleration() != FVector::ZeroVector);

        // 3. 조준 상태 가져오기
        bIsAiming = Widow->GetIsAiming();
    }
}