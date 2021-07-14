#include "pch.h"
#include "CharacterManager.h"

CharacterManager* CharacterManager::m_pInstance = NULL;
int CharacterManager::m_InsCount = 0;

CharacterManager* CharacterManager::GetInstance()
{
    if (!m_pInstance)
    {
        m_pInstance = new CharacterManager;
    }
    m_InsCount++;
    return m_pInstance;
}

void CharacterManager::Release()
{
    m_InsCount--;
    if (m_InsCount <= 0)
    {
        if (m_pInstance != NULL)
        {
            delete m_pInstance;
            m_pInstance = NULL;
        }
    }
}

std::vector<CHARACTER*>& CharacterManager::GetCharList()
{
    return m_CharList;
}

CHARACTER* CharacterManager::GetCharacter(int index)
{
    CHARACTER* pReturn = NULL;
    if (index < 0)
        goto lb_return;
    
    if (index < m_CharList.size())
        pReturn = m_CharList[index];

lb_return:
    return pReturn;
}

//�߰� ���� ����Ʈ ���� ��ȯ
int CharacterManager::AddChar(CHARACTER* pChar)
{
    m_CharList.push_back(pChar);
    return m_CharList.size();
}

//���� �� ����Ʈ ���� ��ȯ
int CharacterManager::DeleteChar(int index)
{
    m_CharList.erase(m_CharList.begin() + index);
    return m_CharList.size();
}

int CharacterManager::GetSize()
{
    return m_CharList.size();
}
