#include "Water.h"
#include "Engine/Model.h"
//�R���X�g���N�^
Water::Water(GameObject* parent)
    :GameObject(parent, "Water"),
    hModel_(-1)
{
}

//�f�X�g���N�^
Water::~Water()
{
}

//������
void Water::Initialize()
{
    //���f���f�[�^�̃��[�h
    hModel_ = Model::Load("Assets/Water.fbx");
    assert(hModel_ >= 0);
}

//�X�V
void Water::Update()
{
    //transform_.rotate_.z--;
}

//�`��
void Water::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//�J��
void Water::Release()
{
}