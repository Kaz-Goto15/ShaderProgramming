#include "PlayScene.h"
#include "Player.h"
#include "Enemy.h"
//�R���X�g���N�^
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene")
{

}

//������
void PlayScene::Initialize()
{
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