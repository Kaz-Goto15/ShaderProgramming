#pragma once
#include "Engine/GameObject.h"

//�n�ʂ��Ǘ�����N���X
class Arrow : public GameObject
{
    int hModel_;    //���f���ԍ�
public:
    Arrow(GameObject* parent);
    ~Arrow();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};