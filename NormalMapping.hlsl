//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D       g_texture   : register(t0); //テクスチャー
SamplerState    g_sampler   : register(s0); //サンプラー
Texture2D       g_normalTex : register(t1); //ノーマルマップテクスチャ

//───────────────────────────────────────
// コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global : register(b0)
{
    float4x4    g_matWVP;           //ワールド・ビュー・プロジェクションの合成行列
    float4x4    g_matN;             //法線変換行列
    float4x4    g_matW;             //ワールド行列
    float4      g_diffuseColor;     //ディフューズカラー（マテリアルの色）
    float4      g_ambientColor;     //アンビエントカラー(影 0は影が消える)
    float4      g_specularColor;    //スペキュラカラー(ハイライト色)
    float       g_shininess;        //ハイライトの強さ
    bool        g_isTextured;        //テクスチャ貼ってあるかどうか
    bool        g_isNormalMap;      //ノーマルマップ有無
};

cbuffer global : register(b1)
{
    float4      g_lightPos;         //光源座標
    float4      g_eyePos;           //視点位置 = カメラ位置
}

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
    float4 pos      :   SV_POSITION;//位置
    float2 uv       :   TEXCOORD;   //UV座標
    float4 eyev     :   POSITION;   //視線ベクトル(ワールド座標)
    float4 nEyev    :   POSITION1;  //視線ベクトル(ノーマルマップ用接空間)
    float4 normal   :   NORMAL;     //法線ベクトル
    float4 light    :   POSITION2;  //ライトを接空間に変換したベクトル
    float4 color    :   COLOR;      //通常ランバートモデルの拡散反射の色
};
//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL, float4 tangent : TANGENT)
{
    //ピクセルシェーダーへ渡す情報
    VS_OUT outData = (VS_OUT)0;

    //ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
    //スクリーン座標に変換し、ピクセルシェーダーへ
    outData.pos = mul(pos, g_matWVP);
    outData.uv = (float2)uv;

    float4 binormal = { cross(tangent, normal), 0 };
    binormal = mul(binormal, g_matN);
    binormal = normalize(binormal); //従法線ベクトルをローカル座標に変換
    //法線を回転
    outData.normal = normalize(mul(normal, g_matN));
    outData.normal.w = 0;

    tangent = mul(tangent, g_matN);
    tangent = normalize(tangent);   //接線ベクトルをローカル座標に変換
    tangent.w = 0;

    outData.eyev = normalize(mul(pos, g_matW) - g_eyePos);  //ワールド座標視線ベクトル

    outData.nEyev.x = dot(outData.eyev, tangent);   //接空間視線ベクトル
    outData.nEyev.y = dot(outData.eyev, binormal);
    outData.nEyev.z = dot(outData.eyev, outData.normal);
    outData.nEyev.w = 0;

    float4 light = normalize(g_lightPos);
    light.w = 0;
    light = normalize(light);

    outData.color = mul(light, outData.normal); //ランバートdiffuse用
    outData.color.w = 0.0;

    outData.light.x = dot(light, tangent);  //接空間光源ベクトル
    outData.light.y = dot(light, binormal);
    outData.light.z = dot(light, outData.normal);
    outData.light.w = 0;

    //まとめて出力
    return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
    float4 lightSource = float4(1.0, 1.0, 1.0, 0.0);
    float4 diffuse = { 0, 0, 0, 0 };
    float4 ambient = { 0, 0, 0, 0 };
    float4 specular = { 0, 0, 0, 0 };

    if (g_isNormalMap) {
        float4 tmpNormal = g_normalTex.Sample(g_sampler, inData.uv) * 2.0f - 1.0f;
        tmpNormal = normalize(tmpNormal);
        tmpNormal.w = 0;

        float4 NL = clamp(dot(normalize(inData.light), tmpNormal), 0, 1);
        float4 r = reflect(normalize(inData.light), tmpNormal);
        specular = pow(saturate(dot(r, normalize(inData.nEyev))), g_shininess) * g_specularColor;

        if (g_isTextured)
        {
            diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv) * NL;
            ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * g_ambientColor;
        }
        else
        {
            diffuse = lightSource * g_diffuseColor * NL;
            ambient = lightSource * g_diffuseColor * g_ambientColor;
        }

        float4 result = diffuse + ambient + specular;
        result.a = (result.r + result.g + result.b) / 3.0f;
        return result;

    }
    else {
        float4 r = reflect(normalize(g_lightPos), inData.normal);
        specular = pow(saturate(dot(r, inData.eyev)), g_shininess) * g_specularColor;

        if (g_isTextured)
        {
            diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv) * inData.color;
            ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * g_ambientColor;
        }
        else
        {
            diffuse = lightSource * g_diffuseColor * inData.color;
            ambient = lightSource * g_diffuseColor * g_ambientColor;
        }
        float4 result = diffuse + ambient + specular;
        result.a = (result.r + result.g + result.b) / 3.0f;
        return result;
    }

}