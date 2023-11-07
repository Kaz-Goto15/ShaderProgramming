#pragma once
#include "Engine/Gameobject.h"

class Item : public GameObject
{
	int hModel;
public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	Item(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	//�����ɓ�������
//�����FpTarget ������������
	void OnCollision(GameObject* pTarget) override;
};