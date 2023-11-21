#pragma once
#include "Engine/GameObject.h"

class Ball;
class Gauge;
//ステージを管理するクラス
class Player : public GameObject{
public:
    Player(GameObject* parent);
    ~Player();
    void Update() override;
    void Draw() override;

    /// <summary>
    /// プレイヤーが操作する玉
    /// </summary>
    /// <param name="_ball">ボール</param>
    void SetMyBall(Ball* _ball) { myBall_ = _ball; }
private:
    int hModel_;
    Ball* myBall_;
    float direction;    //向き(Y回転の角度：ラジアン)
    float power;        //力の大きさ
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