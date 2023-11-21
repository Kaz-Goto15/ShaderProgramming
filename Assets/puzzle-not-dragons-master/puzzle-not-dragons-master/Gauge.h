#pragma once
#include "Engine/GameObject.h"

class Gauge : public GameObject
{
private:
    float maxPower_, nowPower_, animPower_;
    int ImageWidth, ImageHeight;

    std::string fileName_[3] = { "PowerGaugeBG", "PowerGauge", "PowerGaugeFrame" };
    int Powerict_[3];
    Transform ImageTrans;

public:
    static constexpr float MIN = 0.0f;
    static constexpr float MAX = 1.0f;
    //�R���X�g���N�^
    Gauge(GameObject* parent);

    //�f�X�g���N�^
    ~Gauge();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;

    void SetPower(int _nowPower, int _maxPower) {
        //nowPower_ = _nowPower;
        //maxPower_ = _maxPower;
        //animPower_ = (animPower_*9 + nowPower_)/10;
    }
    void SetPosition(float x, float z);
    void AddValue(float v);
    void SetValue(float v);
    float GetValue() {return nowPower_;};
};
