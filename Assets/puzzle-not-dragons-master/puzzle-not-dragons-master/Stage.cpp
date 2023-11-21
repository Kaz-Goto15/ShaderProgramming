#include "Stage.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/Text.h"
#include "Engine/SceneManager.h"
namespace {
    static const int SIZE = 40; //ボール直径
    static const int LEFT = 60; //盤面左位置
    static const int TOP  = 80;  //盤面上位置
}
//コンストラクタ
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

//デストラクタ
Stage::~Stage()
{
}

//初期化
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

//更新
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

//描画
void Stage::Draw()
{
    //表示
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

//開放
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
    //範囲外ドラッグ処理
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

    //離したら切り替え
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
    //滑らかに移動する演出
    for (int h = 0; h < HEIGHT; h++) {
        for (int w = 0; w < WIDTH; w++) {
            //今の位置 field[h][w].x field[h][w].y
            //本来の位置 w*40 h*40
#if 0
            if (field[h][w].counter > 0) {  //本来の玉の位置と今の位置がずれていたら
                //本来の位置に近づける
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
    //滑らかに移動する演出
    for (int h = 0; h < HEIGHT; h++) {
        for (int w = 0; w < WIDTH; w++) {
            //今の位置 field[h][w].x field[h][w].y
            //本来の位置 w*40 h*40
            auto& b = field[h][w];
            if (b.rate < 1.0f) {
                b.rate += 0.05f;    //落下速度
                if (b.rate > 1.0f)b.rate = 1.0f;
                //b.x = (w * 40 - b.bx) * b.rate + b.bx;
                //b.y = (h * 40 - b.by) * b.rate + b.by;
                b.x = GetRateValue(b.bx, w * 40, b.rate);
                b.y = GetRateValue(b.by, h * 40, b.rate);
            }
        }
    }
    //全て落ちたら　全てのfield[h][w].rate>=1.0になったら
    for (int h = 0; h < HEIGHT; h++) {
        for (int w = 0; w < WIDTH; w++) {
            if (field[h][w].rate < 1.0f) {  //落ちてる途中
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
    //透明度、拡縮、座標、回転等　つまりは標準描画ので使う　慣れたものだ
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
                //field[h][w]をholeの数落下させる
                field[h + hole][w] = field[h][w];
                field[h + hole][w].by = field[h + hole][w].y;
                field[h + hole][w].bx = field[h + hole][w].x;
                field[h + hole][w].rate = 0.0f;

            }
        }
        //玉を補充
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
