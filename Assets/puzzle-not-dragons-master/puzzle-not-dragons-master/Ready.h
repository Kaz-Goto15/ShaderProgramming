#pragma once
#include "Engine/GameObject.h"

class Ready : public GameObject
{
public:
	Ready(GameObject* parent);
	~Ready();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	bool Finished();		//ready‚ªI‚í‚Á‚½‚çtrue
private:
	int hImage;
	float EaseOutExpo(float x);
	float EaseInExpo(float x);

};

