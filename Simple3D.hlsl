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
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//ピクセルシェーダーへ渡す情報
	VS_OUT outData = (VS_OUT)0;

	outData.pos = mul(pos, g_matWVP);	//ローカル座標にWVP行列かけスクリーン座標に変換

	outData.uv = uv;

	float4 light = normalize(g_lightPosition);
	light = normalize(light);
	outData.color = saturate(dot(normal, light));

	float4 worldPos = mul(pos, g_matW);				//ローカル座標にワールド行列をかけワールド座標へ
	outData.eye = normalize(g_eyePos - worldPos);	//視点から頂点情報を引き算、視線を求めピクセルシェーダへ

	//法線を変形
	normal.w = 0;							//4次元目は0
	normal = mul(normal, g_matNormal);	//オブジェクト変形に並び法線も変形
	normal = normalize(normal);
	outData.normal = normal;				//ピクセルシェーダへ

	//まとめて出力
	return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{

	float4 lightSource = float4(1.0, 1.0, 1.0, 1.0);	//ライト色&明るさ Iin
	//float4 ambientSource = float4(0.2, 0.2, 0.2, 1.0);	//アンビエント係数Ka

	float4 diffuse;
	float4 ambient;
	float4 NL = dot(inData.normal, normalize(g_lightPosition));				//その面の明るさ
	float4 reflect = normalize(2 * NL * inData.normal - normalize(g_lightPosition));
	float4 specular = pow(saturate(dot(reflect, normalize(inData.eye))), g_shininess) * g_specularColor;

	//トゥーンシェーダ
	struct TOON_DATA {
		float t;
		float4 color;
	};

	const int td_div = 4;	//分割数
	TOON_DATA td[td_div] = {//閾値、色を指定
		{0.2,(float4)0.1},
		{0.5,(float4)0.2},
		{0.8,(float4)0.5},
		{1.0,(float4)1.0},
	};
	float4 toonColor;
	for (int i = 0; i < td_div; i++) {
		if (inData.color.w < td[i].t) {
			toonColor = td[i].color;
			break;
		}
	}

	float2	uv;
	uv.x = inData.color.x;
	uv.y = 0;
	float4 tI = g_toon_texture.Sample(g_sampler, uv);
	//float4 toonColor;
	//if (inData.color.w < 0.2) {
	//	toonColor = 0;
	//}
	//else if (inData.color.w < 0.5) {
	//	toonColor = float4(0.2, 0.1, 0.8, 1);
	//}
	//else if (inData.color.w < 0.8) {
	//	toonColor = 0.5;
	//}
	//else{
	//	toonColor = 0.8;
	//}

	if (g_isTextured == 0) {
		diffuse = lightSource * g_diffuseColor * tI;
		ambient = lightSource * g_diffuseColor * g_ambientColor;
	}
	else {
		diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv) * tI;
		ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * g_ambientColor;
	}
	
	//輪郭 = 視線ベクトルと面の法線の
	if (abs(dot(inData.normal, normalize()))) {
		return float4(0, 0, 0, 0);
	}
	else {
		return float4(1, 1, 1, 0);
	}

	return diffuse + ambient;
	//return toonColor;
	//return g_shininess / 20.0f;
	//return diffuse + ambient + specular;
	//return ambient + specular;
	//return diffuse;
	//return ambient;
	//return specular;
	//float shininess = 8;
	//
	///////////////////////////////////
	////ライトの向き
	////float4 lightDir = g_lightDir;	//グルーバル変数は変更できないので、いったんローカル変数へ
	////lightDir = normalize(lightDir);	//向きだけが必要なので正規化
	//float4 lightDir = { 1,5,0,1 };
	//lightDir = normalize(lightDir);
	////法線はピクセルシェーダーに持ってきた時点で補完され長さが変わっているため正規化
	//inData.normal = normalize(inData.normal);
	//
	////拡散反射光（ディフューズ）
	////法線と光のベクトルの内積が、そこの明るさになる
	//float4 shade = saturate(dot(inData.normal, - lightDir));
	//shade.a = 1;	//暗いところが透明になるので、強制的にアルファは1
	//
	//float4 diffuse;
	////テクスチャ有無で色指定
	//if (g_isTextured)diffuse = g_texture.Sample(g_sampler, inData.uv);
	//else			diffuse = g_diffuseColor;
	//
	////環境光（アンビエント）
	////これはMaya側で指定し、グローバル変数で受け取ったものをそのまま
	//float4 ambient = ambientColor;
	//
	////鏡面反射光（スペキュラー）
	//float4 specular = float4(1, 1, 1, 0);	//とりあえずハイライトは無しにしておいて…
	////if (g_specularColor.a != 0)	//スペキュラーの情報があれば
	////{
	//	float4 R = reflect(lightDir, inData.normal);			//正反射ベクトル
	//	specular = pow(saturate(dot(R, inData.eye)), shininess) * lightSource;	//ハイライトを求める
	////}
	//
	////最終的な色
	//return diffuse * shade + diffuse * ambient + specular;
}
