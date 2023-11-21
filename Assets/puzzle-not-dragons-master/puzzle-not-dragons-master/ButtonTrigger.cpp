#include "ButtonTrigger.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
//デストラクタ
ButtonTrigger::~ButtonTrigger()
{
}

void ButtonTrigger::LoadImg(std::string fileName)
{
	Button::LoadImg(fileName);
	buttonSize = Image::GetSize(hPict_);
}

void ButtonTrigger::SetPosition(int x, int y)
{
	Button::SetPosition(x, y);
	upperLeft = XMFLOAT3(
		center.x - buttonSize.x / 2/(transform_.scale_.x),
		center.y - buttonSize.y / 2 / (transform_.scale_.y),
		0);
}

void ButtonTrigger::SetScale(float x, float y)
{
	Button::SetScale(x, y);
	upperLeft = XMFLOAT3(
		center.x - buttonSize.x / (2 / (transform_.scale_.x)),
		center.y - buttonSize.y / (2 / (transform_.scale_.y)),
		0);
}

void ButtonTrigger::Action()
{
	if (Input::IsMouseButton(0)) {
		if (CheckMousePtr())Image::SetAlpha(hPict_, 0);
		else Image::SetAlpha(hPict_, 255);
	}
	if (Input::IsMouseButtonUp(0)) {
		if (CheckMousePtr())Image::SetAlpha(hPict_, 0);
	}
}

bool ButtonTrigger::CheckMousePtr()
{
	XMFLOAT3 mousept = Input::GetMousePosition();
	if (upperLeft.x <= mousept.x && mousept.x <= buttonSize.x &&
		upperLeft.y <= mousept.y && mousept.y <= buttonSize.y) {
		return true;
	}
	return false;
}
