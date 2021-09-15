#pragma once
#include "ViewDlg.h"
#include "Center.h"
#include "CColliderManager.h"

class Light;
class lightData;
class WAVE;
//���̺� ���� Ŭ������ ����.
class CSaver
{
public:
	static CSaver*	m_pInstance;
	STAGE_HEADER	m_StageHeader;
	//int		m_cntWave;

	int m_ObjectCnt	= 0;
	int m_ColliderCnt = 0;
	int m_LightCnt	= 0;
	int m_WaveCnt	= 0;

	object*			m_pObjList		 = NULL;
	collider*		m_pColliderList  = NULL;
	lightData*		m_pLightDataList = NULL;
	waveData*		m_pWaveList		 = NULL;

private:
	CSaver();

public:
	~CSaver();
	static CSaver* GetInstance();
	void CleanUp();
	void DeleteInstance();

	void Save_WINAPI(CStringW& fileFullPath, CStringW& fileName);
	void Load_WINAPI(CStringW& fileFullPath, CStringW& fileName);
	DWORD WriteHeader(HANDLE hFile, wchar_t* pFileName);
	DWORD SaveObject(HANDLE hFile);
	DWORD SaveCollider(HANDLE hFile);
	DWORD SaveLight(HANDLE hFile);
	DWORD SaveWave(HANDLE hFile);

	BOOL ReadHeader(HANDLE hFile, STAGE_HEADER& out);
	BOOL ReadObject(HANDLE hFile, STAGE_HEADER& header, object* &pOut);
	BOOL ReadCollider(HANDLE hFile, STAGE_HEADER& header, collider* &pOut);
	BOOL ReadLight(HANDLE hFile, STAGE_HEADER& header, lightData* &pOUt);
	BOOL ReadWave(HANDLE hFile, STAGE_HEADER& header, waveData*& pOut);

	void SetHeader(STAGE_HEADER& header);

	//return number of instance copied
	//pOut�� ������ �޸� �Ҵ�� �迭�� �Ѿ�� ���� �ѱ��.
	int	MakeObjectArrayFromVector(const std::vector<DRAW_INSTANCE*>* pDrawInsList);		
	int MakeColliderArrayFromVector(const std::vector<COLLIDER>* pColliderList);		
	int MakeLightArrayFromVector(const std::vector<Light*>* pLightList);
	int MakeWaveArrayFromVector(const std::vector<WAVE*>* pWaveList);

public:
	void Save();
	int  Load(int* iOut);
};

