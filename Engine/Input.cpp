#include "Input.h"
#include <string>
namespace Input
{
	LPDIRECTINPUT8   pDInput = nullptr;
	LPDIRECTINPUTDEVICE8 pKeyDevice = nullptr;
	BYTE keyState[256] = { 0 };
	BYTE prevState[256] = { 0 };
	//�}�E�X
	LPDIRECTINPUTDEVICE8	pMouseDevice;	//�f�o�C�X�I�u�W�F�N�g
	DIMOUSESTATE mouseState;				//�}�E�X�̏��
	DIMOUSESTATE prevMouseState;			//�O�t���[���̃}�E�X�̏��
	POINT mousePosition;				//�}�E�X�J�[�\���̈ʒu

	HRESULT Initialize(HWND hWnd)
	{
		HRESULT hr;
		hr = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, nullptr);
		if (FAILED(hr)) {
			MessageBox(NULL, "DirectInput8�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		}
		hr = pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, nullptr);
		if (FAILED(hr)) {
			MessageBox(NULL, "Input Device�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		}

		pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
		pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

		//�}�E�X
		pDInput->CreateDevice(GUID_SysMouse, &pMouseDevice, nullptr);
		pMouseDevice->SetDataFormat(&c_dfDIMouse);
		pMouseDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

		return S_OK;
	}

	void Update()
	{
		int tes2 = sizeof(keyState) / sizeof(BYTE);
		memcpy(prevState, keyState, tes2);

		pKeyDevice->Acquire();
		pKeyDevice->GetDeviceState(sizeof(keyState), &keyState);

		//�}�E�X
		pMouseDevice->Acquire();
		memcpy(&prevMouseState, &mouseState, sizeof(mouseState));
		pMouseDevice->GetDeviceState(sizeof(mouseState), &mouseState);

	}

	bool IsKey(int keyCode)
	{
		if (keyState[keyCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	bool IsKeyDown(int keyCode)
	{
		//���͉����ĂāA�O��͉����ĂȂ�
		if (!(prevState[keyCode] & 0x80) && IsKey(keyCode))
		{
			return true;
		}
		return false;
	}

	bool IsKeyUp(int keyCode)
	{
		//���͉����ĂȂ��A�O��͉����Ă���
		if ((prevState[keyCode] & 0x80) && !IsKey(keyCode))
		{
			return true;
		}
		return false;
	}

	bool IsMouseButton(int buttonCode)
	{
		//�����Ă�
		if (mouseState.rgbButtons[buttonCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	bool IsMouseButtonDown(int buttonCode)
	{
		//���͉����ĂāA�O��͉����ĂȂ�
		if (IsMouseButton(buttonCode) && !(prevMouseState.rgbButtons[buttonCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	bool IsMouseButtonUp(int buttonCode)
	{
		//���͉����ĂȂ��A�O��͉����Ă���
		if (!IsMouseButton(buttonCode) && (prevMouseState.rgbButtons[buttonCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	XMFLOAT3 GetMousePosition()
	{
		XMFLOAT3 result = XMFLOAT3((float)mousePosition.x, (float)mousePosition.y, 0);
		return result;
	}

	void SetMousePosition(int x, int y)
	{
		mousePosition.x = x;
		mousePosition.y = y;
		//std::string resStr = std::to_string(x) + ", " + std::to_string(y) + "\n";
		//OutputDebugString(resStr.c_str());
	}

	XMFLOAT3 GetMouseMove()
	{
		XMFLOAT3 result = XMFLOAT3(
			(float)mouseState.lX,
			(float)mouseState.lY,
			(float)mouseState.lZ - (float)prevMouseState.lZ
		);
		return result;
	}

	void Release()
	{
		SAFE_RELEASE(pKeyDevice);
		SAFE_RELEASE(pDInput);
	}
}