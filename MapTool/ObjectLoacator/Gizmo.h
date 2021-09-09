#pragma once
#include "EngineInterface.h"        //Engin header

//디버그 렉트 정보 + 인스턴스 핸들 + position
struct Gizmo
{
    enum AXIS
    {
        AXIS_X = 0,
        AXIS_Y,
        AXIS_Z,
        AXIS_MAX,
    };

    HEngine_DX12_3D* _pEngine;			//그래픽 엔진 (엔진 종속적)
    Camera* _pCamera;                   //카메라 클래스 (엔진 종속적)
    
    DirectX::XMFLOAT3 _pos;             //기즈모 중심.
    DirectX::XMFLOAT3 _exPos;           //이전 중심
    float _gizmoSize            = 0.1;  //3방향 직선 사이즈
    float _cubeSize             = 0.02; //큐브 사이즈
    float _calculatedCubeSize   = 0.f;  //계산된 큐브 사이즈

    int _screenWidth;                   //스크린 가로
    int _screenHeight;                  //스크린 세로

    BOOL _bIsCubeTime = FALSE;         //큐브를 선택한 경우
    BOOL _bSelectAxis[AXIS_MAX];        //선택된 축이 있는지 여부.

    //기즈모 표현을 위한 직선, 사각형, 큐브 (엔진 종속적)  
    HProceduralGeometry_line* _pLine[AXIS_MAX] = {};    //기즈모 X, Y, Z라인
    HProceduralGeometry_rect* _pRect[AXIS_MAX] = {};    //기즈모 끝부분 사각형
    HSimplePrimitive*         _pCube           = NULL;  //기즈모 가운데 큐브 ( 

private:
    BOOL IsSelectAxis(AXIS axis, float ndcX, float ndcY);
    BOOL AABB(float xMin, float xMax, float yMin, float yMax, float x, float y);
    float GetNdcX(int x);
    float GetNdcY(int y);

    //Cube
    void  CreateCube(Vector3 size);
    void  SetCubePos(DirectX::XMFLOAT3& pos);

public:
    Gizmo();
    ~Gizmo();

    void Update();                                  //virtual 전환 -> 엔진 독립적
    void Setup(HEngine_DX12_3D* pEngine);           //초기화 엔진 인스턴스 (엔진 종속적)
    void Delete();                                  //기즈모 인스턴스 삭제

    void GetPos(DirectX::XMFLOAT3& out);            //파라미터에 기즈모 좌표 리턴
    void SetPos(DirectX::XMFLOAT3& pos);            //set CPU position data
    void SetArrow(DirectX::XMFLOAT3& pos);          //private 이동 고려
    
    void SetSize(float size);
    BOOL CheckSelectedIndex(unsigned int screenX, unsigned int screenY, int screenWidth, int screenHeight);
    BOOL CheckAxis(AXIS axis);
    BOOL IsPickCube( unsigned int screenX, unsigned int screenY );
    void ReleaseAxis();                                 //축 선택 해제 -> 이름 다시 정할 것. 마우스 컨트롤과 관련.
    void ReleaseCube();                                 //큐브 선택 해제 -> 이름 다시 정할 것.
    HProceduralGeometry_rect* GetDebugRect(AXIS axis);
};


