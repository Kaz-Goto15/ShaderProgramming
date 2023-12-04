#include "Ball.h"
#include "Engine/Model.h"
//コンストラクタ
Ball::Ball(GameObject* parent)
    :GameObject(parent, "Ball"),
    hModel_(-1)
{
}

//デストラクタ
Ball::~Ball()
{
}

//初期化
void Ball::Initialize()
{
    //モデルデータのロード
    hModel_ = Model::Load("Assets/Ball2.fbx");
    assert(hModel_ >= 0);
    transform_.rotate_.z = 180;
    SetPosition({ 1,1,1 });
}

//更新
void Ball::Update()
{
    transform_.rotate_.y++;
}

//描画
void Ball::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//開放
void Ball::Release()
{
}