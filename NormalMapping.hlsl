/*
//������������������������������������������������������������������������������
// �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D g_texture : register(t0); //�e�N�X�`���[
SamplerState g_sampler : register(s0); //�T���v���[
Texture2D   normalTex   :   register(t1);   //�m�[�}���}�b�v�e�N�X�`��

//������������������������������������������������������������������������������
// �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global : register(b0)
{
    float4x4 matWVP; // ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
    float4x4 matW; //���[���h�s��
    float4x4 matN; //�@���ϊ��s��
    float4 diffuseColor; // �f�B�t���[�Y�J���[�i�}�e���A���̐F�j
    float4 ambientColor;
    float4 specularColor;
    float shininess;
    bool isTexture; // �e�N�X�`���\���Ă��邩�ǂ���
    bool    isNormalMap;    //�m�[�}���}�b�v�L��
};
cbuffer global : register(b1)
{
    float4 camPos; //�J�������W
    float4 lightPos; //�������W�A��������Ƃɕ��s�����ɂ���
}

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
    float4 pos : SV_POSITION; //�ʒu
    float2 uv : TEXCOORD; //UV���W
    float4 eyev : POSITION;
    float4 nEyev    :   POSITION1;
    float4 normal   :   POSITION2;
    float4 light    :   POSITION3;  //���C�g��ڋ�Ԃɕϊ������x�N�g��
    float4 color    :   POSITION4;  //�ʏ탉���o�[�g���f���̊g�U���˂̐F
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL, float4 tangent : TANGENT)
{
    //�s�N�Z���V�F�[�_�[�֓n�����
    VS_OUT outData = (VS_OUT)0;

    //���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
    //�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
    outData.pos = mul(pos, matWVP);
    outData.uv = (float2)uv;

    float3 binormal = cross(normal, tangent);

    //�@������]
    normal.w = 0;
    normal = mul(normal, matN);
    normal = normalize(normal);
    outData.normal = normal;

    tangent.w = 0;
    tangent = mul(tangent, matN);
    tangent = normalize(tangent);   //�ڐ��x�N�g�������[�J�����W�ɕϊ�

    binormal = mul(binormal, matN);
    binormal = normalize(binormal); //�]�@���x�N�g�������[�J�����W�ɕϊ�

    float4 posw = mul(pos, matW); //�����x�N�g��
    outData.eyev = camPos - posw;

    outData.nEyev.x = dot(outData.eyev, tangent);   //�ڋ�Ԏ����x�N�g��
    outData.nEyev.y = dot(outData.eyev, binormal);
    outData.nEyev.z = dot(outData.eyev, normal); 
    outData.nEyev.w = 0;

    float4 light = normalize(lightPos);
    light = normalize(light);

    outData.color = mul(light, normal); //�����o�[�gdiffuse�p

    outData.light.x = dot(light, tangent);  //�ڋ�Ԍ����x�N�g��
    outData.light.y = dot(light, binormal);
    outData.light.z = dot(light, normal);
    outData.light.w = 0;

    //�܂Ƃ߂ďo��
    return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
    float4 lightsourse = float4(1.0, 1.0, 1.0, 0.0);
    float4 diffuse = { 0, 0, 0, 0 };
    float4 ambient = { 0, 0, 0, 0 };
    float4 specular = { 0, 0, 0, 0 };

    if (isNormalMap) {
        inData.light = normalize(inData.light);
        float4 tmpNormal = normalTex.Sample(g_sampler, inData.uv) * 2 - 1;
        tmpNormal.w = 0;
        tmpNormal = normalize(tmpNormal);

        float4 s = dot(tmpNormal, normalize(inData.light));
        s = clamp(s, 0, 1);

        float4 r = reflect(-inData.light, tmpNormal);
        specular = pow(saturate(dot(r, inData.nEyev)), shininess) * specularColor;

        if (isTexture)
        {
            diffuse = g_texture.Sample(g_sampler, inData.uv) * s;
            ambient = g_texture.Sample(g_sampler, inData.uv) * ambientColor;
        }
        else
        {
            diffuse = diffuseColor * s;
            ambient = diffuseColor * ambientColor;
        }

        return diffuse + ambient + specular;
    }
    else {
        float4 NL = saturate(dot(inData.normal, normalize(lightPos)));
        float4 reflection = reflect(normalize(-lightPos), inData.normal);
        specular = pow(saturate(dot(reflection, normalize(inData.eyev))), shininess) * specularColor;

        if (isTexture)
        {
            diffuse = lightsourse * g_texture.Sample(g_sampler, inData.uv) * inData.color;
            ambient = lightsourse * g_texture.Sample(g_sampler, inData.uv) * ambientColor;
        }
        else
        {
            diffuse = lightsourse * diffuseColor * inData.color;
            ambient = lightsourse * diffuseColor * ambientColor;
        }

        return (diffuse + ambient + specular);
    }

}
*/

//������������������������������������������������������������������������������
// �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D g_texture : register(t0); //�e�N�X�`���[
SamplerState g_sampler : register(s0); //�T���v���[
Texture2D   normalTex   :   register(t1);   //�m�[�}���}�b�v�e�N�X�`��

//������������������������������������������������������������������������������
// �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global : register(b0)
{
    float4x4 matWVP; // ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
    float4x4 matN; //�@���ϊ��s��
    float4x4 matW; //���[���h�s��
    float4 diffuseColor; // �f�B�t���[�Y�J���[�i�}�e���A���̐F�j
    float4 ambientColor;
    float4 specularColor;
    float shininess;
    bool isTexture; // �e�N�X�`���\���Ă��邩�ǂ���
    bool    isNormalMap;    //�m�[�}���}�b�v�L��
};
cbuffer global : register(b1)
{
    float4 eyePos; //���_�ʒu = �J�����ʒu
    float4 lightPos; //�������W�A��������Ƃɕ��s�����ɂ���
}

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
    float4 pos : SV_POSITION; //�ʒu
    float2 uv : TEXCOORD; //UV���W
    float4 eyev : POSITION;
    float4 nEyev    :   POSITION1;
    float4 normal   :   POSITION2;
    float4 light    :   POSITION3;  //���C�g��ڋ�Ԃɕϊ������x�N�g��
    float4 color    :   POSITION4;  //�ʏ탉���o�[�g���f���̊g�U���˂̐F
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL, float4 tangent : TANGENT)
{
    //�s�N�Z���V�F�[�_�[�֓n�����
    VS_OUT outData = (VS_OUT)0;

    //���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
    //�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
    outData.pos = mul(pos, matWVP);
    outData.uv = (float2)uv;

    float4 binormal = { cross(normal, tangent), 0 };
    binormal = mul(binormal, matN);
    binormal = normalize(binormal); //�]�@���x�N�g�������[�J�����W�ɕϊ�
    //�@������]
    normal.w = 0;
    normal = mul(normal, matN);
    normal = normalize(normal);
    normal.w = 0;
    outData.normal = normal;

    tangent.w = 0;
    tangent = mul(tangent, matN);
    tangent = normalize(tangent);   //�ڐ��x�N�g�������[�J�����W�ɕϊ�

    float4 posw = mul(pos, matW); //�����x�N�g��
    outData.eyev = normalize(posw - eyePos);

    outData.nEyev.x = dot(outData.eyev, tangent);   //�ڋ�Ԏ����x�N�g��
    outData.nEyev.y = dot(outData.eyev, binormal);
    outData.nEyev.z = dot(outData.eyev, normal);
    outData.nEyev.w = 0;

    float4 light = normalize(lightPos);
    light = normalize(light);

    outData.color = mul(light, normal); //�����o�[�gdiffuse�p
    outData.color.w = 0.0;
    outData.light.x = dot(light, tangent);  //�ڋ�Ԍ����x�N�g��
    outData.light.y = dot(light, binormal);
    outData.light.z = dot(light, normal);
    outData.light.w = 0;

    //�܂Ƃ߂ďo��
    return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
    float4 lightSourse = float4(1.0, 1.0, 1.0, 0.0);
    float4 diffuse = { 0, 0, 0, 0 };
    float4 ambient = { 0, 0, 0, 0 };
    float4 specular = { 0, 0, 0, 0 };

    if (isNormalMap) {
        inData.light = normalize(inData.light);
        float4 tmpNormal = normalTex.Sample(g_sampler, inData.uv) * 2 - 1;
        tmpNormal.w = 0;
        tmpNormal = normalize(tmpNormal);

        float4 NL = clamp(dot(tmpNormal, inData.light), 0, 1);
        float4 r = reflect(-inData.light, tmpNormal);
        specular = pow(saturate(dot(r, inData.nEyev)), shininess) * specularColor;

        if (isTexture)
        {
            diffuse = g_texture.Sample(g_sampler, inData.uv) * NL;
            ambient = g_texture.Sample(g_sampler, inData.uv) * ambientColor;
        }
        else
        {
            diffuse = diffuseColor * NL;
            ambient = diffuseColor * ambientColor;
        }

        //return specular;
        return diffuse + ambient + specular;
    }
    else {
        float4 reflection = reflect(normalize(lightPos), inData.normal);
        specular = pow(saturate(dot(normalize(reflection), inData.eyev)), shininess) * specularColor;

        if (isTexture)
        {
            diffuse = lightSourse * g_texture.Sample(g_sampler, inData.uv) * inData.color;
            ambient = lightSourse * g_texture.Sample(g_sampler, inData.uv) * ambientColor;
        }
        else
        {
            diffuse = lightSourse * diffuseColor * inData.color;
            ambient = lightSourse * diffuseColor * ambientColor;
        }

        //return specular;
        return (diffuse + ambient + specular);
    }

}