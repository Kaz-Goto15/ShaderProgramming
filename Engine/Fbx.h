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
	//マテリアル
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
		XMMATRIX	matWVP;			//ワールド・ビュー・プロジェクションの合成行列
		XMMATRIX	matNormal;		//法線行列 スケール×平行移動の逆行列
		XMMATRIX	matW;			//ワールド変換行列
		XMFLOAT4	diffuseColor;	//マテリアルの色 FBXから取ってくる / 拡散反射係数
		XMFLOAT4	ambientColor;	//アンビエントカラー(影)
		XMFLOAT4	specularColor;	//スペキュラカラー(ハイライト色)
		FLOAT		shininess;		//ハイライトの強さ
		BOOL		isTextured;		// テクスチャ貼ってあるかどうか
		BOOL		isNormalTex;
	};
	struct VERTEX
	{
		XMVECTOR position;	//位置
		XMVECTOR uv;		//テクスチャ座標
		XMVECTOR normal;	//法線ベクトル
		XMVECTOR tangent;	//接線
	};

	VERTEX* pVertices_;
	int** ppIndex_;

	int vertexCount_;	//頂点数
	int polygonCount_;	//ポリゴン数
	int materialCount_;	//マテリアルの個数

	ID3D11Buffer* pVertexBuffer_;		//頂点バッファ
	ID3D11Buffer** pIndexBuffer_;		//インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ
	MATERIAL* pMaterialList_;
	vector<int>		indexCount_;		//インデックス数

	void InitVertex(fbxsdk::FbxMesh* mesh);//頂点バッファ準備
	void InitIndex(fbxsdk::FbxMesh* mesh);//インデックスバッファ準備
	void IntConstantBuffer();	//コンスタントバッファ準備
	void InitMaterial(fbxsdk::FbxNode* pNode);
	//各バッファをパイプラインにセット
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