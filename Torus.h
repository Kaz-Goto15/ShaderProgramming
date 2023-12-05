#pragma once
#include "Engine/GameObject.h"

//地面を管理するクラス
class Torus : public GameObject
{
    int hModel_;    //モデル番号
public:
    Torus(GameObject* parent);
    ~Torus();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};