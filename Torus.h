#pragma once
#include "Engine/GameObject.h"

//�n�ʂ��Ǘ�����N���X
class Torus : public GameObject
{
    int hModel_;    //���f���ԍ�
public:
    Torus(GameObject* parent);
    ~Torus();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};