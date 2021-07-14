#pragma once
#include "../src/Defines.h"
#include <vector>

class CharacterManager
{
	static CharacterManager* m_pInstance;
	static int m_InsCount;
	std::vector<CHARACTER*> m_CharList;

public:
	static CharacterManager* GetInstance();
	void Release();

	std::vector<CHARACTER*>& GetCharList();
	CHARACTER* GetCharacter(int index);
	int AddChar(CHARACTER* pChar);
	int DeleteChar(int index);
	int GetSize();
};

