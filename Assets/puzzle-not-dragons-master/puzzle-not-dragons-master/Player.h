#pragma once
#include "Engine/GameObject.h"

class Ball;
class Gauge;
//�X�e�[�W���Ǘ�����N���X
class Player : public GameObject{
public:
    Player(GameObject* parent);
    ~Player();
    void Update() override;
    void Draw() override;

    /// <summary>
    /// �v���C���[�����삷���
    /// </summary>
    /// <param name="_ball">�{�[��</param>
    void SetMyBall(Ball* _ball) { myBall_ = _ball; }
private:
    int hModel_;
    Ball* myBall_;
    float direction;    //����(Y��]�̊p�x�F���W�A��)
    float power;        //�͂̑傫��
    const float dirChangeVal = 0.05f;

    enum STATE {
        S_CHOOSE_ROTATE = 0,
        S_CHOOSE_POWER,
        S_MOVE,
        S_MAX,
    };
    STATE state = S_CHOOSE_ROTATE;
    int maxPow = 1.0f;
    float powerRatio, pSin;

    //std::string fileName_[3] = { "PowerGaugeBG", "PowerGauge", "PowerGaugeFrame" };
    //int hPict_[3];
    Gauge* gauge_;
    Transform ImageTrans;
    XMFLOAT3 mousePos;

    void UpdateRotate();
    void UpdatePower();
    void UpdateMove();

    void DrawModel();
    void DrawPower();
    XMFLOAT3 ConvDrawPos(float x, float z);
};