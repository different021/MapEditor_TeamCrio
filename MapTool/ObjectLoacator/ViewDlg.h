#pragma once
#include <afxdialogex.h>
#include <DirectXMath.h>

#include "../src/Defines.h"
#include "ToolDefine.h"
#include "Camera.h"

#include "EngineInterface.h"        //Engin header
#include "ModelManager.h"
#include "MaterialManager.h"
#include "DrawInsManager.h"
#include "CColliderManager.h"
#include "CWaveManager.h"

#include "Gizmo.h"
#include "CamController.h"
#include "../src/utility/CTextReader.h"

#pragma comment(lib,"HEngine.lib")  //Engin Lib

class CGrid;
class CLightManager;
class WAVE;
enum OBJ_EDIT_MODE
{
    EDIT_POS = 0,
    EDIT_ROT,
    EDIT_SCALE,
    EDIT_MAX,
};

class Viewer :
    public CDialogEx
{
private:
    HEngine_DX12_3D* m_pEngine;				    //�׷��� ����
    DirectX::BoundingFrustum    m_camFrustum;

    //���� ����
    int  m_iScreenWidth  = 0;
    int  m_iScreenHeight = 0;
    
    int  m_iGridWidth    = 0;
    int  m_iGridHeight   = 0;
    float m_fOffset      = 10.f;
    float m_fStac        = 0.f;

    Gizmo m_Gizmo;
    
    std::vector<HProceduralGeometry_line*> m_hDebugLineList;      //�׸��� �׸���. ��� �� ���� �̴´�.
   
    CPoint m_DragPoint[2] = {};
    CPoint m_RBtnDownPos = { 0, 0 };
    
    bool   m_bOffsetMode    = false;
    bool   m_bLbuttonDown   = false;
    BOOL   m_bGizmoTime     = FALSE;
    BOOL   m_bGizmoCubeTime = FALSE;
    bool   m_isCtrlDown     = false;

    float m_fCamVelo = 5.0f;
    float m_MouseSensitivty = 0.1f;
    OBJ_EDIT_MODE m_EditMode = EDIT_POS;

    DrawInsManager*  m_pInsManager = NULL;  //�̵� ����
    MaterialManager* m_pMatManager;         //�̵� ����
    ModelManager*    m_pModelManager;       //�̵� ����
    CColliderManager* m_pColManager;        //
    CLightManager*   m_pLightManager;
    CWaveManager*    m_pWaveManager;

    CamController* m_pCamController;        //ī�޶� ���ۿ�.

public:
    Viewer(CWnd* pParent = nullptr);
    ~Viewer();
    void CleanUp();
    void Init(int iWidth, int iHeight);


    void Update();
    void Draw();
    void DrawEditMode(int x, int y);
    void DrawObjectSelectMode(int x, int y);
    void DrawColliderSelectMode(int x, int y);
    void DrawLightSelectMode(int x, int y);
    void SendModelList(HWND hWnd);
    void SendMatList(HWND hWnd);
    
    BOOL InitGraphicEngin();
    void InitManagers();                

    //Move
    void MoveSelectedOffset(CPoint& curPoint, CPoint& lastPoint);
    void MoveSelected(CPoint& cur, CPoint& last);
    DirectX::XMFLOAT3 CalculateWorldMovementByMouseMove(int curX, int curY, int lastX, int lastY, DirectX::XMFLOAT3& vector);
    void MoveSelected(DirectX::XMFLOAT3& vector);

    //Scale
    void ReScaleSelectedXYZ(CPoint& cur, CPoint& last);
    void ReScaleSelected(CPoint& cur, CPoint& last);
    int  CalculateScaleByDistance(int curY, int lastY);
    DirectX::XMFLOAT3 CalculatScaleByMouseMove(int curX, int curY, int lastX, int lastY, DirectX::XMFLOAT3& vector);
    DirectX::XMFLOAT3 CalculatScaleByMouseMoveVertical(int curY, int lastY, DirectX::XMFLOAT3& vector);
    void ReScaleAllAxis(int dist);
    void ReScaleSelected(DirectX::XMFLOAT3& vScale);

    //Rotation
    void RotationSelected(CPoint& const mouseDownPoint, CPoint& const curPoint);
    DirectX::XMFLOAT4 CalculateRotationByMouseMovement(int curX, int curY, int lastX, int lastY);
    void RotationSelected(DirectX::XMFLOAT4& rot);

    //offset
    void ToggleOffsetMode();        
    
    //Grid
    void SetGridInfo(int iWidth, int iHeight, float offset);
    void DrawGrid(int iWidth, int iHeight, float offset);      //�׸��� �׸���
    void DeleteGrid();
    void ChangeGrid(int iWidth, int iHeight, float offset);


    //Cam
    void SetCam(DirectX::XMFLOAT3& target, DirectX::XMFLOAT3& vFront, DirectX::XMFLOAT3 vUp);
    void MoveCam(eDirection dir);
    void ControlCamVelo(float delta);
    void SetMainView(DirectX::XMFLOAT3 pos);
    void RollCam(DirectX::XMFLOAT3& vFront, DirectX::XMFLOAT3 vUp);

private:
    //Init Resource
    BOOL CreaetModelByText(wchar_t* szFileName = NULL);
    BOOL CreateMaterialByText(wchar_t* szFileName = NULL);
    BOOL CreateModelByFolder();
    BOOL CreateMaterialByFolder();
    //BOOL CreateMaterialByFolder();

    wchar_t* MakeFullPath(CString& dest, CString& path, CString& name, CString& tex ,CString& extend);
    HInstanceData* AddGraphicInstance(object* pObj);


    void RequestCreateObj(object* pObj);
    void ControlGizumo();
    void GetObjInRect();

    float GetNDC_X(CPoint& inPoint);
    float GetNDC_Y(CPoint& inPoint);
    void GetKeyUp(WPARAM wParam);

    void UpdateObjectEditBox();
    void UpdateColliderEditBox();
    void UpdateWaveEditBox();

    void SelectGraphicInstance(unsigned int x, unsigned int y);
    void SelectColliderInstance(CPoint& clickPoint);
    void SelectLightInstance(CPoint& clickPoint);
    void SelectWaveInstance(CPoint& clickPoint);

public:
    DECLARE_MESSAGE_MAP()

    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);          //���콺
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnPaint();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual BOOL OnInitDialog();
    afx_msg void OnClose();
};

extern HWND g_hWndViewer;
