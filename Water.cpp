#include "Water.h"
#include "Engine/Model.h"
//コンストラクタ
Water::Water(GameObject* parent)
    :GameObject(parent, "Water"),
    hModel_(-1)
{
}

//デストラクタ
Water::~Water()
{
}

//初期化
void Water::Initialize()
{
    //モデルデータのロード
    hModel_ = Model::Load("Assets/Water.fbx");
    assert(hModel_ >= 0);
}

//更新
void Water::Update()
{
    //transform_.rotate_.z--;
}

//描画
void Water::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//開放
void Water::Release()
{
}