#include "Defines.h"
#include <SimpleMath.h>

void object::SetPos(DirectX::XMFLOAT3& pos)
{
	this->pos.x = pos.x;
	this->pos.y = pos.y;
	this->pos.z = pos.z;

}

void object::SetRot(DirectX::XMFLOAT4& rot)
{
	this->quaternion.x = rot.x;
	this->quaternion.y = rot.y;
	this->quaternion.z = rot.z;
	this->quaternion.w = rot.w;
}

void object::SetOiler(DirectX::XMFLOAT3& oiler)
{
	this->oiler.x = oiler.x;
	this->oiler.y = oiler.y;
	this->oiler.z = oiler.z;

	DirectX::SimpleMath::Quaternion quatRot = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(oiler.y * DirectX::XM_PI / 180, oiler.x * DirectX::XM_PI / 180, oiler.z * DirectX::XM_PI / 180);
	quaternion.x = quatRot.x;
	quaternion.y = quatRot.y;
	quaternion.z = quatRot.z;
	quaternion.w = quatRot.w;
}

void object::SetScale(DirectX::XMFLOAT3& scale)
{
	this->scale.x = scale.x;
	this->scale.y = scale.y;
	this->scale.z = scale.z;
}

void object::SetPrvRot(DirectX::XMFLOAT4& prvRot)
{
	this->prevRot.x = prvRot.x;
	this->prevRot.y = prvRot.y;
	this->prevRot.z = prvRot.z;
	this->prevRot.w = prvRot.w;
}

void object::SetMatIndex(int index)
{
	matIndex = index;
	if (matIndex == 0)
	{
		eShaderType = eShaderType::eSHADER_CORLORCHIP;
	}
	else
	{
		eShaderType = eShaderType::eSHADER_DEFAULT;
	}
}

void object::SetRegenIndex(int index)
{
	regenIndex = index;
}

void object::CopyTo(object* pDest)
{
	memcpy(pDest, this, sizeof(object));
}

void object::GetTm(DirectX::XMFLOAT4X4& out) const
{
	DirectX::XMFLOAT3 pos = this->pos;
	DirectX::XMFLOAT4 rot = this->quaternion;
	DirectX::XMFLOAT3 scale = this->scale;

	DirectX::XMMATRIX mTrans = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	//DirectX::XMMATRIX mRot   = DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
	DirectX::XMVECTOR vRot = DirectX::XMVectorSet(rot.x, rot.y, rot.z, rot.w);
	DirectX::XMMATRIX mRot = DirectX::XMMatrixRotationQuaternion(vRot);

	DirectX::XMMATRIX mScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	DirectX::XMMATRIX mTM = mScale * mRot * mTrans;

	DirectX::XMStoreFloat4x4(&out, mTM);
}

object::object()
{
	type		= eOBJECT_FIELD_COLLISION_YES;	//오브젝트 타입
	eShaderType = eSHADER_DEFAULT;				//쉐이더 타입.
	regenIndex	= -1;							//콜리전 체크를 할 것인지 말 것인지에 대하여.
	modelIndex	= -1;
	matIndex	= -1;

	pos			= {};
	oiler		= {};
	quaternion	= {};
	prevRot		= {};
	scale		= { 1.f, 1.f, 1.f };

	wmemset(objectName, 0, MAX_LENGTH);
	wmemset(modelName, 0, MAX_LENGTH);
	wmemset(matName, 0, MAX_LENGTH);
}

object::~object()
{
}

DirectX::XMFLOAT4X4 object::GetTm()
{
	DirectX::XMMATRIX mTrans = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	DirectX::XMVECTOR vRot	 = DirectX::XMVectorSet(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	DirectX::XMMATRIX mRot	 = DirectX::XMMatrixRotationQuaternion(vRot);
	DirectX::XMMATRIX mScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	DirectX::XMMATRIX mTM = mScale * mRot * mTrans;
	DirectX::XMFLOAT4X4 out;
	DirectX::XMStoreFloat4x4(&out, mTM);

	return out;
}


DirectX::XMFLOAT4X4 object::GetTmByOiler()
{
	DirectX::XMMATRIX mTrans = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	DirectX::XMMATRIX mRot   = DirectX::XMMatrixRotationRollPitchYaw(quaternion.x, quaternion.y, quaternion.z);
	DirectX::XMMATRIX mScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	DirectX::XMMATRIX mTM = mScale * mRot * mTrans;
	DirectX::XMFLOAT4X4 out;
	DirectX::XMStoreFloat4x4(&out, mTM);

	return out;
}

DirectX::XMFLOAT4X4 object::GetTmByQuaternion()
{
	DirectX::XMMATRIX mTrans = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	DirectX::XMVECTOR vRot   = DirectX::XMVectorSet(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	DirectX::XMMATRIX mRot   = DirectX::XMMatrixRotationQuaternion(vRot);
	DirectX::XMMATRIX mScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	DirectX::XMMATRIX mTM = mScale * mRot * mTrans;
	DirectX::XMFLOAT4X4 out;
	DirectX::XMStoreFloat4x4(&out, mTM);

	return out;
}





//void object::GetTm(DirectX::XMFLOAT4X4* pOut)
//{
//	DirectX::XMFLOAT3 pos = this->pos;
//	DirectX::XMFLOAT4 quaternion = this->rot;
//	DirectX::XMFLOAT3 scale = this->scale;
//
//	DirectX::XMMATRIX mTrans = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
//	//DirectX::XMMATRIX mRot = DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
//	DirectX::XMVECTOR vRot = DirectX::XMVectorSet(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
//	DirectX::XMMATRIX mRot = DirectX::XMMatrixRotationQuaternion(vRot);
//	DirectX::XMMATRIX mScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
//
//	DirectX::XMMATRIX mTM = mScale * mRot * mTrans;
//
//	DirectX::XMStoreFloat4x4(pOut, mTM);
//}

//strTEXTURE::~strTEXTURE()
//{
//	Clear();
//}

void strTEXTURE::Clear()
{
	for (int i = eAlbedo; i < eTEX::eTEX_MAX; i++)
	{
		if (tex[i])
		{
			delete[] tex[i];
			tex[i] = nullptr;
		}
	}
}


void collider::GetTM(DirectX::XMFLOAT4X4& out)
{
	DirectX::XMFLOAT3 pos = this->pos;
	//DirectX::XMFLOAT3 rot = this->rot;
	DirectX::XMFLOAT4 quaternion = this->quaternion;
	DirectX::XMFLOAT3 scale = this->scale;

	DirectX::XMMATRIX mTrans = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	//DirectX::XMMATRIX mRot = DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
	DirectX::XMVECTOR vRot = DirectX::XMVectorSet(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	DirectX::XMMATRIX mRot = DirectX::XMMatrixRotationQuaternion(vRot);
	DirectX::XMMATRIX mScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	DirectX::XMMATRIX mTM = mScale * mRot * mTrans;

	DirectX::XMStoreFloat4x4(&out, mTM);
}

void collider::GetTM(DirectX::XMFLOAT4X4* pOut)
{
	DirectX::XMFLOAT3 pos = this->pos;
	//DirectX::XMFLOAT3 rot = this->rot;
	DirectX::XMFLOAT4 quaternion = this->quaternion;
	DirectX::XMFLOAT3 scale = this->scale;

	DirectX::XMMATRIX mTrans = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	//DirectX::XMMATRIX mRot = DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
	DirectX::XMVECTOR vRot = DirectX::XMVectorSet(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	DirectX::XMMATRIX mRot = DirectX::XMMatrixRotationQuaternion(vRot);
	DirectX::XMMATRIX mScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	DirectX::XMMATRIX mTM = mScale * mRot * mTrans;

	DirectX::XMStoreFloat4x4(pOut, mTM);
}

DirectX::XMMATRIX collider::GetTM()
{
	DirectX::XMFLOAT3 pos = this->pos;
	DirectX::XMFLOAT4 quaternion = this->quaternion;
	DirectX::XMFLOAT3 oiler = this->oiler;
	DirectX::XMFLOAT3 scale = this->scale;

	DirectX::XMMATRIX mTrans = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	//DirectX::XMMATRIX mRot = DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
	DirectX::XMVECTOR vRot = DirectX::XMVectorSet(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	DirectX::XMMATRIX mRot = DirectX::XMMatrixRotationQuaternion(vRot);
	DirectX::XMMATRIX mScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	DirectX::XMMATRIX mTM = mScale * mRot * mTrans;

	return mTM;
}


collider::collider()
{
	type		= eCOLLIDER_NORMAL;				//ver3 추가.
	index		= -1;
	pos			= {};
	oiler		= {};
	quaternion	= {};
	prevRot		= {};
	scale		= {};
	size		= {};
	isVisible	= false;
	memset(name, 0, sizeof(WCHAR) * MAX_LENGTH );
}

void collider::CopyExceptIndex(collider* pDest)
{
	pDest->type = this->type;
	//pDest->index = this->index;
	pDest->isVisible = this->isVisible;

	pDest->pos.x = this->pos.x;				//pos
	pDest->pos.y = this->pos.y;
	pDest->pos.z = this->pos.z;

	pDest->quaternion.x = this->quaternion.x;				//Rot
	pDest->quaternion.y = this->quaternion.y;
	pDest->quaternion.z = this->quaternion.z;
	pDest->quaternion.w = this->quaternion.w;

	pDest->oiler.x = this->oiler.x;
	pDest->oiler.y = this->oiler.y;
	pDest->oiler.z = this->oiler.z;

	pDest->prevRot.x = this->prevRot.x;
	pDest->prevRot.y = this->prevRot.y;
	pDest->prevRot.z = this->prevRot.z;
	pDest->prevRot.w = this->prevRot.w;

	pDest->scale.x = this->scale.x;			//scale
	pDest->scale.y = this->scale.y;
	pDest->scale.z = this->scale.z;

	pDest->size.x = this->size.x;
	pDest->size.y = this->size.y;
	pDest->size.z = this->size.z;

	pDest->MakeName();
}

void collider::MakeName()
{
	const WCHAR* pNameList[eCOLLIDER_MAX] = {
	L"StartPoint",
	L"EndPoint",
	L"RegenPoint",
	L"NormalCollider",
	};
	wsprintfW(name, L"%s%0d", pNameList[static_cast<int>(type)], index);
}

void collider::SaveCollider(collider* pDest)
{
	memcpy(pDest, this, sizeof(collider));
}

void collider::CopyCollider(collider* pDest)
{
	pDest->type	 = this->type;
	pDest->index = this->index;
	pDest->isVisible = this->isVisible;

	pDest->pos.x = this->pos.x;				//pos
	pDest->pos.y = this->pos.y;
	pDest->pos.z = this->pos.z;

	pDest->quaternion.x = this->quaternion.x;				//Rot
	pDest->quaternion.y = this->quaternion.y;
	pDest->quaternion.z = this->quaternion.z;
	pDest->quaternion.w = this->quaternion.w;

	pDest->oiler.x = this->oiler.x;
	pDest->oiler.y = this->oiler.y;
	pDest->oiler.z = this->oiler.z;

	pDest->prevRot.x = this->prevRot.x;
	pDest->prevRot.y = this->prevRot.y;
	pDest->prevRot.z = this->prevRot.z;
	pDest->prevRot.w = this->prevRot.w;

	pDest->scale.x = this->scale.x;			//scale
	pDest->scale.y = this->scale.y;
	pDest->scale.z = this->scale.z;

	pDest->size.x = this->size.x;
	pDest->size.y = this->size.y;
	pDest->size.z = this->size.z;

	pDest->MakeName();
}


waveData::waveData()
{
	//디폴트 값으로 초기화
	row = 128;
	col = 128;
	dx = 1.f;
	speed = 0.03f;
	damping = 0.2f;
	pos = { 0.f, 0.f, 0.f };
	rot = { 0.f, 0.f, 0.f };
	quaternion = { 0.f, 0.f, 0.f, 0.f };
	prevRot = { 0.f, 0.f, 0.f, 0.f };
	scale = { 1.f, 1.f, 1.f };
}

void waveData::SetOiler(DirectX::XMFLOAT3& oil)
{
	rot.x = oil.x;
	rot.y = oil.y;
	rot.z = oil.z;

	DirectX::SimpleMath::Quaternion quatRot = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(rot.y * DirectX::XM_PI / 180, rot.x * DirectX::XM_PI / 180, rot.z * DirectX::XM_PI / 180);
	quaternion.x = quatRot.x;
	quaternion.y = quatRot.y;
	quaternion.z = quatRot.z;
	quaternion.w = quatRot.w;
}

DirectX::XMFLOAT4X4 waveData::GetTm()
{
	DirectX::XMMATRIX mTrans = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	DirectX::XMVECTOR vRot = DirectX::XMVectorSet(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	DirectX::XMMATRIX mRot = DirectX::XMMatrixRotationQuaternion(vRot);
	DirectX::XMMATRIX mScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	DirectX::XMMATRIX mTM = mScale * mRot * mTrans;
	DirectX::XMFLOAT4X4 out;
	DirectX::XMStoreFloat4x4(&out, mTM);

	return out;
}

