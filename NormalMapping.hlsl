//������������������������������������������������������������������������������
// �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D       g_texture   : register(t0); //�e�N�X�`���[
SamplerState    g_sampler   : register(s0); //�T���v���[
Texture2D       g_normalTex : register(t1); //�m�[�}���}�b�v�e�N�X�`��

//������������������������������������������������������������������������������
// �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global : register(b0)
{
    float4x4    g_matWVP;           //���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
    float4x4    g_matN;             //�@���ϊ��s��
    float4x4    g_matW;             //���[���h�s��
    float4      g_diffuseColor;     //�f�B�t���[�Y�J���[�i�}�e���A���̐F�j
    float4      g_ambientColor;     //�A���r�G���g�J���[(�e 0�͉e��������)
    float4      g_specularColor;    //�X�y�L�����J���[(�n�C���C�g�F)
    float       g_shininess;        //�n�C���C�g�̋���
    bool        g_isTextured;        //�e�N�X�`���\���Ă��邩�ǂ���
    bool        g_isNormalMap;      //�m�[�}���}�b�v�L��
};

cbuffer global : register(b1)
{
    float4      g_lightPos;         //�������W
    float4      g_eyePos;           //���_�ʒu = �J�����ʒu
}

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
    float4 pos      :   SV_POSITION;//�ʒu
    float2 uv       :   TEXCOORD;   //UV���W
    float4 eyev     :   POSITION;   //�����x�N�g��(���[���h���W)
    float4 nEyev    :   POSITION1;  //�����x�N�g��(�m�[�}���}�b�v�p�ڋ��)
    float4 normal   :   NORMAL;     //�@���x�N�g��
    float4 light    :   POSITION2;  //���C�g��ڋ�Ԃɕϊ������x�N�g��
    float4 color    :   COLOR;      //�ʏ탉���o�[�g���f���̊g�U���˂̐F
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
    outData.pos = mul(pos, g_matWVP);
    outData.uv = (float2)uv;

    float4 binormal = { cross(tangent, normal), 0 };
    binormal = mul(binormal, g_matN);
    binormal = normalize(binormal); //�]�@���x�N�g�������[�J�����W�ɕϊ�
    //�@������]
    outData.normal = normalize(mul(normal, g_matN));
    outData.normal.w = 0;

    tangent = mul(tangent, g_matN);
    tangent = normalize(tangent);   //�ڐ��x�N�g�������[�J�����W�ɕϊ�
    tangent.w = 0;

    outData.eyev = normalize(mul(pos, g_matW) - g_eyePos);  //���[���h���W�����x�N�g��

    outData.nEyev.x = dot(outData.eyev, tangent);   //�ڋ�Ԏ����x�N�g��
    outData.nEyev.y = dot(outData.eyev, binormal);
    outData.nEyev.z = dot(outData.eyev, outData.normal);
    outData.nEyev.w = 0;

    float4 light = normalize(g_lightPos);
    light.w = 0;
    light = normalize(light);

    outData.color = mul(light, outData.normal); //�����o�[�gdiffuse�p
    outData.color.w = 0.0;

    outData.light.x = dot(light, tangent);  //�ڋ�Ԍ����x�N�g��
    outData.light.y = dot(light, binormal);
    outData.light.z = dot(light, outData.normal);
    outData.light.w = 0;

    //�܂Ƃ߂ďo��
    return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
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