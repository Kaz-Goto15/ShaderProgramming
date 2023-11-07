#pragma once

#include <d3d11.h>
#include <fbxsdk.h>
#include <string>
#include "Transform.h"
#include "Camera.h"



#pragma comment(lib, "LibFbxSDK-MD.lib")
#pragma comment(lib, "LibXml2-MD.lib")
#pragma comment(lib, "zlib-MD.lib")
class Texture;

class Fbx
{
	//�}�e���A��
	struct MATERIAL
	{
		Texture* pTexture;
		XMFLOAT4	diffuse;
	};

	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;
		XMMATRIX	matNormal;	//matW����Normal�悤�ɉ���
		XMFLOAT4 diffuseColor;	//�F
		int isTexture;			//�e�N�X�`�����\���Ă��邩
	};

	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal;
	};

	int vertexCount_;	//���_��
	int indexNum_;//�C���f�b�N�X��
	int polygonCount_;	//�|���S����
	int materialCount_;	//�}�e���A���̌�

	ID3D11Buffer* pVertexBuffer_;		//���_�o�b�t�@
	ID3D11Buffer** pIndexBuffer_;		//�C���f�b�N�X�o�b�t�@
	ID3D11Buffer* pConstantBuffer_;		//�R���X�^���g�o�b�t�@
	MATERIAL* pMaterialList_;

public:

	Fbx();
	HRESULT Load(std::string fileName);
	void InitVertex(fbxsdk::FbxMesh* mesh);//���_�o�b�t�@����
	void InitIndex(fbxsdk::FbxMesh* mesh);//�C���f�b�N�X�o�b�t�@����
	void IntConstantBuffer();	//�R���X�^���g�o�b�t�@����
	void InitMaterial(fbxsdk::FbxNode* pNode);

	void Draw(Transform& transform);

	//	void PassDataToCB(XMMATRIX worldMatrix);
		//�e�o�b�t�@���p�C�v���C���ɃZ�b�g
	void SetBufferToPipeline();
	void Release();
};