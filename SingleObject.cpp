#include "SingleObject.h"
#include "Engine/Model.h"
//�R���X�g���N�^
SingleObject::SingleObject(GameObject* parent)
    :GameObject(parent, "SingleObject"),
    isRotateY(false)
{
}

//�f�X�g���N�^
SingleObject::~SingleObject()
{
}

//������
void SingleObject::Initialize()
{
}

//�X�V
void SingleObject::Update()
{
    if (isRotateY) {
        transform_.rotate_.y++;
    }
}

//�`��
void SingleObject::Draw()
{
    for(int i = 0; i < hModel_.size(); i++){
        Model::SetTransform(hModel_[i], modelTra_[i]);
        Model::Draw(hModel_[i]);
    }
}

//�J��
void SingleObject::Release()
{
}

int SingleObject::AddModel(string fileName)
{
    //���f���f�[�^�̃��[�h
    hModel_.push_back(Model::Load("Assets/" + fileName));
    modelTra_.push_back(transform_);
    assert(hModel_.back() >= 0);
    return hModel_.size() - 1;
}

void SingleObject::SetModelTransform(int modelNo, Transform tra)
{
    modelTra_[modelNo] = tra;
}
