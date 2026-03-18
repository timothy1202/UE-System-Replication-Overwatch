# 🚀 UE-System-Replication-Overwatch
### Overwatch 2 Mechanics Analysis & Re-implementation in Unreal Engine 5

이 프로젝트는 블리자드의 자체 제작 엔진(Prometheus) 기반 게임인 **오버워치 2**의 핵심 시스템을 분석하고, 이를 **UE5 C++** 환경에서 최적화된 아키텍처로 재구현하는 기술 연구 프로젝트입니다.

---

## 🛠️ Tech Stack
* **Engine**: Unreal Engine 5 (UE5)
* **Language**: C++
* **Architecture**: Component-Based Design / Data-Oriented (ECS Concept)
* **Key Focus**: Movement Sync, Ability System, Collision Logic

---

## 🔍 Hero Ability Analysis & Implementation
각 영웅의 핵심 스킬 로직을 분석하고 언리얼 시스템으로 이식합니다.

### ⚡ Tracer (트레이서)
* **[ ] 점멸 (Blink)**
    * **분석**: 거리 기반 순간이동 및 잔상 효과(Ghost Trail) 처리 로직
    * **구현 계획**: `LineTrace`를 활용한 지형 충돌 체크 및 `SetActorLocation` 최적화
* **[ ] 시간 역행 (Recall)**
    * **분석**: 일정 시간 동안의 위치 및 상태값 저장/복구 시스템 (Ring Buffer 구조)

### 🐷 Roadhog (로드호그)
* **[ ] 갈고리 사슬 (Chain Hook)**
    * **분석**: 투사체 히트 판정 및 피격 대상 인입(Pull) 로직
    * **구현 계획**: `Physics Constraint` 또는 `FInterp`를 활용한 부드러운 이동 제어

---

## 🧠 Core Engineering Notes (CS & OS)
프로그래밍 원리와 운영체제 지식을 게임 로직에 연결한 기록입니다.

* **네트워크**: 오버워치의 'Favor the Shooter' 판정 시스템 분석 및 예측(Prediction) 로직 연구
* **최적화**: 수많은 투사체 처리를 위한 **Object Pooling** 적용 (메모리 단편화 방지 및 CPU 캐시 효율 증대)
* **OS 연결**: 사용자 입력(Input)에 따른 **Interrupt** 처리 방식과 게임 루프(Tick)의 동기화 구조 설계

---

## 📚 References
* [GDC] Overwatch Gameplay Architecture and Netcode
* [Blizzard Tech Blog] Overwatch 2 Engine Evolution
* [Study] 반효경
