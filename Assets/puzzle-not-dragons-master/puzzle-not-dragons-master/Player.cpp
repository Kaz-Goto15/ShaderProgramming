#include "Player.h"
#include "Ball.h"
#include "Engine/Input.h"
#include "Engine/Model.h"
#include "Engine/Image.h"
#include "Engine/Debug.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include <string>
#include "Engine/Text.h"
#include "Gauge.h"
#include "Ready.h"
/*
* キーボードのADで方向を決め、スペースキーを押すとパワーを溜めるゲージが表示され、
* スペースキーを離すと、その矢印の方向に離したときに溜まっているパワーで球を打つ
* 全ての玉が静止すると、また方向を決める状態に戻る
* 
* 少し難しい：ゲージを矢印から少しずらして表示させること
* 　　　　　　画像自体を右寄りにしてもできるが、こういう簡素なことはわざわざそんなことをせずともプログラム上で解決したい
* 　　　　　　映像方面ではグループ制御で簡単にできるが、こちらでは少し難航
*/

namespace {
	static const float ROT_SPEED = 90.0f;	//1秒間で移動する角度(度)
}
Player::Player(GameObject* parent)
{
	direction = 0.0f;
	power = 0.3f;
	hModel_ = Model::Load("arrow.fbx");
	//transform_.scale_ = XMFLOAT3(0.2f, 0.2f, 0.2f);
	transform_.scale_ = XMFLOAT3(0.75f, 0.75f, 0.75f);
	//for (int i = 0; i < 3; i++) {
	//	hPict_[i] = Image::Load(fileName_[i] + ".png");
	//}
	ImageTrans.scale_ = XMFLOAT3{ 0.3f,0.3f,0.3f};
	gauge_ = Instantiate<Gauge>(this);
}

Player::~Player()
{
}

void Player::Update()
{
	Ready* pReady = FindGameObject<Ready>();
	if (pReady != nullptr && !pReady->Finished())return;
	switch (state) {
	case S_CHOOSE_ROTATE:
		Debug::Log("rotate",true);
		UpdateRotate();
		break;
	case S_CHOOSE_POWER:
		Debug::Log("pow",true);
		UpdatePower();
		break;
	case S_MOVE:
		Debug::Log("mov",true);
		UpdateMove();
		break;
	}
	//Debug::Log(direction, true);
	//Debug::Log(transform_.rotate_.y, true);

}

void Player::Draw()
{
	//mousePos = Input::GetMousePosition();
	//debug
	Text* pText = new Text;
	pText->Initialize();
	std::string str = "Mouse: " + std::to_string(mousePos.x) + " " + std::to_string(mousePos.y);
	pText->SetScale(0.5f);
	pText->Draw(20, 20, str.c_str());

	switch (state) {
	case S_CHOOSE_POWER:

		DrawPower();
	case S_CHOOSE_ROTATE:
		//DrawPower();
		DrawModel();
		break;
	}
	//transform_.position_を設定
	//白い球が原点にあるとして、白い球のどこに表示するか
	//そこに白い球の座標を足せば
	//表示座標が求まるため、transform_.position_に代入する
}

void Player::UpdateRotate()
{
	if (Input::IsKey(DIK_A))direction -= dirChangeVal;
	if (Input::IsKey(DIK_D))direction += dirChangeVal;
	transform_.rotate_.y = direction * 180 / M_PI;
	if (Input::IsKeyDown(DIK_SPACE)) {
		pSin = M_PI_2;
		state = S_CHOOSE_POWER;
	}
}

void Player::UpdatePower()
{
	if (Input::IsKey(DIK_SPACE)) {
		pSin += 0.05f;
		powerRatio = 1 - abs(sinf(pSin));
		
		Debug::Log("pSin: " + std::to_string(pSin), true);
		Debug::Log(std::to_string(sin(pSin)));
	}
	if (Input::IsKeyUp(DIK_SPACE)) {
		power = maxPow * powerRatio;
		//シュート
		XMVECTOR base = XMVectorSet(0, 0, power, 0);	//回転していない時に移動するベクトル
		XMMATRIX yrot = XMMatrixRotationY(direction);	//回転行列を作って
		XMVECTOR v = XMVector3Transform(base, yrot);	//その回転でベクトルの向きを変える
		myBall_->AddForce(v);							//回転後の移動ベクトル


		state = S_MOVE;
	}
}

void Player::UpdateMove()
{
	bool ballmoving = false;
	std::list<Ball*> balls = GetParent()->FindGameObjects<Ball>();	//全ボールのリスト
	for (std::list<Ball*>::iterator itr = balls.begin(); itr != balls.end(); itr++) {
		if ((*itr)->IsMove()) {
			ballmoving = true;
			break;
		}
	}
	if (!ballmoving)state = S_CHOOSE_ROTATE;
}

void Player::DrawModel()
{
	XMVECTOR base = XMVectorSet(0, 0, 1.2f, 0);		//回転していない時に移動するベクトル
	XMMATRIX yrot = XMMatrixRotationY(direction);	//回転行列を作って
	XMVECTOR v = XMVector3Transform(base, yrot);	//その回転でベクトルの向きを変える
	transform_.position_ = myBall_->GetPosition() + v;
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

XMFLOAT3 Player::ConvDrawPos(float x, float z) {
	XMFLOAT3 convPos;
	convPos.x = x / 37.0f;
	convPos.y = z / 20.7f;
	convPos.z = 0;
	return convPos;
}

void Player::DrawPower()
{

	Debug::Log("x:" + std::to_string(transform_.position_.x) + " z: " + std::to_string(transform_.position_.z), true);
	ImageTrans.position_ = ConvDrawPos(transform_.position_.x, transform_.position_.z) ;
	Debug::Log("ix:" + std::to_string(ImageTrans.position_.x) + " iy: " + std::to_string(ImageTrans.position_.y), true);
	gauge_->SetPosition(ImageTrans.position_.x, ImageTrans.position_.z);
	ImageTrans.rotate_.z = -transform_.rotate_.y + 90.0f;
	
	//for (int i = 0; i < 3; i++) {
	//	if (i == 1) {
	//		Transform tmpTrans = ImageTrans;
	//		tmpTrans.scale_.x *= powerRatio / 1.0f;
	//		Image::SetTransform(hPict_[i], tmpTrans);
	//	}
	//	else {
	//		Image::SetTransform(hPict_[i], ImageTrans);
	//	}
	//	Image::Draw(hPict_[i]);
	//}
}
