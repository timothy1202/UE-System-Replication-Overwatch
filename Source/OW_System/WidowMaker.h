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

    // 조준 시 시야각 (값이 작을수록 크게 보임)
    UPROPERTY(EditAnywhere, Category = "Combat | Zoom")
    float ZoomFOV = 100.f;

    UPROPERTY(EditAnywhere, Category = "Combat | Zoom")
    float DefaultFOV = 90.f;

    // 줌 속도 (값이 클수록 빠르게 줌이 됨)
    UPROPERTY(EditAnywhere, Category = "Combat | Zoom")
    float ZoomInterpSpeed = 10.f;

    // 총구 소켓 이름 (언리얼 에디터의 스켈레탈 메시 소켓 이름과 일치해야 함)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    FName MuzzleSocketName = TEXT("Fire_Socket");

    // 최대 사거리 (100m)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float TraceDistance = 10000.f;

    // 디버그 라인 표시 여부
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    bool bDrawDebug = true;

    // 일반 사격 애니메이션
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    class UAnimMontage* HipFireMontage;

    // 저격(조준) 사격 애니메이션
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    class UAnimMontage* ScopedFireMontage;


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