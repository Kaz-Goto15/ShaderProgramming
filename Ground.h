#pragma once
#include "Engine/GameObject.h"

//�n�ʂ��Ǘ�����N���X
class Ground : public GameObject
{
    int hModel_;    //���f���ԍ�
public:
    Ground(GameObject* parent);
    ~Ground();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};