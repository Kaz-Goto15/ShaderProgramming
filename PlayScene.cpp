#include "PlayScene.h"
#include "Ground.h"
#include "Ball.h"
#include "Engine/Camera.h"
#include "Controller.h"
//コンストラクタ
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene")
{

}

//初期化
void PlayScene::Initialize()
{
	Instantiate<Controller>(this);
	Instantiate<Ground>(this);
	Instantiate<Ball>(this);
	//Instantiate<Arrow>(this);
	pArrowX = (Arrow*)Instantiate<Arrow>(this);
	pArrowY = (Arrow*)Instantiate<Arrow>(this);
	pArrowZ = (Arrow*)Instantiate<Arrow>(this);
	pArrowY->SetRotate({ 0,0,90 });
	pArrowZ->SetRotate({ 0,-90,0 });
	Camera::SetPosition(XMVECTOR{ 3,6,-3,0});
	Camera::SetTarget({ 0, 10, 0, 0 });
}

//更新
void PlayScene::Update()
{
	
}

//描画
void PlayScene::Draw()
{
}

//開放
void PlayScene::Release()
{

}