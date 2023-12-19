#include "Torus.h"
#include "Engine/Model.h"
//�R���X�g���N�^
Torus::Torus(GameObject* parent)
    :GameObject(parent, "Torus"),
    hModel_(-1)
{
}

//�f�X�g���N�^
Torus::~Torus()
{
}

//������
void Torus::Initialize()
{
    //���f���f�[�^�̃��[�h
    hModel_ = Model::Load("Assets/Torus.fbx");
    assert(hModel_ >= 0);
}

//�X�V
void Torus::Update()
{
    //transform_.rotate_.z--;
}

//�`��
void Torus::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//�J��
void Torus::Release()
{
}