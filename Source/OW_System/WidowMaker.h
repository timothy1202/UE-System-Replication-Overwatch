#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "WidowMaker.generated.h"

UCLASS()
class OW_SYSTEM_API AWidowMaker : public ACharacter
{
    GENERATED_BODY()

public:
    AWidowMaker();

protected:
    // --- 변수 영역 ---

    // 1. 에셋을 담을 변수들
    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* LookAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* FireAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* AimAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* JumpAction;

    // 2. 함수 형태가 조금 바뀝니다 (FInputMethodContext 대신 InputActionValue 사용)
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class UCameraComponent* FollowCamera;

    UPROPERTY(VisibleAnywhere)
    class USpringArmComponent* SpringArm;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widow | Aim")
    bool bIsAiming;

    UPROPERTY(EditAnywhere, Category = "Widow | Aim")
    float ZoomFOV = 30.f;

    float DefaultFOV;

    // --- 함수 영역 ---
    virtual void BeginPlay() override;

    // 입력 바인딩용 함수 (딱 한 번씩만 선언!)
    UFUNCTION()
    void StartAiming();

    UFUNCTION()
    void StopAiming();

    UFUNCTION()
    void Fire();


public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};