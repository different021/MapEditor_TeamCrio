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

    HEngine_DX12_3D* _pEngine;			//�׷��� ���� (���� ������)
    Camera* _pCamera;                   //ī�޶� Ŭ���� (���� ������)
    
    DirectX::XMFLOAT3 _pos;             //����� �߽�.
    DirectX::XMFLOAT3 _exPos;           //���� �߽�
    float _gizmoSize            = 0.1;  //3���� ���� ������
    float _cubeSize             = 0.02; //ť�� ������
    float _calculatedCubeSize   = 0.f;  //���� ť�� ������

    int _screenWidth;                   //��ũ�� ����
    int _screenHeight;                  //��ũ�� ����

    BOOL _bIsCubeTime = FALSE;         //ť�긦 ������ ���
    BOOL _bSelectAxis[AXIS_MAX];        //���õ� ���� �ִ��� ����.

    //����� ǥ���� ���� ����, �簢��, ť�� (���� ������)  
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
    Gizmo();
    ~Gizmo();

    void Update();                                  //virtual ��ȯ -> ���� ������
    void Setup(HEngine_DX12_3D* pEngine);           //�ʱ�ȭ ���� �ν��Ͻ� (���� ������)
    void Delete();                                  //����� �ν��Ͻ� ����

    void GetPos(DirectX::XMFLOAT3& out);            //�Ķ���Ϳ� ����� ��ǥ ����
    void SetPos(DirectX::XMFLOAT3& pos);            //set CPU position data
    void SetArrow(DirectX::XMFLOAT3& pos);          //private �̵� ���
    
    void SetSize(float size);
    BOOL CheckSelectedIndex(unsigned int screenX, unsigned int screenY, int screenWidth, int screenHeight);
    BOOL CheckAxis(AXIS axis);
    BOOL IsPickCube( unsigned int screenX, unsigned int screenY );
    void ReleaseAxis();                                 //�� ���� ���� -> �̸� �ٽ� ���� ��. ���콺 ��Ʈ�Ѱ� ����.
    void ReleaseCube();                                 //ť�� ���� ���� -> �̸� �ٽ� ���� ��.
    HProceduralGeometry_rect* GetDebugRect(AXIS axis);
};


