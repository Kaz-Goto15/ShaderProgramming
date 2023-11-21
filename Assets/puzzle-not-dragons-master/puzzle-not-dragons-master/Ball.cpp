#include "Ball.h"
#include "Engine/Model.h"

//�R���X�g���N�^
Ball::Ball(GameObject* parent):
	GameObject(parent, "Ball"),
	hModel_(-1),
	worldBorderX(30.0f),
	worldBorderZ(20.0f)
{
	velocity = XMVectorSet(0, 0, 0, 0);
}
//�f�X�g���N�^
Ball::~Ball()
{
	//���f�������蓖�Ă��Ă����Release
	if(hModel_ > 0)
	Model::Release(hModel_);
}

//�X�V
void Ball::Update()
{
	// 0.001������
	// velocity�̒���len�����߂�
	// len����0.001���炷 0�ȉ��ɂȂ�����0��
	// velocity�̒�����len��
	float len = Length(velocity);
	len -= 0.001;
	if (len <= 0)len = 0;
	velocity = XMVector3Normalize(velocity) * len;

	//�{�[���ǂ����̔���
	//�S�{�[�����擾���A�����ȊO�Ɣ���
	std::list<Ball*> all = GetParent()->FindGameObjects<Ball>();
	for(std::list<Ball*>::iterator itr = all.begin(); itr != all.end(); itr++){
		if (*itr == this)continue;
		XMFLOAT3 next = transform_.position_ + velocity;	//�����̈ړ���̈ʒu
		XMFLOAT3 other = (*itr)->GetNextPosition();			//����̈ړ���̈ʒu
		if (Length(next - other) < 1.0f*2.0f) {	//���̔��a2��
			//����
			XMVECTOR n = other - next;
			n = XMVector3Normalize(n);	//n�̒�����1�ɂ���
			XMVECTOR powDot = XMVector3Dot(velocity, n);
			float pow = XMVectorGetX(powDot);
			//n�͉����͂̌����Apow�͉����͂̑傫��
			XMVECTOR push = n * pow;
			velocity -= push;
			(*itr)->AddForce(push);

			n = next - other;
			n = XMVector3Normalize(n);	//n�̒�����1�ɂ���
			powDot = XMVector3Dot(velocity, n);
			pow = XMVectorGetX(powDot);
			//n�͉����͂̌����Apow�͉����͂̑傫��
			push = n * pow;	//�����x�N�g��������ɓn������
			(*itr)->AddForce(-push);	//���肩�����
			AddForce(push);	//�����ɉ�����
		}
	}

	//�ǔ���
	XMFLOAT3 next = transform_.position_ + velocity;
	if (next.x >= worldBorderX) {
		//XMVECTOR n = XMVectorSet(-1, 0, 0, 0);		//�E�̕ǂ̖@��(������1)
		//n *= -1;									//�t�����ɂȂ� = �ǂɌ������x�N�g��
		//XMVECTOR ipvec = XMVector3Dot(velocity, n);
		//float ip = XMVectorGetX(ipvec);				//�ǂ������͂̑傫��
		//XMVECTOR push = n * ip;						//�ǂ������x�N�g��
		//XMVECTOR th = velocity - push;				//�ǂɉ����Ă���x�N�g��
		//push *= -1;									//�����Ԃ��ꂽ�x�N�g��
		//velocity = push + th;

		XMFLOAT3 f;
		XMStoreFloat3(&f, velocity);
		f.x *= -1;
		velocity = XMLoadFloat3(&f);
	}
	if (next.x <= -worldBorderX) {
		XMFLOAT3 f;
		XMStoreFloat3(&f, velocity);
		f.x *= -1;
		velocity = XMLoadFloat3(&f);

	}
	if (next.z >= worldBorderZ) {
		XMFLOAT3 f;
		XMStoreFloat3(&f, velocity);
		f.z *= -1;
		velocity = XMLoadFloat3(&f);

	}
	if (next.z <= -worldBorderZ) {
		XMFLOAT3 f;
		XMStoreFloat3(&f, velocity);
		f.z *= -1;
		velocity = XMLoadFloat3(&f);

	}
	transform_.position_ += velocity;
}

//�`��
void Ball::Draw()
{
	if (hModel_ < 0)return;
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Ball::SetNumber(int _no)
{
	number_ = _no;
	hModel_ = Model::Load("ball" + std::to_string(_no) + ".fbx");
	if(hModel_ >= 0)Model::Load("ball0.fbx");
	assert(hModel_ >= 0);
}

void Ball::AddForce(XMVECTOR _f)
{
	velocity += _f;
}

bool Ball::IsMove()
{
	if (Length(velocity) > 0)return true;
	return false;
}

void Ball::Reflect()
{
	XMFLOAT3 f;
	XMStoreFloat3(&f, velocity);
	f.z *= -1;
	velocity = XMLoadFloat3(&f);
}
