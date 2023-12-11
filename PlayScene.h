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

	void SetLightPosition(XMFLOAT4& _pos) { lightSourcePosition_ = _pos; }
	XMFLOAT4 GetLightPosition() { return lightSourcePosition_; }
};