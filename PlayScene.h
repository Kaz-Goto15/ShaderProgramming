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
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	PlayScene(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};