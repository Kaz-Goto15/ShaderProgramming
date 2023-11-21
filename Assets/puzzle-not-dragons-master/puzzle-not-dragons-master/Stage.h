#pragma once
#include "Engine/GameObject.h"

//ステージを管理するクラス
class Stage : public GameObject
{

public:
    Stage(GameObject* parent);
    ~Stage();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

private:
    enum COLOR {
        NOCOLOR = -1,
        RED = 0,
        BLUE,
        GREEN,
        YELLOW,
        PURPLE,
        HEART,
        NUM,
    };
    struct BALLINFO{
        COLOR color;
        float x, y;     //表示座標
#if 0
        int counter;    //移動フレーム数
#else
        float bx, by;
        float rate;
#endif
        int doErase;    //消す対象
    };
    static const int WIDTH = 6;
    static const int HEIGHT = 5;
    BALLINFO field[HEIGHT][WIDTH];

    //std::string fileName[COLOR::NUM] = { "ball0.png", "ball1.png", "ball2.png", "ball3.png", "ball4.png", "ball5.png" };
    int hPict_[COLOR::NUM];

    XMFLOAT3 mousePos;
    int selectX, selectY;
    int selectColor;

    int eraseTime;  //消える演出タイマー

    const int DISPLAY_WIDTH = 1280;
    const int DISPLAY_HEIGHT = 720;

    // ドット座標→3D座標変換
    XMFLOAT3 ConvDrawPos(float x, float y);

    //状態(State)遷移
    //1.玉を捕まえるのを待つ
    //  玉を掴まえたら2.
    //2.移動・入替
    //  玉を離すor時間切れで3.
    //3.消えるものがなかったら5.
    //  ３つ並んでいるのが消える表示をする
    //    消え終わったら4.
    //4.空いた場所に玉が落ちる演出
    //  落ちたら3.へ
    //5.攻撃(P to E)演出
    //  1.へ戻る

    //それぞれの状態を定義する
    enum STATE {
        S_IDLE,
        S_MOVE,
        S_ERASE,
        S_FALL,
        S_ATTACK,
    };
    STATE state;
    void UpdateIdle();
    void UpdateMove();
    void UpdateErase();
    void UpdateFall();

    void UpdateAttack();

    void CalcMouseSelect();

    float GetRateValue(float begin, float end, float rate);

    bool CheckErase();

    /// <summary>
    /// 落ちるための準備をする
    /// </summary>
    void PrepareFall();
};