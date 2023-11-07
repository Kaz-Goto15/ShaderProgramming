#pragma once
#include "GameObject.h"

//�V�[���𗅗�
enum SCENE_ID {
	SCENE_ID_TEST = 0,
	SCENE_ID_PLAY
};
class SceneManager : public GameObject
{
	SCENE_ID currentSceneID_, nextSceneID_;
public:
	SceneManager(GameObject* parent);
	~SceneManager();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void ChangeScene(SCENE_ID _next);
};

