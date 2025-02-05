#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <chrono>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_gpu.h>

using namespace std;

int main(int argc, char* args[])
{
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS);

	GPU_Target* window = GPU_InitRenderer(GPU_RENDERER_OPENGL_3, 320, 240, GPU_DEFAULT_INIT_FLAGS);
	GPU_Image* player = GPU_LoadImage("advenurer-sheet.png");

	vector<GPU_Rect> rects;
	size_t numRow = 11;
	size_t numCol = 7;
	size_t spriteWidth = 50;
	size_t spriteHeight = 37;

	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "hello world", "Successful", NULL);

	for (int i = 0; i< numRow; i++)
	{
		for (int j = 0; i < numCol; j++)
		{
			rects.push_back(GPU_Rect{ (float)(j * spriteWidth), (float)(i * spriteHeight), (float)(spriteWidth), (float)(spriteWidth)});
		}
	}

	vector<pair<size_t, size_t>> idle1{ {0,0}, {0,1},{0,2},{0,3} };

	vector<pair<size_t, size_t>> current = idle1;
	size_t index = 0;
	int maxDuration = 150,
		timeBuffer = 0,
		timeElapsed = 0;
	SDL_Event event;

	bool done = false;

	while (!done) 
	{
		typedef chrono::high_resolution_clock Clock;
		short elapsedNano = 0;
		auto t1 = Clock::now();

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
			{
				done = true;
			}
			index = 0;
		}
	}

	GPU_Clear(window);
	auto currentPair = current[index];
	size_t position = currentPair.second + currentPair.first * numCol;
	GPU_BlitTransformX(player, &rects[position], window, 120, 120, 0, 0, 0, 1, 1);

	timeBuffer += timeElapsed;

	if(timeBuffer > maxDuration)
	{
		timeBuffer = 0;
		index++;

		if (index >= current.size())
		{
			index = 0;
		}
	}

	GPU_FreeImage(player);
	GPU_FreeTarget(window);
	GPU_Quit();
	SDL_Quit();
	
	return 0;
}
