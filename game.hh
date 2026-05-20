#ifndef GAME_HH
#define GAME_HH

#include <vector>
#include "mario.hh"
#include "platform.hh"
#include "window.hh"
#include "coin.hh"
#include "finder.hh"

class Game {
	int tick_;

	int score_ = 0;

	Mario                 mario_;
	std::vector<Platform> platforms_;
	Finder<Platform> plat_finder_;
	std::vector<Coin> coins_;
	Finder<Coin> coin_finder_;

	bool finished_;
	bool paused_;

	void process_keys(pro2::Window& window);
	void update_objects(pro2::Window& window);
	void update_camera(pro2::Window& window);

public:
	Game(int width, int height);

	void update(pro2::Window& window);
	void paint(pro2::Window& window);

	bool is_finished() const {
		return finished_;
	}

private:
	static constexpr int sky_blue = 0x005c94fc; // fix opacity
};

#endif
