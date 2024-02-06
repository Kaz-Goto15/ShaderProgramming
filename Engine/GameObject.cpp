#include "GameObject.h"
#include "SphereCollider.h"

GameObject::GameObject()
{
	pParent_ = nullptr;
	isDead = false;
}

GameObject::GameObject(GameObject* parent, const std::string& name) :
	pParent_(parent),
	objectName_(name),
	isDead(false),
	pCollider_(nullptr)
{
	if (pParent_ != nullptr)
		this->transform_.pParent_ = &(parent->transform_);
}

GameObject::~GameObject()
{
}

void GameObject::UpdateSub()
{
	Update();
	RoundRobin(GetRootJob());
	for (auto itr = childList_.begin(); itr != childList_.end(); itr++) {
		(*itr)->UpdateSub();
	}
	for (auto itr = childList_.begin(); itr != childList_.end();) {
		if ((*itr)->isDead) {
			(*itr)->ReleaseSub();
			SAFE_DELETE(*itr);
			itr = childList_.erase(itr);
		}
		else {
			itr++;
		}
	}
}

void GameObject::DrawSub()
{
	Draw();
	//�q��draw
	for (auto itr = childList_.begin(); itr != childList_.end(); itr++) {
		(*itr)->DrawSub();
	}
}

void GameObject::ReleaseSub()
{
	for (auto itr = childList_.begin(); itr != childList_.end(); itr++) {
		(*itr)->ReleaseSub();
		SAFE_DELETE(*itr);
	}
	Release();
}

void GameObject::KillMe()
{
	isDead = true;
}

void GameObject::SetTransform(Transform _tra)
{
	transform_.position_ = _tra.position_;
	transform_.rotate_ = _tra.rotate_;
	transform_.scale_ = _tra.scale_;
}

void GameObject::SetScale(float _sca)
{
	transform_.scale_.x = _sca;
	transform_.scale_.y = _sca;
	transform_.scale_.z = _sca;
}

GameObject* GameObject::GetRootJob()
{
	if (pParent_ == nullptr)return this;
	else return pParent_;
}

GameObject* GameObject::FindChildObject(std::string _objName)
{
	if (_objName == objectName_) {
		return this;	//���O�������������Ƃ����g��Ԃ�
	}
	else {
		for (auto& itr : childList_) {
			GameObject* obj = itr->FindChildObject(_objName);
			if (obj != nullptr)return obj;
		}
	}
}

void GameObject::AddCollider(SphereCollider* pCollider)
{
	pCollider_ = pCollider;
}

void GameObject::Collision(GameObject* pTarget)
{

	//�����̃R���C�_�[�ƃ^�[�Q�b�g�̏Փˎ���onCollision(pTarget)���ďo
	//not attached
	if (pTarget == this || pTarget->pCollider_ == nullptr)return;

	float dist =
		pow(pTarget->transform_.position_.x - transform_.position_.x, 2) +
		pow(pTarget->transform_.position_.y - transform_.position_.y, 2) +
		pow(pTarget->transform_.position_.z - transform_.position_.z, 2);
	float rDist =
		pow((pTarget->pCollider_->GetRadius() + pCollider_->GetRadius()), 2);
	//�����ƃ^�[�Q�b�g�̋��� <= R1+R2�Ȃ�onColli
	if (dist <= rDist) {
		this->OnCollision(pTarget);
	}
}

void GameObject::RoundRobin(GameObject* pTarget)
{
	if (pCollider_ == nullptr)return;
	if (pTarget->pCollider_ != nullptr)Collision(pTarget);
	for (auto itr : pTarget->childList_)RoundRobin(itr);
}
