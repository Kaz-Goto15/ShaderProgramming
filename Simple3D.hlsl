//───────────────────────────────────────
 // テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D		g_texture : register(t0);	//テクスチャー
SamplerState	g_sampler : register(s0);	//サンプラー

//───────────────────────────────────────
// コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global
{
	float4x4	g_matWVP;				// ワールド・ビュー・プロジェクションの合成行列
	float4x4	g_matNormal;          //法線行列
	float4x4	g_matW;				//ワールド変換行列
	float4		g_lightDir;			//ライトの方向ベクトル
	float4		g_diffuseColor;		// ディフューズカラー（マテリアルの色） = 拡散反射係数
	float4		g_ambientColor;		//アンビエントカラー(影)
	float4		g_specularColor;		//スペキュラカラー(ハイライト色)
	float4		g_eyePos;				//カメラ位置
	float4		g_shininess;			//ハイライトの強さ
	bool		g_isTextured;		// テクスチャ貼ってあるかどうか

};

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
	float4 pos  : SV_POSITION;	//位置
	float2 uv	: TEXCOORD0;		//UV座標
	float4 eye	:	TEXCOORD1;
	float4 normal : TEXCOORD2;
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//ピクセルシェーダーへ渡す情報
	VS_OUT outData;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
	outData.pos = mul(pos, matWVP);

	//法線を変形
	normal.w = 0;							//4次元目は0
	normal = mul(normal, matNormalTrans);	//オブジェクト変形に並び法線も変形
	outData.normal = Normal;				//ピクセルシェーダへ

	//視線ベクトル
	float4 worldPos = mul(pos, matW);				//ローカル座標にワールド行列をかけワールド座標へ
	outData.eye = normalize(vecCamPos - worldPos);	//視点から頂点情報を引き算、視線を求めピクセルシェーダへ
	
	//UV座標はそのままピクセルシェーダへ
	outData.uv = uv;

	//まとめて出力
	return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{

	float4 lightSource = float4(1.0, 1.0, 1.0, 1.0);
	float4 ambentSource = float4(0.2, 0.2, 0.2, 1.0);
	float4 diffuse;
	float4 ambient;
	float4 color;
	if (isTextured == false) {
		color = diffuseColor;
		//return diffuse + ambient;
	}
	else {
		color = g_texture.Sample(g_sampler, inData.uv);
	}
	diffuse = lightSource * color * inData.color;
	ambient = lightSource * color * ambentSource;	//return g_texture.Sample(g_sampler, inData.uv);
	return (diffuse + ambient);

	float4 light = normalize(eyePos - inData.pos);
	float4 normal = normalize(inData.normal);
	float4 reflection = 2 * normal * dot(normal, light) - light;

	/////////////////////////////////
	//ライトの向き
	float4 lightDir = g_lightDir;	//グルーバル変数は変更できないので、いったんローカル変数へ
	lightDir = normalize(lightDir);	//向きだけが必要なので正規化

	//法線はピクセルシェーダーに持ってきた時点で補完され長さが変わっている
	//正規化しておかないと面の明るさがおかしくなる
	inData.normal = normalize(inData.normal);

	//拡散反射光（ディフューズ）
	//法線と光のベクトルの内積が、そこの明るさになる
	float4 shade = saturate(dot(inData.normal, - lightDir));
	shade.a = 1;	//暗いところが透明になるので、強制的にアルファは1

	float4 diffuse;
	//テクスチャ有無で色指定
	if (g_isTexture)diffuse = g_texture.Sample(g_sampler, inData.uv);
	else			diffuse = g_diffuseColor;

	//環境光（アンビエント）
	//これはMaya側で指定し、グローバル変数で受け取ったものをそのまま
	float4 ambient = float4(0.2, 0.2, 0.2, 1.0);

	//鏡面反射光（スペキュラー）
	float4 speculer = float4(0, 0, 0, 0);	//とりあえずハイライトは無しにしておいて…
	if (g_vecSpeculer.a != 0)	//スペキュラーの情報があれば
	{
		float4 R = reflect(lightDir, inData.normal);			//正反射ベクトル
		speculer = pow(saturate(dot(R, inData.eye)), g_shuniness) * g_vecSpeculer;	//ハイライトを求める
	}

	//最終的な色
	return diffuse * shade + diffuse * ambient + speculer;
}