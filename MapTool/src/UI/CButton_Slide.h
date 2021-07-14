#pragma once
#include "CButton_Toggle.h"

//SLIDE버튼에서의 이미지(IMG)는 그래픽 데이터와 정보를 함께 갖고 있는 내용을 말합니다.
//SLIDED버튼에서의 info는 실제 그래픽 데이터를 갖고 있지 않은 정보만을 나타냅니다(HUIData* == NULL)

//이 버튼에서 사용될 용어 정리.
//IMG  (HUIData* != NULL) 그래픽 인스턴스를 있다. (ex> CENTER, LEFT, RIGHT)
//info (HUIData* == NULL) 그래픽 인스턴스가 없다. (ex> START, END)

//프레임
#define IMG_FRAME        eEVENT_INACTIVE
#define IMG_FRAME_LEFT   eEVENT_MOUSEIN
#define IMG_FRAME_RIGHT  eEVENT_MOUSEOUT

//현재 바
#define IMG_CENTER  eEVENT_DEFAULT
#define IMG_LEFT    eEVENT_RBUTTONUP 
#define IMG_RIGHT   eEVENT_RBUTTONDOWN

//데이터 정보들.
#define INFO_CUR    eEVENT_MOUSEON
#define INFO_MIN    eEVENT_LBUTTONDOWN
#define INFO_MAX    eEVENT_LBUTTONUP


class CButton_Slide :
    public CMyButton
{
    float m_fMinValue;          //최소값
    float m_fMaxValue;          //사용자 정의 최대값
    float m_fCurValue;          //사용자 정의 현재값
    float m_fPercentage;        // (현재 / 전체)  =  cur / (max - min)
    
    float m_fWidth;             //슬라이드의 길이.(센터만)
    float m_fCurWidth;

private:
    
    void AddWidth(float delta);

public:
    CButton_Slide(eBUTTON_TYPE type);       //생성자
    virtual void  Update();                 //이미지 데이터 수정.

    void UpdateWidth();                     //길이 
    
    //버튼의 값
    float GetMinValue();
    void  SetMinValue(float min);

    //최대값
    float GetMaxValue();
    void  SetMaxValue(float max);

    //현재 값 관련
    void  SetCurValue(float fValue);
    float GetCurValue();
    void  AddCurValue(float delta);

    //현재 값의 비율
    float GetValueRatio();      
    void  SetValueByRatio(float ratio); 

    //IMG / info 생성
    void CreateCenterImg(UI_INFO* pCenter, HEngine_DX12_3D* pEngine);       //기본 세개의 이미지.
    void CreateLeftImg  (UI_INFO* pLeft,   HEngine_DX12_3D* pEngine);
    void CreateRightImg (UI_INFO* pRight,  HEngine_DX12_3D* pEngine);
    void CreateRangeInfo(UI_INFO& start,   UI_INFO& end);                   //슬라이드 사용 가능 범위 정보.

    //SetImg
    void SetLeftImg(IMAGE* pImg);
    void SetRightImg(IMAGE* pImg);
    void SetCenterImg(IMAGE* pImg);
    void SetFrameImg(IMAGE* pImg);

    //info수정        -> 변경 예정.
    void EditInfoMin(UI_INFO& start);
    void EditInfoMax(UI_INFO& end );

    //Slide 컨트롤
    void SetCenterWidth(float width);
    void SetCurWidth(float CurWidth);
    
    void MoveSlide(int curX, int curY);
    
};

