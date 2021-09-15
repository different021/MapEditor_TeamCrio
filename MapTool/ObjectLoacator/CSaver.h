#pragma once
#include "ViewDlg.h"
#include "Center.h"
#include "CColliderManager.h"

class Light;
class lightData;
class WAVE;
//세이브 전용 클래스로 변경.
class CSaver
{
public:
	static CSaver*	m_pInstance;
	STAGE_HEADER	m_StageHeader;
	int		m_cntWave;

	//삭제 에정 변수 아래 포인터만으로 
	//std::vector<DRAW_INSTANCE*>* m_pDrawInsVector;		
	std::vector<COLLIDER>*		m_pColliderVector;
	std::vector<Light*>*		m_pLightVector;
	std::vector<WAVE*>*			m_pWaveVector;

	size_t m_ObjectCnt	= 0;
	size_t m_ColliderCnt = 0;
	size_t m_LightCnt	= 0;
	size_t m_WaveCnt	= 0;

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
	//삭제 예정 함수들. 아래 함수들로 대체 가능
	//void SetObjList(std::vector<DRAW_INSTANCE*>* drawInsList);
	void SetColliderList(std::vector<COLLIDER>* pColliderList);
	void SetLightList(std::vector<Light*>* pLightList);
	void SetWaveList(const std::vector<WAVE*>* pWaveList);

	//return number of instance copied
	//pOut에 새로이 메모리 할당된 배열이 넘어가니 비우고 넘길것.
	int	MakeObjectArrayFromVector(const std::vector<DRAW_INSTANCE*>* pDrawInsList);		
	int MakeColliderArrayFromVector(collider*& pOut, const std::vector<COLLIDER>* pColliderList);		
	int MakeLightArrayFromVector(lightData*& pOut, const std::vector<Light*>* pLightList);
	int MakeWaveArrayFromVector(waveData*& pOut, const std::vector<WAVE*>* pWaveList);

public:
	void Save();
	int  Load(int* iOut);
};

