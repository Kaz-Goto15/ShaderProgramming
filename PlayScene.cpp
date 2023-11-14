#include "PlayScene.h"
#include "Player.h"
#include "Enemy.h"
//コンストラクタ
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene")
{

}

//初期化
void PlayScene::Initialize()
{
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