#pragma once
#include "Engine/GameObject.h"
#include "Arrow.h"

struct CBUFF_PLAYSCENE {
	XMFLOAT4	lightPosition;
	XMFLOAT4	eyePosition;

};
class PlayScene : public GameObject
{
	Arrow* pArrowX;
	Arrow* pArrowY;
	Arrow* pArrowZ;
	//Quad* q;

	//struct CBUFF_PLAYSCENE {
	//	XMFLOAT4	lightPosition;
	//	XMFLOAT4	eyePosition;
	//};

	ID3D11Buffer* pCBPlayScene_;

	int hModel_;
	void InitConstantBuffer();
	XMFLOAT4 lightSourcePosition_;
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

	void SetLightPosition(XMFLOAT4& _pos) { lightSourcePosition_ = _pos; }
	XMFLOAT4 GetLightPosition() { return lightSourcePosition_; }
};