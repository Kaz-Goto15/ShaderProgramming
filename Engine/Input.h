#pragma once

#include <dInput.h>
#include <DirectXMath.h>
#include "Direct3D.h"
using namespace DirectX;

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dInput8.lib")

#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}
using namespace DirectX;

namespace Input
{
	HRESULT Initialize(HWND hWnd);
	void Update();

	bool IsKey(int keyCode);
	bool IsKeyDown(int keyCode);
	bool IsKeyUp(int keyCode);

	bool IsMouseButton(int buttonCode);
	bool IsMouseButtonDown(int buttonCode);
	bool IsMouseButtonUp(int buttonCode);

	XMFLOAT3 GetMousePosition();
	void SetMousePosition(int x, int y);
	//���̃t���[���ł̃}�E�X�̈ړ��ʂ��擾
	//�ߒl�FX,Y �}�E�X�̈ړ��ʁ@�^�@Z,�z�C�[���̉�]��
	XMFLOAT3 GetMouseMove();

	void Release();
};