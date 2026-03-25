// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WidowmakerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class OW_SYSTEM_API UWidowmakerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
    // 매 프레임 호출되어 데이터를 갱신합니다.
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    // 블루프린트에서 애니메이션 전환 조건으로 사용할 변수
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
    float GroundSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
    bool bShouldMove;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    bool bIsAiming;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
    bool bIsFalling;

};
