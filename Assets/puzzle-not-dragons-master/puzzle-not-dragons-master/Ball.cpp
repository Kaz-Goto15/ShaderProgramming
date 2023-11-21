#include "Ball.h"
#include "Engine/Model.h"

//コンストラクタ
Ball::Ball(GameObject* parent):
	GameObject(parent, "Ball"),
	hModel_(-1),
	worldBorderX(30.0f),
	worldBorderZ(20.0f)
{
	velocity = XMVectorSet(0, 0, 0, 0);
}
//デストラクタ
Ball::~Ball()
{
	//モデルが割り当てられていればRelease
	if(hModel_ > 0)
	Model::Release(hModel_);
}

//更新
void Ball::Update()
{
	// 0.001ずつ減る
	// velocityの長さlenを求める
	// lenから0.001減らす 0以下になったら0に
	// velocityの長さをlenに
	float len = Length(velocity);
	len -= 0.001;
	if (len <= 0)len = 0;
	velocity = XMVector3Normalize(velocity) * len;

	//ボールどうしの反射
	//全ボールを取得し、自分以外と判定
	std::list<Ball*> all = GetParent()->FindGameObjects<Ball>();
	for(std::list<Ball*>::iterator itr = all.begin(); itr != all.end(); itr++){
		if (*itr == this)continue;
		XMFLOAT3 next = transform_.position_ + velocity;	//自分の移動後の位置
		XMFLOAT3 other = (*itr)->GetNextPosition();			//相手の移動後の位置
		if (Length(next - other) < 1.0f*2.0f) {	//球の半径2個分
			//処理
			XMVECTOR n = other - next;
			n = XMVector3Normalize(n);	//nの長さを1にする
			XMVECTOR powDot = XMVector3Dot(velocity, n);
			float pow = XMVectorGetX(powDot);
			//nは押す力の向き、powは押す力の大きさ
			XMVECTOR push = n * pow;
			velocity -= push;
			(*itr)->AddForce(push);

			n = next - other;
			n = XMVector3Normalize(n);	//nの長さを1にする
			powDot = XMVector3Dot(velocity, n);
			pow = XMVectorGetX(powDot);
			//nは押す力の向き、powは押す力の大きさ
			push = n * pow;	//押すベクトル→相手に渡した力
			(*itr)->AddForce(-push);	//相手から引く
			AddForce(push);	//自分に加える
		}
	}

	//壁反射
	XMFLOAT3 next = transform_.position_ + velocity;
	if (next.x >= worldBorderX) {
		//XMVECTOR n = XMVectorSet(-1, 0, 0, 0);		//右の壁の法線(長さが1)
		//n *= -1;									//逆向きになる = 壁に向かうベクトル
		//XMVECTOR ipvec = XMVector3Dot(velocity, n);
		//float ip = XMVectorGetX(ipvec);				//壁を押す力の大きさ
		//XMVECTOR push = n * ip;						//壁を押すベクトル
		//XMVECTOR th = velocity - push;				//壁に沿っているベクトル
		//push *= -1;									//押し返されたベクトル
		//velocity = push + th;

		XMFLOAT3 f;
		XMStoreFloat3(&f, velocity);
		f.x *= -1;
		velocity = XMLoadFloat3(&f);
	}
	if (next.x <= -worldBorderX) {
		XMFLOAT3 f;
		XMStoreFloat3(&f, velocity);
		f.x *= -1;
		velocity = XMLoadFloat3(&f);

	}
	if (next.z >= worldBorderZ) {
		XMFLOAT3 f;
		XMStoreFloat3(&f, velocity);
		f.z *= -1;
		velocity = XMLoadFloat3(&f);

	}
	if (next.z <= -worldBorderZ) {
		XMFLOAT3 f;
		XMStoreFloat3(&f, velocity);
		f.z *= -1;
		velocity = XMLoadFloat3(&f);

	}
	transform_.position_ += velocity;
}

//描画
void Ball::Draw()
{
	if (hModel_ < 0)return;
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Ball::SetNumber(int _no)
{
	number_ = _no;
	hModel_ = Model::Load("ball" + std::to_string(_no) + ".fbx");
	if(hModel_ >= 0)Model::Load("ball0.fbx");
	assert(hModel_ >= 0);
}

void Ball::AddForce(XMVECTOR _f)
{
	velocity += _f;
}

bool Ball::IsMove()
{
	if (Length(velocity) > 0)return true;
	return false;
}

void Ball::Reflect()
{
	XMFLOAT3 f;
	XMStoreFloat3(&f, velocity);
	f.z *= -1;
	velocity = XMLoadFloat3(&f);
}
