#include "PlayScene.h"
#include "Engine/Camera.h"
#include "Controller.h"
#include "Engine/Input.h"
#include "SingleObject.h"


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

	// �R���X�^���g�o�b�t�@�̍쐬
	HRESULT hr;
	hr = Direct3D::pDevice_->CreateBuffer(&cb, nullptr, &pCBStageScene_);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
	}

}
//�R���X�g���N�^
PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene"),pCBStageScene_(nullptr), lightSourcePosition_(DEF_LIGHT_POSITION)
{

}

//������
void PlayScene::Initialize()
{
	pCtl = (Controller*)Instantiate<Controller>(this);

	pSO = (SingleObject*)Instantiate<SingleObject>(this);
	pSO->AddModel("Water.fbx");
	//pSO->AddModel("Torus.fbx");
	//pSO->AddModel("Ground.fbx");
	int ball = pSO->AddModel("ball.fbx");
	Transform ballTra;
	ballTra.position_ = { 1,1,1 };
	pSO->SetModelTransform(ball, ballTra);

	//Transform arrowTra;
	//arrowTra.scale_ = { 0.25f, 0.25f, 0.25f };
	//int arrowX = pSO->AddModel("Arrow.fbx");
	//pSO->SetModelTransform(arrowX, arrowTra);
	//arrowTra.rotate_ = { 0,0,90 };
	//int arrowY = pSO->AddModel("Arrow.fbx");
	//pSO->SetModelTransform(arrowY, arrowTra);
	//arrowTra.rotate_ = { 0,-90,0 };
	//int arrowZ = pSO->AddModel("Arrow.fbx");
	//pSO->SetModelTransform(arrowZ, arrowTra);




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
	//Camera::SetPosition(XMVECTOR{ 3,6,-3,0});
	//Camera::SetTarget({ 0, 10, 0, 0 });

	//pBall = (Ball*)Instantiate<Ball>(this);
	//pBall->SetPosition(lightPos_);
	InitConstantBuffer();
}

//�X�V
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

	Direct3D::pContext_->VSSetConstantBuffers(1, 1, &pCBStageScene_);	//���_�V�F�[�_�[�p	
	Direct3D::pContext_->PSSetConstantBuffers(1, 1, &pCBStageScene_);	//�s�N�Z���V�F�[�_�[�p
}

//�`��
void PlayScene::Draw()
{
}

//�J��
void PlayScene::Release()
{

}