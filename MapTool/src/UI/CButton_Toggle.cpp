#include "pch.h"
#include "CButton_Toggle.h"
#include "UIDesignerDlg.h"

CMyButton::CMyButton(eBUTTON_TYPE type)
{
	m_eActive = eACTIVE;
	m_bVisible = true;
	m_eType = type;
	m_eEventState = eEVENT_DEFAULT;
	memset(m_pImgList, 0, sizeof(IMAGE*) * eEVENT_MAX);
}

void CMyButton::Delete()
{
	for (int i = 0; i < eEVENT_MAX; i++)
	{
		if (m_pImgList[i] != NULL)
		{
			delete m_pImgList[i];
			m_pImgList[i] = NULL;
		}
	}
	delete this;

}

void CMyButton::Update()
{
	for (int i = 0; i < eEVENT_MAX; i++)
	{
		if (m_pImgList[i] != NULL)
		{
			m_pImgList[i]->Update();
		}
	}
}

void CMyButton::MoveByDeltaRatio(float deltaX, float deltaY)
{
	for (int i = 0; i < eEVENT_MAX; i++)
	{
		if (m_pImgList[i] == NULL) continue;
		m_pImgList[i]->MoveByDeltaRatio(deltaX, deltaY);
		m_pImgList[i]->GetPos(false);
		m_pImgList[i]->CalculatePosByRatio();
	}
	
}

void CMyButton::CalculateByRatio()
{
	for (int i = 0; i < eEVENT_MAX; i++)
	{
		if (m_pImgList[i] != NULL)
		{
			m_pImgList[i]->CalculatePosByRatio();
		}
	}
}

void CMyButton::SetType(eBUTTON_TYPE type)
{
	m_eType = type;
}

void CMyButton::SetActive(eACTIVE_STATE act)
{
	if (act > eACTIVE_MAX) return;
	m_eActive = act;
}

void CMyButton::SetVisible(bool bIsVisible)
{
	m_bVisible = bIsVisible;
}

void CMyButton::SetVisible(eBUTTON_EVENT btnEvent, bool bVisible)
{
	IMAGE* pImg = m_pImgList[btnEvent];
	if (pImg != NULL)
	{
		pImg->SetVisible(bVisible);
	}
}

eBUTTON_TYPE CMyButton::GetButtonType()
{
	return m_eType;
}

eACTIVE_STATE CMyButton::GetActive()
{
	return m_eActive;
}



void CMyButton::SetWindowSize(int width, int height)
{
	m_iScreenWidth  = width;
	m_iScreenHeight = height;
}



void CMyButton::SetImg(eBUTTON_EVENT btnEvent, IMAGE* pImg)
{
	if (pImg == NULL) return;
	if (btnEvent > eEVENT_MAX) assert(FALSE);

	if (m_pImgList[btnEvent] != NULL)
	{
		//g_bPaint = false;
		IMAGE* pTemp = m_pImgList[btnEvent];
		m_pImgList[btnEvent] = pImg;

		delete pTemp;
		pTemp = NULL;
		//g_bPaint = true;
	}
	else
	{
		m_pImgList[btnEvent] = pImg;
	}
	
}

IMAGE* CMyButton::GetImg(eBUTTON_EVENT state)
{
	return m_pImgList[state];
}

IMAGE* CMyButton::GetImg()
{
	return m_pImgList[m_eEventState];
}

IMAGE* CMyButton::ShowMeTheImage(eBUTTON_EVENT eEvent)
{
	IMAGE* pImg = m_pImgList[eEvent];

	if ( pImg != NULL )
	{
		for (int i = 0; i < eEVENT_MAX; i++)
		{
			if (m_pImgList[i] != NULL)
			{
				bool bIsVisible = true;
				if ( (eBUTTON_EVENT)i == eEvent )
					bIsVisible = true;
				else
					bIsVisible = false;

				m_pImgList[i]->SetVisible(bIsVisible);
				m_pImgList[i]->Update();
			}
		}

	}

	return pImg;
}

IMAGE* CMyButton::Picking(int x, int y)
{
	bool bPick = false;
	for (int i = 0; i < eEVENT_MAX; i++)
	{
		bPick = Picking(x, y, (eBUTTON_EVENT)i);
		if (bPick == true) return m_pImgList[i];
	}

	return NULL;
}

bool CMyButton::Picking(int x, int y, eBUTTON_EVENT state)
{
	if (state > eEVENT_MAX) assert(false);
	if (m_eActive == eINACTIVE) return false;

	IMAGE* pImg = m_pImgList[state];
	if (pImg == NULL) return false;

	XMFLOAT2 ratioPos = GetRatio(x, y);
	HUIData* pUiData = pImg->GetGraphicData();
	if (pUiData == NULL) return false;

	float min[2] = { FLT_MAX, FLT_MAX };
	float max[2] = { FLT_MIN, FLT_MIN };

	float x1 = pUiData->leftTopPosition.x;
	float x2 = pUiData->leftTopPosition.x + pUiData->width;
	min[0] = (x1 < x2) ? x1 : x2;
	max[0] = (x1 > x2) ? x1 : x2;

	float y1 = pUiData->leftTopPosition.y;
	float y2 = pUiData->leftTopPosition.y + pUiData->height;
	min[1] = (y1 < y2) ? y1 : y2;
	max[1] = (y1 > y2) ? y1 : y2;

	if (min[0] > ratioPos.x) return false;
	if (min[1] > ratioPos.y) return false;
	if (max[0] < ratioPos.x) return false;
	if (max[1] < ratioPos.y) return false;

	return true;
}

void CMyButton::OnEvent(eBUTTON_EVENT btnEvent, int x, int y)
{

	bool bHit = Picking(x, y, m_eEventState);
	//bool bHit = Picking(x, y, eEVENT_DEFAULT);

	if (bHit)
	{
		m_eEventState = btnEvent;
		ShowMeTheImage(m_eEventState);
	}
		
}

void CMyButton::OnMouseMove(int x, int y)
{
	SetCurMousePos(x, y);
}

void CMyButton::ProcessMessage(UINT message, WPARAM wpram, LPARAM lPram)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{

		break;
	}

	}
}




void CMyButton::SetCurMousePos(int x, int y)
{
	if (m_bUse != true) return;
	
	//이전 위치 저장
	m_MousePos[eMOUSE_POS_PREVIOUS].x = m_MousePos[eMOUSE_POS_CUR].x;
	m_MousePos[eMOUSE_POS_PREVIOUS].y = m_MousePos[eMOUSE_POS_CUR].y;

	//현재 위치 저장
	m_MousePos[eMOUSE_POS_CUR].x = x;
	m_MousePos[eMOUSE_POS_CUR].y = y;
}

void CMyButton::SetClickedPos(int x, int y)
{
	m_MousePos[eMOUSE_POS_CLICKED].x = x;
	m_MousePos[eMOUSE_POS_CLICKED].y = y;
}

XMFLOAT2 CMyButton::GetRatio(int x, int y)
{
	XMFLOAT2 fResult = {};
	fResult.x = static_cast<float>(x) / m_iScreenWidth;
	fResult.y = static_cast<float>(y) / m_iScreenHeight;

	return fResult;
}


CButton_Toggle::~CButton_Toggle()
{
}



CButton_Toggle::CButton_Toggle(eBUTTON_TYPE type) : CMyButton(type)
{
}

bool CButton_Toggle::GetValue()
{
	return m_bValue;
}

/*
bool CButton_Toggle::Toggle()
{
	//수정할 것.
	//if (m_eActive != eACTIVE) return false;
	//if (m_bUse == false) return false;

	//m_bValue = !m_bValue;
	//if (m_bValue == true)
	//{
	//	m_pImgList[eEVENT_DEFAULT]->info.opacity = 1.f;
	//	m_pImgList[eEVENT_DEFAULT]->info.opacity = 0.f;
	//}
	//else
	//{
	//	m_pImgList[eEVENT_DEFAULT]->info.opacity = 0.f;
	//	m_pImgList[eEVENT_DEFAULT]->info.opacity = 1.f;
	//}
	//m_pImgList[eEVENT_DEFAULT]->Update();

	//return m_bValue;
	return false;
}
*/

