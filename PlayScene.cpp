#include "PlayScene.h"
#include "Ground.h"
#include "Ball.h"
//コンストラクタ
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene")
{

}

//初期化
void PlayScene::Initialize()
{
	//Instantiate<Ground>(this);
	//Instantiate<Ball>(this);
	//Instantiate<Arrow>(this);
	arrowX = new Arrow(this);
	arrowY = new Arrow(this);
	arrowZ = new Arrow(this);
	//Player* pPlayer;
	//pPlayer = new Player(this);
	//pPlayer->Initialize();
	//childList_.push_back(pPlayer);
	//Quad* q = Instantiate<Quad>(this);
	//Instantiate<Enemy>(this);
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