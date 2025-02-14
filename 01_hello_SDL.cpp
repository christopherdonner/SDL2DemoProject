#include <SDL.h>
#include <SDL_image.h>
#include <SDL_gpu.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <chrono>

using namespace std;

int main( int argc, char* args[] )
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

	GPU_Target* window = GPU_InitRenderer(GPU_RENDERER_OPENGL_3, 320, 240, GPU_DEFAULT_INIT_FLAGS);
	GPU_Image* player = GPU_LoadImage("adventurer-sheet.png");

	vector<GPU_Rect> rects;
	size_t  nbRow = 11,
			nbCol = 7,
			widthSpr = 50,
			heightSpr = 37;

	for (size_t i = 0; i < nbRow; i++) {
		for (size_t j = 0; j < nbCol; j++) {
			rects.push_back(GPU_Rect{ (float)(j * widthSpr), (float)(i * heightSpr), (float)widthSpr, (float)heightSpr });
		}
	}

	vector<pair<size_t, size_t>> idle1{ {0, 0}, {0, 1}, {0, 2}, {0, 3} };
	vector<pair<size_t, size_t>> crouch{ {0, 4}, {0, 5}, {0, 6}, {1, 0} };
	vector<pair<size_t, size_t>> run{ {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6} };
	vector<pair<size_t, size_t>> jump{ {2, 0}, {2, 1}, {2, 2}, {2, 3} };
	vector<pair<size_t, size_t>> mid{ {2, 4}, {2, 5}, {2, 6}, {3, 0} };
	vector<pair<size_t, size_t>> fall{ {3, 1}, {3, 2} };
	vector<pair<size_t, size_t>> slide{ {3, 3}, {3, 4}, {3, 5}, {3, 6}, {4, 0} };
	vector<pair<size_t, size_t>> grab{ {4, 1}, {4, 2}, {4, 3}, {4, 4} };
	vector<pair<size_t, size_t>> climb{ {4, 5}, {4, 6}, {5, 0}, {5, 1}, {5, 2} };
	vector<pair<size_t, size_t>> idle2{ {5, 3}, {5, 4}, {5, 5}, {5, 6} };
	vector<pair<size_t, size_t>> attack1{ {6, 0}, {6, 1}, {6, 2}, {6, 3}, {6, 4} };
	vector<pair<size_t, size_t>> attack2{ {6, 5}, {6, 6}, {7, 0}, {7, 1}, {7, 2}, {7, 3} };
	vector<pair<size_t, size_t>> attack3{ {7, 4}, {7, 5}, {7, 6}, {8, 0}, {8, 1}, {8, 2} };
	vector<pair<size_t, size_t>> hurt{ {8, 3}, {8, 4}, {8, 5} };
	vector<pair<size_t, size_t>> die{ {8, 6}, {9, 0}, {9, 1}, {9, 2}, {9, 3}, {9, 4}, {9, 5} };
	vector<pair<size_t, size_t>> jump2{ {9, 6}, {10, 0}, {10, 1} };

	vector<pair<size_t, size_t>> current = idle1;
	size_t index = 0;

	double  maxDuration = 150,
			timeBuffer = 0,
			timeElapsed = 0;
	SDL_Event event;

	bool done = false;


	while (!done) 
	{
		typedef chrono::high_resolution_clock Clock;

		double elapsedNano = 0;
		auto t1 = 	Clock::now();
		while (SDL_PollEvent(&event)) 
		{
			if (event.type == SDL_QUIT)
				done = true;
			else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE)
					done = true;

				if (event.key.keysym.scancode == SDL_SCANCODE_Q) {
					current = idle1;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_W) {
					current = crouch;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_E) {
					current = run;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_R) {
					current = jump;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_T) {
					current = mid;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_Y) {
					current = fall;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_U) {
					current = slide;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_I) {
					current = grab;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_O) {
					current = climb;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_P) {
					current = idle2;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_A) {
					current = attack1;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_S) {
					current = attack2;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_D) {
					current = attack3;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_F) {
					current = hurt;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_G) {
					current = die;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_H) {
					current = jump;
				}
				index = 0;
			}
		}

		GPU_Clear(window);
		auto currentPair = current[index];
		size_t position = currentPair.second + currentPair.first * nbCol;
		GPU_BlitTransformX(player, &rects[position], window, 150, 200, 0, 0, 0, 1, 1);
		GPU_Flip(window);

		timeBuffer = timeBuffer + timeElapsed;

		// update the animation
		if (timeBuffer > maxDuration) {
			timeBuffer = 0;
			index++;

			if (index >= current.size())
				index = 0;
		}

		auto t2 = Clock::now();

		elapsedNano = (double)(chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count());

		if (elapsedNano > 0) {
			double diff = ((1000000000.f / 60.f) - elapsedNano) / 1000000.f;

			if (diff > 0) {
				SDL_Delay((Uint32)diff);
			}
		}

		auto t3 = Clock::now();

		timeElapsed = (double)(chrono::duration_cast<chrono::nanoseconds>(t3 - t1).count()) / 1000000.f;
	}

	GPU_FreeImage(player);
	GPU_FreeTarget(window);
	GPU_Quit();
	SDL_Quit();

	return 0;
}
