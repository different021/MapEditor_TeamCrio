#pragma once
#include <afxdialogex.h>
#include <DirectXMath.h>
#include "CEditorDlg.h"

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

class Viewer :
    public CEditorDlg
{
public:
    enum OBJ_EDIT_MODE
    {
        EDIT_POS = 0,
        EDIT_ROT,
        EDIT_SCALE,
        EDIT_MAX,
    };

private:
    HEngine_DX12_3D* m_pEngine = nullptr;				    //그래픽 엔진    -> Interface로 변경...
    DirectX::BoundingFrustum    m_camFrustum;   //

    //삭제 예정? -> 왜인지 기억 안남?> 필요할 듯? -> 화면 정보에대한 클래스 필요? 
    int  m_iScreenWidth  = 0;
    int  m_iScreenHeight = 0;
    
    Gizmo m_Gizmo;      //엔진 독립적 재설계 필요

    //그리드로 묶을 것인가?      -> class 구성, 엔진 독립적 재설계
    int  m_iGridWidth    = 0;
    int  m_iGridHeight   = 0;
    float m_fOffset      = 10.f;
    float m_fStac        = 0.f;
    std::vector<HProceduralGeometry_line*> m_hDebugLineList;      //그리드 그리기. 경우 에 따라 뽑는다.
   
    CPoint m_DragPoint[2] = {};                 //마우스 관련 묶을 수 있을 듯.
    CPoint m_RBtnDownPos = { 0, 0 };            //마우스

    bool   m_bLbuttonDown   = false;            //마우스
    bool   m_isCtrlDown     = false;            //키보드
    
    BOOL   m_bGizmoTime     = FALSE;
    BOOL   m_bGizmoCubeTime = FALSE;

    //SelectMode(object, collider, Light, wave(아직))
    bool   m_bObjectSelectMode   = true;  //오브젝트 선택 가능? -> 콜라이더만 선택할 필요가 있을 경우가 있다.
    bool   m_bColliderSelectMode = true;
    bool   m_bLightSelectMode    = true;

    float m_fCamVelo = 5.0f;
    float m_MouseSensitivty = 0.1f;
    OBJ_EDIT_MODE m_EditMode = EDIT_POS;

    CamController* m_pCamController;        //카메라 조작용.

public:
    Viewer(CWnd* pParent = nullptr);
    
    //virtual field
    virtual ~Viewer();
    virtual void Initialize(CWnd* pParent, UINT id, int iWidth, int iHeight);
    virtual void CleanUp();

    virtual void Draw();   

    //SelectMode (object, Collider, Light, wave(미구현) )  -> 엔진 종속적.
    virtual void DrawEditMode(int x, int y);
    virtual void DrawObjectSelectMode(int x, int y);
    virtual void DrawColliderSelectMode(int x, int y);
    virtual void DrawLightSelectMode(int x, int y);

    virtual int GetWidth();
    virtual int GetHeight();

    void SetGraphicEngine(HEngine_DX12_3D* pEngine);
    void SetObjectSelectMode(bool bSelect);
    void SetColliderSelectMode(bool bSelect);
    void SetLightSelectMode(bool bSelect);

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

    //Rotation
    void RotationSelected(CPoint& const mouseDownPoint, CPoint& const curPoint);
    DirectX::XMFLOAT4 CalculateRotationByMouseMovement(int curX, int curY, int lastX, int lastY);

    //Grid
    void SetGridInfo(int iWidth, int iHeight, float offset);
    void DrawGrid(int iWidth, int iHeight, float offset);      //그리드 그리기
    void DeleteGrid();
    void ChangeGrid(int iWidth, int iHeight, float offset);


    //Cam
    void SetCam(DirectX::XMFLOAT3& target, DirectX::XMFLOAT3& vFront, DirectX::XMFLOAT3 vUp);
    void MoveCam(eDirection dir);
    void ControlCamVelo(float delta);
    void SetMainView(DirectX::XMFLOAT3 pos);
    void RollCam(DirectX::XMFLOAT3& vFront, DirectX::XMFLOAT3 vUp);

    //Edit Mode
    void SetEditMode(OBJ_EDIT_MODE mode);

private:
    wchar_t* MakeFullPath(CString& dest, CString& path, CString& name, CString& tex ,CString& extend);

    void ControlGizumo();
    void WhenDragMouse();

    float GetNDC_X(CPoint& inPoint);
    float GetNDC_Y(CPoint& inPoint);
    void GetKeyUp(WPARAM wParam);

public:
    DECLARE_MESSAGE_MAP()

    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);          //마우스
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

