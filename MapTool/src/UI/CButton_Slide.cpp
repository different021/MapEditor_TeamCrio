#include "pch.h"
#include "CButton_Slide.h"




void CButton_Slide::AddWidth(float delta)
{
	m_fWidth += delta;
}


CButton_Slide::CButton_Slide(eBUTTON_TYPE type) : CMyButton(type)
{
}

//��ü ü�¹ٿ� ���� �ۼ� ����.
void CButton_Slide::SetCurValue(float fValue)
{
	m_fCurValue = fValue;
	float ratio = GetValueRatio();
	m_fCurWidth = m_fWidth * ratio;

}


void CButton_Slide::Update()
{
	//UpdateWidth();		//-> ����...
	//���θ� ����.
	float width = 0.f;			
	if (m_pImgList[IMG_LEFT])
	{
		m_pImgList[IMG_LEFT]->CalculatePosByRatio();
		width += m_pImgList[IMG_LEFT]->GetWidthRatio();
	}
		

	if (m_pImgList[IMG_CENTER])
	{
		m_pImgList[IMG_CENTER]->CalculatePosByRatio(width, 0.f);
		width += m_pImgList[IMG_CENTER]->GetWidthRatio();
	}
		

	if (m_pImgList[IMG_RIGHT])
	{
		m_pImgList[IMG_RIGHT]->CalculatePosByRatio(width,  0.f);
		width += m_pImgList[IMG_RIGHT]->GetWidthRatio();
	}

	CMyButton::Update();
}

void CButton_Slide::UpdateWidth()
{
	m_pImgList[IMG_CENTER]->SetAbsoluteWidth(m_fCurWidth);
	m_pImgList[IMG_CENTER]->GetPos(true);
}

float CButton_Slide::GetMinValue()
{
	return m_fMinValue;
}

void CButton_Slide::SetMinValue(float min)
{
	m_fMinValue = min;
}

float CButton_Slide::GetMaxValue()
{
	return m_fMaxValue;
	//m_fMaxValue = m_pImgList[IMG_LEFT]->info.RatioPos[eAxisX] - m_pImgList[IMG_RIGHT]->info.RatioPos[eAxisX];
}

void CButton_Slide::SetMaxValue(float max)
{
	m_fMaxValue = max;
}

float CButton_Slide::GetCurValue()
{
	return m_fCurValue;
}

void CButton_Slide::AddCurValue(float delta)
{
	float newValue = m_fCurValue += delta;				//�� ����
	//float ratio = GetValueRatio();	//��ü ũ���� �ۼ�Ʈ���� �� ������ ���Ѵ�
	SetCurWidth(newValue);
	UpdateWidth();
}

float CButton_Slide::GetValueRatio()
{
	return  (m_fCurValue / (m_fMaxValue - m_fMinValue) );
}

void CButton_Slide::SetValueByRatio(float ratio)
{
	float total = m_fMaxValue - m_fMinValue;
	m_fCurValue = total * ratio;
}

void CButton_Slide::CreateCenterImg(UI_INFO* pCenter, HEngine_DX12_3D* pEngine)
{
	m_pImgList[IMG_CENTER] = IMAGE::CreateImg(pCenter, pEngine);
}

void CButton_Slide::CreateLeftImg(UI_INFO* pLeft, HEngine_DX12_3D* pEngine)
{
	m_pImgList[IMG_LEFT] = IMAGE::CreateImg(pLeft, pEngine);
}

void CButton_Slide::CreateRightImg(UI_INFO* pRight, HEngine_DX12_3D* pEngine)
{
	m_pImgList[IMG_RIGHT] = IMAGE::CreateImg(pRight, pEngine);
}

void CButton_Slide::CreateRangeInfo(UI_INFO& start, UI_INFO& end)
{
	if(m_pImgList[INFO_MIN] == NULL)
		m_pImgList[INFO_MIN] = IMAGE::CreateImg(&start);

	if (m_pImgList[INFO_MAX] == NULL)
		m_pImgList[INFO_MAX] = IMAGE::CreateImg(&end);
}

void CButton_Slide::SetLeftImg(IMAGE* pImg)
{
	m_pImgList[IMG_LEFT] = pImg;
}

void CButton_Slide::SetRightImg(IMAGE* pImg)
{
	m_pImgList[IMG_RIGHT] = pImg;
}

void CButton_Slide::SetCenterImg(IMAGE* pImg)
{
	m_pImgList[IMG_CENTER] = pImg;
}

void CButton_Slide::SetFrameImg(IMAGE* pImg)
{
	m_pImgList[IMG_FRAME] = pImg;
}

void CButton_Slide::EditInfoMin(UI_INFO& start)
{
	m_pImgList[INFO_MIN]->SetInfo(&start);
}

void CButton_Slide::EditInfoMax(UI_INFO& end)
{
	m_pImgList[INFO_MAX]->SetInfo(&end);
}

void CButton_Slide::SetCenterWidth(float width)
{
	m_fWidth = width;	//�ִ� ��ġ?
	SetCurWidth(width);	//ó���̶� ���� ���帲.
}

void CButton_Slide::SetCurWidth(float CurWidth)
{
	m_fCurWidth = CurWidth;
}

void CButton_Slide::MoveSlide(int curX, int curY)
{
	//int dx = m_MousePos[eMOUSE_POS_CUR].x - m_MousePos[eMOUSE_POS_PREVIOUS].x;
	//int dy = m_MousePos[eMOUSE_POS_CUR].y - m_MousePos[eMOUSE_POS_PREVIOUS].y;

	//float dx_Ratio = static_cast<float>(dx) / m_iScreenWidth;
	//float dy_Ratio = static_cast<float>(dy) / m_iScreenHeight;

	////������ X,Y�� ������
	//float width = m_pImgList[IMG_CENTER]->GetWidthRatio();		
	//width += dx_Ratio;
	//
	//float height = m_pImgList[IMG_CENTER]->GetHeightRatio();
	//height += dy_Ratio;

	//float maxWidth  = m_pImgList[INFO_MAX]->GetPosRatioX() - m_pImgList[INFO_MIN]->GetPosRatioX();
	//float maxHeight = m_pImgList[INFO_MAX]->GetPosRatioY() - m_pImgList[INFO_MIN]->GetPosRatioY();
	//
	////Start �� ����. end�� �̻����� �Ѿ �� ����.
	//if (width > maxWidth) width = maxWidth;
	//if (width < 0.f) width = 0.f;
	//if (height > maxHeight) width = maxHeight;
	//if (height < 0.f) width = 0.f;

	////����.
	//m_pImgList[IMG_CENTER]->SetPosRatioX(width);	
	//m_pImgList[IMG_CENTER]->SetPosRatioY(height);
}


