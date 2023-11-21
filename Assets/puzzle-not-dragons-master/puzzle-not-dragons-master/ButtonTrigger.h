#pragma once
#include "Button.h"
class ButtonTrigger : public Button
{
public:
    ButtonTrigger(GameObject* parent) : Button(parent) {}
    ~ButtonTrigger();
    void LoadImg(std::string fileName);
    void SetPosition(int x, int y);
    void SetScale(float x, float y);
    void Action();
private:
    XMFLOAT3 buttonSize, upperLeft;
    bool CheckMousePtr();
};

