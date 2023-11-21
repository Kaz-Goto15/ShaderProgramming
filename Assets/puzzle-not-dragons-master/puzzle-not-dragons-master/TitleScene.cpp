#include "TitleScene.h"
#include "Button.h"
#include "ButtonTrigger.h"
#include "ButtonBack.h"
#include "Engine/Input.h"

//コンストラクタ
TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene")
{
}

//初期化
void TitleScene::Initialize()
{
	Button* b = Instantiate<ButtonBack>(this);
	b->LoadImg("CommonButtonBackDisable.png");
	b->SetPosition(640, 260);
	b->SetScale(0.5f,0.5f);
	Button* b2 = Instantiate<ButtonTrigger>(this);
	b2->LoadImg("CommonButtonBack.png");
	b2->SetPosition(640, 260);
	b2->SetScale(0.5f, 0.5f);
	Button* b3 = Instantiate<ButtonBack>(this);
	b3->LoadImg("FoodButtonGiveDisable.png");
	b3->SetPosition(640, 460);
	b3->SetScale(0.5f, 0.5f);
	Button* b4 = Instantiate<ButtonTrigger>(this);
	b4->LoadImg("FoodButtonGive.png");
	b4->SetPosition(640, 460);
	b4->SetScale(0.5f, 0.5f);

}

//更新
void TitleScene::Update()
{

}

//描画
void TitleScene::Draw()
{
}

//開放
void TitleScene::Release()
{
}
