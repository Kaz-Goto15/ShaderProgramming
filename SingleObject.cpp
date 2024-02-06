#include "SingleObject.h"
#include "Engine/Model.h"
//コンストラクタ
SingleObject::SingleObject(GameObject* parent)
    :GameObject(parent, "SingleObject"),
    isRotateY(false)
{
}

//デストラクタ
SingleObject::~SingleObject()
{
}

//初期化
void SingleObject::Initialize()
{
}

//更新
void SingleObject::Update()
{
    if (isRotateY) {
        transform_.rotate_.y++;
    }
}

//描画
void SingleObject::Draw()
{
    for(int i = 0; i < hModel_.size(); i++){
        Model::SetTransform(hModel_[i], modelTra_[i]);
        Model::Draw(hModel_[i]);
    }
}

//開放
void SingleObject::Release()
{
}

int SingleObject::AddModel(string fileName)
{
    //モデルデータのロード
    hModel_.push_back(Model::Load("Assets/" + fileName));
    modelTra_.push_back(transform_);
    assert(hModel_.back() >= 0);
    return hModel_.size() - 1;
}

void SingleObject::SetModelTransform(int modelNo, Transform tra)
{
    modelTra_[modelNo] = tra;
}
