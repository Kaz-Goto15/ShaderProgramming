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
float4 VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL) :SV_POSITION
{
	normal.w = 0;
	pos = pos + normal * 0.05;
	pos = mul(pos, g_matWVP);

	return pos;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
	return float4(0,0,0,1);
}