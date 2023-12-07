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
		Texture*	pTexture;
		XMFLOAT4	diffuse;
		XMFLOAT4	ambient;		//環境光（アンビエント）への反射強度
		XMFLOAT4	specular;		//鏡面反射光（スペキュラ）への反射強度
		float		shininess;		//ハイライトの強さ
	};

	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;			//ワールド・ビュー・プロジェクションの合成行列
		XMMATRIX	matNormal;		//法線行列 スケール×平行移動の逆行列
		XMMATRIX	matW;			//ワールド変換行列
		XMFLOAT4	lightDirection;	//ライトの方向ベクトル
		XMFLOAT4	diffuseColor;	//マテリアルの色 FBXから取ってくる / 拡散反射係数
		XMFLOAT4	ambientColor;	//アンビエントカラー(影)
		XMFLOAT4	specularColor;	//スペキュラカラー(ハイライト色)
		FLOAT		shininess;		//ハイライトの強さ
		XMFLOAT4	eyePos;			//カメラ位置
		BOOL		isTextured;		// テクスチャ貼ってあるかどうか

	};

	struct VERTEX
	{
		XMVECTOR position;	//位置
		XMVECTOR uv;		//テクスチャ座標
		XMVECTOR normal;	//法線ベクトル
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
public:

	Fbx();
	HRESULT Load(std::string fileName);
	void	Draw(Transform& transform);
	void	Release();

	void RayCast(RayCastData& rayData);

	void SetLightSourcePosition(XMFLOAT4 pos) { lightSourcePosition_ = pos; }
};