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
}

void Game::update_camera(pro2::Window& window) {
	const Pt pos = mario_.pos();
	const Pt cam = window.camera_center();

	window.move_camera({pos.x-cam.x, pos.y-cam.y});
}

void Game::update(pro2::Window& window) {
	process_keys(window);

	if (!paused_) {
		update_objects(window);
		update_camera(window);
	}
}

void Game::paint(pro2::Window& window) {
	window.clear(sky_blue);

	for (const Platform& p : platforms_) {
		p.paint(window);
	}

	mario_.paint(window);
	mario2_.paint(window);

	{
		Pt cc = window.camera_center();
		cc.x -= window.width() / 2;
		cc.y -= window.height() / 2;
		int ww = window.width() + cc.x;
		int wh = window.height() + cc.y;

		int col = 0xffff00ff;

		paint_hline(window, cc.x, ww, cc.y, col);	
		paint_hline(window, cc.x, ww, wh-1, col);	
		paint_vline(window, cc.x, cc.y, wh, col);
		paint_vline(window, ww-1, cc.y, wh, col);
	}
}
