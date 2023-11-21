#pragma once
#include "Button.h"
class ButtonBack : public Button
{
public:
    ButtonBack(GameObject* parent) : Button(parent) {}
    ~ButtonBack();
    void Action();
};

