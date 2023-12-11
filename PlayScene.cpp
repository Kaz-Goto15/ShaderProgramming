#include "PlayScene.h"
#include "Ground.h"
#include "Ball.h"
#include "Engine/Camera.h"
#include "Controller.h"
//#include "Torus.h"
#include "Engine/Model.h"

namespace {
	const XMFLOAT4 DEFAULT_LIGHT_POSITION{ 1,2,1,0 };
}
//コンストラクタ
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene"),
	pCBPlayScene_(nullptr),
	lightSourcePosition_(DEFAULT_LIGHT_POSITION),
	hModel_(-1)
{

}

//初期化
void PlayScene::Initialize()
{
	hModel_ = Model::Load("Assets/Torus.fbx");
	Model::SetTransform(hModel_, transform_);
	Instantiate<Controller>(this);
	//Instantiate<Torus>(this);
	Instantiate<Ground>(this);
	//Instantiate<Ball>(this);
	////Instantiate<Arrow>(this);
	//pArrowX = (Arrow*)Instantiate<Arrow>(this);
	//pArrowY = (Arrow*)Instantiate<Arrow>(this);
	//pArrowZ = (Arrow*)Instantiate<Arrow>(this);
	//pArrowY->SetRotate({ 0,0,90 });
	//pArrowZ->SetRotate({ 0,-90,0 });
	Camera::SetPosition(XMVECTOR{ 3,6,-3,0});
	//Camera::SetTarget({ 0, 10, 0, 0 });

	InitConstantBuffer();
}

void PlayScene::InitConstantBuffer()
{
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CBUFF_PLAYSCENE);
	cb.Usage = D3D11_USAGE_DEFAULT;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//cb.CPUAccessFlags = 0;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// コンスタントバッファの作成
	HRESULT hr;
	hr = Direct3D::pDevice_->CreateBuffer(&cb, nullptr, &pCBPlayScene_);
	if (FAILED(hr))	MessageBox(NULL, "コンスタントバッファの作成に失敗しました", "エラー", MB_OK);
}

//更新
void PlayScene::Update()
{
	CBUFF_PLAYSCENE cb;
	cb.lightPosition = lightSourcePosition_;
	XMStoreFloat4(&cb.eyePosition, Camera::GetEyePosition());

	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext_->Map(pCBPlayScene_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// データを値を送る
	Direct3D::pContext_->Unmap(pCBPlayScene_, 0);	//再開
	//Direct3D::pContext_->UpdateSubresource(pCBPlayScene_, 0, NULL, &cb, 0, 0);

	Direct3D::pContext_->VSSetConstantBuffers(1, 1, &pCBPlayScene_);	//頂点シェーダー用	
	Direct3D::pContext_->PSSetConstantBuffers(1, 1, &pCBPlayScene_);	//ピクセルシェーダー用
}

//描画
void PlayScene::Draw()
{
	Model::Draw(hModel_);
}

//開放
void PlayScene::Release()
{

}