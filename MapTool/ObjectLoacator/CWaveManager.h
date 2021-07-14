#pragma once
#include <vector>
#include "Defines.h"

class HWaveData;
class HEngine_DX12_3D;


struct WAVE
{
	waveData  info;
	HWaveData* pGraphicInstance;	//�׷��� �ν��Ͻ�
	
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
	std::vector<WAVE*>	m_ReCreateList;		//�׷��� �ν��Ͻ��� ����� �Ѵ�. info����.

private:
	CWaveManager();
	~CWaveManager();

	//�˻�.
	bool IsExistInList(WAVE* pWave, const std::vector<WAVE*>& list);
	HWaveData* CreateGraphicInstance(waveData* pData);

public:
	// ����/����
	static CWaveManager* GetInstance();
	void Release();
	void CleanupWaves();
	void Initialize();

	void Update();

	//Wave
	WAVE* CreateWave(waveData* pInfo);

	bool DeleteWaveInDeleteList();			//��Ʈ ����Ʈ�� �մ� ���̺� ����.
	bool DeleteWave(WAVE* pWave);			//�Ķ���ͷ� ���� ���̺� ����
	bool AddDeleteList(WAVE* pWave);
	void AddWave(WAVE* pDest);
	void EditWave(WAVE* pDest, waveData* pSrc);
	
	//Selected
	int AddSelected(WAVE* pWave);		//���õ��� �������� ����.
	int AddSelectedOnlyOne(WAVE* pWave);
	//int DeleteSelected(WAVE* pWave);	//���õ� ����Ʈ���� ����.
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
	void AddReCreateList(WAVE* pWave);		//�ν��Ͻ��� �����ϴ� ��. -> ���� ������ �׸��� �������� �ϱ� ���� �۾�.
	void ReCreate();						//����Ʈ ���� �ν��Ͻ� ��� �����.
	WAVE* ReCreateWave(WAVE* pWave);		//�׷��� �ν��Ͻ��� ����� �� �ʿ䰡 ���� ���.

	//List
	const std::vector<WAVE*>* GetList();
	const std::vector<WAVE*>* GetSelectedList();

};

