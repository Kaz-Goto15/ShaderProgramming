#pragma once

//補完しながら移動するクラス
//float
class Mover
{
public:
	Mover();
	~Mover();
	void Update();	//補完する計算をする部分

	/// <summary>
	/// 移動を開始させる
	/// </summary>
	/// <param name="from">スタート値</param>
	/// <param name="target">終了値</param>
	/// <param name="sec">変化させる時間</param>
	void Start(float from, float target, float sec);

	/// <summary>
	/// 今の値から移動させる
	/// </summary>
	/// <param name="target">終了値</param>
	/// <param name="sec">変化させる時間</param>
	void MoveTo(float target, float sec);

	/// <summary>
	/// 今の値を返す
	/// </summary>
	/// <returns>今の値</returns>
	float GetCurrent(){ return currentValue; }

	/// <summary>
	/// 補完移動が終わったかを調べる
	/// </summary>
	/// <returns>終わったらtrue</returns>
	bool IsFinish() {
		return (currentTime >= moveTime);
	}

private:
	float begin;	//開始時の値
	bool end;		//終了時の値
	float moveTime;	//移動総時間
	float currentTime;	//現在の経過時間
	float currentValue;	//現在の値

	//rate(割合) = currentTime / moveTime で求まる
};

