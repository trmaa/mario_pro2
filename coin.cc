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

Coin::Coin(Pt pos)
	: pos_(pos), animation_len_(4)
{
	for (int i = 0; i < animation_len_; i++)
		animation_.push_back(nullsprite());

	for (int y = 0; y < 16; y++)
		for (int x = 0; x < 16; x++) {
			Pt c = { x-8, y-8 };

			float rad = 8*8; // squaring to avoid sqrt
			float dist = c.x*c.x + c.y*c.y; 

			if (dist > rad) {
				animation_[0][y][x] = -1; // transparent
			} else {
				// gradient de vermell a groc.
				float bright = (dist / rad) / 4 + 0.75;
				unsigned char r = 0xff;
				unsigned char g = 0xff * bright;
				int col = 0;
				col |= r << 16;
				col |= g << 8;

				animation_[0][y][x] = col;
			}
		}

	for (int y = 0; y < 16; y++)
		for (int x = 0; x < 16; x++) {
			Pt c = { x-8, y-8 };
			c.x *= 1.8; // estirar el FOV horizontal per veure la
				    // moneda mes estreta.

			float rad = 8*8; // squaring to avoid sqrt
			float dist = c.x*c.x + c.y*c.y; 

			if (dist > rad) {
				animation_[1][y][x] = -1; // transparent
				animation_[3][y][x] = -1; // transparent
			} else {
				// gradient de vermell a groc.
				float bright = (dist / rad) / 4 + 0.75;
				unsigned char r = 0xff;
				unsigned char g = 0xff * bright;
				int col = 0;
				col |= r << 16;
				col |= g << 8;

				animation_[1][y][x] = col;
				animation_[3][y][x] = col;
			}
		}



	for (int j = 0; j < 16; j++)
		for (int i = 0; i < 16; i++) {
			if (i >= 7 && i <= 9)
				animation_[2][j][i] = 0x00ffff00;
			else 
				animation_[2][j][i] = -1; // no blanc, sino
							  // null
		}
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
