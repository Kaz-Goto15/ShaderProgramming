#pragma once
#include "Engine/GameObject.h"

//地面を管理するクラス
class Water : public GameObject
{
    int hModel_;    //モデル番号
public:
    Water(GameObject* parent);
    ~Water();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};