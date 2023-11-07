#include "SceneManager.h"
#include "Model.h"
#include "../PlayScene.h"
#include "../TestScene.h"

SceneManager::SceneManager(GameObject* parent):GameObject(parent, "SceneManager")
{
}

void SceneManager::Initialize()
{
	currentSceneID_ = SCENE_ID_PLAY;
	nextSceneID_ = currentSceneID_;
	Instantiate<PlayScene>(this);
	//Instantiate<TestScene>(this);
}

void SceneManager::Update()
{
	if (currentSceneID_ != nextSceneID_) {
		auto scene = childList_.begin();
		(*scene)->ReleaseSub();
		SAFE_DELETE(*scene);
		childList_.clear();
		Model::Release();

		switch (nextSceneID_) {
		case SCENE_ID_TEST:
			Instantiate<TestScene>(this); break;
		case SCENE_ID_PLAY:
			Instantiate<PlayScene>(this); break;
		default:
			break;
		}
		currentSceneID_ = nextSceneID_;
	}
}

void SceneManager::Draw()
{
}

void SceneManager::Release()
{
}

/// <summary>
/// �w�肵���V�[����nextSceneID_�ɐݒ�
/// </summary>
/// <param name="_next">���̃V�[��</param>
void SceneManager::ChangeScene(SCENE_ID _next)
{//�V�[���؂�ւ�
	//���݂̂��|�C���ƌ��݂P�V�[���͐؂�ւ���
	nextSceneID_ = _next;
}
