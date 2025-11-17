#pragma once
#include "DxLib.h"

namespace Input {
	//キーボード取得関連
	void KeyStateUpdate(); //キーの状態を毎フレーム更新する
	bool IsKeyUP(int keyCode);	//離した瞬間
	bool IsKeyDown(int keyCode); //押した瞬間
	int IsKeepKeyDown(int keyCode); //押している間

	// マウス関連
	bool IsMouseDown(int button); //ボタンが押された瞬間
	bool IsMouseUP(int button); //ボタンが離された瞬間
	bool IsMouseKeep(int button); //ボタンが押されている間

	// Xbox関連
	bool IsJoypadDown(int button);
	bool IsJoypadUp(int button);
	bool IsJoypadKeep(int button);
	// この下or工夫して上に追加する
	// LT, RT, Lスティック, Rスティック
	// 傾きをパーセンテージで返す案
	VECTOR ThumbLeft(); // 左スティックの移動方向ベクトル
}
