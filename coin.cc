#include <math.h>
#include "geometry.hh"
#include "window.hh"
#include "utils.hh"
#include "coin.hh"

using namespace pro2;

static sprite_t nullsprite(int size = 16)
{
	sprite_t res(size, vector<int>(size));

	for (int y = 0; y < size; y++)
		for (int x = 0; x < size; x++) {
			if (x < size/2) {
				if (y < size/2)
					res[y][x] = 0x00ff00ff;
				else
					res[y][x] = 0;
			} else {
				if (y >= size/2)
					res[y][x] = 0x00ff00ff;
				else
					res[y][x] = 0;
			}
		}

	return res;
}

static void coin_texture(Pt uv, sprite_t& spr, int x, int y)
{
	float rad = 8*8; // squaring to avoid sqrt
	float dist = uv.x*uv.x + uv.y*uv.y; 

	if (dist > rad || dist < rad / 8) {
		spr[y][x] = -1; // transparent
	} else {
		// gradient de vermell a groc.
		float bright = (dist / rad) / 2 + 0.5;
		unsigned char r = 0xff;
		unsigned char g = 0xff * bright;
		int col = 0;
		col |= r << 16;
		col |= g << 8;

		spr[y][x] = col;
	}
}

Coin::Coin(Pt pos)
	: pos_(pos), animation_len_(4)
{
	for (int i = 0; i < animation_len_; i++)
		animation_.push_back(nullsprite());

	for (int y = 0; y < 16; y++)
		for (int x = 0; x < 16; x++) {
			Pt uv = { x-8, y-8 };
			coin_texture(uv, animation_[0], x, y);	
			uv.x *= 1.8; // ampliar FOV per fer la imatge fina
			coin_texture(uv, animation_[1], x, y);	
			coin_texture(uv, animation_[3], x, y);	
		}

	for (int j = 0; j < 16; j++)
		for (int i = 0; i < 16; i++)
			if (i >= 7 && i <= 9)
				animation_[2][j][i] = 0x00ffff00;
			else 
				animation_[2][j][i] = -1; // no blanc, sino
							  // null
}

sprite_t Coin::get_sprite(int tick) const
{
	int dur = 60 * 1; // 1 segs

	for (int frame = 0, section = dur/animation_len_;
			section <= dur;
			frame++, section += dur/animation_len_)
		if (tick % dur <= section)
			return animation_[frame];


	return nullsprite();
}

void Coin::paint(pro2::Window& window, int tick)
{
	pos_.y += sin(tick*0.1 + pos_.x*0.02)*2;

	sprite_t spr = get_sprite(tick);
	paint_sprite(window, pos_, spr);
}
