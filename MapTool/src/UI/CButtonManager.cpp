#include "pch.h"
#include "CButtonManager.h"

CButtonManager* CButtonManager::m_pInstance = NULL;

CButtonManager::CButtonManager()
{
}

CButtonManager::~CButtonManager()
{
}

CButtonManager* CButtonManager::GetInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new CButtonManager;
	}

	return m_pInstance;
}


void CButtonManager::CleanUp()
{
	m_SelectedList.clear();

	std::vector<CMyButton*>::iterator it;
	for (it = m_ButtonList.begin(); it != m_ButtonList.end();)
	{
		CMyButton* pBtn = *it;
		if (pBtn != NULL)
		{
			pBtn->Delete();
			pBtn = NULL;
			it = m_ButtonList.erase(it);
		}
	}
}

void CButtonManager::Update()
{
	std::vector<CMyButton*>::iterator it;
	for (it = m_ButtonList.begin(); it != m_ButtonList.end(); it++)
	{
		CMyButton* pBtn = *it;
		if (pBtn != NULL) pBtn->Update();

		/*
		eBUTTON_TYPE type = pBtn->GetButtonType();
		if (type == eBUTTON_TOGGLE)
		{
			CButton_Toggle* pToggle =  (CButton_Toggle*)pBtn;
			pToggle->Update();
		}*/
		
	}

}

void CButtonManager::Calculate()
{
	std::vector<CMyButton*>::iterator it;
	for (it = m_ButtonList.begin(); it != m_ButtonList.end();)
	{
		CMyButton* pBtn = *it;
		eBUTTON_TYPE type = pBtn->GetButtonType();
		if (type == eBUTTON_TOGGLE)
		{
			CButton_Toggle* pToggle = (CButton_Toggle*)pBtn;
			pToggle->CalculateByRatio();
		}
		it++;
	}
}

void CButtonManager::SetWidthHeight(int width, int height)
{
	m_iScreenWidth = width;
	m_iScreenHeight = height;
}

int CButtonManager::GetWidth()
{
	return m_iScreenWidth;
}

int CButtonManager::GetHeight()
{
	return m_iScreenHeight;
}

void CButtonManager::GetRatio(float &outRatioWidth, float &outRatioHeight, float width, float height)
{
	outRatioWidth	= width  / m_iScreenWidth;
	outRatioHeight	= height / m_iScreenHeight;
}

void CButtonManager::AddButton(CMyButton* pBtn)
{
	if(pBtn != NULL)
		m_ButtonList.push_back(pBtn);
}

void CButtonManager::DeleteButton(CMyButton* pBtn)
{
	if (pBtn != NULL)
	{
		std::vector<CMyButton*>::iterator it;
		for (it = m_ButtonList.begin(); it != m_ButtonList.end(); it++)
		{
			CMyButton* pButton = *it;
			if (pBtn == pButton)			//찾던 버튼일 경우
			{
				m_ButtonList.erase(it);		//리스트에서 제거한다.
				delete pButton;				//할당한 메모리 제거
				break;
			}
		}
	}
}


size_t CButtonManager::GetSize()
{
	return m_ButtonList.size();
}

IMAGE* CButtonManager::GetLastSelectedImg()
{
	IMAGE* pImg = NULL;
	static int i = 0;
	int size = m_SelectedList.size();
	if ( size != 0)
	{
		int index = i % size;
		CMyButton* pBtn = m_SelectedList.at(i);
		eBUTTON_TYPE type = pBtn->GetButtonType();
		if (type == eBUTTON_TOGGLE)
		{
			CButton_Toggle* pToggle = (CButton_Toggle*)pBtn;
			pImg = pToggle->GetImg(eEVENT_DEFAULT);				//수정 해야하나? 버튼을 받도록?
		}
		
	}
	else if (size == 0)
	{
		i  = 0;
	}

	return pImg;
}

std::vector<CMyButton*>* CButtonManager::GetSelectedList()
{
	return &m_SelectedList;
}

void CButtonManager::DeleteImg(IMAGE* pImg)
{
	//버튼중에 들어 잇는 이미지를 삭제.
	int aa = 0;
	assert(false);
}

CMyButton* CButtonManager::GetMousePos(int x, int y)
{
	CMyButton* pResult = NULL;
	m_SelectedList.clear();		//기존에 선택된 리스트를 지운다.

	std::vector<CMyButton*>::iterator it;
	for (it = m_ButtonList.begin(); it != m_ButtonList.end(); it++)
	{
		CMyButton* pBtn = *it;
		eBUTTON_TYPE type = pBtn->GetButtonType();
		IMAGE* pImg = pBtn->Picking(x, y);
		if (pImg != NULL)
		{
			pResult = pBtn;
			m_SelectedList.push_back(pBtn);
		}

		//if (type == eBUTTON_TOGGLE)
		//{
		//	CButton_Toggle* pToggle = (CButton_Toggle*)pBtn;
		//	IMAGE* pImg = pToggle->Picking(x, y);
		//	
		//	if (pImg != NULL)
		//	{
		//		pResult = pBtn;
		//		m_SelectedList.push_back(pBtn);
		//	}
		//}
	}

	return pResult;
}

CMyButton* CButtonManager::CreateButton(eBUTTON_TYPE type, IMAGE* pImg)
{
	if (type > eBUTTON_MAX) return NULL;

	CMyButton* pResult = NULL;
	if (type == eBUTTON_NORMAL)
	{
		pResult = new CMyButton(type);
		AddButton(pResult);
		pResult->SetImg(eEVENT_DEFAULT, pImg);

	}
	else if (type == eBUTTON_TOGGLE)
	{
		CButton_Toggle* pBtn = new CButton_Toggle(type);
		
		pResult = (CMyButton*)pBtn;
		AddButton(pResult);
		pBtn->SetImg(eEVENT_DEFAULT, pImg);
	}
	else if (type == eBUTTON_SLIDE)
	{
		CButton_Slide* pBtn = new CButton_Slide(type);

		pResult = (CMyButton*)pBtn;
		AddButton(pResult);
		pBtn->SetImg(eEVENT_DEFAULT, pImg);
	}


	if (pResult != NULL)
	{
		pResult->SetWindowSize(m_iScreenWidth, m_iScreenHeight);
	}
	
	return pResult;
}

CMyButton* CButtonManager::CheckClick(int x, int y)
{
	m_SelectedList.clear();
	bool bCheck = false;
	std::vector<CMyButton*>::iterator it;
	for (it = m_ButtonList.begin(); it != m_ButtonList.end(); it++)
	{
		CMyButton* pButton = *it;
		
		eACTIVE_STATE act = pButton->GetActive();
		if (act == eINACTIVE) continue;

		eBUTTON_TYPE type = pButton->GetButtonType();
		bCheck = pButton->Picking(x, y);
		if (bCheck)
		{
			m_SelectedList.push_back(pButton);	//범위내 충돌한 버튼들을 리스트에 집어 넣는다.
		}

	}

	if (m_SelectedList.size() == 0)
		return NULL;
	else
		return m_SelectedList.at(0);
}

void CButtonManager::ToggleSelected()
{
	std::vector<CMyButton*>::iterator it;
	for (it = m_SelectedList.begin(); it != m_SelectedList.end(); it++)
	{
		CMyButton* pBtn = *it;
		eBUTTON_TYPE type = pBtn->GetButtonType();
		if (type == eBUTTON_TOGGLE)
		{
			CButton_Toggle* pToggle = (CButton_Toggle*)pBtn;
			OutputDebugString(L"[CButtonManager::ToggleSelected]Plese Implement this Part\n");
			//pToggle->Toggle();
		}
	}

}

void CButtonManager::OnEvent(eBUTTON_EVENT btnEvent, int x, int y)
{
	std::vector<CMyButton*>::iterator it;
	for (it = m_ButtonList.begin(); it != m_ButtonList.end(); it++)
	{
		CMyButton* pBtn = *it;
		if (pBtn != NULL)
		{
			pBtn->OnEvent(btnEvent, x, y);
		}
	}
}

