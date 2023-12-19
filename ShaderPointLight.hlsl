//───────────────────────────────────────
 // テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D		g_texture : register(t0);	//テクスチャー
SamplerState	g_sampler : register(s0);	//サンプラー

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

struct PS_IN
{
	float4 pos		: SV_POSITION;	//位置
	float2 uv		: TEXCOORD;		//UV座標
	float4 eye		: POSITION;
	float4 normal	: NORMAL;
	float4 light	: LIGHT;
};


//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
PS_IN VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//ピクセルシェーダーへ渡す情報
	VS_OUT outData = (VS_OUT)0;

	outData.pos = mul(pos, g_matWVP);	//ローカル座標にWVP行列かけスクリーン座標に変換

	outData.uv = uv;

	float4 worldPos = mul(pos, g_matW);				//ローカル座標にワールド行列をかけワールド座標へ
	outData.eye = normalize(g_eyePos - worldPos);	//視点から頂点情報を引き算、視線を求めピクセルシェーダへ

	//法線を変形
	normal.w = 0;							//4次元目は0
	normal = mul(normal, g_matNormal);	//オブジェクト変形に並び法線も変形
	normal = normalize(normal);
	outData.normal = normal;				//ピクセルシェーダへ

	outData.light = normalize(g_lightPosition - worldPos);

	//まとめて出力
	return outData;

}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(PS_IN inData) : SV_Target
{

	float len = length(inData.light); // 光の方向ベクトルを正規化(大きさを 1 にし

	float NL = saturate(dot(inData.normal, inData.light));
	float k = 1.0f / (1.0 * len * len);

	float3 reflect = normalize(2 * NL * inData.normal - inData.light);
	float4 specular = pow(saturate(dot(reflect, inData.eyev)), 8);


	float4 lightSource = { 1,1,1,1 };//光の色
	float4 ambientSource = { 1,1,1,1 };//環境光の色
	float4 ambTerm = 0.5;

	float4 diffuse;
	float4 ambient;
	if (isTextured.x == false)
	{
		diffuse = k * NL * lightSource * diffuseColor;
		ambient = ambTerm * ambientSource * diffuseColor;
	}
	else
	{
		diffuse = k * NL * lightSource * g_texture.Sample(g_sampler, inData.uv);
		ambient = ambTerm * ambientSource * g_texture.Sample(g_sampler, inData.uv);
	}

	return  diffuse + ambient + specular;
}
