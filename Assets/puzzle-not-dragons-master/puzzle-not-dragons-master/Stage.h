#pragma once
#include "Engine/GameObject.h"

//�X�e�[�W���Ǘ�����N���X
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
        float x, y;     //�\�����W
#if 0
        int counter;    //�ړ��t���[����
#else
        float bx, by;
        float rate;
#endif
        int doErase;    //�����Ώ�
    };
    static const int WIDTH = 6;
    static const int HEIGHT = 5;
    BALLINFO field[HEIGHT][WIDTH];

    //std::string fileName[COLOR::NUM] = { "ball0.png", "ball1.png", "ball2.png", "ball3.png", "ball4.png", "ball5.png" };
    int hPict_[COLOR::NUM];

    XMFLOAT3 mousePos;
    int selectX, selectY;
    int selectColor;

    int eraseTime;  //�����鉉�o�^�C�}�[

    const int DISPLAY_WIDTH = 1280;
    const int DISPLAY_HEIGHT = 720;

    // �h�b�g���W��3D���W�ϊ�
    XMFLOAT3 ConvDrawPos(float x, float y);

    //���(State)�J��
    //1.�ʂ�߂܂���̂�҂�
    //  �ʂ�͂܂�����2.
    //2.�ړ��E����
    //  �ʂ𗣂�or���Ԑ؂��3.
    //3.��������̂��Ȃ�������5.
    //  �R����ł���̂�������\��������
    //    �����I�������4.
    //4.�󂢂��ꏊ�ɋʂ������鉉�o
    //  ��������3.��
    //5.�U��(P to E)���o
    //  1.�֖߂�

    //���ꂼ��̏�Ԃ��`����
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
    /// �����邽�߂̏���������
    /// </summary>
    void PrepareFall();
};