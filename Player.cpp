#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Item.h"
//コンストラクタ
Player::Player(GameObject* parent)
	: GameObject(parent, "Player")
{
	hModel_ = -1;

}

//初期化
void Player::Initialize()
{
	hModel_ = Model::Load("Assets/BoxDefault.fbx");
	assert(hModel_ >= 0);
	//this->transform_.scale_.x = 0.5;
	//this->transform_.scale_.y = 0.5;
	//this->transform_.scale_.z = 0.5;
	//Instantiate<Item>(this);
	//GameObject* item = Instantiate<Item>(this);
	//item->SetPosition({ -2.0f, 1.0f, 0.0f });
	Model::SetTransform(hModel_, transform_);
}

//更新
void Player::Update()
{ 
	if (Input::IsKeyDown(DIK_A)) {
		transform_.position_.x--;
	}
	//static float rotValue = 0;
	//static float sinV = 0;
	//static float plusVal = 0;
	//sinV += 0.002f;
	//plusVal = sin(sinV) / 2.0f;
	//rotValue += abs(plusVal);
	//transform_.rotate_.y = rotValue;
	transform_.rotate_.y++;
	//if (Input::IsKeyDown(DIK_0))KillMe();
	if (Input::IsKey(DIK_LEFT))transform_.position_.x -= 0.1f;
	if (Input::IsKey(DIK_RIGHT))transform_.position_.x += 0.1f;
	if (Input::IsKeyDown(DIK_SPACE)) {
		GameObject* pBullet = Instantiate<Item>(pParent_);
		pBullet->SetPosition(transform_.position_);
	}
}

//描画
void Player::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

}

//開放
void Player::Release()
{
	//pFbx->Release();
	//delete pFbx;
}