#pragma once
#include "Engine/GameObject.h"
#include "Engine/Text.h"

//テストシーンを管理するクラス
class BilliardsScene : public GameObject
{
private:
	XMFLOAT3 camPos_, camTarget_;	//カメラ 座標/タゲ
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	BilliardsScene(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};