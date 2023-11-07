#include "Enemy.h"
#include "Engine/Model.h"
#include "SphereCollider.h"

Enemy::Enemy(GameObject* parent):
	GameObject(parent, "Enemy")
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	hModel = Model::Load("Assets/oden.fbx");
	assert(hModel >= 0);
	transform_.position_.z = 5;
	SphereCollider* collision = new SphereCollider(3);
	AddCollider(collision);
}

void Enemy::Update()
{
}

void Enemy::Draw()
{
	Model::SetTransform(hModel, transform_);
	Model::Draw(hModel);
}

void Enemy::Release()
{
}

void Enemy::OnCollision(GameObject* pTarget)
{
	if(pTarget->GetObjectName() == "Item")
	KillMe();
}