#pragma once
#include "CButton_Toggle.h"

//SLIDE��ư������ �̹���(IMG)�� �׷��� �����Ϳ� ������ �Բ� ���� �ִ� ������ ���մϴ�.
//SLIDED��ư������ info�� ���� �׷��� �����͸� ���� ���� ���� �������� ��Ÿ���ϴ�(HUIData* == NULL)

//�� ��ư���� ���� ��� ����.
//IMG  (HUIData* != NULL) �׷��� �ν��Ͻ��� �ִ�. (ex> CENTER, LEFT, RIGHT)
//info (HUIData* == NULL) �׷��� �ν��Ͻ��� ����. (ex> START, END)

//������
#define IMG_FRAME        eEVENT_INACTIVE
#define IMG_FRAME_LEFT   eEVENT_MOUSEIN
#define IMG_FRAME_RIGHT  eEVENT_MOUSEOUT

//���� ��
#define IMG_CENTER  eEVENT_DEFAULT
#define IMG_LEFT    eEVENT_RBUTTONUP 
#define IMG_RIGHT   eEVENT_RBUTTONDOWN

//������ ������.
#define INFO_CUR    eEVENT_MOUSEON
#define INFO_MIN    eEVENT_LBUTTONDOWN
#define INFO_MAX    eEVENT_LBUTTONUP


class CButton_Slide :
    public CMyButton
{
    float m_fMinValue;          //�ּҰ�
    float m_fMaxValue;          //����� ���� �ִ밪
    float m_fCurValue;          //����� ���� ���簪
    float m_fPercentage;        // (���� / ��ü)  =  cur / (max - min)
    
    float m_fWidth;             //�����̵��� ����.(���͸�)
    float m_fCurWidth;

private:
    
    void AddWidth(float delta);

public:
    CButton_Slide(eBUTTON_TYPE type);       //������
    virtual void  Update();                 //�̹��� ������ ����.

    void UpdateWidth();                     //���� 
    
    //��ư�� ��
    float GetMinValue();
    void  SetMinValue(float min);

    //�ִ밪
    float GetMaxValue();
    void  SetMaxValue(float max);

    //���� �� ����
    void  SetCurValue(float fValue);
    float GetCurValue();
    void  AddCurValue(float delta);

    //���� ���� ����
    float GetValueRatio();      
    void  SetValueByRatio(float ratio); 

    //IMG / info ����
    void CreateCenterImg(UI_INFO* pCenter, HEngine_DX12_3D* pEngine);       //�⺻ ������ �̹���.
    void CreateLeftImg  (UI_INFO* pLeft,   HEngine_DX12_3D* pEngine);
    void CreateRightImg (UI_INFO* pRight,  HEngine_DX12_3D* pEngine);
    void CreateRangeInfo(UI_INFO& start,   UI_INFO& end);                   //�����̵� ��� ���� ���� ����.

    //SetImg
    void SetLeftImg(IMAGE* pImg);
    void SetRightImg(IMAGE* pImg);
    void SetCenterImg(IMAGE* pImg);
    void SetFrameImg(IMAGE* pImg);

    //info����        -> ���� ����.
    void EditInfoMin(UI_INFO& start);
    void EditInfoMax(UI_INFO& end );

    //Slide ��Ʈ��
    void SetCenterWidth(float width);
    void SetCurWidth(float CurWidth);
    
    void MoveSlide(int curX, int curY);
    
};

