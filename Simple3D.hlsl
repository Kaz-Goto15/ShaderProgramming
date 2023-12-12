//������������������������������������������������������������������������������
 // �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D		g_texture : register(t0);	//�e�N�X�`���[
SamplerState	g_sampler : register(s0);	//�T���v���[

//������������������������������������������������������������������������������
// �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global:register(b0)
{
	float4x4	g_matWVP;			// ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
	float4x4	g_matNormal;        //�@���s��
	float4x4	g_matW;				//���[���h�ϊ��s��
	float4		g_diffuseColor;		// �f�B�t���[�Y�J���[�i�}�e���A���̐F�j = �g�U���ˌW��
	//float4		g_ambientColor;		//�A���r�G���g�J���[(�e)
	//float4		g_specularColor;	//�X�y�L�����J���[(�n�C���C�g�F)
	//float		g_shininess;		//�n�C���C�g�̋���
	bool		g_isTextured;		// �e�N�X�`���\���Ă��邩�ǂ���

};
cbuffer global:register(b1) {
	float4		g_lightPosition;	//���C�g�ʒu
	float4		g_eyePos;			//�J�����ʒu
}
//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
	float4 pos		: SV_POSITION;	//�ʒu
	float2 uv		: TEXCOORD;		//UV���W
	float4 color	: COLOR;		//
	float4 eye		: POSITION;
	float4 normal	: NORMAL;
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//�s�N�Z���V�F�[�_�[�֓n�����
	VS_OUT outData = (VS_OUT)0;

	outData.pos = mul(pos, g_matWVP);	//���[�J�����W��WVP�s�񂩂��X�N���[�����W�ɕϊ�

	outData.uv = uv;

	float4 light = normalize(g_lightPosition);
	light = normalize(light);
	outData.color = saturate(dot(normal, light));

	float4 worldPos = mul(pos, g_matW);				//���[�J�����W�Ƀ��[���h�s����������[���h���W��
	outData.eye = normalize(g_eyePos - worldPos);	//���_���璸�_���������Z�A���������߃s�N�Z���V�F�[�_��

	//�@����ό`
	normal.w = 0;							//4�����ڂ�0
	normal = mul(normal, g_matNormal);	//�I�u�W�F�N�g�ό`�ɕ��і@�����ό`
	normal = normalize(normal);
	outData.normal = normal;				//�s�N�Z���V�F�[�_��

	//�܂Ƃ߂ďo��
	return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{

	float4 lightSource = float4(1.0, 1.0, 1.0, 1.0);	//���C�g�F&���邳 Iin
	float4 ambientSource = float4(0.2, 0.2, 0.2, 1.0);	//�A���r�G���g�W��Ka

	float4 diffuse;
	float4 ambient;
	float4 NL = dot(inData.normal, normalize(g_lightPosition));				//���̖ʂ̖��邳
	float4 reflect = normalize(2 * NL * inData.normal - normalize(g_lightPosition));
	float4 specular = pow(saturate(dot(reflect, normalize(inData.eye))), 8);			//8�̓n�C���C�g�̑傫��
	if (g_isTextured == 0) {
		diffuse = lightSource * g_diffuseColor * inData.color;
		ambient = lightSource * g_diffuseColor * ambientSource;
	}
	else {
		diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv) * inData.color;
		ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * ambientSource;
	}

	return diffuse + ambient + specular;
	//float shininess = 8;
	//
	///////////////////////////////////
	////���C�g�̌���
	////float4 lightDir = g_lightDir;	//�O���[�o���ϐ��͕ύX�ł��Ȃ��̂ŁA�������񃍁[�J���ϐ���
	////lightDir = normalize(lightDir);	//�����������K�v�Ȃ̂Ő��K��
	//float4 lightDir = { 1,5,0,1 };
	//lightDir = normalize(lightDir);
	////�@���̓s�N�Z���V�F�[�_�[�Ɏ����Ă������_�ŕ⊮���꒷�����ς���Ă��邽�ߐ��K��
	//inData.normal = normalize(inData.normal);
	//
	////�g�U���ˌ��i�f�B�t���[�Y�j
	////�@���ƌ��̃x�N�g���̓��ς��A�����̖��邳�ɂȂ�
	//float4 shade = saturate(dot(inData.normal, - lightDir));
	//shade.a = 1;	//�Â��Ƃ��낪�����ɂȂ�̂ŁA�����I�ɃA���t�@��1
	//
	//float4 diffuse;
	////�e�N�X�`���L���ŐF�w��
	//if (g_isTextured)diffuse = g_texture.Sample(g_sampler, inData.uv);
	//else			diffuse = g_diffuseColor;
	//
	////�����i�A���r�G���g�j
	////�����Maya���Ŏw�肵�A�O���[�o���ϐ��Ŏ󂯎�������̂����̂܂�
	//float4 ambient = ambientColor;
	//
	////���ʔ��ˌ��i�X�y�L�����[�j
	//float4 specular = float4(1, 1, 1, 0);	//�Ƃ肠�����n�C���C�g�͖����ɂ��Ă����āc
	////if (g_specularColor.a != 0)	//�X�y�L�����[�̏�񂪂����
	////{
	//	float4 R = reflect(lightDir, inData.normal);			//�����˃x�N�g��
	//	specular = pow(saturate(dot(R, inData.eye)), shininess) * lightSource;	//�n�C���C�g�����߂�
	////}
	//
	////�ŏI�I�ȐF
	//return diffuse * shade + diffuse * ambient + specular;
}
