#include <cstdlib>
#include <cstdio>

#include "Screens.h"
#include "Gameplay.h"
#include "Scoreboard.h"
#include "New.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Settings.h"
#include "Music.h"

#define STATE_LOADING -1
#define STATE_HOME 0
#define STATE_LEVEL_SELECT 1
#define STATE_LEVEL1 2
#define STATE_SCOREBOARD 3
#define STATE_SETTINGS 4
#define STATE_LEVEL2 5
#define STATE_LEVEL3 6

GameScreens myScreens;
NinjaPlayer myNinja;
Camera2D myCamera;
ShurikenSystem myWeapons;
NewGameScreen myLevelScreen;
ScoreboardScreen myScoreboard;
Level1Stage myLevel1;
Level2Stage myLevel2;
Level3Stage myLevel3;
SettingsScreen mySettings;
MusicSystem myMusic;

int gameState = STATE_LOADING;
int currentLevel = 0;

struct InputState {
	bool left, right, down;
	bool jump, sprint, dash;
	bool attack[4];
	bool power, heal, shuriken;
	bool jumpPressed, dashPressed;
	bool attackPressed[4];
	bool powerPressed, healPressed, shurikenPressed;

	void reset() {
		left = right = down = false;
		jump = sprint = dash = false;
		power = heal = shuriken = false;
		jumpPressed = dashPressed = false;
		powerPressed = healPressed = shurikenPressed = false;

		for (int i = 0; i < 4; i++) {
			attack[i] = false;
			attackPressed[i] = false;
		}
	}

	void clearPressed() {
		jumpPressed = dashPressed = false;
		powerPressed = healPressed = shurikenPressed = false;
		for (int i = 0; i < 4; i++) attackPressed[i] = false;
	}
};

InputState input;
bool keyboardHooksInstalled = false;

void syncProgressToLevelScreen();
void startLevel1();
void startLevel2();
void startLevel3();
void returnToLevelSelect();
void returnHome();

void handleLevel1KeyDown(unsigned char key);
void handleLevel2KeyDown(unsigned char key);
void handleLevel3KeyDown(unsigned char key);
void handleNormalKeyDown(unsigned char key);
void handleNormalKeyUp(unsigned char key);

void handleSpecialKeyDown(int key);
void handleSpecialKeyUp(int key);

void directKeyboardDown(unsigned char key, int x, int y);
void directKeyboardUp(unsigned char key, int x, int y);
void directSpecialDown(int key, int x, int y);
void directSpecialUp(int key, int x, int y);

void installKeyboardHooks();
void processLevel1();
void processLevel2();
void processLevel3();
void updateTimer();
void fixedUpdate();
void syncMusicToState();


void syncMusicToState() {
	myMusic.setEnabled(mySettings.soundEnabled); myMusic.setVolume(mySettings.masterVolume);
	if (gameState == STATE_LOADING)myMusic.play(MUSIC_LOADING);
	else if (gameState == STATE_LEVEL1)myMusic.play(MUSIC_LEVEL1);
	else if (gameState == STATE_LEVEL2)myMusic.play(MUSIC_LEVEL2);
	else if (gameState == STATE_LEVEL3)myMusic.play(myLevel3.isArenaMode() ? MUSIC_LEVEL3_ARENA : MUSIC_LEVEL3_EXPLORE);
	else myMusic.play(MUSIC_MENU);
}

bool keyMatches(unsigned char key, unsigned char mappedKey) {
	return mySettings.normalizeKey(key) == mySettings.normalizeKey(mappedKey);
}

void syncProgressToLevelScreen() {
	for (int i = 1; i <= 3; i++) {
		myLevelScreen.setUnlocked(i, myScoreboard.isUnlocked(i));
		myLevelScreen.setStars(i, myScoreboard.getStars(i));
	}
}

void startLevel1() {
	currentLevel = 1;
	myNinja.reset();
	myWeapons.reset();
	myLevel1.reset();
	myCamera.reset(myLevel1.worldWidth);
	input.reset();
	myLevelScreen.clearHover();
	gameState = STATE_LEVEL1;
}

void startLevel2() {
	currentLevel = 2;
	myLevel2.reset();
	input.reset();
	myLevelScreen.clearHover();
	gameState = STATE_LEVEL2;
}

void startLevel3() {
	currentLevel = 3;
	myNinja.reset();
	myLevel3.reset(myNinja);
	input.reset();
	myLevelScreen.clearHover();
	gameState = STATE_LEVEL3;
}

void returnToLevelSelect() {
	input.reset();
	currentLevel = 0;
	syncProgressToLevelScreen();
	myLevelScreen.clearHover();
	gameState = STATE_LEVEL_SELECT;
}

void returnHome() {
	input.reset();
	currentLevel = 0;
	gameState = STATE_HOME;
}

void handleLevel1KeyDown(unsigned char key) {
	if (keyMatches(key, mySettings.keyLeft)) input.left = true;
	else if (keyMatches(key, mySettings.keyRight)) input.right = true;
	else if (keyMatches(key, mySettings.keyL1Crouch)) input.down = true;

	else if (keyMatches(key, mySettings.keyJump) || key == ' ') {
		if (!input.jump) input.jumpPressed = true;
		input.jump = true;
	}

	else if (keyMatches(key, mySettings.keyL1Sprint)) input.sprint = true;

	else if (keyMatches(key, mySettings.keyDash)) {
		if (!input.dash) input.dashPressed = true;
		input.dash = true;
	}

	else if (key == '1' || keyMatches(key, mySettings.keyL1Shuriken1)) {
		if (!input.attack[0]) input.attackPressed[0] = true;
		input.attack[0] = true;
	}

	else if (key == '2' || keyMatches(key, mySettings.keyL1Shuriken2)) {
		if (!input.attack[1]) input.attackPressed[1] = true;
		input.attack[1] = true;
	}

	else if (key == '3' || keyMatches(key, mySettings.keyL1Shuriken3)) {
		if (!input.attack[2]) input.attackPressed[2] = true;
		input.attack[2] = true;
	}

	else if (key == '4' || keyMatches(key, mySettings.keyL1Shuriken4)) {
		if (!input.attack[3]) input.attackPressed[3] = true;
		input.attack[3] = true;
	}

	else if (key == 'r' || key == 'R') startLevel1();
	else if (key == 27) returnToLevelSelect();
}

void handleLevel2KeyDown(unsigned char key) {
	if (keyMatches(key, mySettings.keyLeft)) input.left = true;
	else if (keyMatches(key, mySettings.keyRight)) input.right = true;

	else if (keyMatches(key, mySettings.keyJump) || key == ' ') {
		if (!input.jump) input.jumpPressed = true;
		input.jump = true;
	}

	else if (keyMatches(key, mySettings.keyDash)) {
		if (!input.dash) input.dashPressed = true;
		input.dash = true;
	}

	else if (keyMatches(key, mySettings.keyL2Heavy)) {
		if (!input.attack[2]) input.attackPressed[2] = true;
		input.attack[2] = true;
	}

	else if (keyMatches(key, mySettings.keyL2Block)) input.attack[3] = true;

	else if (keyMatches(key, mySettings.keyL2Shuriken)) {
		if (!input.shuriken) input.shurikenPressed = true;
		input.shuriken = true;
	}

	else if (keyMatches(key, mySettings.keyL2Power)) {
		if (!input.power) input.powerPressed = true;
		input.power = true;
	}

	else if (keyMatches(key, mySettings.keyL2Heal)) {
		if (!input.heal) input.healPressed = true;
		input.heal = true;
	}

	else if (key == 'r' || key == 'R') startLevel2();
	else if (key == 27) returnToLevelSelect();
}

void handleLevel3KeyDown(unsigned char key) {
	if (keyMatches(key, mySettings.keyLeft))input.left = true;
	else if (keyMatches(key, mySettings.keyRight))input.right = true;
	else if (keyMatches(key, mySettings.keyL3Up))input.jump = true;
	else if (keyMatches(key, mySettings.keyL3Down))input.down = true;

	else if (myLevel3.isArenaMode() && keyMatches(key, mySettings.keyDash)){ if (!input.dash)input.dashPressed = true; input.dash = true; }
	else if (myLevel3.isArenaMode() && keyMatches(key, mySettings.keyL3Action1)){ if (!input.attack[0])input.attackPressed[0] = true; input.attack[0] = true; }
	else if (myLevel3.isArenaMode() && keyMatches(key, mySettings.keyL3Action2)){ if (!input.attack[1])input.attackPressed[1] = true; input.attack[1] = true; }
	else if (myLevel3.isArenaMode() && keyMatches(key, mySettings.keyL3Heavy)){ if (!input.attack[2])input.attackPressed[2] = true; input.attack[2] = true; }
	else if (myLevel3.isArenaMode() && keyMatches(key, mySettings.keyL3Block))input.attack[3] = true;
	else if (myLevel3.isArenaMode() && keyMatches(key, mySettings.keyL3Action3)){ if (!input.shuriken)input.shurikenPressed = true; input.shuriken = true; }
	else if (myLevel3.isArenaMode() && keyMatches(key, mySettings.keyL3Power)){ if (!input.power)input.powerPressed = true; input.power = true; }
	else if (myLevel3.isArenaMode() && keyMatches(key, mySettings.keyL3Heal)){ if (!input.heal)input.healPressed = true; input.heal = true; }

	else if (!myLevel3.isArenaMode() && key >= '1'&&key <= '9')myLevel3.debugJump(key - '1', myNinja);
	else if (!myLevel3.isArenaMode() && key == '0')myLevel3.debugJump(9, myNinja);
	else if (key == 'r' || key == 'R')startLevel3();
	else if (key == 27)returnToLevelSelect();
}

void handleNormalKeyDown(unsigned char key) {
	if (gameState == STATE_HOME) {
		if (key == 27) { myMusic.shutdown(); exit(0); }
		return;
	}

	if (gameState == STATE_LEVEL_SELECT) {
		if (key == '3') { startLevel3(); return; } // temporary Level 3 test shortcut
		if (key == 27) returnHome();
		return;
	}

	if (gameState == STATE_SCOREBOARD) {
		if (key == 27) returnHome();
		return;
	}

	if (gameState == STATE_SETTINGS) {
		if (mySettings.isChangingKey()){
			mySettings.handleKey(key);
			syncMusicToState();
			return;
		}

		if (key == 27) {
			returnHome();
			return;
		}

		return;
	}

	if (gameState == STATE_LEVEL1) {
		handleLevel1KeyDown(key);
		return;
	}

	if (gameState == STATE_LEVEL2) {
		handleLevel2KeyDown(key);
		return;
	}

	if (gameState == STATE_LEVEL3) {
		handleLevel3KeyDown(key);
		return;
	}
}

void handleNormalKeyUp(unsigned char key) {
	if (gameState != STATE_LEVEL1 && gameState != STATE_LEVEL2 && gameState != STATE_LEVEL3) return;

	if (keyMatches(key, mySettings.keyLeft)) input.left = false;
	else if (keyMatches(key, mySettings.keyRight)) input.right = false;
	else if (keyMatches(key, mySettings.keyJump) || key == ' ') input.jump = false;
	else if (keyMatches(key, mySettings.keyDash)) input.dash = false;

	if (gameState == STATE_LEVEL3){
		if (keyMatches(key, mySettings.keyLeft))input.left = false;
		if (keyMatches(key, mySettings.keyRight))input.right = false;
		if (keyMatches(key, mySettings.keyL3Up))input.jump = false;
		if (keyMatches(key, mySettings.keyL3Down))input.down = false;
		if (keyMatches(key, mySettings.keyDash))input.dash = false;
		if (keyMatches(key, mySettings.keyL3Action1))input.attack[0] = false;
		if (keyMatches(key, mySettings.keyL3Action2))input.attack[1] = false;
		if (keyMatches(key, mySettings.keyL3Heavy))input.attack[2] = false;
		if (keyMatches(key, mySettings.keyL3Block))input.attack[3] = false;
		if (keyMatches(key, mySettings.keyL3Action3))input.shuriken = false;
		if (keyMatches(key, mySettings.keyL3Power))input.power = false;
		if (keyMatches(key, mySettings.keyL3Heal))input.heal = false;
	}

	if (gameState == STATE_LEVEL1) {
		if (keyMatches(key, mySettings.keyL1Crouch)) input.down = false;
		else if (keyMatches(key, mySettings.keyL1Sprint)) input.sprint = false;
		else if (key == '1' || keyMatches(key, mySettings.keyL1Shuriken1)) input.attack[0] = false;
		else if (key == '2' || keyMatches(key, mySettings.keyL1Shuriken2)) input.attack[1] = false;
		else if (key == '3' || keyMatches(key, mySettings.keyL1Shuriken3)) input.attack[2] = false;
		else if (key == '4' || keyMatches(key, mySettings.keyL1Shuriken4)) input.attack[3] = false;
	}

	if (gameState == STATE_LEVEL2) {
		if (keyMatches(key, mySettings.keyL2Heavy)) input.attack[2] = false;
		else if (keyMatches(key, mySettings.keyL2Block)) input.attack[3] = false;
		else if (keyMatches(key, mySettings.keyL2Shuriken)) input.shuriken = false;
		else if (keyMatches(key, mySettings.keyL2Power)) input.power = false;
		else if (keyMatches(key, mySettings.keyL2Heal)) input.heal = false;
	}
}

void handleSpecialKeyDown(int key) {
	if (gameState != STATE_LEVEL1 && gameState != STATE_LEVEL2 && gameState != STATE_LEVEL3) return;

	if (key == GLUT_KEY_LEFT) input.left = true;
	else if (key == GLUT_KEY_RIGHT) input.right = true;

	else if (key == GLUT_KEY_UP) {
		if (!input.jump) input.jumpPressed = true;
		input.jump = true;
	}

	else if (key == GLUT_KEY_DOWN && (gameState == STATE_LEVEL1 || gameState == STATE_LEVEL3)) input.down = true;
}

void handleSpecialKeyUp(int key) {
	if (key == GLUT_KEY_LEFT) input.left = false;
	else if (key == GLUT_KEY_RIGHT) input.right = false;
	else if (key == GLUT_KEY_UP) input.jump = false;
	else if (key == GLUT_KEY_DOWN) input.down = false;
}

void directKeyboardDown(unsigned char key, int x, int y) { handleNormalKeyDown(key); }
void directKeyboardUp(unsigned char key, int x, int y) { handleNormalKeyUp(key); }
void directSpecialDown(int key, int x, int y) { handleSpecialKeyDown(key); }
void directSpecialUp(int key, int x, int y) { handleSpecialKeyUp(key); }

void installKeyboardHooks() {
	glutKeyboardFunc(directKeyboardDown);
	glutKeyboardUpFunc(directKeyboardUp);
	glutSpecialFunc(directSpecialDown);
	glutSpecialUpFunc(directSpecialUp);
	keyboardHooksInstalled = true;
}

void iKeyboard(unsigned char key) { handleNormalKeyDown(key); }
void iSpecialKeyboard(unsigned char key) { handleSpecialKeyDown((int)key); }

void processLevel1() {
	int moveDirection = 0;

	if (input.left && !input.right) moveDirection = -1;
	else if (input.right && !input.left) moveDirection = 1;

	if (input.jumpPressed) myNinja.requestJump();
	if (input.dashPressed) myNinja.requestDash();

	for (int i = 0; i < 4; i++) {
		if (input.attackPressed[i] && myWeapons.canFire(i))
			myNinja.requestAttack(i);
	}

	myNinja.update(
		myLevel1.platforms,
		Level1Stage::PLATFORM_COUNT,
		moveDirection,
		input.sprint,
		input.down,
		myLevel1.deathY,
		myLevel1.worldWidth
		);

	int releasedType;

	if (myNinja.consumeAttackRelease(releasedType))
		myWeapons.fire(releasedType, myNinja);

	myWeapons.update(myLevel1.worldWidth);
	myLevel1.update(myNinja, myWeapons);

	if (myLevel1.hasResultToSubmit()) {
		myScoreboard.submitLevelResult(
			1,
			myLevel1.finalScore,
			myLevel1.earnedStars,
			myLevel1.goldCollected,
			Level1Stage::GOLD_COUNT
			);

		myLevel1.markResultSubmitted();
		syncProgressToLevelScreen();
	}

	myCamera.follow(myNinja.x);

	if (myLevel1.shouldReturnToLevelSelect())
		returnToLevelSelect();

	input.clearPressed();
}

void processLevel2() {
	if (input.powerPressed) myLevel2.hero.activatePower();
	if (input.healPressed) myLevel2.hero.heal();

	myLevel2.update(
		input.left,
		input.right,
		input.jumpPressed,
		input.dashPressed,
		input.attackPressed[0],
		input.attackPressed[1],
		input.attackPressed[2],
		input.attack[3],
		input.shurikenPressed
		);

	if (myLevel2.hasResultToSubmit()) {
		myScoreboard.submitLevelResult(
			2,
			myLevel2.finalScore,
			myLevel2.earnedStars,
			0,
			0
			);

		myLevel2.markResultSubmitted();
		syncProgressToLevelScreen();
	}

	if (myLevel2.shouldReturnToLevelSelect())
		returnToLevelSelect();

	input.clearPressed();
}

void processLevel3() {
	myLevel3.update(myNinja, input.left, input.right, input.jump, input.down, input.attackPressed[0], input.attackPressed[1], input.attackPressed[2], input.attack[3], input.shurikenPressed, input.dashPressed, input.powerPressed, input.healPressed);

	if (myLevel3.hasResultToSubmit()){
		myScoreboard.submitLevelResult(3, myLevel3.finalScore, myLevel3.earnedStars, 0, 0);
		myLevel3.markResultSubmitted(); syncProgressToLevelScreen();
	}

	if (myLevel3.shouldReturnToLevelSelect()) returnToLevelSelect();
	input.clearPressed();
}

void updateTimer() {
	if (!keyboardHooksInstalled) installKeyboardHooks();

	if (gameState == STATE_LOADING) {
		myScreens.loadingTimer++;

		if (myScreens.loadingTimer >= myScreens.maxLoadingTime)
			gameState = STATE_HOME;
	}

	else if (gameState == STATE_HOME) myScreens.updateAnimation();
	else if (gameState == STATE_LEVEL_SELECT) myLevelScreen.update();
	else if (gameState == STATE_LEVEL1) processLevel1();
	else if (gameState == STATE_LEVEL2) processLevel2();
	else if (gameState == STATE_LEVEL3) processLevel3();

	syncMusicToState();
}

void fixedUpdate() {
	// Required by this iGraphics build.
}

void iDraw() {
	iClear();

	if (gameState == STATE_LOADING) {
		myScreens.drawLoading();
		return;
	}

	if (gameState == STATE_HOME) {
		myScreens.drawHome();
		return;
	}

	if (gameState == STATE_LEVEL_SELECT) {
		myLevelScreen.draw();
		return;
	}

	if (gameState == STATE_SCOREBOARD) {
		myScoreboard.draw();
		return;
	}

	if (gameState == STATE_SETTINGS) {
		mySettings.draw();
		return;
	}

	if (gameState == STATE_LEVEL1) {
		myLevel1.draw(myCamera.x);
		myWeapons.draw(myCamera.x);
		myNinja.draw(myCamera.x);
		return;
	}

	if (gameState == STATE_LEVEL2) {
		myLevel2.draw();
		return;
	}

	if (gameState == STATE_LEVEL3) {
		myLevel3.draw(myNinja);
		return;
	}
}

void iPassiveMouseMove(int mx, int my) {
	if (gameState == STATE_LEVEL_SELECT)
		myLevelScreen.updateHover(mx, my);
}

void iMouseMove(int mx, int my) {
	if (gameState == STATE_LEVEL_SELECT)
		myLevelScreen.updateHover(mx, my);
}

void iMouse(int button, int state, int mx, int my) {
	if (state != GLUT_DOWN) return;

	// SETTINGS SCROLL MUST BE BEFORE LEFT-CLICK FILTER.
	if (gameState == STATE_SETTINGS) {
		if (button == 3) {
			mySettings.scrollUp();
			return;
		}

		if (button == 4) {
			mySettings.scrollDown();
			return;
		}
	}

	// SCOREBOARD
	if (gameState == STATE_SCOREBOARD) {
		if (button == GLUT_LEFT_BUTTON) {
			if (myScoreboard.isBackClicked(mx, my)) {
				returnHome();
				return;
			}

			myScoreboard.nextPage();
		}
		return;
	}

	// LEVEL 2 mouse attacks
	if (gameState == STATE_LEVEL2) {
		if (button == GLUT_LEFT_BUTTON) input.attackPressed[0] = true;
		else if (button == GLUT_RIGHT_BUTTON) input.attackPressed[1] = true;
		else if (button == GLUT_MIDDLE_BUTTON) input.shurikenPressed = true;
		return;
	}

	// LEVEL 3 arena mouse attacks: left=punch, right=kick, middle=shuriken.
	if (gameState == STATE_LEVEL3 && myLevel3.isArenaMode()){
		if (button == GLUT_LEFT_BUTTON) input.attackPressed[0] = true;
		else if (button == GLUT_RIGHT_BUTTON) input.attackPressed[1] = true;
		else if (button == GLUT_MIDDLE_BUTTON) input.shurikenPressed = true;
		return;
	}

	if (button != GLUT_LEFT_BUTTON) return;

	// HOME
	if (gameState == STATE_HOME) {
		if (myScreens.btnQuit.isClicked(mx, my)) {
			myMusic.shutdown(); exit(0);
		}

		else if (myScreens.btnNewGame.isClicked(mx, my)) {
			syncProgressToLevelScreen();
			myLevelScreen.clearHover();
			gameState = STATE_LEVEL_SELECT;
		}

		else if (myScreens.btnScores.isClicked(mx, my)) {
			myScoreboard.resetPage();
			gameState = STATE_SCOREBOARD;
		}

		else if (myScreens.btnSettings.isClicked(mx, my)) {
			mySettings.resetPage();
			gameState = STATE_SETTINGS;
		}

		return;
	}

	// SETTINGS
	if (gameState == STATE_SETTINGS) {
		if (mySettings.isBackClicked(mx, my)) {
			returnHome();
			return;
		}

		mySettings.handleClick(mx, my);
		syncMusicToState();
		return;
	}

	// LEVEL SELECT
	if (gameState == STATE_LEVEL_SELECT) {
		int selected = myLevelScreen.handleClick(mx, my);

		if (selected == 1) startLevel1();
		else if (selected == 2) startLevel2();
		else if (selected == 3) startLevel3();
		return;
	}

	// LEVEL 1
	if (gameState == STATE_LEVEL1) {
		myNinja.requestJump();
		return;
	}
}

int main() {
	iSetTimer(30, updateTimer);

	iInitialize(
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		(char*)"Ninja Fight"
		);

	myScreens.loadAllImages();

	myNinja.load();
	myNinja.reset();

	myWeapons.load();
	myWeapons.reset();

	myLevelScreen.load();

	myScoreboard.loadImages();
	myScoreboard.loadProgress();
	myScoreboard.resetPage();

	mySettings.load();
	myMusic.load(); myMusic.setEnabled(mySettings.soundEnabled); myMusic.setVolume(mySettings.masterVolume);

	myLevel1.load();
	myLevel1.reset();

	myLevel2.load();
	myLevel2.reset();

	myLevel3.load();
	myLevel3.reset(myNinja);

	syncProgressToLevelScreen();
	myCamera.reset(myLevel1.worldWidth);
	input.reset();
	syncMusicToState();

	iStart();
	return 0;
}
