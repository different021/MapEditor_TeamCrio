#pragma once
enum eShaderType;
class IGraphicInstance;

//툴내부 에서 사용될 모델 인터페이스
//인스턴스 드로우 지원 -> 모델에서 인스턴스를 추가
//

class IModel abstract
{

public:
	virtual IGraphicInstance* AddInstance(eShaderType type) = 0;
	virtual void Delete() = 0;
};

