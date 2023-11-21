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
    //コンストラクタ
    Gauge(GameObject* parent);

    //デストラクタ
    ~Gauge();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
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
