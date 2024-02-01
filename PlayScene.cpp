#include "PlayScene.h"
#include "Ground.h"
#include "Ball.h"
#include "Engine/Camera.h"
#include "Controller.h"
#include "Torus.h"
#include "Engine/Input.h"
#include "Water.h"

namespace {
	const XMFLOAT4 DEF_LIGHT_POSITION{ 1, 2, 1, 0 };
}

void PlayScene::InitConstantBuffer()
{
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CBUFF_STAGESCENE);
	cb.Usage = D3D11_USAGE_DEFAULT;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = 0;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// コンスタントバッファの作成
	HRESULT hr;
	hr = Direct3D::pDevice_->CreateBuffer(&cb, nullptr, &pCBStageScene_);
	if (FAILED(hr))
	{
		MessageBox(NULL, "コンスタントバッファの作成に失敗しました", "エラー", MB_OK);
	}

}
//コンストラクタ
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene"),pCBStageScene_(nullptr), lightSourcePosition_(DEF_LIGHT_POSITION)
{

}

//初期化
void PlayScene::Initialize()
{
	pCtl = (Controller*)Instantiate<Controller>(this);
	//Instantiate<Torus>(this);
	Instantiate<Ball>(this);
	Instantiate<Water>(this);
	//pArrow = (Arrow*)Instantiate<Arrow>(this);
	//XMFLOAT3 BallPos = pCtl->GetPosition();
	//XMFLOAT3 BallPosNormal;
	//XMStoreFloat3(&BallPosNormal,
	//	XMVector3Normalize( XMLoadFloat3(&BallPos) )
	//);
	//XMVECTOR vx = XMVectorSet(1, 0, 0, 0);
	//XMVECTOR ballVecNormal = XMVector3Normalize(XMLoadFloat3(&BallPos));
	//XMVECTOR vxNormal = XMVector3Normalize(XMVectorSet(1, 0, 0, 0));
	//pArrow->SetRotate({
	//	acosf(XMVectorGetX(XMVector3Dot(ballVecNormal, vxNormal))),
	//	0, 0
	//	}
	//);
	//Instantiate<Ground>(this);
	//Instantiate<Ball>(this);
	//////Instantiate<Arrow>(this);
	//pArrowX = (Arrow*)Instantiate<Arrow>(this);
	//pArrowY = (Arrow*)Instantiate<Arrow>(this);
	//pArrowZ = (Arrow*)Instantiate<Arrow>(this);
	//pArrowY->SetRotate({ 0,0,90 });
	//pArrowZ->SetRotate({ 0,-90,0 });
	//Camera::SetPosition(XMVECTOR{ 3,6,-3,0});
	//Camera::SetTarget({ 0, 10, 0, 0 });

	//pBall = (Ball*)Instantiate<Ball>(this);
	//pBall->SetPosition(lightPos_);
	InitConstantBuffer();
}

//更新
void PlayScene::Update()
{
	XMFLOAT3 BallPos = pCtl->GetPosition();
	XMFLOAT3 BallPosNormal;
	XMStoreFloat3(&BallPosNormal,
		XMVector3Normalize(XMLoadFloat3(&BallPos))
	);

	//pArrow->SetRotate();
	XMVECTOR ballVecNormal = XMVector3Normalize(XMLoadFloat3(&BallPos));
	XMVECTOR vxNormal = XMVectorSet(1, 0, 0, 0);
	//XMVECTOR vxNormal = XMVector3Normalize(XMVectorSet(1, 0, 0, 0));
	//XMVECTOR vyNormal = XMVector3Normalize(XMVectorSet(0, 1, 0, 0));
	//XMVECTOR vzNormal = XMVector3Normalize(XMVectorSet(0, 0, 1, 0));

	//Input::IsKeyUp(DIK_SPACE) {
	//	Model::ChangeShader();
	//}


	lightPos_ = pCtl->GetPosition();
	CBUFF_STAGESCENE cb;
	cb.lightPosition = { lightPos_.x, lightPos_.y, lightPos_.z, 0 };
	//cb.lightPosition = Camera::GetTarget();
	XMStoreFloat4(&cb.eyePos, Camera::GetEyePosition());

	Direct3D::pContext_->UpdateSubresource(pCBStageScene_,
		0, NULL, &cb, 0, 0);

	Direct3D::pContext_->VSSetConstantBuffers(1, 1, &pCBStageScene_);	//頂点シェーダー用	
	Direct3D::pContext_->PSSetConstantBuffers(1, 1, &pCBStageScene_);	//ピクセルシェーダー用
}

//描画
void PlayScene::Draw()
{
}

//開放
void PlayScene::Release()
{

}