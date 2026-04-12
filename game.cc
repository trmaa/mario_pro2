#include "game.hh"
#include "assert.hh"
#include "utils.hh"
#include "geometry.hh"

using namespace pro2;

Game::Game(int width, int height)
: mario_({width / 2, 150}, {Space, Left, Right}),
mario2_({width / 2 -30, 150}, {'W', 'A', 'D'}),
platforms_{
	Platform(100, 300, 200, 211),
	Platform(0, 200, 250, 261),
	Platform(250, 400, 150, 161),
},
finished_(false)
{
	assert(width > 0 && height > 0,
		"L'amplada i l'alcada del joc han de ser positives.");
	for (int i = 1; i < 20; i++) {
		platforms_.push_back(
			Platform(250 + i * 200, 400 + i * 200, 150, 161));
	}

	for (int i = 1; i < 20; i++) {
		coins_.push_back(
			Coin({250 + i * 20, 130}));
	}
}

void Game::process_keys(pro2::Window& window) {
	if (window.is_key_down(Keys::Escape)) {
		finished_ = true;
		return;
	}

	if (window.is_key_down('P') && !window.was_key_pressed('P'))
		paused_ = !paused_;
}

void Game::update_objects(pro2::Window& window) {
	mario_.update(window, platforms_);
	mario2_.update(window, platforms_);

	for (auto it = coins_.begin(); it != coins_.end();)
		if (mario_.coliding(it->pos()) || mario2_.coliding(it->pos())) {
			it = coins_.erase(it);  // erase returns next valid iterator
			score_++;
		} else {
			it++;
		}
}

void Game::update_camera(pro2::Window& window) {
	const Pt pos = mario_.pos();
	const Pt cam = window.camera_center();

	window.move_camera({pos.x-cam.x, pos.y-cam.y});
}

void Game::update(pro2::Window& window) {
	tick_++;

	process_keys(window);

	if (!paused_) {
		update_objects(window);
		update_camera(window);
	}
}

void Game::paint(pro2::Window& window) {
	window.clear(sky_blue);

	{
		Pt c;
		c.x = window.width() / 2;
		c.y = window.height() / 2;

		struct Rect rec = { c.x-10, c.y-10, c.x+10, c.y+10 };
		
		// si el tick l'ultim digit de tick en (base anim. len.)
		// esta entre 0 i anim. len. / 2, es veu, si no, no.
		int animation_length = 60 * 2;
		if (tick_ % animation_length < animation_length / 2)
			paint_rect(window, rec, 0x00ffff00);
	}

	for (const Platform& p : platforms_) {
		p.paint(window);
	}

	for (auto coin : coins_)
		coin.paint(window, tick_);

	mario_.paint(window);
	mario2_.paint(window);

	{
		Pt cc = window.camera_center();
		cc.x -= window.width() / 2;
		cc.y -= window.height() / 2;
		int ww = window.width() + cc.x;
		int wh = window.height() + cc.y;

		int col = 0x0055ffaa;
		struct Rect rec = { cc.x, cc.y, ww, wh };

		paint_hline(window, rec.left, rec.right, rec.top, col);
		paint_hline(window, rec.left, rec.right, rec.bottom-1, col);
		paint_vline(window, rec.left, rec.top, rec.bottom, col);
		paint_vline(window, rec.right-1, rec.top, rec.bottom, col);
	}
}
