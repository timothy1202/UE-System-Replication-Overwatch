#include "WidowMaker.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/Widget.h" // 위젯 가시성 제어를 위해 필요
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

#include<Camera/CameraComponent.h>

AWidowMaker::AWidowMaker()
{
    PrimaryActorTick.bCanEverTick = true;

    // 1. 스프링 암(셀카봉) 설정
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 400.0f;        // 캐릭터와의 거리
    SpringArm->bUsePawnControlRotation = true; // 컨트롤러 회전에 따라 셀카봉 회전
    SpringArm->SocketOffset = FVector(0.f, 75.0f, 0.f);

    // 2. 카메라 설정
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));

    // ⭐ 중요: 카메라를 스프링 암의 '소켓(끝부분)'에 장착합니다!
    FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

    // 스프링 암을 쓰면 카메라는 소켓 위치를 따라가므로 
    // SetRelativeLocation은 보통 (0,0,0)이거나 아주 살짝만 조정합니다.
    FollowCamera->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

    // 3. 줌 관련 설정
    DefaultFOV = 90.f;
    ZoomFOV = 30.0f;
}

void AWidowMaker::BeginPlay()
{

    Super::BeginPlay();
    // 1. 플레이어 컨트롤러에 Mapping Context를 주입(Add)합니다.
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }

    // 게임 시작 시 실제 카메라의 FOV를 기본값으로 저장해두면 더 정확합니다.
    if (FollowCamera)
    {
        DefaultFOV = FollowCamera->FieldOfView;
    }

    // 1. 내가 조종하는 로컬 플레이어일 때만 UI를 띄웁니다. (멀티플레이 고려)
    if (IsLocallyControlled() && HUDWidgetClass)
    {
        // 2. 위젯 생성
        CurrentHUD = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);

        // 3. 화면(Viewport)에 추가
        if (CurrentHUD)
        {
            CurrentHUD->AddToViewport();
        }
    }
}

void AWidowMaker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (FollowCamera)
    {
        // 1. 현재 상태에 따른 목표 FOV 결정
        float TargetFOV = bIsAiming ? ZoomFOV : DefaultFOV;

        // 2. 현재 FOV에서 목표 FOV로 부드럽게 보간 (FInterpTo)
        float CurrentFOV = FollowCamera->FieldOfView;
        float NewFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, ZoomInterpSpeed);

        // 3. 카메라에 새로운 FOV 적용
        FollowCamera->SetFieldOfView(NewFOV);
    }
}

void AWidowMaker::StartAiming()
{
    bIsAiming = true;

    // UI 제어 로직 추가
    if (CurrentHUD)
    {
        // WBP에서 'is Variable' 체크한 이름을 가져옵니다.
        UWidget* ScopeImg = CurrentHUD->GetWidgetFromName(TEXT("SniperScope"));
        UWidget* CrosshairImg = CurrentHUD->GetWidgetFromName(TEXT("Crosshair"));

        if (ScopeImg) ScopeImg->SetVisibility(ESlateVisibility::Visible);
        if (CrosshairImg) CrosshairImg->SetVisibility(ESlateVisibility::Hidden);
    }

    GetCharacterMovement()->MaxWalkSpeed = AimMovementSpeed;
}

void AWidowMaker::StopAiming()
{
    bIsAiming = false;

    if (CurrentHUD)
    {
        UWidget* ScopeImg = CurrentHUD->GetWidgetFromName(TEXT("SniperScope"));
        UWidget* CrosshairImg = CurrentHUD->GetWidgetFromName(TEXT("Crosshair"));

        if (ScopeImg) ScopeImg->SetVisibility(ESlateVisibility::Hidden);
        if (CrosshairImg) CrosshairImg->SetVisibility(ESlateVisibility::Visible);
    }

    GetCharacterMovement()->MaxWalkSpeed = NormalMovementSpeed;
}

void AWidowMaker::Fire()
{
    UWorld* World = GetWorld();
    if (!World || !FollowCamera || !GetMesh()) return;

    // 1. 애니메이션 재생 로직
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance)
    {
        // 조준 상태에 따라 재생할 몽타주 선택
        UAnimMontage* MontageToPlay = bIsAiming ? ScopedFireMontage : HipFireMontage;

        if (MontageToPlay)
        {
            AnimInstance->Montage_Play(MontageToPlay);
        }
    }

    // 1. 카메라 판정 정보
    FVector CameraStart = FollowCamera->GetComponentLocation();
    FVector ForwardVector = FollowCamera->GetForwardVector();

    // [수정] 하드코딩된 10000.f 대신 TraceDistance 변수 활용!
    FVector TraceEnd = CameraStart + (ForwardVector * TraceDistance);

    // 2. 캐릭터 메쉬에 박아둔 소켓 정보 가져오기
    // [수정] TEXT("Fire_Socket") 대신 MuzzleSocketName 변수 활용!
    FTransform MuzzleTransform = GetMesh()->GetSocketTransform(MuzzleSocketName);
    FVector MuzzleLocation = MuzzleTransform.GetLocation();

    // 3. 라인 트레이스 (판정은 카메라 기준)
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    bool bHit = World->LineTraceSingleByChannel(HitResult, CameraStart, TraceEnd, ECC_Visibility, Params);

    // 4. 시각적 피드백 (총구 소켓 -> 충돌 지점)
    FVector ActualLineEnd = bHit ? HitResult.ImpactPoint : TraceEnd;

    // [수정] bDrawDebug가 true일 때만 디버그 라인을 그리도록 활용!
    if (bDrawDebug)
    {
        DrawDebugLine(World, MuzzleLocation, ActualLineEnd, FColor::Yellow, false, 1.0f, 0, 2.0f);
    }

    if (bHit)
    {
        // ... 생략 (대미지 로직 등)
        UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *HitResult.GetActor()->GetName());
    }
}

// 1. 키보드 이동 연결
void AWidowMaker::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // 부모의 기능을 EnhancedInputComponent로 캐스팅해서 사용합니다.
    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // 1. 이동과 회전 (계속 실행되는 Triggered 이벤트)
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWidowMaker::Move);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWidowMaker::Look);

        // 2. 사격 (누르는 순간 실행되는 Started 이벤트)
        // ※ FireAction 변수를 헤더에 추가해줘야 합니다!
        EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AWidowMaker::Fire);

        // 3. 조준 (누르면 시작, 떼면 종료)
        // ※ AimAction 변수를 헤더에 추가해줘야 합니다!
        EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &AWidowMaker::StartAiming);
        EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AWidowMaker::StopAiming);

        // 4. 점프 (ACharacter에 이미 기능이 내장되어 있어 &ACharacter::Jump만 연결하면 끝!)
        // ※ JumpAction 변수를 헤더에 추가해줘야 합니다!
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
    }
}

// 3. 실제 이동 로직 (FVector2D로 X, Y값을 한 번에 받습니다!)
void AWidowMaker::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(ForwardDirection, MovementVector.Y); // W, S
        AddMovementInput(RightDirection, MovementVector.X);   // A, D
    }
}

void AWidowMaker::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}