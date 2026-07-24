#include "Fade.h"
#include <algorithm>

using namespace KamataEngine;

void Fade::Initialize()
{
	sprite_ = Sprite::Create(0, {0.0f, 0.0f});

	sprite_->SetSize(Vector2(1280.0f, 720.0f));

	sprite_->SetColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
}

void Fade::Start(Status status, float duration)
{

	status_ = status;

	duration_ = duration;

	counter_ = 0.0f;

}

void Fade::Stop()
{
	status_ = Status::None;
}



void Fade::Update() 
{

	switch (status_) {
	case Status::None:

		break;
	case Status::FadeIn:

		// タイマーを進める
		counter_ += 1.0f / 60.0f;

		if (counter_ >= duration_) {
			counter_ = duration_;
			status_ = Status::None; // 終了したら None に戻す
		}

		// アルファ値を 1.0 -> 0.0 へ徐々に減らす（黒から透明へ）
		{
			float alpha = 1.0f - (counter_ / duration_);
			sprite_->SetColor(Vector4(0.0f, 0.0f, 0.0f, std::clamp(alpha, 0.0f, 1.0f)));
		}

		break;
	case Status::FadeOut:

		counter_ += 1.0f / 60.0f;

		if (counter_ >= duration_)
		{
			counter_ = duration_;
		}

		sprite_->SetColor(Vector4(0, 0, 0, std::clamp(counter_ / duration_, 0.0f, 1.0f)));

		break;
	
	}
}

// フェード終了判定関数の実装
bool Fade::IsFinished() const 
{
	switch (status_) 
	{
	case Status::FadeIn:
	case Status::FadeOut:
		if (counter_ >= duration_)
		{
			return true;
		} else {
			return false;
		}
	}

	return true;
}

void Fade::Draw()
{ 
	if (status_ == Status::None)
	{
		return;
	}

	Sprite::PreDraw();
	sprite_->Draw();
	Sprite::PostDraw();
}

