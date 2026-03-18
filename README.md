1. 프로젝트 타이틀 및 소개
🚀 UE-System-Replication-Overwatch
Overwatch 2 Mechanics Analysis & Re-implementation in Unreal Engine 5

이 프로젝트는 블리자드의 자체 엔진(Prometheus) 기반 게임인 오버워치 2의 핵심 시스템을 분석하고, 이를 UE5 C++ 환경에서 최적화된 아키텍처로 재구현하는 기술 연구 프로젝트입니다.

2. 기술 스택 (Tech Stack)
Engine: Unreal Engine 5.x

Language: C++

Architecture: Component-Based Design / Data-Oriented (ECS Concept)

Key Focus: Movement Sync, Ability System, Collision Logic

3. 분석 및 구현 리스트 (여기를 계속 업데이트하세요!)
현재 분석 중이거나 구현 완료된 기능을 적습니다.

[ ] 트레이서 - 점멸 (Blink)

분석: 거리 기반 순간이동 및 잔상 효과 처리 로직

구현 포인트: LineTrace를 활용한 지형 체크 및 SetActorLocation 최적화

[ ] 로드호그 - 갈고리 사슬 (Chain Hook)

분석: 투사체 판정과 피격 대상 인입(Pull) 로직

구현 포인트: Physics Constraint 또는 Interp를 활용한 부드러운 이동

4. 핵심 기술 메모 (Learned & Applied)
공부한 내용을 짧게 기록합니다. (면접관들이 가장 좋아함!)

네트워크: 오버워치의 'Favor the Shooter' 판정 시스템 분석

최적화: 수많은 투사체 처리를 위한 오브젝트 풀링(Object Pooling) 적용

5. 참고 자료 (References)
[GDC] Overwatch Gameplay Architecture and Netcode

[Blizzard Tech Blog] Overwatch 2 Engine Evolution
