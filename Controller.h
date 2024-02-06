#include "Engine/GameObject.h"
#include <dinput.h>
class SingleObject;
namespace {
    enum MOVE_KEY {
        MV_FRONT = 0,
        MV_BACK,
        MV_LEFT,
        MV_RIGHT,
        MV_UP,
        MV_DOWN,
        MV_MAX
    };

    enum MODIFIER_KEY {
        MD_DEFAULT = 0,
        MD_ALT = DIK_LALT,
        MD_CTRL = DIK_LCONTROL,
        MD_MAX = sizeof(MODIFIER_KEY)-1
    };
}
//ëÄçÏÇä«óùÇ∑ÇÈÉNÉâÉX
class Controller : public GameObject
{
    bool lock;
    int movcode[MV_MAX]{ 0 };
    float movSpdRotate_[MV_MAX]{ 0 };
    float movSpd_, movMaxSpd_;
    float mouseMovReg_, keyMovAcc_, keyMovDec_ ;
    float camDistance;
    float maxCamDist, minCamDist;
    float camDistUnit, camDistReg;
    MODIFIER_KEY modifyMode;
    bool enMovInertia_;
    bool canMouseCtlRotate_;
    bool trigger_;
    SingleObject* pSO;
    int hBall_;
    Transform ballTra_;
public:
    Controller(GameObject* parent);
    ~Controller();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    XMFLOAT3 GetPosition() { return transform_.position_; }
};