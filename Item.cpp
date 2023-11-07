#include "Item.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/SphereCollider.h"
//�R���X�g���N�^
Item::Item(GameObject* parent)
	: GameObject(parent, "Item")
{
	hModel = -1;

}

//������
void Item::Initialize()
{
	hModel = Model::Load("Assets/BoxDefault.fbx");
	assert(hModel >= 0);

	this->transform_.scale_.x = 0.25;
	this->transform_.scale_.y = 0.25;
	this->transform_.scale_.z = 0.25;
	//this->transform_.position_.x = 2.0f;
	//this->transform_.position_.y = 1.0f;
	Model::SetTransform(hModel, transform_);

	SphereCollider* collision = new SphereCollider(1);
	AddCollider(collision);

}

//�X�V
void Item::Update()
{
	transform_.rotate_.y++;
	transform_.position_.z += 0.5f;
	if (transform_.position_.z > 50.0f)KillMe();
}

//�`��
void Item::Draw()
{
	Model::SetTransform(hModel, transform_);
	Model::Draw(hModel);
}

//�J��
void Item::Release()
{
	//pFbx->Release();
	//delete pFbx;
}

void Item::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Enemy")
		pTarget->KillMe();
		KillMe();
}