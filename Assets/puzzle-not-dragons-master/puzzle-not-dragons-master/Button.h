#pragma once
#include "Engine/GameObject.h"

//ステージを管理するクラス
class Button : public GameObject
{

public:
    Button(GameObject* parent);
    ~Button();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    virtual void SetPosition(int x, int y);
    virtual void SetScale(float x, float y);
    virtual void LoadImg(std::string fileName);
    virtual void Action();
protected:
    int hPict_;
    XMFLOAT3 center;
};