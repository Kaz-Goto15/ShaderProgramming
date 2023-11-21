#include "BilliardsScene.h"
#include "Ball.h"
#include "Player.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include <sstream>
#include "Ready.h"

//コンストラクタ
BilliardsScene::BilliardsScene(GameObject* parent)
	: GameObject(parent, "BilliardsScene")
{
	//camPos_ = { 0,2,-17 };
	//camTarget_ = { 0,5,0 };
}

//初期化
void BilliardsScene::Initialize()
{
	Camera::SetPosition(XMFLOAT3(0, 50, 0));
	Camera::SetTarget(XMFLOAT3(0, 0, 0));
	for (int i = 0; i < 15; i++) {
		Ball* b = Instantiate<Ball>(this);
		b->SetNumber(i + 1);
		b->SetPosition((i - 4) * 1.4f, 0, (i - 4) * 1.4f);
	}
	Player* p = Instantiate<Player>(this);
	Ball* b = Instantiate<Ball>(this);
	b->SetPosition(-10, 0, 10);
	b->SetNumber(0);
	p->SetMyBall(b);
	Ready* rea = Instantiate<Ready>(this);
}

//更新
void BilliardsScene::Update()
{
	//ここでめり込んでる玉を探して引きはがす
	std::list<Ball*> balls = FindGameObjects<Ball>();	//全ボールのリスト
	for (auto itr1 = balls.begin(); itr1 != balls.end(); itr1++) {
		for (auto itr2 = balls.begin(); itr2 != balls.end(); itr2++) {
			if (itr1 == itr2)continue;
			//*itr1と*itr2の座標を見て、無理やり剥がす
			XMVECTOR distance = (*itr1)->GetPosition() - (*itr2)->GetPosition();
			if (Length(distance) < 1.0f * 2.0f){	//重なっている
				float depth = 1.0f * 2.0f - Length(distance);	//めりこみ量

				//めり込み量の半分ずつ移動する
				distance = XMVector3Normalize(distance) * depth / 2.0f;	//押す量
				(*itr1)->SetPosition((*itr1)->GetPosition() + distance);
				distance *= -1.0f;
				(*itr2)->SetPosition((*itr2)->GetPosition() + distance);
			}
		}
	}
	//if (1) {
	//	if (Input::IsKeyDown(DIK_1))camPos_.x++;
	//	if (Input::IsKeyDown(DIK_2))camPos_.x--;
	//	if (Input::IsKeyDown(DIK_3))camPos_.y++;
	//	if (Input::IsKeyDown(DIK_4))camPos_.y--;
	//	if (Input::IsKeyDown(DIK_5))camPos_.z++;
	//	if (Input::IsKeyDown(DIK_6))camPos_.z--;
	//	if (Input::IsKeyDown(DIK_7))camTarget_.x++;
	//	if (Input::IsKeyDown(DIK_8))camTarget_.x--;
	//	if (Input::IsKeyDown(DIK_9))camTarget_.y++;
	//	if (Input::IsKeyDown(DIK_0))camTarget_.y--;
	//	if (Input::IsKeyDown(DIK_MINUS))camTarget_.z++;
	//	if (Input::IsKeyDown(DIK_CIRCUMFLEX))camTarget_.z--;

	//	//camPos_ = b->GetPosition();
	//	//camTarget_ = camPos_;
	//	//camPos_.y += 10;
	//	Camera::SetPosition(camPos_);
	//	Camera::SetTarget(camTarget_);
	//}
}

//描画
void BilliardsScene::Draw()
{
}

//開放
void BilliardsScene::Release()
{
}
