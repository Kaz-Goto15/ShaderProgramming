#pragma once
#include "Engine/GameObject.h"

//地面を管理するクラス
class Ground : public GameObject
{
    int hModel_;    //モデル番号
public:
    Ground(GameObject* parent);
    ~Ground();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};