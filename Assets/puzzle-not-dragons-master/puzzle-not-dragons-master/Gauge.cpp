#include "Gauge.h"
#include "Engine/Image.h"
#include <assert.h>
//コンストラクタ
Gauge::Gauge(GameObject* parent):
    GameObject(parent, "Gauge"),
    maxPower_(1),
	nowPower_(0),
	animPower_(0)
{
}

//デストラクタ
Gauge::~Gauge()
{

}

//初期化
void Gauge::Initialize()
{
	for (int i = 0; i < 3; i++) {
		Powerict_[i] = Image::Load(fileName_[i] + ".png");
	}

    ImageWidth = 512;
    ImageHeight = 64;
}

//更新
void Gauge::Update()
{
	nowPower_ += 0.05f;
	if (nowPower_ > maxPower_)nowPower_ = 0.0f;
}

//描画
void Gauge::Draw()
{
	for (int i = 0; i < 3; i++) {
		if (i == 1) {
			int left = ImageWidth / 2 - ImageWidth / 2 * nowPower_;
			int width = ImageWidth * nowPower_;
			Image::SetRect(Powerict_[i], left, 0, width, ImageHeight);
		}

		Image::SetTransform(Powerict_[i], transform_);
		Image::Draw(Powerict_[i]);
	}
}

//開放
void Gauge::Release()
{
}

void Gauge::SetPosition(float x, float z)
{
	transform_.position_.x = x;
	transform_.position_.z = z;
}

void Gauge::AddValue(float v)
{
	v += nowPower_;
	SetValue(v);
	//nowPower_ += v;
	//if (nowPower_ < 0)nowPower_ = 0;
	//else if (nowPower_ > maxPower_)nowPower_ = maxPower_;
}

void Gauge::SetValue(float v)
{
	//assert(v >= 0.0f && v <= 1.0f);
	nowPower_ = v;
	if (nowPower_ < 0)nowPower_ = 0;
	else if (nowPower_ > maxPower_)nowPower_ = maxPower_;
}
