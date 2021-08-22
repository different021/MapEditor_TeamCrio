#pragma once
enum eShaderType;
class IGraphicInstance;

//������ ���� ���� �� �������̽�
//�ν��Ͻ� ��ο� ���� -> �𵨿��� �ν��Ͻ��� �߰�
//

class IModel abstract
{

public:
	virtual IGraphicInstance* AddInstance(eShaderType type) = 0;
	virtual void Delete() = 0;
};

