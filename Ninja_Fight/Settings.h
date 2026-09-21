#ifndef SETTINGS_H
#define SETTINGS_H

#include <cstdio>

struct SettingsScreen {
	static const int PAGE_COUNT = 4;

	int imgBackground, imgBack, imgClear, imgEdit, imgNext, imgNinja, imgPressKey, imgPrevious, imgReset;
	int imgSettings, imgShuriken, imgGameplay, imgLevel1, imgLevel2, imgLevel3, imgVolume;

	int currentPage, scrollY, masterVolume, changingKey;
	bool soundEnabled;

	unsigned char keyLeft, keyRight, keyJump, keyDash;
	unsigned char keyL1Shuriken1, keyL1Shuriken2, keyL1Shuriken3, keyL1Shuriken4, keyL1Sprint, keyL1Crouch;
	unsigned char keyL2Heavy, keyL2Block, keyL2Shuriken, keyL2Power, keyL2Heal;

	// Level 3 has its own top-down movement/combat bindings.
	unsigned char keyL3Up, keyL3Down;
	unsigned char keyL3Action1, keyL3Action2, keyL3Action3;
	unsigned char keyL3Heavy, keyL3Block, keyL3Power, keyL3Heal;

	int backX, backY, backW, backH;
	int prevX, prevY, prevW, prevH, nextX, nextY, nextW, nextH;
	int contentX, contentW, rowH;

	int volumeX, volumeY, volumeW, volumeH;
	int volumeMinusX, volumeMinusY, volumeMinusW, volumeMinusH;
	int volumePlusX, volumePlusY, volumePlusW, volumePlusH;
	int volumeTrackX, volumeTrackY, volumeTrackW, volumeTrackH;

	int soundX, soundY, soundW, soundH;

	void setDefaultKeys(){
		keyLeft = 'A'; keyRight = 'D'; keyJump = 'W'; keyDash = 'E';

		keyL1Shuriken1 = 'J'; keyL1Shuriken2 = 'K'; keyL1Shuriken3 = 'L'; keyL1Shuriken4 = 'I';
		keyL1Sprint = 'Q'; keyL1Crouch = 'S';

		keyL2Heavy = 'F'; keyL2Block = 'Q'; keyL2Shuriken = 'J'; keyL2Power = 'P'; keyL2Heal = 'H';

		keyL3Up = 'W'; keyL3Down = 'S';
		keyL3Action1 = 'J'; keyL3Action2 = 'K'; keyL3Action3 = 'L';
		keyL3Heavy = 'F'; keyL3Block = 'Q'; keyL3Power = 'P'; keyL3Heal = 'H';
	}

	void setDefaultSettings(){
		masterVolume = 70; soundEnabled = true; changingKey = -1; scrollY = 0;
		setDefaultKeys(); saveSettings();
	}

	void load(){
		imgBackground = iLoadImage("Images\\UI\\Common\\lb.png");
		imgBack = iLoadImage("Images\\UI\\Common\\1.png");
		imgClear = iLoadImage("Images\\UI\\Common\\2.png");
		imgEdit = iLoadImage("Images\\UI\\Common\\3.png");
		imgNext = iLoadImage("Images\\UI\\Common\\4.png");
		imgNinja = iLoadImage("Images\\UI\\Common\\5.png");
		imgPressKey = iLoadImage("Images\\UI\\Common\\6.png");
		imgPrevious = iLoadImage("Images\\UI\\Common\\7.png");
		imgReset = iLoadImage("Images\\UI\\Common\\8.png");
		imgSettings = iLoadImage("Images\\UI\\Common\\10.png");
		imgShuriken = iLoadImage("Images\\UI\\Common\\11.png");
		imgGameplay = iLoadImage("Images\\UI\\Common\\12.png");
		imgLevel1 = iLoadImage("Images\\UI\\Common\\13.png");
		imgLevel2 = iLoadImage("Images\\UI\\Common\\14.png");
		imgLevel3 = iLoadImage("Images\\UI\\Common\\15.png");
		imgVolume = iLoadImage("Images\\UI\\Common\\17.png");

		currentPage = 0; scrollY = 0; masterVolume = 70; soundEnabled = true; changingKey = -1;

		backX = 30; backY = 18; backW = 150; backH = 48;
		prevX = 300; prevY = 18; prevW = 155; prevH = 45;
		nextX = 570; nextY = 18; nextW = 155; nextH = 45;
		contentX = 245; contentW = 535; rowH = 42;

		volumeW = 500; volumeH = 92;
		volumeX = (SCREEN_WIDTH - volumeW) / 2; volumeY = 230;
		volumeMinusX = volumeX + 8; volumeMinusY = volumeY + 10; volumeMinusW = 78; volumeMinusH = 72;
		volumePlusX = volumeX + volumeW - 86; volumePlusY = volumeY + 10; volumePlusW = 78; volumePlusH = 72;
		volumeTrackX = volumeX + 105; volumeTrackY = volumeY + 43; volumeTrackW = volumeW - 210; volumeTrackH = 7;

		soundX = 412; soundY = 118; soundW = 200; soundH = 38;

		setDefaultKeys(); loadSettings();
	}

	unsigned char normalizeKey(unsigned char key){
		if (key >= 'a'&&key <= 'z')return key - 32;
		return key;
	}

	bool inside(int mx, int my, int x, int y, int w, int h){ return mx >= x&&mx <= x + w&&my >= y&&my <= y + h; }
	bool isBackClicked(int mx, int my){ return inside(mx, my, backX, backY, backW, backH); }
	bool isChangingKey(){ return changingKey >= 0; }

	void resetPage(){ currentPage = 0; scrollY = 0; changingKey = -1; }

	void nextPage(){
		currentPage++; if (currentPage >= PAGE_COUNT)currentPage = 0;
		scrollY = 0; changingKey = -1;
	}

	void previousPage(){
		currentPage--; if (currentPage<0)currentPage = PAGE_COUNT - 1;
		scrollY = 0; changingKey = -1;
	}

	void scrollUp(){ scrollY += 45; if (scrollY>0)scrollY = 0; }

	void scrollDown(){
		scrollY -= 45;
		int minScroll = 0;
		if (currentPage == 1)minScroll = -260;
		else if (currentPage == 2)minScroll = -220;
		else if (currentPage == 3)minScroll = -390;
		if (scrollY<minScroll)scrollY = minScroll;
	}

	void saveSettings(){
		FILE *file = 0;
		if (fopen_s(&file, "game_settings.txt", "w") != 0 || file == 0)return;

		fprintf(file, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
			masterVolume, soundEnabled ? 1 : 0,
			(int)keyLeft, (int)keyRight, (int)keyJump, (int)keyDash,
			(int)keyL1Shuriken1, (int)keyL1Shuriken2, (int)keyL1Shuriken3, (int)keyL1Shuriken4, (int)keyL1Sprint, (int)keyL1Crouch,
			(int)keyL2Heavy, (int)keyL2Block, (int)keyL2Shuriken, (int)keyL2Power, (int)keyL2Heal,
			(int)keyL3Action1, (int)keyL3Action2, (int)keyL3Action3,
			(int)keyL3Up, (int)keyL3Down, (int)keyL3Heavy, (int)keyL3Block, (int)keyL3Power, (int)keyL3Heal);

		fclose(file);
	}

	void loadSettings(){
		FILE *file = 0;
		if (fopen_s(&file, "game_settings.txt", "r") != 0 || file == 0)return;

		int sound, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x;
		int count = fscanf_s(file, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
			&masterVolume, &sound, &a, &b, &c, &d, &e, &f, &g, &h, &i, &j, &k, &l, &m, &n, &o, &p, &q, &r, &s, &t, &u, &v, &w, &x);

		fclose(file);
		if (count<20)return;

		soundEnabled = sound != 0;
		keyLeft = (unsigned char)a; keyRight = (unsigned char)b; keyJump = (unsigned char)c; keyDash = (unsigned char)d;

		keyL1Shuriken1 = (unsigned char)e; keyL1Shuriken2 = (unsigned char)f; keyL1Shuriken3 = (unsigned char)g; keyL1Shuriken4 = (unsigned char)h;
		keyL1Sprint = (unsigned char)i; keyL1Crouch = (unsigned char)j;

		keyL2Heavy = (unsigned char)k; keyL2Block = (unsigned char)l; keyL2Shuriken = (unsigned char)m; keyL2Power = (unsigned char)n; keyL2Heal = (unsigned char)o;

		keyL3Action1 = (unsigned char)p; keyL3Action2 = (unsigned char)q; keyL3Action3 = (unsigned char)r;

		// Old 20-value settings files still load. New Level 3 controls use defaults
		// until the file has been saved once in the new 26-value format.
		if (count >= 26){
			keyL3Up = (unsigned char)s; keyL3Down = (unsigned char)t;
			keyL3Heavy = (unsigned char)u; keyL3Block = (unsigned char)v;
			keyL3Power = (unsigned char)w; keyL3Heal = (unsigned char)x;
		}

		if (masterVolume<0)masterVolume = 0; if (masterVolume>100)masterVolume = 100;
	}

	int textWidth(char *text, void *font){
		int width = 0; for (int i = 0; text[i] != '\0'; i++)width += glutBitmapWidth(font, text[i]);
		return width;
	}

	void centerText(int y, char *text, void *font){ iText((SCREEN_WIDTH - textWidth(text, font)) / 2, y, text, font); }

	void keyText(unsigned char key, char *text){
		if (key == ' ')sprintf_s(text, 20, "SPACE");
		else sprintf_s(text, 20, "%c", key);
	}

	void drawTabs(){
		int y = 465;
		iShowImage(215, y, 145, 45, imgGameplay); iShowImage(365, y, 145, 45, imgLevel1);
		iShowImage(515, y, 145, 45, imgLevel2); iShowImage(665, y, 145, 45, imgLevel3);

		int x = 215 + currentPage * 150;
		iSetColor(105, 26, 24); iRectangle(x, y, 145, 45); iRectangle(x + 1, y + 1, 143, 43);
	}

	void drawHeader(){
		iShowImage(380, 525, 265, 55, imgSettings); iShowImage(118, 470, 80, 80, imgNinja); iShowImage(830, 475, 95, 65, imgShuriken);
		drawTabs();
	}

	void drawControlRow(int baseY, char *action, unsigned char key, int keyID){
		int y = baseY + scrollY; if (y<85 || y>440)return;

		iSetColor(20, 20, 22); iFilledRectangle(contentX, y, contentW, rowH);
		iSetColor(110, 80, 45); iRectangle(contentX, y, contentW, rowH);
		iSetColor(245, 215, 155); iText(contentX + 25, y + 14, action, GLUT_BITMAP_HELVETICA_18);

		char text[20]; keyText(key, text);
		iSetColor(255, 235, 175); iText(contentX + 320, y + 14, text, GLUT_BITMAP_HELVETICA_18);

		if (changingKey == keyID)iShowImage(contentX + 395, y + 5, 120, 32, imgPressKey);
		else iShowImage(contentX + 405, y + 5, 95, 32, imgEdit);
	}

	void drawVolumeControl(){
		iShowImage(volumeX, volumeY, volumeW, volumeH, imgVolume);

		float ratio = masterVolume / 100.0f; if (ratio<0)ratio = 0; if (ratio>1)ratio = 1;
		int fillW = (int)(volumeTrackW*ratio);

		iSetColor(105, 26, 24); if (fillW>0)iFilledRectangle(volumeTrackX, volumeTrackY, fillW, volumeTrackH);

		int knobX = volumeTrackX + fillW;
		if (knobX<volumeTrackX + 4)knobX = volumeTrackX + 4;
		if (knobX>volumeTrackX + volumeTrackW - 4)knobX = volumeTrackX + volumeTrackW - 4;

		iSetColor(188, 143, 72); iFilledCircle(knobX, volumeTrackY + volumeTrackH / 2, 6);

		char volumeText[20]; sprintf_s(volumeText, sizeof(volumeText), "%d%%", masterVolume);
		iSetColor(225, 205, 165); centerText(volumeY - 24, volumeText, GLUT_BITMAP_HELVETICA_18);
	}

	void drawSoundToggle(){
		iSetColor(22, 20, 20); iFilledRectangle(soundX, soundY, soundW, soundH);
		iSetColor(soundEnabled ? 188 : 105, soundEnabled ? 143 : 26, soundEnabled ? 72 : 24); iRectangle(soundX, soundY, soundW, soundH);

		char text[40];
		if (soundEnabled)sprintf_s(text, sizeof(text), "GAME AUDIO: ON");
		else sprintf_s(text, sizeof(text), "GAME AUDIO: OFF");

		iSetColor(235, 220, 185); centerText(soundY + 12, text, GLUT_BITMAP_HELVETICA_18);
	}

	void drawGameplayPage(){
		iSetColor(245, 215, 150); centerText(425, (char*)"GAME SETTINGS", GLUT_BITMAP_HELVETICA_18);
		iSetColor(235, 230, 215); centerText(390, (char*)"Customize audio and controls before entering battle.", GLUT_BITMAP_HELVETICA_18);
		centerText(360, (char*)"Changes are saved automatically.", GLUT_BITMAP_HELVETICA_18);

		iSetColor(245, 210, 145); centerText(330, (char*)"MASTER VOLUME", GLUT_BITMAP_HELVETICA_18);
		drawVolumeControl();

		iSetColor(245, 210, 145); centerText(175, (char*)"AUDIO", GLUT_BITMAP_HELVETICA_18);
		drawSoundToggle();

		iShowImage(437, 70, 150, 37, imgReset);
	}

	void drawLevel1Page(){
		iSetColor(245, 215, 150); centerText(425, (char*)"LEVEL 1 CONTROLS", GLUT_BITMAP_HELVETICA_18);
		iSetColor(220, 215, 200); centerText(395, (char*)"Platform movement and shuriken combat", GLUT_BITMAP_HELVETICA_18);

		drawControlRow(335, (char*)"MOVE LEFT", keyLeft, 0); drawControlRow(284, (char*)"MOVE RIGHT", keyRight, 1);
		drawControlRow(233, (char*)"JUMP", keyJump, 2); drawControlRow(182, (char*)"DASH", keyDash, 3);
		drawControlRow(131, (char*)"SPRINT", keyL1Sprint, 8); drawControlRow(80, (char*)"CROUCH / FAST FALL", keyL1Crouch, 9);
		drawControlRow(29, (char*)"STANDARD SHURIKEN", keyL1Shuriken1, 4); drawControlRow(-22, (char*)"FAST SHURIKEN", keyL1Shuriken2, 5);
		drawControlRow(-73, (char*)"HEAVY SHURIKEN", keyL1Shuriken3, 6); drawControlRow(-124, (char*)"CRIMSON SHURIKEN", keyL1Shuriken4, 7);
	}

	void drawLevel2Page(){
		iSetColor(245, 215, 150); centerText(425, (char*)"LEVEL 2 CONTROLS", GLUT_BITMAP_HELVETICA_18);
		iSetColor(220, 215, 200); centerText(395, (char*)"Arena combat against the Shadow Fighter", GLUT_BITMAP_HELVETICA_18);

		drawControlRow(335, (char*)"MOVE LEFT", keyLeft, 0); drawControlRow(284, (char*)"MOVE RIGHT", keyRight, 1);
		drawControlRow(233, (char*)"JUMP", keyJump, 2); drawControlRow(182, (char*)"DASH", keyDash, 3);
		drawControlRow(131, (char*)"HEAVY STRIKE", keyL2Heavy, 10); drawControlRow(80, (char*)"BLOCK", keyL2Block, 11);
		drawControlRow(29, (char*)"THROW SHURIKEN", keyL2Shuriken, 12); drawControlRow(-22, (char*)"POWER-UP", keyL2Power, 13);
		drawControlRow(-73, (char*)"HEAL", keyL2Heal, 14);
	}

	void drawLevel3Page(){
		iSetColor(245, 215, 150); centerText(425, (char*)"LEVEL 3 CONTROLS", GLUT_BITMAP_HELVETICA_18);
		iSetColor(220, 215, 200); centerText(395, (char*)"Top-down exploration and arena combat", GLUT_BITMAP_HELVETICA_18);

		drawControlRow(335, (char*)"MOVE LEFT", keyLeft, 0); drawControlRow(284, (char*)"MOVE RIGHT", keyRight, 1);
		drawControlRow(233, (char*)"MOVE UP / FORWARD", keyL3Up, 18); drawControlRow(182, (char*)"MOVE DOWN / BACK", keyL3Down, 19);
		drawControlRow(131, (char*)"DASH", keyDash, 3); drawControlRow(80, (char*)"PUNCH", keyL3Action1, 15);
		drawControlRow(29, (char*)"KICK", keyL3Action2, 16); drawControlRow(-22, (char*)"HEAVY STRIKE", keyL3Heavy, 20);
		drawControlRow(-73, (char*)"BLOCK", keyL3Block, 21); drawControlRow(-124, (char*)"THROW SHURIKEN", keyL3Action3, 17);
		drawControlRow(-175, (char*)"POWER-UP", keyL3Power, 22); drawControlRow(-226, (char*)"HEAL", keyL3Heal, 23);
	}

	void drawBottomNavigation(){
		iShowImage(backX, backY, backW, backH, imgBack); iShowImage(prevX, prevY, prevW, prevH, imgPrevious); iShowImage(nextX, nextY, nextW, nextH, imgNext);
		char page[20]; sprintf_s(page, sizeof(page), "%d / 4", currentPage + 1);
		iSetColor(245, 215, 160); centerText(32, page, GLUT_BITMAP_HELVETICA_18);
	}

	void draw(){
		iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, imgBackground); drawHeader();
		if (currentPage == 0)drawGameplayPage(); else if (currentPage == 1)drawLevel1Page();
		else if (currentPage == 2)drawLevel2Page(); else drawLevel3Page();
		drawBottomNavigation();
	}

	int rowKeyAt(int mx, int my, int baseY, int keyID){
		int y = baseY + scrollY; if (y<85 || y>440)return -1;
		if (inside(mx, my, contentX + 390, y, 125, rowH))return keyID;
		return -1;
	}

	void handleLevel1Edit(int mx, int my){
		int r;
		r = rowKeyAt(mx, my, 335, 0); if (r >= 0){ changingKey = r; return; }r = rowKeyAt(mx, my, 284, 1); if (r >= 0){ changingKey = r; return; }
		r = rowKeyAt(mx, my, 233, 2); if (r >= 0){ changingKey = r; return; }r = rowKeyAt(mx, my, 182, 3); if (r >= 0){ changingKey = r; return; }
		r = rowKeyAt(mx, my, 131, 8); if (r >= 0){ changingKey = r; return; }r = rowKeyAt(mx, my, 80, 9); if (r >= 0){ changingKey = r; return; }
		r = rowKeyAt(mx, my, 29, 4); if (r >= 0){ changingKey = r; return; }r = rowKeyAt(mx, my, -22, 5); if (r >= 0){ changingKey = r; return; }
		r = rowKeyAt(mx, my, -73, 6); if (r >= 0){ changingKey = r; return; }r = rowKeyAt(mx, my, -124, 7); if (r >= 0){ changingKey = r; return; }
	}

	void handleLevel2Edit(int mx, int my){
		int r;
		r = rowKeyAt(mx, my, 335, 0); if (r >= 0){ changingKey = r; return; }r = rowKeyAt(mx, my, 284, 1); if (r >= 0){ changingKey = r; return; }
		r = rowKeyAt(mx, my, 233, 2); if (r >= 0){ changingKey = r; return; }r = rowKeyAt(mx, my, 182, 3); if (r >= 0){ changingKey = r; return; }
		r = rowKeyAt(mx, my, 131, 10); if (r >= 0){ changingKey = r; return; }r = rowKeyAt(mx, my, 80, 11); if (r >= 0){ changingKey = r; return; }
		r = rowKeyAt(mx, my, 29, 12); if (r >= 0){ changingKey = r; return; }r = rowKeyAt(mx, my, -22, 13); if (r >= 0){ changingKey = r; return; }
		r = rowKeyAt(mx, my, -73, 14); if (r >= 0){ changingKey = r; return; }
	}

	void handleLevel3Edit(int mx, int my){
		int r;
		r = rowKeyAt(mx, my, 335, 0); if (r >= 0){ changingKey = r; return; }r = rowKeyAt(mx, my, 284, 1); if (r >= 0){ changingKey = r; return; }
		r = rowKeyAt(mx, my, 233, 18); if (r >= 0){ changingKey = r; return; }r = rowKeyAt(mx, my, 182, 19); if (r >= 0){ changingKey = r; return; }
		r = rowKeyAt(mx, my, 131, 3); if (r >= 0){ changingKey = r; return; }r = rowKeyAt(mx, my, 80, 15); if (r >= 0){ changingKey = r; return; }
		r = rowKeyAt(mx, my, 29, 16); if (r >= 0){ changingKey = r; return; }r = rowKeyAt(mx, my, -22, 20); if (r >= 0){ changingKey = r; return; }
		r = rowKeyAt(mx, my, -73, 21); if (r >= 0){ changingKey = r; return; }r = rowKeyAt(mx, my, -124, 17); if (r >= 0){ changingKey = r; return; }
		r = rowKeyAt(mx, my, -175, 22); if (r >= 0){ changingKey = r; return; }r = rowKeyAt(mx, my, -226, 23); if (r >= 0){ changingKey = r; return; }
	}

	void setVolumeFromTrack(int mx){
		int value = (int)(((float)(mx - volumeTrackX) / (float)volumeTrackW)*100.0f);
		if (value<0)value = 0; if (value>100)value = 100;
		masterVolume = value; saveSettings();
	}

	void handleClick(int mx, int my){
		if (inside(mx, my, prevX, prevY, prevW, prevH)){ previousPage(); return; }
		if (inside(mx, my, nextX, nextY, nextW, nextH)){ nextPage(); return; }

		if (inside(mx, my, 215, 465, 145, 45)){ currentPage = 0; scrollY = 0; changingKey = -1; return; }
		if (inside(mx, my, 365, 465, 145, 45)){ currentPage = 1; scrollY = 0; changingKey = -1; return; }
		if (inside(mx, my, 515, 465, 145, 45)){ currentPage = 2; scrollY = 0; changingKey = -1; return; }
		if (inside(mx, my, 665, 465, 145, 45)){ currentPage = 3; scrollY = 0; changingKey = -1; return; }

		if (currentPage == 0){
			if (inside(mx, my, volumeMinusX, volumeMinusY, volumeMinusW, volumeMinusH)){ masterVolume -= 10; if (masterVolume<0)masterVolume = 0; saveSettings(); return; }
			if (inside(mx, my, volumePlusX, volumePlusY, volumePlusW, volumePlusH)){ masterVolume += 10; if (masterVolume>100)masterVolume = 100; saveSettings(); return; }
			if (inside(mx, my, volumeTrackX, volumeTrackY - 10, volumeTrackW, volumeTrackH + 20)){ setVolumeFromTrack(mx); return; }
			if (inside(mx, my, soundX, soundY, soundW, soundH)){ soundEnabled = !soundEnabled; saveSettings(); return; }
			if (inside(mx, my, 437, 70, 150, 37)){ setDefaultSettings(); return; }
		}
		else if (currentPage == 1)handleLevel1Edit(mx, my);
		else if (currentPage == 2)handleLevel2Edit(mx, my);
		else handleLevel3Edit(mx, my);
	}

	void handleKey(unsigned char key){
		if (changingKey<0)return;
		if (key == 27){ changingKey = -1; return; }

		key = normalizeKey(key);

		if (changingKey == 0)keyLeft = key;
		else if (changingKey == 1)keyRight = key;
		else if (changingKey == 2)keyJump = key;
		else if (changingKey == 3)keyDash = key;
		else if (changingKey == 4)keyL1Shuriken1 = key;
		else if (changingKey == 5)keyL1Shuriken2 = key;
		else if (changingKey == 6)keyL1Shuriken3 = key;
		else if (changingKey == 7)keyL1Shuriken4 = key;
		else if (changingKey == 8)keyL1Sprint = key;
		else if (changingKey == 9)keyL1Crouch = key;
		else if (changingKey == 10)keyL2Heavy = key;
		else if (changingKey == 11)keyL2Block = key;
		else if (changingKey == 12)keyL2Shuriken = key;
		else if (changingKey == 13)keyL2Power = key;
		else if (changingKey == 14)keyL2Heal = key;
		else if (changingKey == 15)keyL3Action1 = key;
		else if (changingKey == 16)keyL3Action2 = key;
		else if (changingKey == 17)keyL3Action3 = key;
		else if (changingKey == 18)keyL3Up = key;
		else if (changingKey == 19)keyL3Down = key;
		else if (changingKey == 20)keyL3Heavy = key;
		else if (changingKey == 21)keyL3Block = key;
		else if (changingKey == 22)keyL3Power = key;
		else if (changingKey == 23)keyL3Heal = key;

		changingKey = -1; saveSettings();
	}
};

#endif
