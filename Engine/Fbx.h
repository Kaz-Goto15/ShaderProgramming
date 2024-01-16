#pragma once

#include <d3d11.h>
#include <fbxsdk.h>
#include <string>
#include "Transform.h"
#include "Camera.h"
#include <vector>


#pragma comment(lib, "LibFbxSDK-MD.lib")
#pragma comment(lib, "LibXml2-MD.lib")
#pragma comment(lib, "zlib-MD.lib")

using std::vector;

enum RENDER_STATE
{
	RENDER_DIRLIGHT,
	RENDER_PNTLIGHT,
};

class Texture;

struct RayCastData {
	XMFLOAT4 start;
	XMFLOAT4 dir;
	bool hit;
	float dist;

	RayCastData() { dist = 99999.0f; }
};

class Fbx
{
	//�}�e���A��
	struct MATERIAL
	{
		Texture* pTexture;
		Texture* pNormalMap;
		XMFLOAT4 diffuse;
		XMFLOAT4 ambient;
		XMFLOAT4 specular;
		float	 shininess;
	};

	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;			//���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
		XMMATRIX	matNormal;		//�@���s�� �X�P�[���~���s�ړ��̋t�s��
		XMMATRIX	matW;			//���[���h�ϊ��s��
		XMFLOAT4	diffuseColor;	//�}�e���A���̐F FBX�������Ă��� / �g�U���ˌW��
		XMFLOAT4	ambientColor;	//�A���r�G���g�J���[(�e)
		XMFLOAT4	specularColor;	//�X�y�L�����J���[(�n�C���C�g�F)
		FLOAT		shininess;		//�n�C���C�g�̋���
		BOOL		isTextured;		// �e�N�X�`���\���Ă��邩�ǂ���
		BOOL		isNormalTex;
	};
	struct VERTEX
	{
		XMVECTOR position;	//�ʒu
		XMVECTOR uv;		//�e�N�X�`�����W
		XMVECTOR normal;	//�@���x�N�g��
		XMVECTOR tangent;	//�ڐ�
	};

	VERTEX* pVertices_;
	int** ppIndex_;

	int vertexCount_;	//���_��
	int polygonCount_;	//�|���S����
	int materialCount_;	//�}�e���A���̌�

	ID3D11Buffer* pVertexBuffer_;		//���_�o�b�t�@
	ID3D11Buffer** pIndexBuffer_;		//�C���f�b�N�X�o�b�t�@
	ID3D11Buffer* pConstantBuffer_;	//�R���X�^���g�o�b�t�@
	MATERIAL* pMaterialList_;
	vector<int>		indexCount_;		//�C���f�b�N�X��

	void InitVertex(fbxsdk::FbxMesh* mesh);//���_�o�b�t�@����
	void InitIndex(fbxsdk::FbxMesh* mesh);//�C���f�b�N�X�o�b�t�@����
	void IntConstantBuffer();	//�R���X�^���g�o�b�t�@����
	void InitMaterial(fbxsdk::FbxNode* pNode);
	//�e�o�b�t�@���p�C�v���C���ɃZ�b�g
	//void SetBufferToPipeline();

	XMFLOAT4 lightSourcePosition_ = {0,0,0,0};
	RENDER_STATE state_;
public:

	Fbx();
	HRESULT Load(std::string fileName);
	void	Draw(Transform& transform);
	void	Release();

	void RayCast(RayCastData& rayData);

	void SetLightSourcePosition(XMFLOAT4 pos) { lightSourcePosition_ = pos; }
	void	SetRenderingShader(RENDER_STATE _state) { state_ = _state; }
};