#include "BilliardsScene.h"
#include "Ball.h"
#include "Player.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include <sstream>
#include "Ready.h"

//�R���X�g���N�^
BilliardsScene::BilliardsScene(GameObject* parent)
	: GameObject(parent, "BilliardsScene")
{
	//camPos_ = { 0,2,-17 };
	//camTarget_ = { 0,5,0 };
}

//������
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

//�X�V
void BilliardsScene::Update()
{
	//�����ł߂荞��ł�ʂ�T���Ĉ����͂���
	std::list<Ball*> balls = FindGameObjects<Ball>();	//�S�{�[���̃��X�g
	for (auto itr1 = balls.begin(); itr1 != balls.end(); itr1++) {
		for (auto itr2 = balls.begin(); itr2 != balls.end(); itr2++) {
			if (itr1 == itr2)continue;
			//*itr1��*itr2�̍��W�����āA������蔍����
			XMVECTOR distance = (*itr1)->GetPosition() - (*itr2)->GetPosition();
			if (Length(distance) < 1.0f * 2.0f){	//�d�Ȃ��Ă���
				float depth = 1.0f * 2.0f - Length(distance);	//�߂肱�ݗ�

				//�߂荞�ݗʂ̔������ړ�����
				distance = XMVector3Normalize(distance) * depth / 2.0f;	//������
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

//�`��
void BilliardsScene::Draw()
{
}

//�J��
void BilliardsScene::Release()
{
}
