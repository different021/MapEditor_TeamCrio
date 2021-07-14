#include "MAP_v1.h"

void object_v1::SetPos(DirectX::XMFLOAT3& pos)
{
	this->pos.x = pos.x;
	this->pos.y = pos.y;
	this->pos.z = pos.z;

}

void object_v1::SetRot(DirectX::XMFLOAT4& rot)
{
	this->quaternion.x = rot.x;
	this->quaternion.y = rot.y;
	this->quaternion.z = rot.z;
	this->quaternion.w = rot.w;
}

void object_v1::SetOiler(DirectX::XMFLOAT3& oiler)
{
	this->oiler.x = oiler.x;
	this->oiler.y = oiler.y;
	this->oiler.z = oiler.z;
}

void object_v1::SetScale(DirectX::XMFLOAT3& scale)
{
	this->scale.x = scale.x;
	this->scale.y = scale.y;
	this->scale.z = scale.z;
}

void object_v1::SetPrvRot(DirectX::XMFLOAT4& prvRot)
{
	this->prevRot.x = prvRot.x;
	this->prevRot.y = prvRot.y;
	this->prevRot.z = prvRot.z;
	this->prevRot.w = prvRot.w;
}

void object_v1::SetMatIndex(int index)
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

void object_v1::CopyObject(object_v1* pDest)
{
	memcpy(pDest, this, sizeof(object));
}

void object_v1::GetTm(DirectX::XMFLOAT4X4& out) const
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

object_v1::object_v1()
{
	type = eOBJECTv1_FIELD;				//오브젝트 타입
	eShaderType = eSHADER_DEFAULT;		//쉐이더 타입.
	isCollision = eCollisionYes;		//콜리전 체크를 할 것인지 말 것인지에 대하여.

	pos = {};
	oiler = {};
	quaternion = {};
	prevRot = {};
	scale = {};

	wmemset(objectName, 0, MAX_LENGTH);
	wmemset(modelName, 0, MAX_LENGTH);
	wmemset(matName, 0, MAX_LENGTH);

	modelIndex = -1;
	matIndex = -1;
}

DirectX::XMFLOAT4X4 object_v1::GetTm()
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
	DirectX::XMFLOAT4X4 out;
	DirectX::XMStoreFloat4x4(&out, mTM);

	return out;
}

DirectX::XMFLOAT4X4 object_v1::GetTmByOiler()
{
	DirectX::XMFLOAT3 pos = this->pos;
	DirectX::XMFLOAT4 rot = this->quaternion;
	DirectX::XMFLOAT3 scale = this->scale;

	DirectX::XMMATRIX mTrans = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	DirectX::XMMATRIX mRot = DirectX::XMMatrixRotationRollPitchYaw(quaternion.x, quaternion.y, quaternion.z);
	//DirectX::XMVECTOR vRot = DirectX::XMVectorSet(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	//DirectX::XMMATRIX mRot = DirectX::XMMatrixRotationQuaternion(vRot);

	DirectX::XMMATRIX mScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	DirectX::XMMATRIX mTM = mScale * mRot * mTrans;
	DirectX::XMFLOAT4X4 out;
	DirectX::XMStoreFloat4x4(&out, mTM);

	return out;
}

DirectX::XMFLOAT4X4 object_v1::GetTmByQuaternion()
{
	DirectX::XMFLOAT3 pos = this->pos;
	DirectX::XMFLOAT4 rot = this->quaternion;
	DirectX::XMFLOAT3 scale = this->scale;

	DirectX::XMMATRIX mTrans = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	//DirectX::XMMATRIX mRot = DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
	DirectX::XMVECTOR vRot = DirectX::XMVectorSet(rot.x, rot.y, rot.z, rot.w);
	DirectX::XMMATRIX mRot = DirectX::XMMatrixRotationQuaternion(vRot);

	DirectX::XMMATRIX mScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	DirectX::XMMATRIX mTM = mScale * mRot * mTrans;
	DirectX::XMFLOAT4X4 out;
	DirectX::XMStoreFloat4x4(&out, mTM);

	return out;
}



