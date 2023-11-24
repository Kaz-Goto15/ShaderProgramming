#include "Arrow.h"
#include "Engine/Model.h"
//�R���X�g���N�^
Arrow::Arrow(GameObject* parent)
    :GameObject(parent, "Arrow"),
    hModel_(-1)
{
}

//�f�X�g���N�^
Arrow::~Arrow()
{
}

//������
void Arrow::Initialize()
{
    //���f���f�[�^�̃��[�h
    hModel_ = Model::Load("Assets/Arrow.fbx");
    assert(hModel_ >= 0);
    SetScale({ 0.25f, 0.25f, 0.25f });
}

//�X�V
void Arrow::Update()
{
}

//�`��
void Arrow::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//�J��
void Arrow::Release()
{
}