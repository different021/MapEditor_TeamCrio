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

	//벡터 제거  
	std::vector<DRAW_INSTANCE*>* m_pDrawInsVector;				//매니저에 있는걸 가져왔다는 의미.....
	std::vector<COLLIDER>*		m_pColliderVector;
	std::vector<Light*>*		m_pLightVector;
	std::vector<WAVE*>*			m_pWaveVector;

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
	void SetObjList(std::vector<DRAW_INSTANCE*>* drawInsList);
	void SetColliderList(std::vector<COLLIDER>* pColliderList);
	void SetLightList(std::vector<Light*>* pLightList);
	void SetWaveList(const std::vector<WAVE*>* pWaveList);

	//return number of instance copied
	int	MakeObjectArrayFromVector(object* &pOut, const std::vector<DRAW_INSTANCE*>* pDrawInsList);		
	int MakeColliderArrayFromVector(collider*& pOut, const std::vector<COLLIDER>* pColliderList);		

public:
	void Save();
	int  Load(int* iOut);
};

