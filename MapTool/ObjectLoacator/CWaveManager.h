#pragma once
#include <vector>
#include "Defines.h"

class HWaveData;
class HEngine_DX12_3D;


struct WAVE
{
	waveData  info;
	HWaveData* pGraphicInstance;	//그래픽 인스턴스
	
	WAVE();
	~WAVE();

	void Update();
	void AddPos(float dx, float dy, float dz);
	void Rotation(DirectX::XMFLOAT4& quaternion);
};

class CWaveManager
{
private:
	static int			m_InstanceCounter;
	static CWaveManager* m_pInstance;
	HEngine_DX12_3D*	m_pEngine;
	std::vector<WAVE*>	m_WaveList;
	std::vector<WAVE*>  m_SelectedList;
	std::vector<WAVE*>	m_DeleteList;
	std::vector<WAVE*>	m_ReCreateList;		//그래픽 인스턴스를 재생성 한다. info기준.

private:
	CWaveManager();
	~CWaveManager();

	//검사.
	bool IsExistInList(WAVE* pWave, const std::vector<WAVE*>& list);
	HWaveData* CreateGraphicInstance(waveData* pData);

public:
	// 생성/삭제
	static CWaveManager* GetInstance();
	void Release();
	void CleanupWaves();
	void Initialize();

	void Update();

	//Wave
	WAVE* CreateWave(waveData* pInfo);

	bool DeleteWaveInDeleteList();			//디릴트 리스트에 잇는 웨이브 삭제.
	bool DeleteWave(WAVE* pWave);			//파라미터로 받은 웨이브 삭제
	bool AddDeleteList(WAVE* pWave);
	void AddWave(WAVE* pDest);
	void EditWave(WAVE* pDest, waveData* pSrc);
	
	//Selected
	int AddSelected(WAVE* pWave);		//선택된적 있을때는 제외.
	int AddSelectedOnlyOne(WAVE* pWave);
	//int DeleteSelected(WAVE* pWave);	//선택된 리스트에서 제외.
	void DeleteSelectedAll();
	WAVE* GetLastSelected();

	DirectX::XMFLOAT3 GetSelectedCenterPos();
	int GetNumberOfSelected();
	
	//Edit
	void MoveSelected(float x, float y, float z);
	void RotateSelected(DirectX::XMFLOAT4& quaternion);
	void ReScaleSelectedByRatio(float ratio);
	void SetSelectedPrvRot();

	//ReCreate
	void AddReCreateList(WAVE* pWave);		//인스턴스를 삭제하는 일. -> 삭제 시점을 그리기 이전으로 하기 위한 작업.
	void ReCreate();						//리스트 내의 인스턴스 모두 재생성.
	WAVE* ReCreateWave(WAVE* pWave);		//그래픽 인스턴스를 재생성 할 필요가 잇을 경우.

	//List
	const std::vector<WAVE*>* GetList();
	const std::vector<WAVE*>* GetSelectedList();

};

