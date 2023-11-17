#pragma once
#include "Engine/GameObject.h"

//地面を管理するクラス
class Arrow : public GameObject
{
    int hModel_;    //モデル番号
public:
    Arrow(GameObject* parent);
    ~Arrow();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};