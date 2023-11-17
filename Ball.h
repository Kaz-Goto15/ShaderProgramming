#pragma once
#include "Engine/GameObject.h"

//地面を管理するクラス
class Ball : public GameObject
{
    int hModel_;    //モデル番号
public:
    Ball(GameObject* parent);
    ~Ball();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};