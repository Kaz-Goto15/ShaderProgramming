#include "PlayScene.h"
#include "Ground.h"
#include "Ball.h"
//�R���X�g���N�^
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene")
{

}

//������
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

//�X�V
void PlayScene::Update()
{
	
}

//�`��
void PlayScene::Draw()
{
}

//�J��
void PlayScene::Release()
{

}