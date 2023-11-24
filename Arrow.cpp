#include "Arrow.h"
#include "Engine/Model.h"
//コンストラクタ
Arrow::Arrow(GameObject* parent)
    :GameObject(parent, "Arrow"),
    hModel_(-1)
{
}

//デストラクタ
Arrow::~Arrow()
{
}

//初期化
void Arrow::Initialize()
{
    //モデルデータのロード
    hModel_ = Model::Load("Assets/Arrow.fbx");
    assert(hModel_ >= 0);
    SetScale({ 0.25f, 0.25f, 0.25f });
}

//更新
void Arrow::Update()
{
}

//描画
void Arrow::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//開放
void Arrow::Release()
{
}