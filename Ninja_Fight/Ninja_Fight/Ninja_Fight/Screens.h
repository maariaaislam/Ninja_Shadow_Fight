#ifndef SCREENS_H
#define SCREENS_H

#include "iGraphics.h"
#include "Button.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 600

struct GameScreens {
	int imgLoading, imgHome;
	int loadingTimer, maxLoadingTime, buttonAnimTimer;

	MenuButton btnNewGame, btnOptions, btnSettings, btnScores, btnQuit;

	void loadAllImages() {
		imgLoading = iLoadImage("Images\\UI\\Home\\1.png");
		imgHome = iLoadImage("Images\\UI\\Home\\Home.jpg");

		int imgNewGame = iLoadImage("Images\\UI\\Home\\new_game.png");
		int imgOptions = iLoadImage("Images\\UI\\Home\\options.png");
		int imgSettings = iLoadImage("Images\\UI\\Home\\settings.png");
		int imgScores = iLoadImage("Images\\UI\\Common\\score.png");
		int imgQuit = iLoadImage("Images\\UI\\Home\\quit.png");

		loadingTimer = 0;
		maxLoadingTime = 90;
		buttonAnimTimer = 0;

		int startX = (int)(SCREEN_WIDTH * 0.65);
		int btnW = (int)(SCREEN_WIDTH * 0.25);
		int btnH = (int)(SCREEN_HEIGHT * 0.08);

		btnNewGame.setup(startX, (int)(SCREEN_HEIGHT * 0.75), btnW, btnH, imgNewGame);
		btnOptions.setup(startX, (int)(SCREEN_HEIGHT * 0.65), btnW, btnH, imgOptions);
		btnSettings.setup(startX, (int)(SCREEN_HEIGHT * 0.55), btnW, btnH, imgSettings);
		btnScores.setup(startX, (int)(SCREEN_HEIGHT * 0.45), btnW, btnH, imgScores);
		btnQuit.setup(startX, (int)(SCREEN_HEIGHT * 0.35), btnW, btnH, imgQuit);
	}

	void drawLoading() {
		iSetColor(0, 0, 0);
		iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		iShowImage(
			0,
			0,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			imgLoading
			);
	}

	void drawHome() {
		iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, imgHome);

		btnNewGame.draw();
		btnOptions.draw();
		btnSettings.draw();
		btnScores.draw();
		btnQuit.draw();
	}

	void updateAnimation() {
		buttonAnimTimer++;

		if (buttonAnimTimer > 10) btnNewGame.isVisible = true;
		if (buttonAnimTimer > 20) btnOptions.isVisible = true;
		if (buttonAnimTimer > 30) btnSettings.isVisible = true;
		if (buttonAnimTimer > 40) btnScores.isVisible = true;
		if (buttonAnimTimer > 50) btnQuit.isVisible = true;
	}
};

#endif