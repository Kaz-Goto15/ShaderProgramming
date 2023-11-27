#include "Ground.h"
#include "Engine/Model.h"
//�R���X�g���N�^
Ground::Ground(GameObject* parent)
    :GameObject(parent, "Ground"),
    hModel_(-1)
{
}

//�f�X�g���N�^
Ground::~Ground()
{
}

//������
void Ground::Initialize()
{
    //���f���f�[�^�̃��[�h
    hModel_ = Model::Load("Assets/Ground.fbx");
    assert(hModel_ >= 0);
}

//�X�V
void Ground::Update()
{
    transform_.rotate_.z--;
}

//�`��
void Ground::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//�J��
void Ground::Release()
{
}