# AnimProj

# 스켈레톤

## _dfsRoute : DFS경로
루트 본으로부터 리프 본까지의 깊이 우선 탐색 경로(Depth First Search) 를 추출합니다.  
언제나 부모 노드가 자식 노드보다 인덱스가 낮습니다. 또한 루트는 0번 인덱스를 가집니다.  

## _boneData : 본 데이터
본 데이터를 저장합니다.  

## _boneNameList : 본 이름 목록
본 데이터의 이름을 std::vector 를 이용해 따로 저장합니다.  
이름이 저장된 순서가 본 데이터가 저장된 순서와 동일해야 합니다.  

c++ 스탠다드 라이브러리, algorithm 의 std::find 기능을 활용하여
std::vector 를 순회하며 이름을 찾습니다. 해당하는 이름이 저장된 인덱스를 활용해 본 데이터를 조회합니다.  

# 애니메이션
## 문제점
1. AMC 로더와 애니메이션이 완전히 분리되지 않았습니다.
2. ASF 로더와 커플링되어 있습니다. ASF 로더에서 AMC 로더를 friend 로 선언합니다.

## _data



# 랜더링
스켈레톤을 렌더링할 때 인스턴싱을 적용합니다.
글로벌 트랜스폼 배열을 상수버퍼에 맵핑하여 본 매쉬를 동시에 모두 랜더링합니다.

개발 편의성과 유연한 확장을 위하여 버텍스 어트리뷰트 별로 버텍스 버퍼를 여러개 바인딩합니다.
(데이터 인터리빙을 하지 않습니다.)
현재 버텍스 포지션과 칼라 두 어트리뷰트가 구현되어있습니다. 따라서 버택스 버퍼 바인딩 시 2개의 버퍼를 바인딩합니다.


