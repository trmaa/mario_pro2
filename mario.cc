#include "mario.hh"
#include "utils.hh"
#include "geometry.hh"

#include "window.hh"

using namespace std;
using namespace pro2;

const int _ = -1;
const int r = pro2::red;
const int s = 0xecc49b;
const int b = 0x5e6ddc;
const int y = pro2::yellow;
const int h = pro2::black;
const int g = 0xaaaaaa;
const int w = 0x8d573c;

// clang-format off
const vector<vector<int>> Mario::mario_sprite_normal_ = {
	{_, _, _, r, r, r, r, r, _, _, _, _},
	{_, _, r, r, r, r, r, r, r, r, r, _},
	{_, _, h, h, h, s, s, h, s, _, _, _},
	{_, h, s, h, s, s, s, h, s, s, s, _},
	{_, h, s, h, h, s, s, s, h, s, s, s},
	{_, h, h, s, s, s, s, h, h, h, h, _},
	{_, _, _, s, s, s, s, s, s, s, _, _},
	{_, _, r, r, b, r, r, r, _, _, _, _},
	{_, r, r, r, b, r, r, b, r, r, r, _},
	{r, r, r, r, b, b, b, b, r, r, r, r},
	{s, s, r, b, y, b, b, y, b, r, s, s},
	{s, s, s, b, b, b, b, b, b, s, s, s},
	{s, s, b, b, b, b, b, b, b, b, s, s},
	{_, _, b, b, b, _, _, b, b, b, _, _},
	{_, w, w, w, _, _, _, _, w, w, w, _},
	{w, w, w, w, _, _, _, _, w, w, w, w},
};
// clang-format on

bool Mario::coliding(Pt p)
{
	if (p.x > pos_.x-10 && p.x < pos_.x+10)
		if (p.y > pos_.y-16 && p.y < pos_.y)
			return true;

	return false;
}

void Mario::paint(pro2::Window& window) const {
	const Pt top_left = {pos_.x - 6, pos_.y - 15};
	paint_sprite(window, top_left, mario_sprite_normal_, looking_left_);
}

void Mario::apply_physics_() {
	if (grounded_) {
		speed_.y = 0;
		accel_.y = 0;
	}

	// Always falling to check if we aren't grounded
	// If we are, we will return to the same spot

	const int gravity = 1;  // gravity = 1 pixel / frame_time^2
	speed_.y += gravity;

	if (accel_time_ > 0) {
		speed_.y += accel_.y;
		accel_time_--;
	}

	pos_.x += speed_.x;
	pos_.y += speed_.y;
}

void Mario::jump() {
	if (grounded_) {
		accel_.y = -6;
		grounded_ = false;
		accel_time_ = 2;
	}
}

void Mario::update(pro2::Window& window, set<const Platform *> platforms) {
	last_pos_ = pos_;
	if (window.is_key_down(controler_.up)) {
		jump();
	}

	// Velocitat horitzontal
	speed_.x = 0; 
	if (window.is_key_down(controler_.left)) {
		speed_.x = -4;
	} else if (window.is_key_down(controler_.right)) {
		speed_.x = 4;
	}
	if (speed_.x != 0) {
		looking_left_ = speed_.x < 0;
	}

	// Apply acceleration and speed
	apply_physics_();

	// Check position
	set_grounded(false);

	for (auto platform : platforms) {
		if (platform->has_crossed_floor_downwards(last_pos_, pos_)) {
			set_grounded(true);
			set_y(platform->top());
		}
	}

	if (pos_.y > 1000)
		pos_.y -= 1000;
}
