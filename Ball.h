#pragma once
#include "Engine/GameObject.h"

//�n�ʂ��Ǘ�����N���X
class Ball : public GameObject
{
    int hModel_;    //���f���ԍ�
public:
    Ball(GameObject* parent);
    ~Ball();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};