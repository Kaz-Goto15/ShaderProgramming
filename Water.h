#pragma once
#include "Engine/GameObject.h"

//�n�ʂ��Ǘ�����N���X
class Water : public GameObject
{
    int hModel_;    //���f���ԍ�
public:
    Water(GameObject* parent);
    ~Water();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};