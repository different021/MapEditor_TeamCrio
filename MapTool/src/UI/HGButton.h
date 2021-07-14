#pragma once
#include <vector>
#include "UIDefine.h"
class IMAGE;

class HGButton
{
	int		m_index;
	int		m_iNumOfUI;
	int		m_iScreenWidth;
	int		m_iScreenHeight;
	wchar_t m_name[256];

	std::vector<IMAGE*> m_ImgList;
	
public:
	void AddImg(IMAGE* pUI);
	void CheckOnMouse(int x, int y);
};

