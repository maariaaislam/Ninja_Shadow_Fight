#ifndef BUTTON_H
#define BUTTON_H

struct MenuButton {
	int x, y, width, height;
	int imageID;
	bool isVisible;

	void setup(int startX, int startY, int w, int h, int id) {
		x = startX; y = startY;
		width = w; height = h;
		imageID = id;
		isVisible = false;
	}

	bool isClicked(int mx, int my) {
		if (!isVisible) return false;
		return mx >= x && mx <= x + width && my >= y && my <= y + height;
	}

	void draw() {
		if (isVisible) iShowImage(x, y, width, height, imageID);
	}
};

#endif
