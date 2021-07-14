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

    HEngine_DX12_3D* _pEngine;			//그래픽 엔진
    Camera* _pCamera;
    
    DirectX::XMFLOAT3 _pos;             //기즈모 중심.
    DirectX::XMFLOAT3 _exPos;           //이전 중심
    float _gizmoSize            = 0.1;  //기즈모 화살표? 크기
    float _cubeSize             = 0.02; //큐브 사이즈
    float _calculatedCubeSize   = 0.f;  

    int _screenWidth;                   //스크린 가로
    int _screenHeight;                  //스크린 세로

    BOOL _bIsCubeTime = FALSE;         //큐브를 선택한 경우
    BOOL _bSelectAxis[AXIS_MAX];        //선택된 축이 있는지 여부.

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
    ~Gizmo();
    void GetPos(DirectX::XMFLOAT3& dest);
    void Setup(HEngine_DX12_3D* pEngine);
    void Delete();
    void SetPos(DirectX::XMFLOAT3& pos);
    void SetArrow(DirectX::XMFLOAT3& pos);
    
    void Reset();
    void SetSize(float size);
    BOOL CheckSelectedIndex(unsigned int screenX, unsigned int screenY, int screenWidth, int screenHeight);
    BOOL CheckAxis(AXIS axis);
    BOOL IsPickCube( unsigned int screenX, unsigned int screenY );
    void ReleaseAxis();
    void ReleaseCube();
    HProceduralGeometry_rect* GetDebugRect(AXIS axis);
};


