#pragma once
#include "Engine/GameObject.h"

class Controller;
class SingleObject;

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
	Controller* pCtl;
	SingleObject* pSO;
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