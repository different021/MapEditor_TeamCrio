#include "object_v0.h"


void object_v0::SetPos(DirectX::XMFLOAT3& pos)
{
	this->pos.x = pos.x;
	this->pos.y = pos.y;
	this->pos.z = pos.z;

}

void object_v0::SetRot(DirectX::XMFLOAT4& rot)
{
	this->quaternion.x = rot.x;
	this->quaternion.y = rot.y;
	this->quaternion.z = rot.z;
	this->quaternion.w = rot.w;
}

void object_v0::SetOiler(DirectX::XMFLOAT3& oiler)
{
	this->oiler.x = oiler.x;
	this->oiler.y = oiler.y;
	this->oiler.z = oiler.z;
}

void object_v0::SetScale(DirectX::XMFLOAT3& scale)
{
	this->scale.x = scale.x;
	this->scale.y = scale.y;
	this->scale.z = scale.z;
}

void object_v0::SetPrvRot(DirectX::XMFLOAT4& prvRot)
{
	this->prevRot.x = prvRot.x;
	this->prevRot.y = prvRot.y;
	this->prevRot.z = prvRot.z;
	this->prevRot.w = prvRot.w;
}

void object_v0::CopyObject(object* pDest)
{
	wcscpy_s(pDest->objectName, sizeof(pDest->objectName) / sizeof(WCHAR), this->objectName);
	wcscpy_s(pDest->modelName, sizeof(pDest->modelName) / sizeof(WCHAR), this->modelName);
	wcscpy_s(pDest->matName, sizeof(pDest->matName) / sizeof(WCHAR), this->matName);
	pDest->type = this->type;
	pDest->SetPos(this->pos);
	pDest->SetRot(this->quaternion);
	pDest->SetOiler(this->oiler);
	pDest->SetScale(this->scale);
	pDest->modelIndex = this->modelIndex;
	pDest->matIndex = this->matIndex;
}

void object_v0::GetTm(DirectX::XMFLOAT4X4& out) const
{
	DirectX::XMFLOAT3 pos = this->pos;
	DirectX::XMFLOAT4 rot = this->quaternion;
	DirectX::XMFLOAT3 scale = this->scale;

	DirectX::XMMATRIX mTrans = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	//DirectX::XMMATRIX mRot   = DirectX::XMMatrixRotationRollPitchYaw(oiler.x, oiler.y, oiler.z);
	DirectX::XMVECTOR vRot = DirectX::XMVectorSet(rot.x, rot.y, rot.z, rot.w);
	DirectX::XMMATRIX mRot = DirectX::XMMatrixRotationQuaternion(vRot);

	DirectX::XMMATRIX mScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	DirectX::XMMATRIX mTM = mScale * mRot * mTrans;

	DirectX::XMStoreFloat4x4(&out, mTM);
}

DirectX::XMFLOAT4X4 object_v0::GetTm()
{
	DirectX::XMFLOAT3 pos = this->pos;
	DirectX::XMFLOAT4 rot = this->quaternion;
	DirectX::XMFLOAT3 scale = this->scale;

	DirectX::XMMATRIX mTrans = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	//DirectX::XMMATRIX mRot   = DirectX::XMMatrixRotationRollPitchYaw(oiler.x, oiler.y, oiler.z);
	DirectX::XMVECTOR vRot = DirectX::XMVectorSet(rot.x, rot.y, rot.z, rot.w);
	DirectX::XMMATRIX mRot = DirectX::XMMatrixRotationQuaternion(vRot);

	DirectX::XMMATRIX mScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	DirectX::XMMATRIX mTM = mScale * mRot * mTrans;
	DirectX::XMFLOAT4X4 out;
	DirectX::XMStoreFloat4x4(&out, mTM);

	return out;
}

DirectX::XMFLOAT4X4 object_v0::GetTmByOiler()
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

DirectX::XMFLOAT4X4 object_v0::GetTmByQuaternion()
{
	DirectX::XMFLOAT3 pos = this->pos;
	DirectX::XMFLOAT4 rot = this->quaternion;
	DirectX::XMFLOAT3 scale = this->scale;

	DirectX::XMMATRIX mTrans = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	//DirectX::XMMATRIX mRot = DirectX::XMMatrixRotationRollPitchYaw(oiler.x, oiler.y, oiler.z);
	DirectX::XMVECTOR vRot = DirectX::XMVectorSet(rot.x, rot.y, rot.z, rot.w);
	DirectX::XMMATRIX mRot = DirectX::XMMatrixRotationQuaternion(vRot);

	DirectX::XMMATRIX mScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	DirectX::XMMATRIX mTM = mScale * mRot * mTrans;
	DirectX::XMFLOAT4X4 out;
	DirectX::XMStoreFloat4x4(&out, mTM);

	return out;
}


