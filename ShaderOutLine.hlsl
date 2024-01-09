//������������������������������������������������������������������������������
 // �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D		g_texture : register(t0);	//�e�N�X�`���[
SamplerState	g_sampler : register(s0);	//�T���v���[

Texture2D		g_toon_texture : register(t1);
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
	float4		g_ambientColor;		//�A���r�G���g�J���[(�e 0�͉e��������)
	float4		g_specularColor;	//�X�y�L�����J���[(�n�C���C�g�F )
	float		g_shininess;		//�n�C���C�g�̋���
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

	pos = pos + normal * 0.5;
	outData.pos = mul(pos, g_matWVP);	//���[�J�����W��WVP�s�񂩂��X�N���[�����W�ɕϊ�

	outData.uv = uv;

	float4 light = normalize(g_lightPosition);
	light = normalize(light);
	outData.color = saturate(dot(normal, light));

	float4 worldPos = mul(pos, g_matW);				//���[�J�����W�Ƀ��[���h�s����������[���h���W��
	//outData.eye = normalize(g_eyePos - worldPos);	//���_���璸�_���������Z�A���������߃s�N�Z���V�F�[�_��
	outData.eye = g_eyePos - worldPos;
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
	float4 diffuse;
	float4 ambient;
	float4 NL = saturate(dot(inData.normal, normalize(g_lightPosition)));				//���̖ʂ̖��邳
	//float4 reflect = normalize(2 * NL * inData.normal - normalize(g_lightPosition));
	float4 reflects = reflect(normalize(-g_lightPosition), inData.normal);
	float4 specular = pow(saturate(dot(reflects, normalize(inData.eye))), g_shininess) * g_specularColor;

	float2	uv;
	uv.x = inData.color.x;
	uv.y = 0;
	float4 tI = g_toon_texture.Sample(g_sampler, uv);

	if (g_isTextured == 0) {
		diffuse = lightSource * g_diffuseColor * tI;
		ambient = lightSource * g_diffuseColor * g_ambientColor;
	}
	else {
		diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv) * tI;
		ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * g_ambientColor;
	}

	////�֊s = �����x�N�g���Ɩʂ̖@����
	//float NV = dot(inData.normal, normalize(inData.eye));
	//if (abs(NV) < 0.3)
	//	return float4(0, 0, 0, 0);
	//else

	return diffuse + ambient + specular;
}
