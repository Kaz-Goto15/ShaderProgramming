#pragma once
#include "Engine/GameObject.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

//地面を管理するクラス
class SingleObject : public GameObject
{
    vector<int> hModel_;    //モデル番号
    vector<Transform> modelTra_;
    bool isRotateY;
public:
    SingleObject(GameObject* parent);
    ~SingleObject();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    int AddModel(string fileName);
    void SetModelTransform(int modelNo, Transform tra);
    bool EnableRotateY() { isRotateY = true; }
};