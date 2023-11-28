//������������������������������������������������������������������������������
 // �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D		g_texture : register(t0);	//�e�N�X�`���[
SamplerState	g_sampler : register(s0);	//�T���v���[

//������������������������������������������������������������������������������
// �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global
{
	float4x4	g_matWVP;				// ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
	float4x4	g_matNormal;          //�@���s��
	float4x4	g_matW;				//���[���h�ϊ��s��
	float4		g_lightDir;			//���C�g�̕����x�N�g��
	float4		g_diffuseColor;		// �f�B�t���[�Y�J���[�i�}�e���A���̐F�j = �g�U���ˌW��
	float4		g_ambientColor;		//�A���r�G���g�J���[(�e)
	float4		g_specularColor;		//�X�y�L�����J���[(�n�C���C�g�F)
	float4		g_eyePos;				//�J�����ʒu
	float4		g_shininess;			//�n�C���C�g�̋���
	bool		g_isTextured;		// �e�N�X�`���\���Ă��邩�ǂ���

};

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
	float4 pos  : SV_POSITION;	//�ʒu
	float2 uv	: TEXCOORD0;		//UV���W
	float4 eye	:	TEXCOORD1;
	float4 normal : TEXCOORD2;
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//�s�N�Z���V�F�[�_�[�֓n�����
	VS_OUT outData;

	//���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
	//�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
	outData.pos = mul(pos, matWVP);

	//�@����ό`
	normal.w = 0;							//4�����ڂ�0
	normal = mul(normal, matNormalTrans);	//�I�u�W�F�N�g�ό`�ɕ��і@�����ό`
	outData.normal = Normal;				//�s�N�Z���V�F�[�_��

	//�����x�N�g��
	float4 worldPos = mul(pos, matW);				//���[�J�����W�Ƀ��[���h�s����������[���h���W��
	outData.eye = normalize(vecCamPos - worldPos);	//���_���璸�_���������Z�A���������߃s�N�Z���V�F�[�_��
	
	//UV���W�͂��̂܂܃s�N�Z���V�F�[�_��
	outData.uv = uv;

	//�܂Ƃ߂ďo��
	return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
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
	//���C�g�̌���
	float4 lightDir = g_lightDir;	//�O���[�o���ϐ��͕ύX�ł��Ȃ��̂ŁA�������񃍁[�J���ϐ���
	lightDir = normalize(lightDir);	//�����������K�v�Ȃ̂Ő��K��

	//�@���̓s�N�Z���V�F�[�_�[�Ɏ����Ă������_�ŕ⊮���꒷�����ς���Ă���
	//���K�����Ă����Ȃ��Ɩʂ̖��邳�����������Ȃ�
	inData.normal = normalize(inData.normal);

	//�g�U���ˌ��i�f�B�t���[�Y�j
	//�@���ƌ��̃x�N�g���̓��ς��A�����̖��邳�ɂȂ�
	float4 shade = saturate(dot(inData.normal, - lightDir));
	shade.a = 1;	//�Â��Ƃ��낪�����ɂȂ�̂ŁA�����I�ɃA���t�@��1

	float4 diffuse;
	//�e�N�X�`���L���ŐF�w��
	if (g_isTexture)diffuse = g_texture.Sample(g_sampler, inData.uv);
	else			diffuse = g_diffuseColor;

	//�����i�A���r�G���g�j
	//�����Maya���Ŏw�肵�A�O���[�o���ϐ��Ŏ󂯎�������̂����̂܂�
	float4 ambient = float4(0.2, 0.2, 0.2, 1.0);

	//���ʔ��ˌ��i�X�y�L�����[�j
	float4 speculer = float4(0, 0, 0, 0);	//�Ƃ肠�����n�C���C�g�͖����ɂ��Ă����āc
	if (g_vecSpeculer.a != 0)	//�X�y�L�����[�̏�񂪂����
	{
		float4 R = reflect(lightDir, inData.normal);			//�����˃x�N�g��
		speculer = pow(saturate(dot(R, inData.eye)), g_shuniness) * g_vecSpeculer;	//�n�C���C�g�����߂�
	}

	//�ŏI�I�ȐF
	return diffuse * shade + diffuse * ambient + speculer;
}