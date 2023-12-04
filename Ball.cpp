#include "Ball.h"
#include "Engine/Model.h"
//�R���X�g���N�^
Ball::Ball(GameObject* parent)
    :GameObject(parent, "Ball"),
    hModel_(-1)
{
}

//�f�X�g���N�^
Ball::~Ball()
{
}

//������
void Ball::Initialize()
{
    //���f���f�[�^�̃��[�h
    hModel_ = Model::Load("Assets/Ball2.fbx");
    assert(hModel_ >= 0);
    transform_.rotate_.z = 180;
    SetPosition({ 1,1,1 });
}

//�X�V
void Ball::Update()
{
    transform_.rotate_.y++;
}

//�`��
void Ball::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//�J��
void Ball::Release()
{
}