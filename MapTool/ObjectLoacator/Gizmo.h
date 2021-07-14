#pragma once
#include "EngineInterface.h"        //Engin header

//����� ��Ʈ ���� + �ν��Ͻ� �ڵ� + position
struct Gizmo
{
    enum AXIS
    {
        AXIS_X = 0,
        AXIS_Y,
        AXIS_Z,
        AXIS_MAX,
    };

    HEngine_DX12_3D* _pEngine;			//�׷��� ����
    Camera* _pCamera;
    
    DirectX::XMFLOAT3 _pos;             //����� �߽�.
    DirectX::XMFLOAT3 _exPos;           //���� �߽�
    float _gizmoSize            = 0.1;  //����� ȭ��ǥ? ũ��
    float _cubeSize             = 0.02; //ť�� ������
    float _calculatedCubeSize   = 0.f;  

    int _screenWidth;                   //��ũ�� ����
    int _screenHeight;                  //��ũ�� ����

    BOOL _bIsCubeTime = FALSE;         //ť�긦 ������ ���
    BOOL _bSelectAxis[AXIS_MAX];        //���õ� ���� �ִ��� ����.

    HProceduralGeometry_line* _pLine[AXIS_MAX] = {};    //����� X, Y, Z����
    HProceduralGeometry_rect* _pRect[AXIS_MAX] = {};    //����� ���κ� �簢��
    HSimplePrimitive*         _pCube           = NULL;  //����� ��� ť�� ( 

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


