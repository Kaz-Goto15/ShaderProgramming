#include "PlayScene.h"
#include "Ground.h"
#include "Ball.h"
#include "Engine/Camera.h"
//�R���X�g���N�^
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene")
{

}

//������
void PlayScene::Initialize()
{
	Instantiate<Ground>(this);
	Instantiate<Ball>(this);
	//Instantiate<Arrow>(this);
	pArrowX = (Arrow*)Instantiate<Arrow>(this);
	pArrowY = (Arrow*)Instantiate<Arrow>(this);
	pArrowZ = (Arrow*)Instantiate<Arrow>(this);
	pArrowY->SetRotate({ 0,0,90 });
	pArrowZ->SetRotate({ 0,-90,0 });
	Camera::SetPosition(XMVECTOR{ 3,6,-3,0});
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