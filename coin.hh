#pragma once

#include <vector>
#include "geometry.hh"
#include "window.hh"

using namespace std;
using namespace pro2;

typedef vector<vector<int>> sprite_t;

class Coin {
public:
	Coin(Pt pos);

	sprite_t get_sprite(int tick) const;
	void paint(pro2::Window& window, int tick) const;
	void move(int tick);

	Pt pos() const { return { pos_.x+8, pos_.y+8 }; }

	Rect get_rect() const;

private:
	Pt pos_;

	int animation_len_;
	vector<sprite_t> animation_;
};
