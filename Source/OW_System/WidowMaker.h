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

    // 에디터의 'Details' 창에서 WBP_PlayerHUD를 선택할 수 있게 해줍니다.
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<class UUserWidget> HUDWidgetClass;

    // 생성된 위젯을 담아둘 변수 (나중에 조준 상태에 따라 조절할 때 사용)
    UPROPERTY()
    class UUserWidget* CurrentHUD;

    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    bool bIsAiming;

    // 조준 시 이동 속도 설정
    UPROPERTY(EditAnywhere, Category = "Combat")
    float AimMovementSpeed = 100.f;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float NormalMovementSpeed = 400.f;

    UPROPERTY(EditAnywhere, Category = "Widow | Aim")
    float ZoomFOV = 100.f;

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

    UFUNCTION(BlueprintCallable, Category = "Combat")
    bool GetIsAiming() const { return bIsAiming; }
};