#ifndef LEVEL1_H
#define LEVEL1_H

#include <cstdio>

struct Level1Gold { float x, y; bool collected; };

enum Level1TrapType {
	LEVEL1_TRAP_STATIC = 0,
	LEVEL1_TRAP_DROP = 1,
	LEVEL1_TRAP_ARROW = 2,
	LEVEL1_TRAP_SLIDE = 3,
	LEVEL1_TRAP_SWING = 4
};

struct Level1Trap {
	int type, image, width, height, damage;
	float x, y, startX, startY;
	float triggerDistance, velocity, limitValue, activeLaneMaxY;
	int timer, cooldown, warningTime;
	bool triggered, returning, movePositive;
	RectF damageBox;
};

struct Level1HazardArrow {
	bool active;
	float x, y, vx;
	int damage;
};

struct Level1Enemy {
	int hp, maxHP;
	int attackCooldown, attackTimer, animTimer;
	int idleTimer, hurtTimer;
	float x, y, patrolMinX, patrolMaxX, patrolSpeed, chaseSpeed;
	bool movingRight, facingRight, alive, attackFired;
};

struct Level1EnemyShot {
	bool active;
	int width, height, damage;
	float x, y, vx;
};

struct Level1EnemyEffect {
	bool active;
	int image, width, height, timer;
	float x, y;
};

struct Level1Stage {
	static const int PLATFORM_COUNT = 32;
	static const int GOLD_COUNT = 18;
	static const int TRAP_COUNT = 14;
	static const int ENEMY_COUNT = 4;
	static const int MAX_ENEMY_SHOTS = 14;
	static const int MAX_ENEMY_EFFECTS = 10;
	static const int MAX_HAZARD_ARROWS = 20;

	float worldWidth, deathY;
	PlatformLine platforms[PLATFORM_COUNT];

	Level1Gold gold[GOLD_COUNT];
	Level1Trap trap[TRAP_COUNT];
	Level1HazardArrow hazardArrow[MAX_HAZARD_ARROWS];
	Level1Enemy enemy[ENEMY_COUNT];
	Level1EnemyShot enemyShot[MAX_ENEMY_SHOTS];
	Level1EnemyEffect enemyEffect[MAX_ENEMY_EFFECTS];

	int imgBackground, imgExit, imgArrow;
	int imgHUD1, imgHUD2, imgHUDNinja, imgHUDScore, imgHUDShadow, imgHUDLives, imgHUDScoreAlt, imgHUDLivesAlt;
	int imgPlatform1, imgPlatform2, imgBridge;

	int imgTrapSpike, imgTrapDropBlade, imgTrapFire;
	int imgArrowWall, imgArrowTurret, imgSwingBlade, imgSlideBlade, imgCeilingSpear;

	int imgShadowIdleR, imgShadowRunR1, imgShadowRunR2;
	int imgShadowAttackR1, imgShadowAttackR2, imgShadowHurtR;
	int imgShadowIdleL, imgShadowRunL1, imgShadowRunL2;
	int imgShadowAttackL1, imgShadowAttackL2;
	int imgShadowShot, imgShadowImpact;

	int score, goldCollected, enemiesDefeated, deaths, elapsedTicks;
	int playerHUDHP, playerHUDMaxHP;
	int earnedStars, finalScore, completeTimer, previousPlayerState;
	bool completed, resultSubmitted;

	RectF exitBox;

	void load() {
		imgBackground = iLoadImage("Images\\Levels\\Level1\\level-1.png");
		imgExit = iLoadImage("Images\\Levels\\Level1\\exit.png");
		imgArrow = iLoadImage("Images\\Weapons\\arrow.png");

		imgHUD1 = iLoadImage("Images\\Levels\\Level1\\1.png");
		imgHUD2 = iLoadImage("Images\\Levels\\Level1\\2.png");
		imgHUDNinja = iLoadImage("Images\\Levels\\Level1\\4.png");
		imgHUDScore = iLoadImage("Images\\Levels\\Level1\\5.png");
		imgHUDShadow = iLoadImage("Images\\Levels\\Level1\\6.png");
		imgHUDLives = iLoadImage("Images\\Levels\\Level1\\7.png");
		imgHUDScoreAlt = iLoadImage("Images\\Levels\\Level1\\10.png");
		imgHUDLivesAlt = iLoadImage("Images\\Levels\\Level1\\11.png");

		imgPlatform1 = iLoadImage("Images\\Environment\\Platforms\\o-01.png");
		imgPlatform2 = iLoadImage("Images\\Environment\\Platforms\\o-02.png");
		imgBridge = iLoadImage("Images\\Environment\\Traps\\t-14.png");

		imgTrapSpike = iLoadImage("Images\\Environment\\Traps\\t-04.png");
		imgTrapDropBlade = iLoadImage("Images\\Environment\\Traps\\t-06.png");
		imgTrapFire = iLoadImage("Images\\Environment\\Traps\\t-13.png");
		imgArrowWall = iLoadImage("Images\\Environment\\Traps\\t-15.png");
		imgArrowTurret = iLoadImage("Images\\Environment\\Traps\\t-16.png");
		imgSwingBlade = iLoadImage("Images\\Environment\\Traps\\t-17.png");
		imgSlideBlade = iLoadImage("Images\\Environment\\Traps\\t-18.png");
		imgCeilingSpear = iLoadImage("Images\\Environment\\Traps\\t-19.png");

		imgShadowIdleR = iLoadImage("Images\\Enemies\\Shadow\\v-01.png");
		imgShadowRunR1 = iLoadImage("Images\\Enemies\\Shadow\\v-02.png");
		imgShadowRunR2 = iLoadImage("Images\\Enemies\\Shadow\\v-03.png");
		imgShadowHurtR = iLoadImage("Images\\Enemies\\Shadow\\v-04.png");
		imgShadowAttackR1 = iLoadImage("Images\\Enemies\\Shadow\\v-06.png");
		imgShadowAttackR2 = iLoadImage("Images\\Enemies\\Shadow\\v-07.png");
		imgShadowShot = iLoadImage("Images\\Enemies\\Shadow\\v-09.png");
		imgShadowImpact = iLoadImage("Images\\Enemies\\Shadow\\v-10.png");

		imgShadowIdleL = iLoadImage("Images\\Enemies\\Shadow\\v-11.png");
		imgShadowRunL1 = iLoadImage("Images\\Enemies\\Shadow\\v-12.png");
		imgShadowRunL2 = iLoadImage("Images\\Enemies\\Shadow\\v-13.png");
		imgShadowAttackL1 = iLoadImage("Images\\Enemies\\Shadow\\v-14.png");
		imgShadowAttackL2 = iLoadImage("Images\\Enemies\\Shadow\\v-15.png");
	}

	void reset() {
		worldWidth = 5200.0f; deathY = -170.0f;

		// MAIN ROUTE: long continuous route with small height changes.
		setPlatform(0, 0, 90, 550); setPlatform(1, 570, 115, 380);
		setPlatform(2, 970, 140, 380); setPlatform(3, 1370, 120, 400);
		setPlatform(4, 1790, 145, 390); setPlatform(5, 2200, 170, 380);
		setPlatform(6, 2600, 145, 380); setPlatform(7, 3000, 165, 380);
		setPlatform(8, 3400, 125, 400); setPlatform(9, 3820, 155, 380);
		setPlatform(10, 4220, 135, 390); setPlatform(11, 4630, 165, 350);
		setPlatform(12, 5000, 120, 180);

		// UPPER / ESCAPE ROUTE: gives alternate paths around harder trap zones.
		setPlatform(13, 650, 205, 250); setPlatform(14, 930, 230, 250);
		setPlatform(15, 1210, 250, 240); setPlatform(16, 1470, 250, 250);
		setPlatform(17, 1740, 255, 240); setPlatform(18, 2000, 250, 250);
		setPlatform(19, 2270, 260, 250); setPlatform(20, 2540, 250, 250);
		setPlatform(21, 2810, 260, 240); setPlatform(22, 3080, 245, 240);
		setPlatform(23, 3350, 255, 250); setPlatform(24, 3630, 270, 245);
		setPlatform(25, 3900, 250, 250); setPlatform(26, 4180, 265, 240);
		setPlatform(27, 4450, 250, 240); setPlatform(28, 4710, 265, 235);
		setPlatform(29, 4930, 230, 170); setPlatform(30, 515, 185, 115);
		setPlatform(31, 4870, 195, 110);

		// Gold is spread through both routes to reward exploration.
		setGold(0, 220, 135); setGold(1, 625, 160); setGold(2, 745, 245);
		setGold(3, 1020, 185); setGold(4, 1280, 290); setGold(5, 1460, 165);
		setGold(6, 1810, 195); setGold(7, 2100, 290); setGold(8, 2300, 215);
		setGold(9, 2630, 195); setGold(10, 2890, 300); setGold(11, 3150, 205);
		setGold(12, 3470, 295); setGold(13, 3740, 315); setGold(14, 4060, 295);
		setGold(15, 4380, 185); setGold(16, 4690, 305); setGold(17, 5010, 260);

		// SECTION 1: introductory traps.
		setStaticTrap(0, imgTrapSpike, 780, 115, 72, 58, 795, 115, 42, 24, 6);
		setSwingTrap(1, imgSwingBlade, 1130, 205, 96, 135, 7, 24.0f);
		setArrowTrap(2, imgArrowWall, 1430, 120, 108, 96, 5, 250.0f, 185, 210.0f);

		// SECTION 2: vertical and moving hazards.
		setDropTrap(3, imgTrapDropBlade, 1770, 350, 94, 130, 8, 125.0f, 170.0f, 220.0f);
		setSlideTrap(4, imgSlideBlade, 2120, 145, 145, 76, 7, 115.0f, 42.0f, 215.0f);
		setArrowTrap(5, imgArrowTurret, 2475, 145, 108, 100, 6, 270.0f, 190, 220.0f);
		setDropTrap(6, imgCeilingSpear, 2810, 345, 102, 125, 8, 125.0f, 220.0f, 225.0f);

		// SECTION 3: mixed trap combinations.
		setStaticTrap(7, imgTrapFire, 3160, 120, 62, 80, 3172, 120, 38, 48, 7);
		setSwingTrap(8, imgSwingBlade, 3515, 190, 96, 135, 8, 32.0f);
		setDropTrap(9, imgTrapDropBlade, 3870, 365, 94, 130, 9, 135.0f, 180.0f, 230.0f);
		setArrowTrap(10, imgArrowWall, 4160, 135, 108, 96, 7, 300.0f, 165, 225.0f);

		// FINAL SECTION: more pressure near the exit.
		setStaticTrap(11, imgTrapSpike, 4510, 165, 74, 58, 4525, 165, 44, 24, 8);
		setSlideTrap(12, imgSlideBlade, 4740, 165, 145, 76, 8, 125.0f, 52.0f, 225.0f);
		setStaticTrap(13, imgTrapFire, 4935, 120, 62, 80, 4947, 120, 38, 48, 8);

		// Four Shadow villains are spread through the longer level.
		setEnemy(0, 1015, 140, 990, 1150, 34);
		setEnemy(1, 2240, 170, 2215, 2380, 38);
		setEnemy(2, 3610, 125, 3460, 3740, 42);
		setEnemy(3, 4515, 165, 4380, 4680, 46);

		for (int i = 0; i < MAX_HAZARD_ARROWS; i++) hazardArrow[i].active = false;
		for (int i = 0; i < MAX_ENEMY_SHOTS; i++) enemyShot[i].active = false;
		for (int i = 0; i < MAX_ENEMY_EFFECTS; i++) enemyEffect[i].active = false;

		exitBox.x = 5040; exitBox.y = 120; exitBox.w = 135; exitBox.h = 180;

		score = 0; goldCollected = 0; enemiesDefeated = 0; deaths = 0; elapsedTicks = 0;
		playerHUDHP = 100; playerHUDMaxHP = 100;
		completed = false; earnedStars = 0; finalScore = 0; completeTimer = 0;
		resultSubmitted = false; previousPlayerState = NINJA_IDLE;
	}

	void setPlatform(int i, float x, float y, float width) {
		platforms[i].x = x; platforms[i].y = y; platforms[i].width = width;
	}

	void setGold(int i, float x, float y) {
		gold[i].x = x; gold[i].y = y; gold[i].collected = false;
	}

	void clearTrap(int i) {
		trap[i].type = LEVEL1_TRAP_STATIC; trap[i].image = 0;
		trap[i].width = trap[i].height = trap[i].damage = 0;
		trap[i].x = trap[i].y = trap[i].startX = trap[i].startY = 0;
		trap[i].triggerDistance = trap[i].velocity = trap[i].limitValue = 0;
		trap[i].activeLaneMaxY = 9999.0f;
		trap[i].timer = trap[i].cooldown = trap[i].warningTime = 0;
		trap[i].triggered = trap[i].returning = false; trap[i].movePositive = true;
		trap[i].damageBox.x = trap[i].damageBox.y = trap[i].damageBox.w = trap[i].damageBox.h = 0;
	}

	void setStaticTrap(int i, int image, float x, float y, int w, int h,
		float hx, float hy, float hw, float hh, int damage) {
		clearTrap(i);
		trap[i].type = LEVEL1_TRAP_STATIC; trap[i].image = image;
		trap[i].x = trap[i].startX = x; trap[i].y = trap[i].startY = y;
		trap[i].width = w; trap[i].height = h; trap[i].damage = damage;
		trap[i].damageBox.x = hx; trap[i].damageBox.y = hy;
		trap[i].damageBox.w = hw; trap[i].damageBox.h = hh;
	}

	void setDropTrap(int i, int image, float x, float y, int w, int h, int damage,
		float triggerDistance, float lowestY, float activeLaneMaxY) {
		clearTrap(i);
		trap[i].type = LEVEL1_TRAP_DROP; trap[i].image = image;
		trap[i].x = trap[i].startX = x; trap[i].y = trap[i].startY = y;
		trap[i].width = w; trap[i].height = h; trap[i].damage = damage;
		trap[i].triggerDistance = triggerDistance; trap[i].limitValue = lowestY;
		trap[i].activeLaneMaxY = activeLaneMaxY; trap[i].warningTime = 20;
	}

	void setArrowTrap(int i, int image, float x, float y, int w, int h, int damage,
		float range, int cooldown, float activeLaneMaxY) {
		clearTrap(i);
		trap[i].type = LEVEL1_TRAP_ARROW; trap[i].image = image;
		trap[i].x = trap[i].startX = x; trap[i].y = trap[i].startY = y;
		trap[i].width = w; trap[i].height = h; trap[i].damage = damage;
		trap[i].triggerDistance = range; trap[i].cooldown = cooldown;
		trap[i].activeLaneMaxY = activeLaneMaxY; trap[i].warningTime = 22;
	}

	void setSlideTrap(int i, int image, float x, float y, int w, int h, int damage,
		float range, float travel, float activeLaneMaxY) {
		clearTrap(i);
		trap[i].type = LEVEL1_TRAP_SLIDE; trap[i].image = image;
		trap[i].x = trap[i].startX = x; trap[i].y = trap[i].startY = y;
		trap[i].width = w; trap[i].height = h; trap[i].damage = damage;
		trap[i].triggerDistance = range; trap[i].limitValue = x - travel;
		trap[i].activeLaneMaxY = activeLaneMaxY; trap[i].warningTime = 18;
	}

	void setSwingTrap(int i, int image, float x, float y, int w, int h, int damage, float travel) {
		clearTrap(i);
		trap[i].type = LEVEL1_TRAP_SWING; trap[i].image = image;
		trap[i].x = trap[i].startX = x; trap[i].y = trap[i].startY = y;
		trap[i].width = w; trap[i].height = h; trap[i].damage = damage;
		trap[i].limitValue = travel;
	}

	void setEnemy(int i, float x, float y, float minX, float maxX, int hp) {
		enemy[i].x = x; enemy[i].y = y;
		enemy[i].patrolMinX = minX; enemy[i].patrolMaxX = maxX;
		enemy[i].patrolSpeed = 0.75f; enemy[i].chaseSpeed = 1.05f;
		enemy[i].movingRight = true; enemy[i].facingRight = true;
		enemy[i].alive = true; enemy[i].attackFired = false;
		enemy[i].hp = hp; enemy[i].maxHP = hp;
		enemy[i].attackCooldown = 135 + i * 20; enemy[i].attackTimer = 0;
		enemy[i].animTimer = 0; enemy[i].idleTimer = 15; enemy[i].hurtTimer = 0;
	}

	RectF getEnemyBox(int i) {
		RectF box;
		box.x = enemy[i].x + 18; box.y = enemy[i].y + 7; box.w = 56; box.h = 72;
		return box;
	}

	void updateTrapBox(Level1Trap &t) {
		if (t.type == LEVEL1_TRAP_ARROW) {
			t.damageBox.x = t.damageBox.y = t.damageBox.w = t.damageBox.h = 0; return;
		}

		if (t.type == LEVEL1_TRAP_SLIDE) {
			t.damageBox.x = t.x + 10; t.damageBox.y = t.y + 18;
			t.damageBox.w = t.width - 22; t.damageBox.h = t.height - 34; return;
		}

		t.damageBox.x = t.x + 20; t.damageBox.y = t.y + 10;
		t.damageBox.w = t.width - 40; t.damageBox.h = t.height - 22;
	}

	void spawnHazardArrow(float x, float y, int damage, float speed) {
		for (int i = 0; i < MAX_HAZARD_ARROWS; i++) {
			if (!hazardArrow[i].active) {
				hazardArrow[i].active = true; hazardArrow[i].x = x; hazardArrow[i].y = y;
				hazardArrow[i].vx = speed; hazardArrow[i].damage = damage; return;
			}
		}
	}

	void fireArrowVolley(Level1Trap &t) {
		float muzzleX = t.x + t.width - 8;

		// Two arrows only, with enough vertical space to jump/crouch between them.
		spawnHazardArrow(muzzleX, t.y + 30, t.damage, 5.8f);
		spawnHazardArrow(muzzleX, t.y + 75, t.damage, 5.8f);
	}

	void startTrapWarning(Level1Trap &t) {
		t.triggered = true;
		t.timer = t.warningTime;
		t.velocity = 0.0f;
	}

	void updateOneTrap(Level1Trap &t, NinjaPlayer &ninja) {
		float dx = absFloat(ninja.x - t.x);
		bool playerInLowerLane = ninja.y < t.activeLaneMaxY;

		if (t.type == LEVEL1_TRAP_STATIC) return;

		if (t.type == LEVEL1_TRAP_ARROW) {
			if (t.returning) {
				if (t.timer > 0) t.timer--;
				else t.returning = false;
				return;
			}

			if (!t.triggered && playerInLowerLane &&
				ninja.x > t.x + 40 && ninja.x < t.x + t.triggerDistance) {
				startTrapWarning(t);
				return;
			}

			if (t.triggered) {
				if (t.timer > 0) t.timer--;
				else {
					fireArrowVolley(t);
					t.triggered = false; t.returning = true; t.timer = t.cooldown;
				}
			}
			return;
		}

		if (t.type == LEVEL1_TRAP_DROP) {
			if (!t.triggered && !t.returning && playerInLowerLane && dx < t.triggerDistance)
				startTrapWarning(t);

			if (t.triggered) {
				if (t.timer > 0) { t.timer--; return; }

				t.velocity += 0.50f; t.y -= t.velocity;

				if (t.y <= t.limitValue) {
					t.y = t.limitValue; t.triggered = false; t.returning = true;
					t.timer = 45; t.velocity = 0.0f;
				}
			}
			else if (t.returning) {
				if (t.timer > 0) t.timer--;
				else {
					t.y += 2.0f;
					if (t.y >= t.startY) { t.y = t.startY; t.returning = false; }
				}
			}

			updateTrapBox(t); return;
		}

		if (t.type == LEVEL1_TRAP_SLIDE) {
			if (!t.triggered && !t.returning && playerInLowerLane && dx < t.triggerDistance)
				startTrapWarning(t);

			if (t.triggered) {
				if (t.timer > 0) { t.timer--; return; }

				t.x -= 3.8f;

				if (t.x <= t.limitValue) {
					t.x = t.limitValue; t.triggered = false; t.returning = true; t.timer = 35;
				}
			}
			else if (t.returning) {
				if (t.timer > 0) t.timer--;
				else {
					t.x += 2.2f;
					if (t.x >= t.startX) { t.x = t.startX; t.returning = false; }
				}
			}

			updateTrapBox(t); return;
		}

		if (t.type == LEVEL1_TRAP_SWING) {
			float left = t.startX - t.limitValue, right = t.startX + t.limitValue;

			if (t.movePositive) {
				t.x += 1.15f;
				if (t.x >= right) { t.x = right; t.movePositive = false; }
			}
			else {
				t.x -= 1.15f;
				if (t.x <= left) { t.x = left; t.movePositive = true; }
			}

			updateTrapBox(t);
		}
	}

	void updateHazardArrows(NinjaPlayer &ninja) {
		RectF playerBox = ninja.getHitbox();

		for (int i = 0; i < MAX_HAZARD_ARROWS; i++) {
			if (!hazardArrow[i].active) continue;

			hazardArrow[i].x += hazardArrow[i].vx;

			RectF box;
			box.x = hazardArrow[i].x + 5; box.y = hazardArrow[i].y + 3;
			box.w = 38; box.h = 9;

			if (rectOverlap(playerBox, box)) {
				ninja.takeDamage(hazardArrow[i].damage, hazardArrow[i].x, 4.0f);
				hazardArrow[i].active = false; continue;
			}

			if (hazardArrow[i].x > worldWidth + 100) hazardArrow[i].active = false;
		}
	}

	void spawnEnemyShot(int enemyIndex) {
		int slot = -1;

		for (int i = 0; i < MAX_ENEMY_SHOTS; i++) {
			if (!enemyShot[i].active) { slot = i; break; }
		}

		if (slot < 0) return;

		Level1Enemy &e = enemy[enemyIndex];
		Level1EnemyShot &shot = enemyShot[slot];
		float direction = e.facingRight ? 1.0f : -1.0f;

		shot.active = true;
		shot.x = e.facingRight ? e.x + 58 : e.x - 8;
		shot.y = e.y + 48;
		shot.vx = 4.8f * direction;
		shot.width = 46; shot.height = 34; shot.damage = 7;
	}

	void spawnEnemyImpact(float x, float y) {
		for (int i = 0; i < MAX_ENEMY_EFFECTS; i++) {
			if (!enemyEffect[i].active) {
				enemyEffect[i].active = true; enemyEffect[i].image = imgShadowImpact;
				enemyEffect[i].x = x - 42; enemyEffect[i].y = y - 42;
				enemyEffect[i].width = 84; enemyEffect[i].height = 84;
				enemyEffect[i].timer = 8; return;
			}
		}
	}

	void updateGold(NinjaPlayer &ninja) {
		RectF playerBox = ninja.getHitbox();

		for (int i = 0; i < GOLD_COUNT; i++) {
			if (gold[i].collected) continue;

			RectF coinBox;
			coinBox.x = gold[i].x - 12; coinBox.y = gold[i].y - 12;
			coinBox.w = 24; coinBox.h = 24;

			if (rectOverlap(playerBox, coinBox)) {
				gold[i].collected = true; goldCollected++; score += 100;
			}
		}
	}

	void updateTraps(NinjaPlayer &ninja) {
		RectF playerBox = ninja.getHitbox();

		for (int i = 0; i < TRAP_COUNT; i++) {
			updateOneTrap(trap[i], ninja);

			if (trap[i].damage <= 0 || trap[i].type == LEVEL1_TRAP_ARROW) continue;

			// A player clearly above the trap's active lane is using the bypass route.
			if (ninja.y >= trap[i].activeLaneMaxY && trap[i].activeLaneMaxY < 9000.0f) continue;

			if (rectOverlap(playerBox, trap[i].damageBox)) {
				float sourceX = trap[i].damageBox.x + trap[i].damageBox.w / 2.0f;
				ninja.takeDamage(trap[i].damage, sourceX, 4.8f);
			}
		}

		updateHazardArrows(ninja);
	}

	void updateEnemy(int index, NinjaPlayer &ninja, ShurikenSystem &weapons) {
		Level1Enemy &e = enemy[index];
		if (!e.alive) return;

		int damage = 0, weaponType = 0;

		if (weapons.checkEnemyCollision(getEnemyBox(index), damage, weaponType)) {
			e.hp -= damage;
			e.hurtTimer = 8;
			e.attackTimer = 0;
			e.attackFired = false;

			if (e.hp <= 0) {
				e.hp = 0; e.alive = false; enemiesDefeated++; score += 250; return;
			}
		}

		if (e.hurtTimer > 0) {
			e.hurtTimer--; e.animTimer++; return;
		}

		// Contact damage remains low for Level 1.
		if (rectOverlap(ninja.getHitbox(), getEnemyBox(index)))
			ninja.takeDamage(5, e.x + 45, 5.0f);

		if (e.attackCooldown > 0) e.attackCooldown--;

		// Complete the attack animation before moving again.
		if (e.attackTimer > 0) {
			e.attackTimer--;

			if (!e.attackFired && e.attackTimer <= 9) {
				spawnEnemyShot(index);
				e.attackFired = true;
			}

			if (e.attackTimer == 0) {
				e.attackFired = false;
				e.attackCooldown = 150;
			}

			e.animTimer++;
			return;
		}

		// Short pause at patrol edges makes movement look intentional instead of robotic.
		if (e.idleTimer > 0) {
			e.idleTimer--; e.animTimer++; return;
		}

		float signedDistance = ninja.x - e.x;
		float distance = absFloat(signedDistance);
		float verticalDistance = absFloat(ninja.y - e.y);

		bool sameCombatLane = verticalDistance < 105.0f;
		float attackRange = 190.0f;
		float awareness = 270.0f;

		// Upper bypass route also bypasses the enemy's ranged pressure.
		if (sameCombatLane && distance < awareness) e.facingRight = signedDistance >= 0;

		if (sameCombatLane && distance < attackRange && e.attackCooldown <= 0) {
			e.attackTimer = 22;
			e.attackFired = false;
			e.animTimer = 0;
			return;
		}

		// Chase only inside the patrol region and only if player is on a similar height.
		if (sameCombatLane && distance < awareness && distance > 120.0f) {
			if (signedDistance > 0 && e.x < e.patrolMaxX) {
				e.x += e.chaseSpeed; e.movingRight = true; e.facingRight = true;
			}
			else if (signedDistance < 0 && e.x > e.patrolMinX) {
				e.x -= e.chaseSpeed; e.movingRight = false; e.facingRight = false;
			}

			e.animTimer++;
			return;
		}

		// Normal patrol.
		if (e.movingRight) {
			e.x += e.patrolSpeed; e.facingRight = true;

			if (e.x >= e.patrolMaxX) {
				e.x = e.patrolMaxX; e.movingRight = false;
				e.facingRight = false; e.idleTimer = 18;
			}
		}
		else {
			e.x -= e.patrolSpeed; e.facingRight = false;

			if (e.x <= e.patrolMinX) {
				e.x = e.patrolMinX; e.movingRight = true;
				e.facingRight = true; e.idleTimer = 18;
			}
		}

		e.animTimer++;
	}

	void updateEnemyShots(NinjaPlayer &ninja) {
		for (int i = 0; i < MAX_ENEMY_SHOTS; i++) {
			if (!enemyShot[i].active) continue;

			Level1EnemyShot &shot = enemyShot[i];
			shot.x += shot.vx;

			RectF box;
			box.x = shot.x + 5; box.y = shot.y + 5;
			box.w = shot.width - 10; box.h = shot.height - 10;

			if (rectOverlap(box, ninja.getHitbox())) {
				float hitX = shot.x + shot.width / 2.0f;
				float hitY = shot.y + shot.height / 2.0f;

				ninja.takeDamage(shot.damage, shot.x, 4.8f);
				spawnEnemyImpact(hitX, hitY);
				shot.active = false;
				continue;
			}

			if (shot.x < -120 || shot.x > worldWidth + 120) shot.active = false;
		}

		for (int i = 0; i < MAX_ENEMY_EFFECTS; i++) {
			if (!enemyEffect[i].active) continue;

			enemyEffect[i].timer--;
			if (enemyEffect[i].timer <= 0) enemyEffect[i].active = false;
		}
	}

	void updateDeathCounter(NinjaPlayer &ninja) {
		if (ninja.state == NINJA_DEAD && previousPlayerState != NINJA_DEAD) {
			deaths++; score -= 100;
			if (score < 0) score = 0;
		}

		previousPlayerState = ninja.state;
	}

	void checkFinish(NinjaPlayer &ninja) {
		if (completed || !rectOverlap(ninja.getHitbox(), exitBox)) return;

		completed = true; earnedStars = 1;

		int goldPercent = (goldCollected * 100) / GOLD_COUNT;
		if (goldPercent >= 70) earnedStars = 2;
		if (goldCollected == GOLD_COUNT) earnedStars = 3;

		score += 1000;
		score += ninja.hp * 5;

		int seconds = elapsedTicks / 33;
		int timeBonus = 2000 - seconds * 8;
		if (timeBonus < 0) timeBonus = 0;

		score += timeBonus;
		finalScore = score;
		completeTimer = 80;
	}

	void update(NinjaPlayer &ninja, ShurikenSystem &weapons) {
		playerHUDHP = ninja.hp; playerHUDMaxHP = ninja.maxHP;

		if (completed) {
			if (completeTimer > 0) completeTimer--;
			return;
		}

		elapsedTicks++;
		updateGold(ninja);
		updateTraps(ninja);

		for (int i = 0; i < ENEMY_COUNT; i++) updateEnemy(i, ninja, weapons);

		updateEnemyShots(ninja);
		updateDeathCounter(ninja);
		checkFinish(ninja);
	}

	bool hasResultToSubmit() { return completed && !resultSubmitted; }
	void markResultSubmitted() { resultSubmitted = true; }
	bool shouldReturnToLevelSelect() { return completed && completeTimer <= 0; }

	void drawBackground() {
		iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, imgBackground);
	}

	void drawPlatforms(float cameraX) {
		for (int i = 0; i < PLATFORM_COUNT; i++) {
			float screenX = platforms[i].x - cameraX;

			if (screenX < -platforms[i].width || screenX > SCREEN_WIDTH + 80) continue;

			int image = (i % 2 == 0) ? imgPlatform1 : imgPlatform2;
			iShowImage((int)screenX, (int)platforms[i].y - 30,
				(int)platforms[i].width, 38, image);
		}

		// Escape bridges visually mark safer alternate paths.
		iShowImage((int)(1510 - cameraX), 238, 180, 62, imgBridge);
		iShowImage((int)(2330 - cameraX), 248, 180, 62, imgBridge);
		iShowImage((int)(2860 - cameraX), 248, 180, 62, imgBridge);
		iShowImage((int)(3540 - cameraX), 258, 180, 62, imgBridge);
		iShowImage((int)(4090 - cameraX), 253, 180, 62, imgBridge);
		iShowImage((int)(4630 - cameraX), 253, 180, 62, imgBridge);
	}

	void drawGold(float cameraX) {
		for (int i = 0; i < GOLD_COUNT; i++) {
			if (gold[i].collected) continue;

			int x = (int)(gold[i].x - cameraX);
			int y = (int)gold[i].y;

			if (x < -30 || x > SCREEN_WIDTH + 30) continue;

			iSetColor(225, 165, 20);
			iFilledCircle(x, y, 11);

			iSetColor(255, 235, 120);
			iCircle(x, y, 11);
			iCircle(x, y, 7);
		}
	}

	void drawTraps(float cameraX) {
		for (int i = 0; i < TRAP_COUNT; i++) {
			int x = (int)(trap[i].x - cameraX);

			if (x < -trap[i].width || x > SCREEN_WIDTH + 80) continue;

			iShowImage(x, (int)trap[i].y, trap[i].width, trap[i].height, trap[i].image);

			// Warning marker gives the player reaction time before triggered traps move/fire.
			if (trap[i].triggered && trap[i].timer > 0) {
				iSetColor(255, 205, 70);
				iText(x + trap[i].width / 2 - 3, (int)trap[i].y + trap[i].height + 8,
					(char*)"!", GLUT_BITMAP_HELVETICA_18);
			}
		}
	}

	void drawHazardArrows(float cameraX) {
		for (int i = 0; i < MAX_HAZARD_ARROWS; i++) {
			if (!hazardArrow[i].active) continue;

			int x = (int)(hazardArrow[i].x - cameraX);
			int y = (int)hazardArrow[i].y;

			if (x < -70 || x > SCREEN_WIDTH + 70) continue;

			iShowImage(x, y - 10, 58, 20, imgArrow);
		}
	}

	int getEnemyMoveImage(Level1Enemy &e) {
		int phase = (e.animTimer / 5) % 4;

		if (e.facingRight) {
			if (phase == 0) return imgShadowIdleR;
			if (phase == 1) return imgShadowRunR1;
			if (phase == 2) return imgShadowRunR2;
			return imgShadowRunR1;
		}

		if (phase == 0) return imgShadowIdleL;
		if (phase == 1) return imgShadowRunL1;
		if (phase == 2) return imgShadowRunL2;
		return imgShadowRunL1;
	}

	void drawEnemies(float cameraX) {
		for (int i = 0; i < ENEMY_COUNT; i++) {
			Level1Enemy &e = enemy[i];
			if (!e.alive) continue;

			int x = (int)(e.x - cameraX);
			if (x < -120 || x > SCREEN_WIDTH + 120) continue;

			int image;

			if (e.hurtTimer > 0) {
				image = e.facingRight ? imgShadowHurtR : imgShadowIdleL;
			}
			else if (e.attackTimer > 0) {
				if (e.facingRight)
					image = (e.attackTimer > 10) ? imgShadowAttackR1 : imgShadowAttackR2;
				else
					image = (e.attackTimer > 10) ? imgShadowAttackL1 : imgShadowAttackL2;
			}
			else if (e.idleTimer > 0) {
				image = e.facingRight ? imgShadowIdleR : imgShadowIdleL;
			}
			else {
				image = getEnemyMoveImage(e);
			}

			iShowImage(x, (int)e.y, 92, 92, image);

			float hpRatio = (float)e.hp / (float)e.maxHP;

			iSetColor(50, 20, 25);
			iFilledRectangle(x + 12, (int)e.y + 96, 58, 6);

			iSetColor(190, 45, 70);
			iFilledRectangle(x + 12, (int)e.y + 96, (int)(58 * hpRatio), 6);
		}
	}

	void drawEnemyShots(float cameraX) {
		for (int i = 0; i < MAX_ENEMY_SHOTS; i++) {
			if (!enemyShot[i].active) continue;

			iShowImage((int)(enemyShot[i].x - cameraX), (int)enemyShot[i].y,
				enemyShot[i].width, enemyShot[i].height, imgShadowShot);
		}

		for (int i = 0; i < MAX_ENEMY_EFFECTS; i++) {
			if (!enemyEffect[i].active) continue;

			iShowImage((int)(enemyEffect[i].x - cameraX), (int)enemyEffect[i].y,
				enemyEffect[i].width, enemyEffect[i].height, enemyEffect[i].image);
		}
	}

	void drawExit(float cameraX) {
		int x = (int)(exitBox.x - cameraX);

		if (x < -180 || x > SCREEN_WIDTH + 180) return;

		iShowImage(x - 8, (int)exitBox.y - 5, 155, 190, imgExit);
	}

	void drawHUD() {
		char scoreText[32], goldText[32], enemyText[32], lifeText[16];
		sprintf_s(scoreText, sizeof(scoreText), "%d", score);
		sprintf_s(goldText, sizeof(goldText), "GOLD %d/%d", goldCollected, GOLD_COUNT);
		sprintf_s(enemyText, sizeof(enemyText), "%d/%d", enemiesDefeated, ENEMY_COUNT);

		int lives = 3 - deaths;
		if (lives < 0) lives = 0;
		sprintf_s(lifeText, sizeof(lifeText), "%d", lives);

		// NINJA HP ONLY follows the Level-2 mechanism:
		// dynamic HP fill first, decorative frame over it, then NINJA image label.
		float hpRatio = playerHUDMaxHP > 0 ? (float)playerHUDHP / (float)playerHUDMaxHP : 0.0f;
		if (hpRatio < 0.0f) hpRatio = 0.0f;
		if (hpRatio > 1.0f) hpRatio = 1.0f;

		iSetColor(180, 38, 42);
		iFilledRectangle(45, 548, (int)(245 * hpRatio), 12);

		iShowImage(18, 536, 300, 38, imgHUD1);
		iShowImage(25, 570, 118, 26, imgHUDNinja);

		// Keep the rest of the Level-1 HUD as Level-1.
		iShowImage(438, 548, 115, 28, imgHUDScore);
		iSetColor(245, 215, 150);
		iText(478, 522, scoreText, GLUT_BITMAP_HELVETICA_18);

		iShowImage(805, 545, 175, 30, imgHUDShadow);
		iSetColor(245, 215, 150);
		iText(875, 520, enemyText, GLUT_BITMAP_HELVETICA_18);

		iShowImage(18, 470, 145, 26, imgHUDLives);
		iSetColor(245, 215, 150);
		iText(175, 476, lifeText, GLUT_BITMAP_HELVETICA_18);

		iSetColor(245, 215, 150);
		iText(438, 480, goldText, GLUT_BITMAP_HELVETICA_18);
	}

	void drawCompleteOverlay() {
		if (!completed) return;

		iSetColor(18, 16, 18);
		iFilledRectangle(300, 205, 425, 190);

		iSetColor(245, 205, 95);
		iText(420, 350, (char*)"LEVEL 1 COMPLETE", GLUT_BITMAP_TIMES_ROMAN_24);

		char scoreText[80], starText[50], goldText[50];

		sprintf_s(scoreText, sizeof(scoreText), "FINAL SCORE: %d", finalScore);
		sprintf_s(starText, sizeof(starText), "STARS EARNED: %d / 3", earnedStars);
		sprintf_s(goldText, sizeof(goldText), "GOLD: %d / %d", goldCollected, GOLD_COUNT);

		iSetColor(255, 255, 255);
		iText(430, 310, scoreText, GLUT_BITMAP_HELVETICA_18);
		iText(430, 280, starText, GLUT_BITMAP_HELVETICA_18);
		iText(430, 250, goldText, GLUT_BITMAP_HELVETICA_18);
	}

	void draw(float cameraX) {
		drawBackground();
		drawPlatforms(cameraX);
		drawGold(cameraX);
		drawTraps(cameraX);
		drawHazardArrows(cameraX);
		drawExit(cameraX);
		drawEnemyShots(cameraX);
		drawEnemies(cameraX);
		drawHUD();
		drawCompleteOverlay();
	}
};

#endif
