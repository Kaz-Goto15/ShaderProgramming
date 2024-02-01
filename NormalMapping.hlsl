//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー
Texture2D   normalTex   :   register(t1);   //ノーマルマップテクスチャ

//───────────────────────────────────────
// コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global : register(b0)
{
    float4x4 matWVP; // ワールド・ビュー・プロジェクションの合成行列
    float4x4 matN; //法線変換行列
    float4x4 matW; //ワールド行列
    float4 diffuseColor; // ディフューズカラー（マテリアルの色）
    float4 ambientColor;
    float4 specularColor;
    float shininess;
    bool isTexture; // テクスチャ貼ってあるかどうか
    bool    isNormalMap;    //ノーマルマップ有無
};
cbuffer global : register(b1)
{
    float4 lightPos; //視点位置 = カメラ位置
    float4 eyePos; //光源座標、これをもとに平行光源にする
}

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
    float4 pos : SV_POSITION; //位置
    float2 uv : TEXCOORD; //UV座標
    float4 eyev : POSITION;
    float4 nEyev    :   POSITION1;
    float4 normal   :   POSITION2;
    float4 light    :   POSITION3;  //ライトを接空間に変換したベクトル
    float4 color    :   POSITION4;  //通常ランバートモデルの拡散反射の色
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
    outData.pos = mul(pos, matWVP);
    outData.uv = (float2)uv;

    float3 binormal = cross(tangent, normal);
    binormal = mul(binormal, matN);
    binormal = normalize(binormal); //従法線ベクトルをローカル座標に変換

    //法線を回転
    outData.normal = normalize(mul(normal, matN));
    outData.normal.w = 0;

    tangent = mul(tangent, matN);
    tangent = normalize(tangent);   //接線ベクトルをローカル座標に変換
    tangent.w = 0;

    float4 eye = normalize(mul(pos, matW) - eyePos);
    outData.eyev = eye;  //ワールド座標視線ベクトル

    outData.nEyev.x = dot(eye, tangent);   //接空間視線ベクトル
    outData.nEyev.y = dot(eye, binormal);
    outData.nEyev.z = dot(eye, outData.normal);
    outData.nEyev.w = 0;

    float4 light = normalize(lightPos);
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

    if (isNormalMap) {
        float4 tmpNormal = normalTex.Sample(g_sampler, inData.uv) * 2.0f - 1.0f;
        tmpNormal = normalize(tmpNormal);
        tmpNormal.w = 0;

        float4 NL = clamp(dot(normalize(inData.light), tmpNormal), 0, 1);
        float4 ref = reflect(normalize(inData.light), tmpNormal);
        specular = pow(saturate(dot(ref, normalize(inData.nEyev))), shininess) * specularColor;

        if (isTexture)
        {
            diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv) * NL;
            ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * ambientColor;
        }
        else
        {
            diffuse = lightSource * diffuseColor * NL;
            ambient = lightSource * diffuseColor * ambientColor;
        }

        return diffuse + ambient + specular;
    }
    else {
        float4 ref = reflect(normalize(lightPos), inData.normal);
        specular = pow(saturate(dot(ref, inData.eyev)), shininess) * specularColor;

        if (isTexture)
        {
            diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv) * inData.color;
            ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * ambientColor;
        }
        else
        {
            diffuse = lightSource * diffuseColor * inData.color;
            ambient = lightSource * diffuseColor * ambientColor;
        }

        //return specular;
        return (diffuse + ambient + specular);
    }

}