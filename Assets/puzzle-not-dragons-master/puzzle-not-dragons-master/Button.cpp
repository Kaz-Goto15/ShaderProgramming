#include "Button.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/Direct3D.h"
//�R���X�g���N�^
Button::Button(GameObject* parent):
    GameObject(parent, "Button"),
    hPict_(-1)
{
}

//�f�X�g���N�^
Button::~Button()
{
}

//������
void Button::Initialize()
{
}

//�X�V
void Button::Update()
{
    Action();
}

//�`��
void Button::Draw()
{
    Image::SetTransform(hPict_, transform_);
    Image::Draw(hPict_);
}

//�J��
void Button::Release()
{
}
void Button::SetPosition(int x, int y)
{
    transform_.position_.x = (float)(x - Direct3D::screenWidth_ / 2) / Direct3D::screenWidth_;
    transform_.position_.y = -(float)(y - Direct3D::screenHeight_ / 2) / (Direct3D::screenHeight_ / 2);
    center = XMFLOAT3(x, y, 0);

}

void Button::SetScale(float x, float y)
{
    transform_.scale_.x = x;
    transform_.scale_.y = y;

}

void Button::LoadImg(std::string fileName)
{
    //�摜�f�[�^�̃��[�h
    hPict_ = Image::Load(fileName);
    assert(hPict_ >= 0);
}

void Button::Action()
{
    //if (Input::IsMouseButtonUp(0)) {
    //    this->KillMe();
    //}
}
