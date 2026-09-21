#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <cstdio>

struct LevelScoreRecord {
	bool unlocked, completed;
	int bestScore, bestStars;
	int bestGold, totalGold;
};

struct ScoreboardScreen {
	static const int LEVEL_COUNT = 3;

	LevelScoreRecord record[LEVEL_COUNT];

	int imgBackground, imgTitle, imgStar, imgBackHome;
	int imgPage[LEVEL_COUNT];
	int currentPage;

	void setDefaults() {
		for (int i = 0; i < LEVEL_COUNT; i++) {
			record[i].unlocked = false;
			record[i].completed = false;
			record[i].bestScore = record[i].bestStars = 0;
			record[i].bestGold = record[i].totalGold = 0;
		}

		record[0].unlocked = true;
		currentPage = 0;
	}

	void loadImages() {
		imgBackground = iLoadImage("Images\\UI\\Common\\lb.png");
		imgTitle = iLoadImage("Images\\UI\\Common\\score.png");
		imgStar = iLoadImage("Images\\UI\\Common\\s.png");
		imgBackHome = iLoadImage("Images\\UI\\Common\\level-01.png");

		imgPage[0] = iLoadImage("Images\\UI\\LevelSelect\\1.png");
		imgPage[1] = iLoadImage("Images\\UI\\LevelSelect\\2.png");
		imgPage[2] = iLoadImage("Images\\UI\\LevelSelect\\3.png");
	}

	void loadProgress() {
		setDefaults();

		FILE *file = 0;
		errno_t openResult = fopen_s(&file, "level_progress.txt", "r");
		if (openResult != 0 || file == 0) return;

		for (int i = 0; i < LEVEL_COUNT; i++) {
			int unlockedValue = 0, completedValue = 0;
			int score = 0, stars = 0, gold = 0, total = 0;

			int readCount = fscanf_s(file, "%d %d %d %d %d %d",
				&unlockedValue, &completedValue, &score, &stars, &gold, &total);

			if (readCount != 6) break;

			record[i].unlocked = unlockedValue != 0;
			record[i].completed = completedValue != 0;
			record[i].bestScore = score;
			record[i].bestStars = stars;
			record[i].bestGold = gold;
			record[i].totalGold = total;
		}

		fclose(file);
		record[0].unlocked = true;
	}

	void saveProgress() {
		FILE *file = 0;
		errno_t openResult = fopen_s(&file, "level_progress.txt", "w");
		if (openResult != 0 || file == 0) return;

		for (int i = 0; i < LEVEL_COUNT; i++) {
			fprintf(file, "%d %d %d %d %d %d\n",
				record[i].unlocked ? 1 : 0,
				record[i].completed ? 1 : 0,
				record[i].bestScore,
				record[i].bestStars,
				record[i].bestGold,
				record[i].totalGold);
		}

		fclose(file);
	}

	void submitLevelResult(int levelNumber, int score, int stars,
		int goldCollected, int totalGold) {

		if (levelNumber < 1 || levelNumber > LEVEL_COUNT) return;

		int index = levelNumber - 1;

		record[index].completed = true;
		record[index].unlocked = true;

		if (score > record[index].bestScore) record[index].bestScore = score;
		if (stars > record[index].bestStars) record[index].bestStars = stars;
		if (goldCollected > record[index].bestGold) record[index].bestGold = goldCollected;

		record[index].totalGold = totalGold;

		if (stars >= 1 && levelNumber < LEVEL_COUNT)
			record[index + 1].unlocked = true;

		saveProgress();
	}

	bool isUnlocked(int levelNumber) {
		if (levelNumber < 1 || levelNumber > LEVEL_COUNT) return false;
		return record[levelNumber - 1].unlocked;
	}

	int getStars(int levelNumber) {
		if (levelNumber < 1 || levelNumber > LEVEL_COUNT) return 0;
		return record[levelNumber - 1].bestStars;
	}

	int getBestScore(int levelNumber) {
		if (levelNumber < 1 || levelNumber > LEVEL_COUNT) return 0;
		return record[levelNumber - 1].bestScore;
	}

	void resetPage() { currentPage = 0; }

	void nextPage() {
		currentPage++;
		if (currentPage >= LEVEL_COUNT) currentPage = 0;
	}

	void previousPage() {
		currentPage--;
		if (currentPage < 0) currentPage = LEVEL_COUNT - 1;
	}

	bool isBackClicked(int mx, int my) {
		return mx >= 30 && mx <= 255 && my >= 18 && my <= 63;
	}

	int textWidthApprox(char *text, void *font) {
		int length = 0;

		while (text[length] != '\0')
			length++;

		int charWidth = 9;

		if (font == GLUT_BITMAP_HELVETICA_18)
			charWidth = 10;

		return length * charWidth;
	}

	void drawCenteredText(int y, char *text, void *font) {
		int x = (SCREEN_WIDTH - textWidthApprox(text, font)) / 2;
		iText(x, y, text, font);
	}

	void drawStarsCentered(int y, int count) {
		int starSize = 42, gap = 16;
		int totalWidth = starSize * 3 + gap * 2;
		int startX = (SCREEN_WIDTH - totalWidth) / 2;

		for (int i = 0; i < 3; i++) {
			int x = startX + i * (starSize + gap);

			iSetColor(85, 70, 32);
			iCircle(x + starSize / 2, y + starSize / 2, 18);

			if (i < count)
				iShowImage(x, y, starSize, starSize, imgStar);
		}
	}

	void drawCurrentLevel() {
		int index = currentPage;

		char levelText[40];
		sprintf_s(levelText, sizeof(levelText), "LEVEL %d", index + 1);

		iSetColor(245, 214, 135);
		drawCenteredText(410, levelText, GLUT_BITMAP_HELVETICA_18);

		if (!record[index].unlocked) {
			iSetColor(175, 175, 175);
			drawCenteredText(330, (char*)"LOCKED", GLUT_BITMAP_HELVETICA_18);

			iSetColor(215, 205, 180);
			drawCenteredText(292, (char*)"COMPLETE THE PREVIOUS LEVEL",
				GLUT_BITMAP_HELVETICA_12);

			return;
		}

		char scoreText[70];
		sprintf_s(scoreText, sizeof(scoreText),
			"BEST SCORE: %d", record[index].bestScore);

		iSetColor(235, 225, 200);
		drawCenteredText(350, scoreText, GLUT_BITMAP_HELVETICA_18);

		if (record[index].completed) {
			iSetColor(215, 205, 180);

			if (index == 0) {
				char goldText[70];

				sprintf_s(goldText, sizeof(goldText),
					"GOLD: %d / %d",
					record[index].bestGold,
					record[index].totalGold);

				drawCenteredText(305, goldText,
					GLUT_BITMAP_HELVETICA_18);
			}

			else if (index == 1) {
				drawCenteredText(305,
					(char*)"ARENA FIGHT COMPLETED",
					GLUT_BITMAP_HELVETICA_18);
			}

			else {
				drawCenteredText(305,
					(char*)"LEVEL COMPLETED",
					GLUT_BITMAP_HELVETICA_18);
			}
		}

		else {
			iSetColor(175, 175, 175);

			drawCenteredText(305,
				(char*)"NOT COMPLETED",
				GLUT_BITMAP_HELVETICA_18);
		}

		iSetColor(245, 214, 135);

		drawCenteredText(250,
			(char*)"BEST STARS",
			GLUT_BITMAP_HELVETICA_18);

		drawStarsCentered(185, record[index].bestStars);
	}

	void drawPageIndicator() {
		int w = 110, h = 36;
		int x = (SCREEN_WIDTH - w) / 2;
		int y = 92;

		iShowImage(x, y, w, h, imgPage[currentPage]);
	}

	void draw() {
		iShowImage(0, 0,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			imgBackground);

		iShowImage(395, 515,
			235, 62,
			imgTitle);

		iSetColor(235, 220, 185);

		drawCenteredText(475,
			(char*)"BEST LEVEL RESULT",
			GLUT_BITMAP_HELVETICA_18);

		drawCurrentLevel();
		drawPageIndicator();

		iShowImage(30, 18, 225, 45, imgBackHome);
	}
};

#endif