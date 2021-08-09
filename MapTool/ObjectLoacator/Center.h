#pragma once
#include "CObjectDlg.h"
#include "ViewDlg.h"
#include "CGrid.h"
//#include "CCharSetting.h"
#include "CCmd.h"
#include "CColliderDlg.h"
#include "CHelpDlg.h"
#include "CLightDlg.h"

#include "EngineInterface.h"

class CWaveDlg;

class CSaver;
class DrawInsManager;
class CColliderManager;
class CLightManager;
class CWaveManager;

struct MODEL;
struct MATERIAL;

struct waveData;
struct WAVE;

void RequestCenter(UINT msg, WPARAM from, LPARAM para);

// MainDlg 대화 상자
enum eDlgs
{
	eObjDlg = 0,
	eColliderDlg,
	eLightDlg,
	eWaveDlg,
	eViewer,
	eGridDlg,
	eCmdDlg,
	eCharacterDlg,
	eDlgs_MAX,
};

//이름 변경 Center -> 클래스 추가 데이터 Manager
//SendMessage로 통신하는 체계 구축.
//message를 확인/유효성 검사 후 필요한 조치
class Center : public CDialogEx
{
	DECLARE_DYNAMIC(Center)
	
	HEngine_DX12_3D* m_pEngine;		//그래픽 엔진
	STAGE_HEADER m_MapHeader;			//헤터

	//Dialogs
	CObjectDlg		m_ObjectDlg;		//오브젝트
	CColliderDlg	m_ColliderDlg;	//콜라이더
	Viewer*		 m_Viewer;					//뷰어
	CGrid		 m_GridController;			//그리드 조절
	CHelpDlg	 m_HelpDlg;					//도움말
	CLightDlg	 m_pLightDlg;				//라이트 
	CWaveDlg*	 m_pWaveDlg;
	//CCmd		 m_Cmd;						//MFC
	
	CMenu m_MainMenu;
	bool m_bIsDockingViewr = true;			//삭제 대상.
	
	CSaver* m_pSaver;			//SAVE/Load

	//resource
	MaterialManager* m_pMatManager;			//메테리얼
	ModelManager*	 m_pModelManager;		//모델

	//instance
	DrawInsManager*		m_pDrawInsManager;	//오브젝트 관리
	CColliderManager*	m_pColliderManager;	//콜라이더 매니저
	CLightManager*		m_pLightManager;	//빛 매니저
	CWaveManager*		m_pWaveManager;		//물.

public:
	Center(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~Center();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MainDlg };
#endif
private:
	//Dialog상자 초기화
	void InitializeDlgs(CWnd* pParent);

	void Init_MainMenu();		//상단 메뉴
	
	//삭제.
	void CleanUp();
	void CleanupDlg();
	void CleanupManager();

public:					
	void	Update();
	void	UpdateMaterial(DRAW_INSTANCE* pDrawIns);		//인스턴스의 마테리얼 교체
	void	UpdateMaterial(object* pObj);					//인스턴스의 마테리얼 교체(object로 찾는다)
	void	UpdateModel(DRAW_INSTANCE* pDrawIns);			//인스턴스의 모델 교체(drawIns)
	void	UpdateModel(object* pObj);						//인스턴스의 모델 교체(object)
	
	void	UpdateWaveList();								//그래픽인스턴스 데이터 최신화.

	//인스턴스 삭제
	void DeleteInDeleteList();

	//오브젝트 생성/삭제/수정	//Object 관련	
	object* GetLastSelectedObject();						//선택된 오브젝트 중 마지막
	void	DeleteAllDrawInsList();							//오브젝트 전부 삭제.
	bool	CreateObj(object* pObj);						//오브젝트 생성.
	void	RescaleSelected(float ratio);				//선택한 모두의 스케일 조절

	//그래픽 인스턴스
	void	EidtGraphicInstance(object* pObj);				//그래픽 인스턴스 수정.
	bool	DeleteGraphicInstance(object* pObj);			//그래픽 인스턴스 삭제.

	size_t	GetNumberOfSelectedObj();		
	void	DeleteSelectedObject();						//선택된 오브젝트 삭제. 	
	void	DuplicateObjectInSelectedList();			//선택된 오브젝트 복사
	
	//리젠 콜라이더
	void	UpdateRegenColliderListInObjLocator();

	//모델
	int		SetModelIndexByName(object* pObj);
	MODEL*	GetModelByInde(int index);
	
	//마테리얼
	int		SetMatIndexByName(object* pObj);
	MATERIAL* GetMaterialByIndex(int index);
	void	EditMaterialSelectedALL(int matIndex);

	//파일 로드
	void CreateObjectsFromList(int CountOfObject, object* pList);
	void CreateCollidersFromList(STAGE_HEADER* pHeader, collider* pColliderList);
	void CreateLightDataFromList(STAGE_HEADER* pHeader, lightData* pLightList);
	void CreateWaveFromList(int numOfWave, waveData* pWaveList);
	void UpdateModelList();
	void LoadFbxFile();
	void LoadTextureFiles();

	//저장 관련
	void SaveMap();	
	void LoadMap();
	void LoadFbx();
	void LoadTextureSet();
	void BoardCast(MSG* pMsg);

	//콜라이더
	collider* CreateCollider();					//생성
	void DeleteCollider(collider* pCollider);	//삭제
	void ClearColliderList();					//전체 삭제
	void DeleteSelectedCollider();				//선택된 모든 콜라이더 삭제.
	void DuplicateColliderInSelectedList();		//선택된 모든 콜라이더 복제.

	void ToggleColliderSelect();
	void ToggleObjectSelect();
	void ToggleLightSelect();
	void SelectChangeObject(object* pObj);


	collider* PickingCollider(int screenX, int screenY);

	//에딧 박스 값 수정.
	void UpdateObjEditControl(object* pObj);
	void UpdateColliderEditControl(collider* pCollider);
	void UpdateWaveEditControl(WAVE* pWave);

	//각각의 리스트 박스 업데이트
	void UpdateListBox();
	void UpdateObjList();
	void UpdateColliderList();
	void UpdateLightList();
	void UpdateRegenColliderList();
	void UpdateWaveLlist();

	//라이트.
	void DuplicateLightSelectedList();						
	void DeleteSelectedLight();					
	Light* PickingLight(int screenX, int screenY);
	void UpdateSelectedLight();

	//그리드
	void DrawGrid(int width, int height, int offset);
	void ChangeGrid(int width, int height, int offset);

	
	void UpdateEditBoxByObj(object* pObj);
	void SelectCollider(collider* pCollider);
	void SetGizmo(collider* pCol);
	void SetGizmo(object* pObj);
	
	
public:	//Wave
	WAVE* CreateWave(waveData* pInfo);
	bool  AddDeleteList(WAVE* pWave);
	void  EditWave(WAVE* pDest, waveData* pSrc);
	void  AddSelectedWave(WAVE* pWave);
	void  AddSelectedWaveOnlyOne(WAVE* pWave);
	void  DeleteSelectedListAll();

	//Load에 의한 다시 그리기
	void ReDraw(STAGE_HEADER* pHeader, CSaver* pSaver);



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();


	DECLARE_MESSAGE_MAP()
public:
	

	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnShowWindowObjlocator();
	afx_msg void OnShowWindowViewer();
	afx_msg void OnShowWindowGridcon();
	afx_msg void OnShowWindowCharEidt();
	afx_msg void OnShowWindowCmd();
	afx_msg void OnColliderWindow();

public:
	afx_msg LRESULT OnHideWindow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeGrid(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRequestModelList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRequestMatList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEditObj(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDeleteObj(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateModelList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateMatList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSaveMap(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLoadMap(WPARAM wParam, LPARAM lParam);
	
	afx_msg LRESULT OnListBoxNone(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCreateCollider(WPARAM wParam, LPARAM lParam);
	//afx_msg LRESULT OnDeleteCollider(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEditCollider(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLoadFbx(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPickingCollider(WPARAM wParam, LPARAM lParam);
	
public:
	afx_msg void OnMenuSave();
	afx_msg void OnMenuLoad();
	afx_msg void OnMenuLoadFbx();
	afx_msg void OnMenuExit();
	
	afx_msg void OnLightWindow();
	afx_msg void OnClose();
protected:
	afx_msg LRESULT OnObjectCreate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnViewerLbuttondown(WPARAM wParam, LPARAM lParam);
};

extern HWND g_hCenter;
extern Center* g_pCenter;
