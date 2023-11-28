#include "TestScene.h"
#include "Engine/SceneManager.h"
#include "Engine/Input.h"
#include "Controller.h"
TestScene::TestScene(GameObject* parent):GameObject(parent, "TestScene")
{
}

void TestScene::Initialize()
{
	Instantiate<Controller>(this);
}

void TestScene::Update()
{
	//GameObject* dbgTest = FindObject("SceneManager");
	if (Input::IsKey(DIK_ESCAPE)) {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY);
	}
}

void TestScene::Draw()
{
}

void TestScene::Release()
{
}
