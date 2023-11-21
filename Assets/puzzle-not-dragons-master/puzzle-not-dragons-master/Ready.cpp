#include "Ready.h"
#include "Engine/Image.h"
#include "Engine/Debug.h"
#include <cmath>
#include <vector>

namespace{
	double startX;		//�ړ��J�nX���W
	double endX;			//�ړ��I��X���W
	float totalTime;	//�ړ�����(s)
	float currentTime;	//���݂̎���

	int seq_line;		//���ݎ��s���Ă�s
	float seq_time;
	enum ACT {			//��邱�Ƃ��R�}���h��
		A_SLIDEIN = 1,
		A_WAIT,
		A_SLIDEOUT,
		A_END
	};
	struct Sequence {
		float time;		//����
		ACT action;		//do
		float param;	//�K�v�Ȓl
	};
	std::vector<Sequence> tbl = {
		{0.0f,A_SLIDEIN,5.0f},		//�X���C�h�C��
		{2.0f,A_WAIT, 0.0f},		//1s�҂�
		{2.5f,A_SLIDEOUT, -5.0f},   /*�X���C�h�A�E�g*/
		{4.0f,A_END, 0.0f}			//������
	};
	ACT currrentAction;
	bool canMove;
}
Ready::Ready(GameObject* parent)
{
	seq_line = -1;
	seq_time = 0.0f;
	canMove = false;
}

Ready::~Ready()
{
}

void Ready::Initialize()
{
	hImage = Image::Load("ready.png");
	//startX = 5.0f;
	//endX = 0;
	//totalTime = 3.0f;
	//currentTime = 0.0f;
}

void Ready::Update()
{
	seq_time += 1.0f / 60.0f;	//���Ԃ�i�߂�
	if (seq_time >= tbl[seq_line + 1].time) {
		seq_line++;
		switch (tbl[seq_line].action) {
		case A_END:
			KillMe();
			break;
		case A_SLIDEIN:
			startX = tbl[seq_line].param;
			endX = 0;
			totalTime = tbl[seq_line + 1].time - seq_time;
			currentTime = 0.0f;
			break;
		case A_WAIT:
			startX = transform_.position_.x;
			endX = transform_.position_.x;
			totalTime = tbl[seq_line + 1].time - seq_time;
			currentTime = 0.0f;
			break;
		case A_SLIDEOUT:
			canMove = true;
			startX = transform_.position_.x;
			endX = tbl[seq_line].param;
			totalTime = tbl[seq_line + 1].time - seq_time;
			currentTime = 0.0f;
			break;
		}
	}

	currentTime += 1.0f / 60.0f;	//1�t���[�����̎��Ԃ�
	if (currentTime > totalTime)currentTime = totalTime;
	float t = currentTime / totalTime;	//0~1�͈̔�
	float val = 0;
	if(tbl[seq_line].action == A_SLIDEIN)
	val = EaseOutExpo(t);
	if (tbl[seq_line].action == A_SLIDEOUT)
		val = EaseInExpo(t);
	transform_.position_.x = val * (endX - startX) + startX;
	Debug::Log(std::to_string(transform_.position_.x), true);
}

void Ready::Draw()
{
	Image::SetTransform(hImage, transform_);
	Image::Draw(hImage);
}

bool Ready::Finished()
{
	//if (currentTime >= totalTime)return true;
	//return false;
	return canMove;
}

float Ready::EaseInExpo(float x)
{
	//Debug::Log(std::to_string(pow(2, 10 * x - 10)), true);
	return pow(2, 10 * x - 10);
}

float Ready::EaseOutExpo(float x)
{
	//Debug::Log(std::to_string(1 - pow(2, -10 * x)), true);
	return  1 - pow(2, -10 * x);
}

//�C�[�W���O	0.0~1.0���Ԃ���Ȑ�
//�X�v���C��	4�ȏ�̓_�����炩�ɒʂ�Ȑ�