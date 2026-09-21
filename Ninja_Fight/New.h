#ifndef NEW_H
#define NEW_H

struct LevelCard {
	int x, y, width, height;
	int lockedImage, unlockedImage;
	bool unlocked;
	int stars;

	void setup(int startX, int startY, int w, int h,
		int lockedID, int unlockedID, bool isUnlocked) {

		x = startX; y = startY;
		width = w; height = h;

		lockedImage = lockedID;
		unlockedImage = unlockedID;

		unlocked = isUnlocked;
		stars = 0;
	}

	bool containsPoint(int mx, int my) {
		return mx >= x && mx <= x + width &&
			my >= y && my <= y + height;
	}

	void draw(bool hovered) {
		int image = unlocked ? unlockedImage : lockedImage;

		if (hovered) {
			int grow = 7;

			iShowImage(
				x - grow,
				y - grow,
				width + grow * 2,
				height + grow * 2,
				image
				);
		}
		else {
			iShowImage(
				x,
				y,
				width,
				height,
				image
				);
		}
	}
};


struct NewGameScreen {
	static const int LEVEL_COUNT = 3;

	int imgBackground, imgTitle, imgBackHome;

	// Hover messages
	int imgClickLevel1;
	int imgClickLevel2;
	int imgClickLevel3;

	// Locked messages
	int imgLevelLocked;
	int imgCompletePrevious;

	// Level cards
	int imgLevel1;

	int imgLevel2Locked;
	int imgLevel3Locked;

	int imgLevel2Unlocked;
	int imgLevel3Unlocked;

	int imgStar;

	LevelCard level[LEVEL_COUNT];

	int hoveredLevel;


	void load() {
		imgBackground =
			iLoadImage("Images\\UI\\Common\\lb.png");

		imgTitle =
			iLoadImage("Images\\UI\\LevelSelect\\level.png");

		imgBackHome =
			iLoadImage("Images\\UI\\Common\\level-01.png");


		// CLICK LEVEL 1 TO START
		imgClickLevel1 =
			iLoadImage("Images\\UI\\LevelSelect\\level-02.png");

		// CLICK LEVEL 2 TO START
		imgClickLevel2 =
			iLoadImage("Images\\UI\\LevelSelect\\11.png");

		// CLICK LEVEL 3 TO START
		imgClickLevel3 =
			iLoadImage("Images\\UI\\LevelSelect\\22.png");


		imgLevelLocked =
			iLoadImage("Images\\UI\\LevelSelect\\level-03.png");

		imgCompletePrevious =
			iLoadImage("Images\\UI\\LevelSelect\\level-04.png");

		imgLevel1 =
			iLoadImage("Images\\UI\\LevelSelect\\l-01.png");


		imgLevel2Locked =
			iLoadImage("Images\\UI\\LevelSelect\\l-02.png");

		imgLevel2Unlocked =
			iLoadImage("Images\\UI\\LevelSelect\\ul-02.png");


		imgLevel3Locked =
			iLoadImage("Images\\UI\\LevelSelect\\l-03.png");

		imgLevel3Unlocked =
			iLoadImage("Images\\UI\\LevelSelect\\ul-03.png");


		imgStar =
			iLoadImage("Images\\UI\\Common\\s.png");


		hoveredLevel = -1;

		int cardW = 125;
		int cardH = 150;

		int gap = 55;

		int totalWidth =
			cardW * LEVEL_COUNT +
			gap * (LEVEL_COUNT - 1);

		int startX =
			(SCREEN_WIDTH - totalWidth) / 2;

		int cardY = 240;


		level[0].setup(
			startX,
			cardY,
			cardW,
			cardH,
			imgLevel1,
			imgLevel1,
			true
			);


		level[1].setup(
			startX + cardW + gap,
			cardY,
			cardW,
			cardH,
			imgLevel2Locked,
			imgLevel2Unlocked,
			false
			);


		level[2].setup(
			startX + (cardW + gap) * 2,
			cardY,
			cardW,
			cardH,
			imgLevel3Locked,
			imgLevel3Unlocked,
			false
			);
	}


	void setUnlocked(int levelNumber, bool value) {
		if (levelNumber < 1 || levelNumber > LEVEL_COUNT)
			return;

		level[levelNumber - 1].unlocked = value;

		level[0].unlocked = true;
	}


	void setStars(int levelNumber, int starCount) {
		if (levelNumber < 1 || levelNumber > LEVEL_COUNT)
			return;

		if (starCount < 0)
			starCount = 0;

		if (starCount > 3)
			starCount = 3;

		level[levelNumber - 1].stars = starCount;
	}




	void updateHover(int mx, int my) {
		hoveredLevel = -1;

		for (int i = 0; i < LEVEL_COUNT; i++) {
			if (level[i].containsPoint(mx, my)) {
				hoveredLevel = i;
				return;
			}
		}
	}


	void clearHover() {
		hoveredLevel = -1;
	}


	void update() {
	}


	// ========================================================
	// DRAW STARS
	// ========================================================

	void drawStarsForLevel(int index) {
		if (!level[index].unlocked)
			return;

		int starSize = 24;
		int gap = 5;

		int totalWidth =
			starSize * 3 +
			gap * 2;

		int startX =
			level[index].x +
			(level[index].width - totalWidth) / 2;

		int starY =
			level[index].y - 32;


		for (int i = 0; i < 3; i++) {
			int starX =
				startX +
				i * (starSize + gap);


			// Empty star position
			iSetColor(85, 70, 32);

			iCircle(
				starX + starSize / 2,
				starY + starSize / 2,
				10
				);


			if (i < level[index].stars) {
				iShowImage(
					starX,
					starY,
					starSize,
					starSize,
					imgStar
					);
			}
		}
	}




	void drawHoverMessage() {
		if (hoveredLevel < 0)
			return;


		if (hoveredLevel == 0) {
			iShowImage(
				355,
				145,
				315,
				52,
				imgClickLevel1
				);

			return;
		}


		if (!level[hoveredLevel].unlocked) {
			iShowImage(
				397,
				150,
				230,
				43,
				imgLevelLocked
				);


			iShowImage(
				367,
				112,
				290,
				38,
				imgCompletePrevious
				);

			return;
		}



		if (hoveredLevel == 1) {
			iShowImage(
				355,
				145,
				315,
				52,
				imgClickLevel2
				);

			return;
		}


		// ====================================================
		// LEVEL 3 UNLOCKED
		// ====================================================

		if (hoveredLevel == 2) {
			iShowImage(
				355,
				145,
				315,
				52,
				imgClickLevel3
				);

			return;
		}
	}


	void draw() {
		iShowImage(
			0,
			0,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			imgBackground
			);


		iShowImage(
			374,
			448,
			276,
			72,
			imgTitle
			);


		for (int i = 0; i < LEVEL_COUNT; i++) {
			level[i].draw(
				hoveredLevel == i
				);

			drawStarsForLevel(i);
		}


		drawHoverMessage();


		iShowImage(
			38,
			22,
			225,
			45,
			imgBackHome
			);
	}



	int handleClick(int mx, int my) {
		for (int i = 0; i < LEVEL_COUNT; i++) {
			if (level[i].containsPoint(mx, my)) {

				if (level[i].unlocked)
					return i + 1;

				return 0;
			}
		}

		return 0;
	}
};

#endif