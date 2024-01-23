//───────────────────────────────────────
 // テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D		g_texture : register(t0);	//テクスチャー
SamplerState	g_sampler : register(s0);	//サンプラー

Texture2D		g_toon_texture : register(t1);
//───────────────────────────────────────
// コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global:register(b0)
{
	float4x4	g_matWVP;			// ワールド・ビュー・プロジェクションの合成行列
	float4x4	g_matNormal;        //法線行列
	float4x4	g_matW;				//ワールド変換行列
	float4		g_diffuseColor;		// ディフューズカラー（マテリアルの色） = 拡散反射係数
	float4		g_ambientColor;		//アンビエントカラー(影 0は影が消える)
	float4		g_specularColor;	//スペキュラカラー(ハイライト色 )
	float		g_shininess;		//ハイライトの強さ
	bool		g_isTextured;		// テクスチャ貼ってあるかどうか

};
cbuffer global:register(b1) {
	float4		g_lightPosition;	//ライト位置
	float4		g_eyePos;			//カメラ位置
}
//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
	float4 pos		: SV_POSITION;	//位置
	float2 uv		: TEXCOORD;		//UV座標
	float4 color	: COLOR;		//
	float4 eye		: POSITION;
	float4 normal	: NORMAL;
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
float4 VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL) :SV_POSITION
{
	normal.w = 0;
	pos = pos + normal * 0.05;
	pos = mul(pos, g_matWVP);

	return pos;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
	return float4(0,0,0,1);
}