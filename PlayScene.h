#pragma once
#include "Engine/GameObject.h"
#include "Arrow.h"

class Ball;
class Controller;
struct CBUFF_STAGESCENE
{
	XMFLOAT4	lightPosition;
	XMFLOAT4	eyePos;
};

class PlayScene : public GameObject
{
	ID3D11Buffer* pCBStageScene_;
	XMFLOAT4	lightSourcePosition_;
	void InitConstantBuffer();
	Arrow* pArrow;
	//Arrow* pArrowX;
	//Arrow* pArrowY;
	//Arrow* pArrowZ;
	Ball* pBall;
	Controller* pCtl;
	XMFLOAT3 lightPos_;
	//Quad* q;
public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	PlayScene(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;
};