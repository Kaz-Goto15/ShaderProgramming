#include "Stage.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/Text.h"
#include "Engine/SceneManager.h"
namespace {
    static const int SIZE = 40; //�{�[�����a
    static const int LEFT = 60; //�Ֆʍ��ʒu
    static const int TOP  = 80;  //�Ֆʏ�ʒu
}
//�R���X�g���N�^
Stage::Stage(GameObject* parent)
    :GameObject(parent, "Stage")
{
    std::fill(hPict_, hPict_ + COLOR::NUM, -1);
    for (int h = 0; h < HEIGHT; h++) {
        for (int w = 0; w < WIDTH; w++) {
            //field[h][w] = COLOR::RED;
            field[h][w].color = (COLOR)(rand() % COLOR::NUM);
            field[h][w].x = w * 40;
            field[h][w].y = h * 40;
            field[h][w].bx = w * 40;
            field[h][w].by = h * 40;
            field[h][w].rate = 1.0f;
            field[h][w].doErase = 0;
        }
    }
    selectX = -1; selectY = -1;
    mousePos = { 0,0,0 };
    selectColor = COLOR::NOCOLOR;
}

//�f�X�g���N�^
Stage::~Stage()
{
}

//������
void Stage::Initialize()
{
 
    for (int i = 0; i < COLOR::NUM; i++) {
        //hPict_[i] = Image::Load(fileName[i]);
        std::string s = "ball";
        s += std::to_string(i) + ".png";
        hPict_[i] = Image::Load(s);
        //hPict_[i] = Image::Load("ball" + std::to_string(i) + ".png");
        //hPict_[i] = Image::Load(scanf("ball%d.png", &i));
        assert(hPict_[i] >= 0);
    }
    state = STATE::S_IDLE;
}

//�X�V
void Stage::Update()
{
    switch (state) {
    case STATE::S_IDLE:
        UpdateIdle();
        break;
    case STATE::S_MOVE:
        UpdateMove();
        break;
    case STATE::S_ERASE:
        UpdateErase();
        break;
    case STATE::S_FALL:
        UpdateFall();
        break;
    case STATE::S_ATTACK:
        UpdateAttack();
        break;
    }

}

//�`��
void Stage::Draw()
{
    //�\��
    for (int h = 0; h < HEIGHT; h++) {
        for (int w = 0; w < WIDTH; w++) {
            int color = field[h][w].color;
            Transform imageTra;
            imageTra.position_ = ConvDrawPos(field[h][w].x, field[h][w].y);
            if (w == selectX && h == selectY) {
                imageTra.scale_ = { 1.2f, 1.2f, 0 };
            }
            else if (field[h][w].doErase>0){
                imageTra.scale_ = { 0.5f,0.5f,0 };
            }
            else {
                imageTra.scale_ = { 1,1,0 };
            }
            Image::SetTransform(hPict_[color], imageTra);
            Image::Draw(hPict_[color]);
        }
    }
    if (selectColor != NOCOLOR) {
        Transform imageTra;
        imageTra.position_ = ConvDrawPos(mousePos.x - 20, mousePos.y - 20);;
        Image::SetTransform(hPict_[selectColor], imageTra);
        Image::Draw(hPict_[selectColor]);
    }
 
    //debug
    Text* pText = new Text;
    pText->Initialize();
    std::string str = "Mouse: " + std::to_string(mousePos.x) + " " + std::to_string(mousePos.y);
    str = str + "select: " + std::to_string(selectX) + " " + std::to_string(selectY);
    pText->Draw(20, 20, str.c_str());
}

//�J��
void Stage::Release()
{
}

XMFLOAT3 Stage::ConvDrawPos(float x, float y) {
    XMFLOAT3 convPos;
    convPos.x = x / 40 * 0.062f;
    convPos.y = y / -40 * 0.110f;
    convPos.z = 0;
    return convPos;
}

void Stage::UpdateIdle()
{
    if (Input::IsKeyDown(DIK_B)) {
        SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
        pSceneManager->ChangeScene(SCENE_ID_BIL);
    }
    if (Input::IsMouseButtonDown(0)) {
        CalcMouseSelect();
        if (selectX >= 0 && selectX < WIDTH &&
            selectY >= 0 && selectY < HEIGHT)
        state = STATE::S_MOVE;
    }
}

void Stage::UpdateMove()
{
    int lastX = selectX;
    int lastY = selectY;
    CalcMouseSelect();
    //�͈͊O�h���b�O����
    if (selectY < 0)selectY = 0;
    if (selectY >= HEIGHT)selectY = HEIGHT - 1;
    if (selectX < 0)selectX = 0;
    if (selectX >= WIDTH)selectX = WIDTH - 1;
    if (selectX != lastX || selectY != lastY) {
        auto tmp = field[selectY][selectX];
        field[selectY][selectX] = field[lastY][lastX];
        field[lastY][lastX] = tmp;
#if 0
        field[selectY][selectX].counter = 10;
        field[lastY][lastX].counter = 10;
#else
        auto& s = field[selectY][selectX];
        s.bx = s.x;
        s.by = s.y;
        s.rate = 0.0f;
        auto& la = field[lastY][lastX];
        la.bx = la.x;
        la.by = la.y;
        la.rate = 0.0f;
#endif
    }

    //��������؂�ւ�
    if (Input::IsMouseButtonUp(0)) {
        selectColor = COLOR::NOCOLOR;
        selectX = -1;
        selectY = -1;
        if (CheckErase()) {
            eraseTime = 30;
            state = STATE::S_ERASE;
        }
        else {
            state = STATE::S_IDLE;
        }
    }
    //���炩�Ɉړ����鉉�o
    for (int h = 0; h < HEIGHT; h++) {
        for (int w = 0; w < WIDTH; w++) {
            //���̈ʒu field[h][w].x field[h][w].y
            //�{���̈ʒu w*40 h*40
#if 0
            if (field[h][w].counter > 0) {  //�{���̋ʂ̈ʒu�ƍ��̈ʒu������Ă�����
                //�{���̈ʒu�ɋ߂Â���
                field[h][w].x += (w * 40 - field[h][w].x) / field[h][w].counter;
                field[h][w].y += (h * 40 - field[h][w].y) / field[h][w].counter;
                field[h][w].counter--;
            }
#else
            auto& b = field[h][w];
            if (b.rate < 1.0f) {
                b.rate += 0.1f;
                if (b.rate > 1.0f)b.rate = 1.0f;
                //b.x = (w * 40 - b.bx) * b.rate + b.bx;
                //b.y = (h * 40 - b.by) * b.rate + b.by;
                b.x = GetRateValue(b.bx, w * 40, b.rate);
                b.y = GetRateValue(b.by, h * 40, b.rate);
#endif
            }
        }
    }
}

void Stage::UpdateErase()
{
    eraseTime--;
    if (eraseTime <= 0) {
        PrepareFall();
        state = STATE::S_FALL;
    }
}

void Stage::UpdateFall()
{
    //���炩�Ɉړ����鉉�o
    for (int h = 0; h < HEIGHT; h++) {
        for (int w = 0; w < WIDTH; w++) {
            //���̈ʒu field[h][w].x field[h][w].y
            //�{���̈ʒu w*40 h*40
            auto& b = field[h][w];
            if (b.rate < 1.0f) {
                b.rate += 0.05f;    //�������x
                if (b.rate > 1.0f)b.rate = 1.0f;
                //b.x = (w * 40 - b.bx) * b.rate + b.bx;
                //b.y = (h * 40 - b.by) * b.rate + b.by;
                b.x = GetRateValue(b.bx, w * 40, b.rate);
                b.y = GetRateValue(b.by, h * 40, b.rate);
            }
        }
    }
    //�S�ė�������@�S�Ă�field[h][w].rate>=1.0�ɂȂ�����
    for (int h = 0; h < HEIGHT; h++) {
        for (int w = 0; w < WIDTH; w++) {
            if (field[h][w].rate < 1.0f) {  //�����Ă�r��
                return;
            }
        }
    }
    if (CheckErase()) {
            eraseTime = 30;
            state = STATE::S_ERASE;
    }
    else {
        state = STATE::S_IDLE;
    }
}

void Stage::UpdateAttack()
{
}

void Stage::CalcMouseSelect()
{
    mousePos = Input::GetMousePosition();
    mousePos.x -= 608;
    mousePos.y -= 330;
    if (mousePos.x >= 0)selectX = mousePos.x / 40; else selectX = -1;
    if (mousePos.y >= 0)selectY = mousePos.y / 40; else selectY = -1;
    if (selectX >= 0 && selectX < WIDTH &&
        selectY >= 0 && selectY < HEIGHT)
        selectColor = field[selectY][selectX].color;
    else selectColor = -1;
}

float Stage::GetRateValue(float begin, float end, float rate)
{
    return (end - begin) * rate + begin;
    //y=ax+b [0<=x<=1][0<=y<=1]
    //�����x�A�g�k�A���W�A��]���@�܂�͕W���`��̂Ŏg���@���ꂽ���̂�
}

bool Stage::CheckErase()
{
    bool ret = false;
    for (int h = 0; h < HEIGHT; h++) {
        for (int w = 0; w < WIDTH; w++) {
            if (w < WIDTH - 2 && field[h][w].color == field[h][w + 1].color &&
                field[h][w].color == field[h][w + 2].color) {
                field[h][w].doErase = 1;
                field[h][w + 1].doErase = 1;
                field[h][w + 2].doErase = 1;
                ret = true;
            }

            if (h < HEIGHT-2 && field[h][w].color == field[h+1][w].color &&
                field[h][w].color == field[h+2][w].color) {
                field[h][w].doErase = 1;
                field[h+1][w].doErase = 1;
                field[h+2][w].doErase = 1;
                ret = true;
            }
        }
    }
    return ret;
}

void Stage::PrepareFall()
{
    for (int w = 0; w < WIDTH; w++) {
        int hole = 0;
        for (int h = HEIGHT - 1; h >= 0; h--) {
            if (field[h][w].doErase > 0) {
                hole += 1;
            }
            else {
                //field[h][w]��hole�̐�����������
                field[h + hole][w] = field[h][w];
                field[h + hole][w].by = field[h + hole][w].y;
                field[h + hole][w].bx = field[h + hole][w].x;
                field[h + hole][w].rate = 0.0f;

            }
        }
        //�ʂ��[
        for (int b = 0; b < hole; b++) {
            field[b][w].color = (COLOR)(rand() % COLOR::NUM);
            field[b][w].rate = 0.0f;
            field[b][w].doErase = 0;
            field[b][w].x = w * 40;
            field[b][w].bx = w * 40;
            field[b][w].y = (b-hole ) * 40;
            field[b][w].by = (b-hole) * 40;
        }
    }
}
