#ifndef LEVEL3_H
#define LEVEL3_H

#include <cstdio>
#include <cmath>
#include "Gameplay.h"

enum L3Dir { L3_LEFT = 0, L3_RIGHT = 1, L3_UP = 2, L3_DOWN = 3 };
enum L3PickupType { L3_PICK_STAR = 0, L3_PICK_CHEST = 1, L3_PICK_TREASURE = 2 };
enum L3TrapType { L3_TRAP_SPIKE = 0, L3_TRAP_BLADE = 1, L3_TRAP_FIRE = 2 };
enum L3ArenaHeroState { L3AH_IDLE = 0, L3AH_MOVE, L3AH_DASH, L3AH_PUNCH, L3AH_KICK, L3AH_HEAVY, L3AH_BLOCK, L3AH_THROW, L3AH_HURT, L3AH_KO };
enum L3ArenaEnemyState { L3AE_ENTER = 0, L3AE_CHASE, L3AE_IDLE, L3AE_PUNCH, L3AE_KICK, L3AE_POWER, L3AE_BLOCK, L3AE_HURT, L3AE_KO, L3AE_DODGE, L3AE_LEAP, L3AE_SPIN };

struct L3Rect { float x, y, w, h; };

struct L3Exit {
	int target;
	L3Rect trigger;
	float spawnX, spawnY;
};

struct L3Pickup {
	int map, type, sealID, img;
	float x, y;
	bool taken;
};

struct L3Trap {
	int map, type, img, cycleTimer, phase;
	float x, y, w, h;
	bool hidden, revealed, active;
};

struct L3Puzzle {
	int map, sealID;
	float x, y;
	bool solved;
};

struct L3ArenaEnemy {
	bool active, alive, entering, attackDone, projectileDone;
	int state, dir, actionDir, stateTimer, animTimer, hp, maxHP, attackCooldown, powerCooldown, invincibleTimer, decisionCooldown, aiCycle, defenseCycle, wave, id;
	float x, y, vx, vy, moveSpeed, entryX, entryY;
};

struct L3ArenaProjectile {
	bool active, fromHero;
	int img[4], frameCount, animTimer, damage, w, h;
	float x, y, vx, vy;
};

struct L3ArenaEffect {
	bool active, mirror;
	int img, timer, life, w, h;
	float x, y;
};

struct Level3Stage {
	static const int MAP_COUNT = 10;
	static const int MAX_WALK = 10;
	static const int MAX_BLOCK = 20;
	static const int MAX_PICKUP = 20;
	static const int MAX_TRAP = 30;
	static const int PUZZLE_COUNT = 3;

	static const int ARENA_ENEMY_COUNT = 8;
	static const int ARENA_PROJECTILE_COUNT = 32;
	static const int ARENA_EFFECT_COUNT = 24;
	static const int ARENA_HERO_GROUND_Y = 92;
	static const int ARENA_ENEMY_GROUND_Y = 82;
	static const int ARENA_FLOOR_LEFT = 115;
	static const int ARENA_FLOOR_RIGHT = 910;
	static const int ARENA_FLOOR_BOTTOM = 70;
	static const int ARENA_FLOOR_TOP = 420;

	int areaImage[MAP_COUNT], arenaImage, imgUp[4], imgDown[4];
	int imgSpike, imgBlade, imgFire, imgChest, imgTreasure, imgPuzzle, imgPortal, imgStar;
	int imgArenaHUD[19], imgArenaHeroPunch[6], imgArenaHeroKick[5], imgArenaHeroHeavy[6], imgArenaHeroBlock[4], imgArenaHeroThrow[8], imgArenaHeroHurt[5], imgArenaHeroKO[5], imgArenaHeroDash[6];
	int imgArenaImpact, imgArenaGroundBurst, imgArenaCrescent, imgArenaSmoke, imgHeroSlash, imgHeroPowerSlash, imgHeroHit, imgHeroPowerHit, imgHeroFastShuriken;
	int imgVillainIdle, imgVillainRun[6], imgVillainPunch[5], imgVillainKick[4], imgVillainBlock[3], imgVillainHurt[3], imgVillainDodge[4], imgVillainLeap[5], imgVillainSpin[3], imgVillainPower[8], imgVillainProjectile[5], imgVillainKO;
	int imgVillainVertical[14], imgHeroShuriken[4];

	L3Rect walkArea[MAP_COUNT][MAX_WALK], blockArea[MAP_COUNT][MAX_BLOCK];
	L3Exit exitArea[MAP_COUNT][4];
	L3Pickup pickup[MAX_PICKUP];
	L3Trap trap[MAX_TRAP];
	L3Puzzle puzzle[PUZZLE_COUNT];
	L3ArenaEnemy arenaEnemy[ARENA_ENEMY_COUNT];
	L3ArenaProjectile arenaProjectile[ARENA_PROJECTILE_COUNT];
	L3ArenaEffect arenaEffect[ARENA_EFFECT_COUNT];

	int walkCount[MAP_COUNT], blockCount[MAP_COUNT], pickupCount, trapCount, currentArea;
	int verticalFrame, verticalTimer, lastMoveDir, treasureCount, playerHP, invincibleTimer, globalTick;
	float moveSpeed, playerRadius, footOffsetY, portalX[MAP_COUNT], portalY[MAP_COUNT];
	bool seal[3], portalActive, portalEntered, arenaMode, showHUD, showCollisionDebug;

	int arenaWave, arenaWaveDelay, arenaIntroTimer, arenaHeroState, arenaHeroTimer, arenaHeroDir, arenaHeroActionDir, arenaHeroHP, arenaHeroMaxHP, arenaHeroLives;
	int arenaHeroInvincible, arenaAttackCooldown, arenaThrowCooldown, arenaDashCooldown, arenaDashTimer, arenaScore, arenaCombo, arenaComboTimer, arenaPowerMeter, arenaPowerTimer, arenaHealCharges, arenaHealCooldown;
	int arenaDamageReduction, arenaDamageBonus, arenaResultTimer, finalScore, earnedStars;
	float arenaDashVX, arenaDashVY;
	bool arenaHeroAttackDone, arenaHeroThrowDone, arenaFinished, arenaWon, resultReady, resultSubmitted, returnRequested, arenaFightStarted;

	void setRect(L3Rect &r, float x, float y, float w, float h){ r.x = x; r.y = y; r.w = w; r.h = h; }
	bool inside(float px, float py, L3Rect &r){ return px >= r.x&&px <= r.x + r.w&&py >= r.y&&py <= r.y + r.h; }
	float distance(float x1, float y1, float x2, float y2){ float dx = x1 - x2, dy = y1 - y2; return sqrtf(dx*dx + dy*dy); }
	float absF(float v){ return v<0 ? -v : v; }
	float clampF(float v, float a, float b){ if (v<a)return a; if (v>b)return b; return v; }

	void addWalk(int map, float x, float y, float w, float h){
		if (map<0 || map >= MAP_COUNT || walkCount[map] >= MAX_WALK)return;
		setRect(walkArea[map][walkCount[map]++], x, y, w, h);
	}

	void addBlock(int map, float x, float y, float w, float h){
		if (map<0 || map >= MAP_COUNT || blockCount[map] >= MAX_BLOCK)return;
		setRect(blockArea[map][blockCount[map]++], x, y, w, h);
	}

	void setExit(int map, int dir, int target, float x, float y, float w, float h, float spawnX, float spawnY){
		L3Exit &e = exitArea[map][dir]; e.target = target; setRect(e.trigger, x, y, w, h); e.spawnX = spawnX; e.spawnY = spawnY;
	}

	void addPickup(int map, int type, int sealID, int img, float x, float y){
		if (pickupCount >= MAX_PICKUP)return;
		L3Pickup &p = pickup[pickupCount++]; p.map = map; p.type = type; p.sealID = sealID; p.img = img; p.x = x; p.y = y; p.taken = false;
	}

	void addTrap(int map, int type, int img, float x, float y, float w, float h, bool hidden, int phase){
		if (trapCount >= MAX_TRAP)return;
		L3Trap &t = trap[trapCount++]; t.map = map; t.type = type; t.img = img; t.cycleTimer = 0; t.phase = phase; t.x = x; t.y = y; t.w = w; t.h = h;
		t.hidden = hidden; t.revealed = !hidden; t.active = false;
	}

	void setPuzzle(int id, int map, int sealID, float x, float y){
		if (id<0 || id >= PUZZLE_COUNT)return;
		puzzle[id].map = map; puzzle[id].sealID = sealID; puzzle[id].x = x; puzzle[id].y = y; puzzle[id].solved = false;
	}

	void load(){
		areaImage[0] = iLoadImage("Images\\Levels\\Level-3\\m-1.png");
		areaImage[1] = iLoadImage("Images\\Levels\\Level-3\\m-2.png");
		areaImage[2] = iLoadImage("Images\\Levels\\Level-3\\m-3.png");
		areaImage[3] = iLoadImage("Images\\Levels\\Level-3\\1.png");
		areaImage[4] = iLoadImage("Images\\Levels\\Level-3\\2.png");
		areaImage[5] = iLoadImage("Images\\Levels\\Level-3\\3.png");
		areaImage[6] = iLoadImage("Images\\Levels\\Level-3\\4.png");
		areaImage[7] = iLoadImage("Images\\Levels\\Level-3\\5.png");
		areaImage[8] = iLoadImage("Images\\Levels\\Level-3\\6.png");
		areaImage[9] = iLoadImage("Images\\Levels\\Level-3\\7.png");
		arenaImage = iLoadImage("Images\\Levels\\Level-3\\4.png");

		imgUp[0] = iLoadImage("Images\\Levels\\Level-3\\c-05.png"); imgUp[1] = iLoadImage("Images\\Levels\\Level-3\\c-06.png");
		imgUp[2] = iLoadImage("Images\\Levels\\Level-3\\c-07.png"); imgUp[3] = iLoadImage("Images\\Levels\\Level-3\\c-08.png");
		imgDown[0] = iLoadImage("Images\\Levels\\Level-3\\c-01.png"); imgDown[1] = iLoadImage("Images\\Levels\\Level-3\\c-02.png");
		imgDown[2] = iLoadImage("Images\\Levels\\Level-3\\c-03.png"); imgDown[3] = iLoadImage("Images\\Levels\\Level-3\\c-04.png");

		imgSpike = iLoadImage("Images\\Levels\\Level-3\\t-1.png"); imgBlade = iLoadImage("Images\\Levels\\Level-3\\t-2.png");
		imgFire = iLoadImage("Images\\Levels\\Level-3\\t-3.png"); imgChest = iLoadImage("Images\\Levels\\Level-3\\t-7.png");
		imgTreasure = iLoadImage("Images\\Levels\\Level-3\\t-8.png"); imgPuzzle = iLoadImage("Images\\Levels\\Level-3\\t-9.png");
		imgPortal = iLoadImage("Images\\Levels\\Level-3\\t-10.png"); imgStar = iLoadImage("Images\\UI\\Common\\s.png");

		for (int i = 1; i <= 18; i++){ char p[100]; sprintf_s(p, sizeof(p), "Images\\Enemies\\Level2\\%d.png", i); imgArenaHUD[i] = iLoadImage(p); }
		int hpunch[6] = { 45, 46, 47, 48, 50, 52 }, hkick[5] = { 49, 51, 53, 54, 55 }, hheavy[6] = { 55, 56, 57, 58, 59, 60 };
		int hblock[4] = { 61, 63, 64, 63 }, hthrow[8] = { 25, 26, 27, 28, 29, 30, 31, 34 }, hhurt[5] = { 40, 41, 44, 62, 63 }, hko[5] = { 11, 23, 40, 42, 43 }, hdash[6] = { 33, 34, 35, 36, 43, 44 };
		for (int i = 0; i<6; i++){ char p[100]; sprintf_s(p, sizeof(p), "Images\\Player\\c-%02d.png", hpunch[i]); imgArenaHeroPunch[i] = iLoadImage(p); sprintf_s(p, sizeof(p), "Images\\Player\\c-%02d.png", hheavy[i]); imgArenaHeroHeavy[i] = iLoadImage(p); sprintf_s(p, sizeof(p), "Images\\Player\\c-%02d.png", hdash[i]); imgArenaHeroDash[i] = iLoadImage(p); }
		for (int i = 0; i<5; i++){ char p[100]; sprintf_s(p, sizeof(p), "Images\\Player\\c-%02d.png", hkick[i]); imgArenaHeroKick[i] = iLoadImage(p); sprintf_s(p, sizeof(p), "Images\\Player\\c-%02d.png", hhurt[i]); imgArenaHeroHurt[i] = iLoadImage(p); sprintf_s(p, sizeof(p), "Images\\Player\\c-%02d.png", hko[i]); imgArenaHeroKO[i] = iLoadImage(p); }
		for (int i = 0; i<4; i++){ char p[100]; sprintf_s(p, sizeof(p), "Images\\Player\\c-%02d.png", hblock[i]); imgArenaHeroBlock[i] = iLoadImage(p); sprintf_s(p, sizeof(p), "Images\\Weapons\\Shuriken\\s-%02d.png", i + 1); imgHeroShuriken[i] = iLoadImage(p); }
		for (int i = 0; i<8; i++){ char p[100]; sprintf_s(p, sizeof(p), "Images\\Player\\c-%02d.png", hthrow[i]); imgArenaHeroThrow[i] = iLoadImage(p); }

		imgArenaImpact = iLoadImage("Images\\Enemies\\Level2\\l2-47.png"); imgArenaGroundBurst = iLoadImage("Images\\Enemies\\Level2\\l2-48.png");
		imgArenaCrescent = iLoadImage("Images\\Enemies\\Level2\\l2-49.png"); imgArenaSmoke = iLoadImage("Images\\Enemies\\Level2\\l2-50.png");
		imgHeroSlash = iLoadImage("Images\\Weapons\\Shuriken\\s-05.png"); imgHeroHit = iLoadImage("Images\\Weapons\\Shuriken\\s-06.png");
		imgHeroPowerSlash = iLoadImage("Images\\Weapons\\Shuriken\\s-08.png"); imgHeroPowerHit = iLoadImage("Images\\Weapons\\Shuriken\\s-09.png"); imgHeroFastShuriken = iLoadImage("Images\\Weapons\\Shuriken\\s-10.png");

		imgVillainIdle = iLoadImage("Images\\Enemies\\Level2\\l2-1.png");
		int vr[6] = { 2, 3, 2, 3, 2, 3 }, vp[5] = { 4, 13, 25, 29, 38 }, vk[4] = { 6, 15, 26, 34 }, vb[3] = { 8, 18, 30 }, vh[3] = { 17, 24, 32 }, vd[4] = { 19, 20, 28, 33 }, vl[5] = { 21, 22, 23, 26, 40 }, vs[3] = { 35, 39, 40 }, vpow[8] = { 5, 7, 14, 36, 37, 38, 41, 30 };
		for (int i = 0; i<6; i++){ char p[100]; sprintf_s(p, sizeof(p), "Images\\Enemies\\Level2\\l2-%d.png", vr[i]); imgVillainRun[i] = iLoadImage(p); }
		for (int i = 0; i<5; i++){ char p[100]; sprintf_s(p, sizeof(p), "Images\\Enemies\\Level2\\l2-%d.png", vp[i]); imgVillainPunch[i] = iLoadImage(p); sprintf_s(p, sizeof(p), "Images\\Enemies\\Level2\\l2-%d.png", vl[i]); imgVillainLeap[i] = iLoadImage(p); }
		for (int i = 0; i<4; i++){ char p[100]; sprintf_s(p, sizeof(p), "Images\\Enemies\\Level2\\l2-%d.png", vk[i]); imgVillainKick[i] = iLoadImage(p); sprintf_s(p, sizeof(p), "Images\\Enemies\\Level2\\l2-%d.png", vd[i]); imgVillainDodge[i] = iLoadImage(p); }
		for (int i = 0; i<3; i++){ char p[100]; sprintf_s(p, sizeof(p), "Images\\Enemies\\Level2\\l2-%d.png", vb[i]); imgVillainBlock[i] = iLoadImage(p); sprintf_s(p, sizeof(p), "Images\\Enemies\\Level2\\l2-%d.png", vh[i]); imgVillainHurt[i] = iLoadImage(p); sprintf_s(p, sizeof(p), "Images\\Enemies\\Level2\\l2-%d.png", vs[i]); imgVillainSpin[i] = iLoadImage(p); }
		for (int i = 0; i<8; i++){ char p[100]; sprintf_s(p, sizeof(p), "Images\\Enemies\\Level2\\l2-%d.png", vpow[i]); imgVillainPower[i] = iLoadImage(p); }
		for (int i = 0; i<5; i++){ char p[100]; sprintf_s(p, sizeof(p), "Images\\Enemies\\Level2\\l2-%d.png", 42 + i); imgVillainProjectile[i] = iLoadImage(p); }
		imgVillainKO = iLoadImage("Images\\Enemies\\Level2\\l2-21.png");
		for (int i = 0; i<14; i++){ char p[100]; sprintf_s(p, sizeof(p), "Images\\Enemies\\Level2\\l%d.png", i + 1); imgVillainVertical[i] = iLoadImage(p); }

		for (int i = 0; i<MAP_COUNT; i++){
			walkCount[i] = blockCount[i] = 0;
			for (int d = 0; d<4; d++){ exitArea[i][d].target = -1; setRect(exitArea[i][d].trigger, 0, 0, 0, 0); exitArea[i][d].spawnX = exitArea[i][d].spawnY = 0; }
		}

		addWalk(0, 230, 205, 570, 250); addWalk(0, 405, 0, 220, 225); addWalk(0, 405, 420, 220, 90);
		addWalk(1, 420, 0, 200, 600); addWalk(1, 0, 280, 1024, 110); addWalk(1, 300, 210, 420, 220);
		addWalk(2, 100, 180, 800, 320); addWalk(2, 430, 0, 180, 195); addWalk(2, 430, 465, 180, 135); addWalk(2, 0, 300, 165, 110); addWalk(2, 860, 300, 164, 110);
		addWalk(3, 175, 150, 675, 320); addWalk(3, 425, 0, 180, 180); addWalk(3, 425, 445, 180, 155); addWalk(3, 0, 300, 225, 105); addWalk(3, 805, 300, 219, 105);
		addWalk(4, 115, 245, 790, 255); addWalk(4, 450, 0, 130, 260); addWalk(4, 430, 440, 170, 105);
		addWalk(5, 465, 210, 100, 300); addWalk(5, 285, 400, 455, 115); addWalk(5, 0, 200, 215, 185); addWalk(5, 810, 200, 214, 185);
		addWalk(5, 390, 95, 250, 125); addWalk(5, 430, 0, 170, 110); addWalk(5, 170, 385, 190, 130); addWalk(5, 665, 385, 190, 130);
		addWalk(6, 100, 100, 825, 315); addWalk(6, 430, 0, 180, 110); addWalk(6, 420, 355, 190, 95);
		addWalk(7, 85, 210, 855, 300); addWalk(7, 425, 0, 180, 220); addWalk(7, 425, 475, 180, 125);
		addWalk(8, 430, 0, 165, 600); addWalk(8, 80, 425, 860, 90); addWalk(8, 0, 255, 1024, 90); addWalk(8, 80, 95, 860, 90); addWalk(8, 35, 135, 120, 330); addWalk(8, 870, 135, 120, 330);
		addWalk(9, 180, 100, 665, 445); addWalk(9, 380, 0, 265, 245); addWalk(9, 380, 405, 265, 195); addWalk(9, 0, 205, 280, 190); addWalk(9, 745, 205, 279, 190);

		addBlock(0, 220, 300, 180, 150); addBlock(0, 640, 300, 180, 150); addBlock(0, 280, 180, 90, 100); addBlock(0, 660, 180, 90, 100);
		addBlock(1, 625, 405, 85, 105);
		addBlock(2, 185, 300, 165, 170); addBlock(2, 690, 300, 155, 180); addBlock(2, 325, 170, 90, 115); addBlock(2, 610, 170, 90, 115);
		addBlock(3, 295, 390, 95, 130); addBlock(3, 640, 390, 95, 130); addBlock(3, 200, 210, 120, 130); addBlock(3, 710, 210, 120, 130);
		addBlock(4, 270, 320, 90, 80); addBlock(4, 360, 395, 85, 70); addBlock(4, 520, 375, 100, 110); addBlock(4, 690, 370, 155, 130); addBlock(4, 755, 300, 95, 120);
		addBlock(5, 345, 390, 70, 100); addBlock(5, 610, 390, 70, 100);
		addBlock(6, 120, 280, 95, 120); addBlock(6, 810, 280, 95, 120); addBlock(6, 285, 95, 95, 110); addBlock(6, 650, 95, 95, 110);
		addBlock(7, 110, 410, 165, 100); addBlock(7, 440, 340, 175, 160); addBlock(7, 710, 415, 145, 95); addBlock(7, 70, 300, 160, 120);
		addBlock(7, 800, 280, 160, 140); addBlock(7, 280, 300, 110, 150); addBlock(7, 620, 320, 90, 110);
		addBlock(8, 365, 430, 65, 110); addBlock(8, 590, 430, 65, 110); addBlock(8, 365, 180, 65, 120); addBlock(8, 590, 180, 65, 120);
		addBlock(9, 320, 160, 95, 95); addBlock(9, 610, 160, 95, 95);

		setExit(0, L3_UP, 1, 430, 420, 170, 90, 467, 55);
		setExit(1, L3_DOWN, 0, 450, 0, 120, 55, 467, 370); setExit(1, L3_UP, 2, 450, 540, 120, 60, 467, 55);
		setExit(1, L3_LEFT, 3, 0, 285, 65, 100, 755, 300); setExit(1, L3_RIGHT, 5, 959, 285, 65, 100, 45, 255);
		setExit(2, L3_DOWN, 1, 450, 0, 120, 55, 467, 485); setExit(2, L3_LEFT, 3, 0, 300, 65, 105, 755, 300);
		setExit(2, L3_RIGHT, 6, 959, 300, 65, 105, 90, 250); setExit(2, L3_UP, 4, 450, 535, 120, 65, 467, 55);
		setExit(3, L3_DOWN, 1, 450, 0, 120, 55, 467, 485); setExit(3, L3_RIGHT, 2, 959, 300, 65, 105, 105, 300); setExit(3, L3_UP, 6, 450, 535, 120, 65, 467, 55);
		setExit(4, L3_DOWN, 2, 455, 0, 120, 65, 467, 470); setExit(4, L3_UP, 7, 440, 465, 150, 75, 467, 55);
		setExit(5, L3_LEFT, 1, 0, 210, 65, 160, 850, 300); setExit(5, L3_UP, 6, 465, 455, 100, 60, 467, 55); setExit(5, L3_DOWN, 8, 445, 0, 140, 70, 467, 460);
		setExit(6, L3_LEFT, 2, 100, 215, 55, 150, 820, 300); setExit(6, L3_DOWN, 5, 450, 0, 120, 65, 467, 430); setExit(6, L3_RIGHT, 7, 870, 215, 55, 150, 95, 300);
		setExit(7, L3_LEFT, 6, 85, 255, 60, 150, 805, 300); setExit(7, L3_DOWN, 4, 450, 0, 120, 65, 467, 430); setExit(7, L3_RIGHT, 8, 880, 255, 60, 150, 55, 300);
		setExit(8, L3_LEFT, 7, 0, 265, 65, 80, 840, 300); setExit(8, L3_UP, 5, 450, 535, 120, 65, 467, 120); setExit(8, L3_RIGHT, 9, 959, 265, 65, 80, 300, 220);
		setExit(9, L3_LEFT, 8, 0, 205, 75, 190, 830, 300);

		pickupCount = trapCount = 0;
		addPickup(1, L3_PICK_CHEST, -1, imgChest, 355, 335); addPickup(3, L3_PICK_TREASURE, -1, imgTreasure, 510, 270);
		addPickup(5, L3_PICK_CHEST, -1, imgChest, 510, 450); addPickup(7, L3_PICK_TREASURE, -1, imgTreasure, 510, 275); addPickup(8, L3_PICK_CHEST, -1, imgChest, 500, 120);

		setPuzzle(0, 2, 0, 500, 260); setPuzzle(1, 4, 1, 470, 285); setPuzzle(2, 6, 2, 510, 230);

		addTrap(1, L3_TRAP_SPIKE, imgSpike, 330, 285, 70, 55, true, 0); addTrap(1, L3_TRAP_FIRE, imgFire, 585, 300, 58, 78, true, 45);
		addTrap(2, L3_TRAP_BLADE, imgBlade, 415, 225, 72, 72, true, 20); addTrap(2, L3_TRAP_SPIKE, imgSpike, 560, 410, 72, 55, true, 70);
		addTrap(3, L3_TRAP_FIRE, imgFire, 400, 250, 58, 82, true, 10); addTrap(3, L3_TRAP_BLADE, imgBlade, 570, 305, 72, 72, true, 80);
		addTrap(4, L3_TRAP_SPIKE, imgSpike, 430, 275, 72, 55, true, 30); addTrap(4, L3_TRAP_BLADE, imgBlade, 610, 285, 74, 74, true, 95); addTrap(4, L3_TRAP_FIRE, imgFire, 210, 300, 56, 80, true, 55);
		addTrap(5, L3_TRAP_FIRE, imgFire, 480, 245, 58, 82, true, 25); addTrap(5, L3_TRAP_SPIKE, imgSpike, 305, 435, 72, 55, true, 85); addTrap(5, L3_TRAP_BLADE, imgBlade, 720, 435, 72, 72, true, 125);
		addTrap(6, L3_TRAP_BLADE, imgBlade, 390, 235, 72, 72, true, 35); addTrap(6, L3_TRAP_FIRE, imgFire, 610, 230, 58, 82, true, 105);
		addTrap(7, L3_TRAP_SPIKE, imgSpike, 390, 250, 72, 55, true, 15); addTrap(7, L3_TRAP_BLADE, imgBlade, 650, 250, 72, 72, true, 65); addTrap(7, L3_TRAP_FIRE, imgFire, 500, 470, 58, 82, true, 115);
		addTrap(8, L3_TRAP_FIRE, imgFire, 480, 275, 58, 82, true, 40); addTrap(8, L3_TRAP_SPIKE, imgSpike, 120, 270, 72, 55, true, 100); addTrap(8, L3_TRAP_BLADE, imgBlade, 835, 275, 72, 72, true, 145);
		addTrap(9, L3_TRAP_SPIKE, imgSpike, 430, 175, 72, 55, true, 20); addTrap(9, L3_TRAP_FIRE, imgFire, 590, 340, 58, 82, true, 90);

		// Once all puzzles are solved, t-10 appears on EVERY exploration map.
		float px[MAP_COUNT] = { 510, 510, 500, 510, 500, 540, 510, 510, 510, 510 };
		float py[MAP_COUNT] = { 260, 330, 390, 280, 255, 345, 330, 250, 350, 300 };
		for (int i = 0; i<MAP_COUNT; i++){ portalX[i] = px[i]; portalY[i] = py[i]; }

		moveSpeed = 5.0f; playerRadius = 8.0f; footOffsetY = 15.0f;
		showHUD = true; showCollisionDebug = false;
	}

	const char *getAreaName(){
		switch (currentArea){
		case 0:return "ENTRANCE COURTYARD"; case 1:return "BAMBOO CROSSING"; case 2:return "CRESCENT COURTYARD";
		case 3:return "CENTRAL HALL"; case 4:return "NORTH WATCHTOWER"; case 5:return "MOON SHRINE";
		case 6:return "SYMBOL CHAMBER"; case 7:return "SHADOW BARRACKS"; case 8:return "SHADOW PASSAGE";
		default:return "PORTAL SANCTUARY";
		}
	}

	void placePlayer(NinjaPlayer &ninja, float x, float y){
		ninja.x = x; ninja.y = y; ninja.vx = 0; ninja.vy = 0; ninja.grounded = true; ninja.state = NINJA_IDLE; ninja.runFrame = 0; ninja.runTimer = 0;
	}

	bool pointInWalk(float px, float py){
		for (int i = 0; i<walkCount[currentArea]; i++)if (inside(px, py, walkArea[currentArea][i]))return true;
		return false;
	}

	bool pointBlocked(float px, float py){
		for (int i = 0; i<blockCount[currentArea]; i++)if (inside(px, py, blockArea[currentArea][i]))return true;
		return false;
	}

	bool footValid(float px, float py){ return pointInWalk(px, py) && !pointBlocked(px, py); }

	bool canStandAt(float x, float y){
		float cx = x + NinjaPlayer::DRAW_W*0.5f, cy = y + footOffsetY, r = playerRadius;
		if (!footValid(cx, cy))return false;
		if (!footValid(cx - r, cy) || !footValid(cx + r, cy) || !footValid(cx, cy - r) || !footValid(cx, cy + r))return false;
		return true;
	}

	bool findSafeSpawn(float &x, float &y){
		if (canStandAt(x, y))return true;
		float bx = x, by = y;
		for (int radius = 5; radius <= 260; radius += 5){
			for (int dx = -radius; dx <= radius; dx += 5){
				float tx = bx + dx, ty = by + radius; if (canStandAt(tx, ty)){ x = tx; y = ty; return true; }
				ty = by - radius; if (canStandAt(tx, ty)){ x = tx; y = ty; return true; }
			}
			for (int dy = -radius; dy <= radius; dy += 5){
				float tx = bx + radius, ty = by + dy; if (canStandAt(tx, ty)){ x = tx; y = ty; return true; }
				tx = bx - radius; if (canStandAt(tx, ty)){ x = tx; y = ty; return true; }
			}
		}
		return false;
	}

	void resetArenaData(){
		for (int i = 0; i<ARENA_ENEMY_COUNT; i++){ arenaEnemy[i].active = false; arenaEnemy[i].alive = false; arenaEnemy[i].entering = false; }
		for (int i = 0; i<ARENA_PROJECTILE_COUNT; i++)arenaProjectile[i].active = false;
		for (int i = 0; i<ARENA_EFFECT_COUNT; i++)arenaEffect[i].active = false;
		arenaWave = 0; arenaWaveDelay = 0; arenaIntroTimer = 0; arenaFightStarted = false; arenaHeroState = L3AH_IDLE; arenaHeroTimer = 0; arenaHeroDir = arenaHeroActionDir = L3_DOWN;
		arenaHeroMaxHP = 140; arenaHeroHP = arenaHeroMaxHP; arenaHeroLives = 3; arenaHeroInvincible = 0; arenaAttackCooldown = arenaThrowCooldown = arenaDashCooldown = arenaDashTimer = 0;
		arenaScore = arenaCombo = arenaComboTimer = 0; arenaPowerMeter = arenaPowerTimer = 0; arenaHealCharges = 3; arenaHealCooldown = 0; arenaDamageReduction = 20; arenaDamageBonus = 0;
		arenaDashVX = arenaDashVY = 0; arenaHeroAttackDone = arenaHeroThrowDone = false; arenaFinished = arenaWon = resultReady = resultSubmitted = returnRequested = false; arenaResultTimer = 0; finalScore = earnedStars = 0;
	}

	void reset(NinjaPlayer &ninja){
		currentArea = 0; verticalFrame = verticalTimer = 0; lastMoveDir = L3_DOWN; treasureCount = 0; playerHP = 5; invincibleTimer = 0; globalTick = 0;
		seal[0] = seal[1] = seal[2] = false; portalActive = false; portalEntered = false; arenaMode = false;
		for (int i = 0; i<pickupCount; i++)pickup[i].taken = false;
		for (int i = 0; i<trapCount; i++){ trap[i].cycleTimer = 0; trap[i].revealed = !trap[i].hidden; trap[i].active = false; }
		for (int i = 0; i<PUZZLE_COUNT; i++)puzzle[i].solved = false;
		resetArenaData();
		float x = 467, y = 60; if (!findSafeSpawn(x, y)){ x = 467; y = 230; findSafeSpawn(x, y); }placePlayer(ninja, x, y);
	}

	void moveAxis(NinjaPlayer &ninja, float amount, bool horizontal){
		if (amount == 0)return;
		float left = amount;
		while (left>0.001f || left<-0.001f){
			float step = left; if (step>1.0f)step = 1.0f; if (step<-1.0f)step = -1.0f;
			float nx = ninja.x + (horizontal ? step : 0), ny = ninja.y + (horizontal ? 0 : step);
			if (!canStandAt(nx, ny))break;
			ninja.x = nx; ninja.y = ny; left -= step;
		}
	}

	void updatePlayerAnimation(float dx, float dy){
		if (dy>0)lastMoveDir = L3_UP; else if (dy<0)lastMoveDir = L3_DOWN; else if (dx<0)lastMoveDir = L3_LEFT; else if (dx>0)lastMoveDir = L3_RIGHT;
		if (dy != 0){ verticalTimer++; if (verticalTimer >= 3){ verticalTimer = 0; verticalFrame = (verticalFrame + 1) % 4; } }
		else { verticalTimer = 0; verticalFrame = 0; }
	}

	bool heroInExit(L3Exit &e, NinjaPlayer &ninja){
		if (e.target<0)return false;
		float cx = ninja.x + NinjaPlayer::DRAW_W*0.5f, cy = ninja.y + footOffsetY;
		return inside(cx, cy, e.trigger);
	}

	void changeArea(L3Exit &e, NinjaPlayer &ninja){
		if (e.target<0)return;
		currentArea = e.target;
		float x = e.spawnX, y = e.spawnY;
		if (!findSafeSpawn(x, y)){ x = SCREEN_WIDTH / 2 - NinjaPlayer::DRAW_W / 2; y = SCREEN_HEIGHT / 2 - NinjaPlayer::DRAW_H / 2; findSafeSpawn(x, y); }
		placePlayer(ninja, x, y); verticalFrame = verticalTimer = 0;
	}

	void debugJump(int map, NinjaPlayer &ninja){
		if (map<0 || map >= MAP_COUNT)return;
		arenaMode = false; portalEntered = false; currentArea = map;
		float x = SCREEN_WIDTH / 2 - NinjaPlayer::DRAW_W / 2, y = SCREEN_HEIGHT / 2 - NinjaPlayer::DRAW_H / 2;
		if (!findSafeSpawn(x, y))for (int i = 0; i<walkCount[currentArea]; i++){
			L3Rect &r = walkArea[currentArea][i]; x = r.x + r.w*0.5f - NinjaPlayer::DRAW_W*0.5f; y = r.y + r.h*0.5f - footOffsetY;
			if (findSafeSpawn(x, y))break;
		}
		placePlayer(ninja, x, y);
	}

	int solvedPuzzleCount(){ return (puzzle[0].solved ? 1 : 0) + (puzzle[1].solved ? 1 : 0) + (puzzle[2].solved ? 1 : 0); }
	int currentDisplayScore(){ return arenaScore + treasureCount * 250 + solvedPuzzleCount() * 300; }
	bool arenaHeroAttacking(){ return arenaHeroState == L3AH_PUNCH || arenaHeroState == L3AH_KICK || arenaHeroState == L3AH_HEAVY || arenaHeroState == L3AH_THROW; }
	bool arenaHeroCanAct(){ return arenaHeroState != L3AH_HURT&&arenaHeroState != L3AH_KO&&arenaHeroState != L3AH_DASH&&!arenaHeroAttacking(); }

	void damagePlayer(NinjaPlayer &ninja, int damage){
		if (invincibleTimer>0 || arenaMode)return; playerHP -= damage; if (playerHP<0)playerHP = 0; invincibleTimer = 65;
		if (playerHP <= 0){ playerHP = 5; float x = 467, y = 230; if (!findSafeSpawn(x, y)){ x = 467; y = 230; }placePlayer(ninja, x, y); }
	}

	void updateTraps(NinjaPlayer &ninja){
		float px = ninja.x + NinjaPlayer::DRAW_W*0.5f, py = ninja.y + footOffsetY;
		for (int i = 0; i<trapCount; i++){
			L3Trap &t = trap[i]; if (t.map != currentArea)continue; float cx = t.x + t.w*0.5f, cy = t.y + t.h*0.5f;
			if (t.hidden&&!t.revealed&&distance(px, py, cx, cy)<95)t.revealed = true; if (!t.revealed){ t.active = false; continue; }
			t.cycleTimer++; int tick = (t.cycleTimer + t.phase) % 180; if (t.type == L3_TRAP_SPIKE)t.active = tick >= 25 && tick<90; else if (t.type == L3_TRAP_BLADE)t.active = tick >= 15 && tick<145; else t.active = tick >= 45 && tick<105;
			float tx = t.x; if (t.type == L3_TRAP_BLADE)tx += sinf((t.cycleTimer + t.phase)*0.08f)*12.0f; L3Rect hit; setRect(hit, tx + 8, t.y + 6, t.w - 16, t.h - 12); if (t.active&&inside(px, py, hit))damagePlayer(ninja, 1);
		}
	}

	void enterArena(NinjaPlayer &ninja){
		portalEntered = true; arenaMode = true; resetArenaData(); placePlayer(ninja, (float)(SCREEN_WIDTH / 2 - NinjaPlayer::DRAW_W / 2), 185.0f); ninja.grounded = true; ninja.vx = ninja.vy = 0; lastMoveDir = L3_DOWN;
	}

	void updatePuzzlesAndPickups(NinjaPlayer &ninja){
		float px = ninja.x + NinjaPlayer::DRAW_W*0.5f, py = ninja.y + NinjaPlayer::DRAW_H*0.5f;
		for (int i = 0; i<PUZZLE_COUNT; i++){ L3Puzzle &q = puzzle[i]; if (q.solved || q.map != currentArea)continue; if (distance(px, py, q.x, q.y)<58){ q.solved = true; if (q.sealID >= 0 && q.sealID<3)seal[q.sealID] = true; } }
		for (int i = 0; i<pickupCount; i++){ L3Pickup &p = pickup[i]; if (p.taken || p.map != currentArea || p.type == L3_PICK_STAR)continue; if (distance(px, py, p.x, p.y)<42){ p.taken = true; treasureCount++; } }
		portalActive = puzzle[0].solved&&puzzle[1].solved&&puzzle[2].solved; if (portalActive&&!portalEntered&&distance(px, py, portalX[currentArea], portalY[currentArea])<62)enterArena(ninja);
	}

	// ==================== LEVEL-2 STYLE ARENA COMBAT ====================
	float arenaHeroCX(NinjaPlayer &ninja){ return ninja.x + NinjaPlayer::DRAW_W*0.5f; }
	float arenaHeroCY(NinjaPlayer &ninja){ return ninja.y + NinjaPlayer::DRAW_H*0.5f; }
	float arenaEnemyCX(L3ArenaEnemy &e){ return e.x + 55.0f; }
	float arenaEnemyCY(L3ArenaEnemy &e){ return e.y + 55.0f; }
	void dirVector(int dir, float &dx, float &dy){ dx = dy = 0; if (dir == L3_LEFT)dx = -1; else if (dir == L3_RIGHT)dx = 1; else if (dir == L3_UP)dy = 1; else dy = -1; }
	int vectorDir(float dx, float dy){ if (absF(dx) >= absF(dy))return dx >= 0 ? L3_RIGHT : L3_LEFT; return dy >= 0 ? L3_UP : L3_DOWN; }
	void clampArenaHero(NinjaPlayer &n){ n.x = clampF(n.x, (float)ARENA_FLOOR_LEFT, (float)(ARENA_FLOOR_RIGHT - NinjaPlayer::DRAW_W)); n.y = clampF(n.y, (float)ARENA_FLOOR_BOTTOM, (float)(ARENA_FLOOR_TOP - NinjaPlayer::DRAW_H)); }
	void clampArenaEnemy(L3ArenaEnemy &e){ e.x = clampF(e.x, (float)(ARENA_FLOOR_LEFT - 100), (float)ARENA_FLOOR_RIGHT); e.y = clampF(e.y, (float)(ARENA_FLOOR_BOTTOM - 100), (float)ARENA_FLOOR_TOP); }
	void clampArenaEnemyInside(L3ArenaEnemy &e){ e.x = clampF(e.x, (float)ARENA_FLOOR_LEFT, (float)(ARENA_FLOOR_RIGHT - 110)); e.y = clampF(e.y, (float)ARENA_FLOOR_BOTTOM, (float)(ARENA_FLOOR_TOP - 110)); }

	void addArenaEffect(int img, float x, float y, int w, int h, int life, bool mirror = false){ for (int i = 0; i<ARENA_EFFECT_COUNT; i++)if (!arenaEffect[i].active){ L3ArenaEffect &e = arenaEffect[i]; e.active = true; e.mirror = mirror; e.img = img; e.x = x; e.y = y; e.w = w; e.h = h; e.life = life; e.timer = 0; return; } }
	void updateArenaEffects(){ for (int i = 0; i<ARENA_EFFECT_COUNT; i++)if (arenaEffect[i].active&&++arenaEffect[i].timer >= arenaEffect[i].life)arenaEffect[i].active = false; }
	void clearArenaProjectiles(){ for (int i = 0; i<ARENA_PROJECTILE_COUNT; i++)arenaProjectile[i].active = false; }
	void clearArenaEffects(){ for (int i = 0; i<ARENA_EFFECT_COUNT; i++)arenaEffect[i].active = false; }
	void clearArenaEnemies(){ for (int i = 0; i<ARENA_ENEMY_COUNT; i++){ arenaEnemy[i].active = false; arenaEnemy[i].alive = false; arenaEnemy[i].entering = false; } }

	void addCombo(int base){ arenaCombo = arenaComboTimer>0 ? arenaCombo + 1 : 1; arenaComboTimer = 48; arenaScore += base; if (arenaCombo >= 3)arenaScore += arenaCombo * 20; }
	void arenaGainPower(int amount){ if (arenaPowerTimer>0)return; arenaPowerMeter += amount; if (arenaPowerMeter>100)arenaPowerMeter = 100; }
	bool activateArenaPower(){ if (arenaPowerMeter<100 || arenaPowerTimer>0 || arenaHeroState == L3AH_KO || arenaHeroState == L3AH_HURT)return false; arenaPowerMeter = 0; arenaPowerTimer = 165; return true; }
	bool arenaHeal(){ if (arenaHealCharges <= 0 || arenaHealCooldown>0 || arenaHeroHP <= 0 || arenaHeroHP >= arenaHeroMaxHP || arenaHeroState == L3AH_KO || arenaHeroState == L3AH_HURT || arenaHeroAttacking())return false; arenaHeroHP += 40; if (arenaHeroHP>arenaHeroMaxHP)arenaHeroHP = arenaHeroMaxHP; arenaHealCharges--; arenaHealCooldown = 180; return true; }
	int heroDamage(int damage){ int result = damage + damage*arenaDamageBonus / 100; if (arenaPowerTimer>0)result = result * 135 / 100; return result; }
	int reduceHeroDamage(int damage){ int r = damage*(100 - arenaDamageReduction) / 100; return r<1 ? 1 : r; }
	void applyWaveHeroUpgrade(){ arenaDamageBonus = arenaWave == 1 ? 0 : (arenaWave == 2 ? 8 : 12); }

	void arenaDoorSpawn(int entrance, int slot, float &x, float &y, float &tx, float &ty, int &dir){
		float cx = (ARENA_FLOOR_LEFT + ARENA_FLOOR_RIGHT)*0.5f - 55.0f, cy = (ARENA_FLOOR_BOTTOM + ARENA_FLOOR_TOP)*0.5f - 55.0f, off = (slot % 2)*55.0f - 27.0f;
		if (entrance == L3_LEFT){ x = ARENA_FLOOR_LEFT - 125.0f; y = cy + off; tx = ARENA_FLOOR_LEFT + 55.0f; ty = y; dir = L3_RIGHT; }
		else if (entrance == L3_RIGHT){ x = ARENA_FLOOR_RIGHT + 15.0f; y = cy + off; tx = ARENA_FLOOR_RIGHT - 165.0f; ty = y; dir = L3_LEFT; }
		else if (entrance == L3_UP){ x = cx + off; y = ARENA_FLOOR_TOP + 20.0f; tx = x; ty = ARENA_FLOOR_TOP - 145.0f; dir = L3_DOWN; }
		else{ x = cx + off; y = ARENA_FLOOR_BOTTOM - 125.0f; tx = x; ty = ARENA_FLOOR_BOTTOM + 55.0f; dir = L3_UP; }
	}

	void configureArenaEnemy(L3ArenaEnemy &e, int wave){ e.wave = wave; e.moveSpeed = wave == 1 ? 2.35f : (wave == 2 ? 2.85f : 3.25f); e.maxHP = wave == 1 ? 320 : (wave == 2 ? 260 : 190); e.hp = e.maxHP; }
	void spawnArenaEnemy(int slot, int entrance, int wave){
		if (slot<0 || slot >= ARENA_ENEMY_COUNT)return; L3ArenaEnemy &e = arenaEnemy[slot]; float sx, sy, tx, ty; int dir; arenaDoorSpawn(entrance, slot, sx, sy, tx, ty, dir);
		e.active = e.alive = e.entering = true; e.attackDone = e.projectileDone = false; e.state = L3AE_ENTER; e.dir = e.actionDir = dir; e.stateTimer = e.animTimer = 0; e.attackCooldown = 0; e.powerCooldown = wave == 1 ? 105 : (wave == 2 ? 70 : 48); e.invincibleTimer = e.decisionCooldown = e.aiCycle = e.defenseCycle = 0; e.id = slot; e.x = sx; e.y = sy; e.entryX = tx; e.entryY = ty; e.vx = e.vy = 0; configureArenaEnemy(e, wave);
	}

	void startArenaWave(int wave){
		clearArenaEnemies(); clearArenaProjectiles(); clearArenaEffects(); arenaWave = wave; arenaWaveDelay = 0; arenaIntroTimer = 0; arenaFightStarted = false; applyWaveHeroUpgrade();
		if (wave == 1)spawnArenaEnemy(0, L3_LEFT, 1); else if (wave == 2){ spawnArenaEnemy(0, L3_LEFT, 2); spawnArenaEnemy(1, L3_RIGHT, 2); }
		else{ spawnArenaEnemy(0, L3_LEFT, 3); spawnArenaEnemy(1, L3_RIGHT, 3); spawnArenaEnemy(2, L3_UP, 3); spawnArenaEnemy(3, L3_DOWN, 3); }
	}
	int aliveArenaEnemies(){ int n = 0; for (int i = 0; i<ARENA_ENEMY_COUNT; i++)if (arenaEnemy[i].active&&arenaEnemy[i].alive)n++; return n; }
	bool allEnemiesEntered(){ for (int i = 0; i<ARENA_ENEMY_COUNT; i++)if (arenaEnemy[i].active&&arenaEnemy[i].entering)return false; return true; }
	int arenaEnemyTotalHP(){ int n = 0; for (int i = 0; i<ARENA_ENEMY_COUNT; i++)if (arenaEnemy[i].active&&arenaEnemy[i].alive)n += arenaEnemy[i].hp; return n; }
	int arenaEnemyTotalMaxHP(){ int n = 0; for (int i = 0; i<ARENA_ENEMY_COUNT; i++)if (arenaEnemy[i].active)n += arenaEnemy[i].maxHP; return n; }

	void damageArenaHero(NinjaPlayer &ninja, int raw, float sx, float sy){
		if (arenaHeroInvincible>0 || arenaHeroState == L3AH_KO)return; int damage = reduceHeroDamage(raw); arenaHeroHP -= damage; if (arenaHeroHP<0)arenaHeroHP = 0; arenaGainPower(4); arenaHeroState = arenaHeroHP <= 0 ? L3AH_KO : L3AH_HURT; arenaHeroTimer = 0; arenaHeroInvincible = 16; arenaCombo = arenaComboTimer = 0;
		float dx = arenaHeroCX(ninja) - sx, dy = arenaHeroCY(ninja) - sy, len = sqrtf(dx*dx + dy*dy); if (len<0.001f){ dx = 1; dy = 0; len = 1; }ninja.vx = dx / len*(arenaHeroHP <= 0 ? 4.0f : 2.2f); ninja.vy = dy / len*(arenaHeroHP <= 0 ? 4.0f : 2.2f);
	}

	void damageArenaEnemy(L3ArenaEnemy &e, int damage){
		if (!e.alive || e.invincibleTimer>0)return; if (e.wave == 2)damage = damage * 90 / 100; else if (e.wave == 3)damage = damage * 82 / 100; if (damage<1)damage = 1; e.hp -= damage; if (e.hp<0)e.hp = 0; e.state = L3AE_HURT; e.stateTimer = 0; e.invincibleTimer = e.wave == 3 ? 7 : 9;
		float dx, dy; dirVector(e.dir, dx, dy); e.vx = -dx*2.0f; e.vy = -dy*2.0f; if (e.hp <= 0){ e.state = L3AE_KO; e.stateTimer = 0; e.alive = false; e.vx = e.vy = 0; arenaScore += 700 + arenaWave * 180; addArenaEffect(imgArenaSmoke, e.x - 10, e.y + 20, 170, 100, 28); }
	}

	bool enemyInHeroAttack(L3ArenaEnemy &e, NinjaPlayer &ninja, float range){
		float hx = arenaHeroCX(ninja), hy = arenaHeroCY(ninja), ex = arenaEnemyCX(e), ey = arenaEnemyCY(e), dx = ex - hx, dy = ey - hy; int dir = arenaHeroActionDir;
		if (dir == L3_LEFT)return dx <= 0 && dx >= -range&&absF(dy) <= 58; if (dir == L3_RIGHT)return dx >= 0 && dx <= range&&absF(dy) <= 58; if (dir == L3_UP)return dy >= 0 && dy <= range&&absF(dx) <= 58; return dy <= 0 && dy >= -range&&absF(dx) <= 58;
	}

	void heroMeleeHit(NinjaPlayer &ninja, int damage, int hitScore, float range){
		int best = -1; float bestD = 9999; for (int i = 0; i<ARENA_ENEMY_COUNT; i++){ L3ArenaEnemy &e = arenaEnemy[i]; if (!e.active || !e.alive || !enemyInHeroAttack(e, ninja, range))continue; float d = distance(arenaHeroCX(ninja), arenaHeroCY(ninja), arenaEnemyCX(e), arenaEnemyCY(e)); if (d<bestD){ bestD = d; best = i; } }
		if (best<0)return; L3ArenaEnemy &e = arenaEnemy[best]; if (e.state == L3AE_BLOCK){ arenaScore += 12; arenaGainPower(4); addArenaEffect(imgArenaImpact, e.x + 25, e.y + 30, 65, 65, 7); return; }
		damageArenaEnemy(e, damage); arenaGainPower(12); addCombo(hitScore); addArenaEffect(imgArenaImpact, e.x + 24, e.y + 28, 72, 72, 8); if (arenaHeroState == L3AH_HEAVY)addArenaEffect(imgArenaCrescent, e.x - 10, e.y + 15, 130, 95, 10);
	}

	void spawnProjectileSingle(bool fromHero, int img, float x, float y, float vx, float vy, int w, int h, int damage){ for (int i = 0; i<ARENA_PROJECTILE_COUNT; i++)if (!arenaProjectile[i].active){ L3ArenaProjectile &p = arenaProjectile[i]; p.active = true; p.fromHero = fromHero; p.img[0] = img; p.frameCount = 1; p.animTimer = 0; p.x = x; p.y = y; p.vx = vx; p.vy = vy; p.w = w; p.h = h; p.damage = damage; return; } }
	void spawnProjectileAnimated(bool fromHero, float x, float y, float vx, float vy, int w, int h, int damage){ for (int i = 0; i<ARENA_PROJECTILE_COUNT; i++)if (!arenaProjectile[i].active){ L3ArenaProjectile &p = arenaProjectile[i]; p.active = true; p.fromHero = fromHero; for (int k = 0; k<4; k++)p.img[k] = imgHeroShuriken[k]; p.frameCount = 4; p.animTimer = 0; p.x = x; p.y = y; p.vx = vx; p.vy = vy; p.w = w; p.h = h; p.damage = damage; return; } }

	void spawnHeroArenaShuriken(NinjaPlayer &ninja){
		float dx, dy; dirVector(arenaHeroActionDir, dx, dy); bool powered = arenaPowerTimer>0; float speed = powered ? 11.5f : 8.5f, x = arenaHeroCX(ninja) - 25 + dx * 45, y = arenaHeroCY(ninja) - 25 + dy * 45;
		addArenaEffect(powered ? imgHeroPowerSlash : imgHeroSlash, x - 20, y - 15, 95, 80, 9, arenaHeroActionDir == L3_LEFT);
		if (powered)spawnProjectileSingle(true, imgHeroFastShuriken, x, y, dx*speed, dy*speed, 66, 42, heroDamage(26)); else spawnProjectileAnimated(true, x, y, dx*speed, dy*speed, 50, 50, heroDamage(18));
	}

	void spawnEnemyArenaPower(L3ArenaEnemy &e, NinjaPlayer &ninja){
		int type = e.aiCycle % 5, w = 108, h = 64; if (type == 1){ w = 118; h = 75; }
		else if (type == 2){ w = 120; h = 90; }
		else if (type == 3){ w = 118; h = 82; }
		else if (type == 4){ w = 125; h = 64; }
		float sx = arenaEnemyCX(e), sy = arenaEnemyCY(e), dx = arenaHeroCX(ninja) - sx, dy = arenaHeroCY(ninja) - sy, len = sqrtf(dx*dx + dy*dy); if (len<0.001f)len = 1; float speed = 5.0f + arenaWave*0.7f; int damage = 18 + type * 3; if (arenaWave == 2)damage += 10; else if (arenaWave == 3)damage += 20;
		spawnProjectileSingle(false, imgVillainProjectile[type], sx - w*0.5f, sy - h*0.5f, dx / len*speed, dy / len*speed, w, h, damage);
	}

	void startHeroPunch(){ if (!arenaHeroCanAct() || arenaAttackCooldown>0)return; arenaHeroState = L3AH_PUNCH; arenaHeroTimer = 0; arenaHeroActionDir = arenaHeroDir; arenaAttackCooldown = 14; arenaHeroAttackDone = false; }
	void startHeroKick(){ if (!arenaHeroCanAct() || arenaAttackCooldown>0)return; arenaHeroState = L3AH_KICK; arenaHeroTimer = 0; arenaHeroActionDir = arenaHeroDir; arenaAttackCooldown = 19; arenaHeroAttackDone = false; }
	void startHeroHeavy(){ if (!arenaHeroCanAct() || arenaAttackCooldown>0)return; arenaHeroState = L3AH_HEAVY; arenaHeroTimer = 0; arenaHeroActionDir = arenaHeroDir; arenaAttackCooldown = 27; arenaHeroAttackDone = false; }
	void startHeroThrow(){ if (!arenaHeroCanAct() || arenaThrowCooldown>0)return; arenaHeroState = L3AH_THROW; arenaHeroTimer = 0; arenaHeroActionDir = arenaHeroDir; arenaThrowCooldown = 38; arenaHeroThrowDone = false; }
	void startHeroDash(float dx, float dy){
		if (!arenaHeroCanAct() || arenaDashCooldown>0)return; if (dx == 0 && dy == 0)dirVector(arenaHeroDir, dx, dy); float len = sqrtf(dx*dx + dy*dy); if (len<0.001f)len = 1; arenaHeroState = L3AH_DASH; arenaHeroTimer = 0; arenaDashCooldown = 38; arenaDashTimer = 8; arenaDashVX = dx / len*10.0f; arenaDashVY = dy / len*10.0f; arenaHeroActionDir = arenaHeroDir;
	}

	void startEnemyPunch(L3ArenaEnemy &e){ e.state = L3AE_PUNCH; e.stateTimer = 0; e.actionDir = e.dir; e.attackDone = false; e.attackCooldown = e.wave == 1 ? 31 : (e.wave == 2 ? 22 : 16); e.vx = e.vy = 0; }
	void startEnemyKick(L3ArenaEnemy &e){ e.state = L3AE_KICK; e.stateTimer = 0; e.actionDir = e.dir; e.attackDone = false; e.attackCooldown = e.wave == 1 ? 36 : (e.wave == 2 ? 26 : 19); e.vx = e.vy = 0; }
	void startEnemySpin(L3ArenaEnemy &e){ e.state = L3AE_SPIN; e.stateTimer = 0; e.actionDir = e.dir; e.attackDone = false; e.attackCooldown = e.wave == 1 ? 42 : (e.wave == 2 ? 29 : 21); e.vx = e.vy = 0; }
	void startEnemyPower(L3ArenaEnemy &e){ e.state = L3AE_POWER; e.stateTimer = 0; e.actionDir = e.dir; e.attackDone = e.projectileDone = false; e.attackCooldown = e.wave == 1 ? 44 : (e.wave == 2 ? 30 : 22); e.powerCooldown = e.wave == 1 ? 115 : (e.wave == 2 ? 76 : 52); e.vx = e.vy = 0; }
	void startEnemyBlock(L3ArenaEnemy &e){ e.state = L3AE_BLOCK; e.stateTimer = 0; e.vx = e.vy = 0; }
	void startEnemyDodge(L3ArenaEnemy &e, NinjaPlayer &ninja){ e.state = L3AE_DODGE; e.stateTimer = 0; float dx = arenaEnemyCX(e) - arenaHeroCX(ninja), dy = arenaEnemyCY(e) - arenaHeroCY(ninja), len = sqrtf(dx*dx + dy*dy); if (len<0.001f){ dx = 1; dy = 0; len = 1; }e.vx = dx / len*4.0f; e.vy = dy / len*4.0f; e.dir = vectorDir(-dx, -dy); }
	void startEnemyLeap(L3ArenaEnemy &e, NinjaPlayer &ninja){ e.state = L3AE_LEAP; e.stateTimer = 0; e.actionDir = e.dir; e.attackDone = false; float dx = arenaHeroCX(ninja) - arenaEnemyCX(e), dy = arenaHeroCY(ninja) - arenaEnemyCY(e), len = sqrtf(dx*dx + dy*dy); if (len<0.001f)len = 1; e.vx = dx / len*2.6f; e.vy = dy / len*2.6f; }

	void villainMeleeHit(L3ArenaEnemy &e, NinjaPlayer &ninja, int damage, bool powerLike){
		float d = distance(arenaHeroCX(ninja), arenaHeroCY(ninja), arenaEnemyCX(e), arenaEnemyCY(e)); if (d>(powerLike ? 155.0f : 145.0f))return;
		if (arenaHeroState == L3AH_BLOCK){ arenaGainPower(powerLike ? 8 : 8); arenaScore += powerLike ? 25 : 45; if (powerLike)damageArenaHero(ninja, damage / 4, arenaEnemyCX(e), arenaEnemyCY(e)); return; }
		damageArenaHero(ninja, damage, arenaEnemyCX(e), arenaEnemyCY(e)); arenaScore -= 25; if (arenaScore<0)arenaScore = 0; addArenaEffect(imgArenaImpact, ninja.x + 15, ninja.y + 20, 72, 72, 8);
	}
	int villainPunchDamage(){ return arenaWave == 1 ? 18 : (arenaWave == 2 ? 27 : 34); }int villainKickDamage(){ return arenaWave == 1 ? 24 : (arenaWave == 2 ? 35 : 44); }int villainSpinDamage(){ return arenaWave == 1 ? 26 : (arenaWave == 2 ? 40 : 50); }int villainLeapDamage(){ return arenaWave == 1 ? 28 : (arenaWave == 2 ? 42 : 52); }

	void updateArenaHero(NinjaPlayer &ninja, bool left, bool right, bool up, bool down, bool punch, bool kick, bool heavy, bool block, bool shuriken, bool dash, bool power, bool heal){
		if (arenaAttackCooldown>0)arenaAttackCooldown--; if (arenaDashCooldown>0)arenaDashCooldown--; if (arenaThrowCooldown>0)arenaThrowCooldown--; if (arenaHeroInvincible>0)arenaHeroInvincible--; if (arenaPowerTimer>0)arenaPowerTimer--; if (arenaHealCooldown>0)arenaHealCooldown--;
		float dx = 0, dy = 0; if (left&&!right)dx = -1; else if (right&&!left)dx = 1; if (up&&!down)dy = 1; else if (down&&!up)dy = -1; if (dx != 0 && dy != 0){ dx *= 0.7071f; dy *= 0.7071f; }if (dy>0)arenaHeroDir = L3_UP; else if (dy<0)arenaHeroDir = L3_DOWN; else if (dx<0)arenaHeroDir = L3_LEFT; else if (dx>0)arenaHeroDir = L3_RIGHT; updatePlayerAnimation(dx, dy);
		if (power)activateArenaPower(); if (heal)arenaHeal();

		if (arenaHeroState == L3AH_KO){ arenaHeroTimer++; ninja.x += ninja.vx; ninja.y += ninja.vy; ninja.vx *= 0.92f; ninja.vy *= 0.92f; clampArenaHero(ninja); if (arenaHeroTimer >= 80){ arenaHeroLives--; if (arenaHeroLives <= 0){ arenaHeroLives = 0; finishArena(false); return; }arenaHeroHP = arenaHeroMaxHP; placePlayer(ninja, (float)(SCREEN_WIDTH / 2 - NinjaPlayer::DRAW_W / 2), 185.0f); arenaHeroState = L3AH_IDLE; arenaHeroDir = L3_DOWN; startArenaWave(arenaWave); }return; }
		if (arenaHeroState == L3AH_HURT){ ninja.x += ninja.vx; ninja.y += ninja.vy; ninja.vx *= 0.78f; ninja.vy *= 0.78f; arenaHeroTimer++; clampArenaHero(ninja); if (arenaHeroTimer >= 9){ arenaHeroState = L3AH_IDLE; arenaHeroTimer = 0; }return; }
		if (arenaHeroState == L3AH_DASH){ ninja.x += arenaDashVX; ninja.y += arenaDashVY; arenaDashVX *= 0.86f; arenaDashVY *= 0.86f; arenaHeroTimer++; clampArenaHero(ninja); if (arenaHeroTimer >= 8){ arenaHeroState = L3AH_IDLE; arenaHeroTimer = 0; }return; }
		if (arenaHeroState == L3AH_BLOCK){ if (block){ ninja.vx *= 0.25f; ninja.vy *= 0.25f; return; }arenaHeroState = L3AH_IDLE; }
		if (arenaHeroAttacking()){
			ninja.x += ninja.vx; ninja.y += ninja.vy; ninja.vx *= 0.48f; ninja.vy *= 0.48f; arenaHeroTimer++; int end = 12; if (arenaHeroState == L3AH_KICK)end = 15; else if (arenaHeroState == L3AH_HEAVY)end = 21; else if (arenaHeroState == L3AH_THROW)end = 18;
			if (arenaHeroState == L3AH_PUNCH&&arenaHeroTimer == 5 && !arenaHeroAttackDone){ arenaHeroAttackDone = true; heroMeleeHit(ninja, heroDamage(20), 90, 130); }
			if (arenaHeroState == L3AH_KICK&&arenaHeroTimer == 7 && !arenaHeroAttackDone){ arenaHeroAttackDone = true; heroMeleeHit(ninja, heroDamage(27), 130, 145); }
			if (arenaHeroState == L3AH_HEAVY&&arenaHeroTimer == 10 && !arenaHeroAttackDone){ arenaHeroAttackDone = true; heroMeleeHit(ninja, heroDamage(36), 190, 165); }
			if (arenaHeroState == L3AH_THROW&&arenaHeroTimer == 8 && !arenaHeroThrowDone){ arenaHeroThrowDone = true; spawnHeroArenaShuriken(ninja); }
			if (arenaHeroTimer >= end){ arenaHeroState = L3AH_IDLE; arenaHeroTimer = 0; }clampArenaHero(ninja); return;
		}

		if (punch)startHeroPunch(); else if (kick)startHeroKick(); else if (heavy)startHeroHeavy(); else if (shuriken)startHeroThrow(); if (dash)startHeroDash(dx, dy);
		if (arenaHeroState != L3AH_IDLE&&arenaHeroState != L3AH_MOVE)return;
		if (block){ arenaHeroState = L3AH_BLOCK; arenaHeroActionDir = arenaHeroDir; ninja.vx *= 0.25f; ninja.vy *= 0.25f; return; }
		float accel = 0.65f, maxSpeed = 4.0f; if (dx != 0 || dy != 0){ ninja.vx += dx*accel; ninja.vy += dy*accel; float speed = sqrtf(ninja.vx*ninja.vx + ninja.vy*ninja.vy); if (speed>maxSpeed){ ninja.vx = ninja.vx / speed*maxSpeed; ninja.vy = ninja.vy / speed*maxSpeed; }arenaHeroState = L3AH_MOVE; }
		else{ ninja.vx *= 0.62f; ninja.vy *= 0.62f; if (absF(ninja.vx)<0.08f)ninja.vx = 0; if (absF(ninja.vy)<0.08f)ninja.vy = 0; arenaHeroState = (ninja.vx != 0 || ninja.vy != 0) ? L3AH_MOVE : L3AH_IDLE; }
		ninja.x += ninja.vx; ninja.y += ninja.vy; clampArenaHero(ninja); ninja.grounded = true;
	}

	void updateEnemyEntrance(L3ArenaEnemy &e){
		float dx = e.entryX - e.x, dy = e.entryY - e.y, d = sqrtf(dx*dx + dy*dy); if (d <= 4){ e.x = e.entryX; e.y = e.entryY; e.vx = e.vy = 0; e.entering = false; e.state = L3AE_IDLE; e.attackCooldown = 24; return; }e.state = L3AE_ENTER; e.dir = vectorDir(dx, dy); e.vx = dx / d*e.moveSpeed; e.vy = dy / d*e.moveSpeed; e.x += e.vx; e.y += e.vy;
	}

	void updateArenaEnemy(L3ArenaEnemy &e, NinjaPlayer &ninja){
		if (!e.active)return; if (e.invincibleTimer>0)e.invincibleTimer--; if (e.attackCooldown>0)e.attackCooldown--; if (e.powerCooldown>0)e.powerCooldown--; if (e.decisionCooldown>0)e.decisionCooldown--; e.animTimer++;
		if (e.entering){ updateEnemyEntrance(e); return; }if (e.state == L3AE_KO){ e.stateTimer++; return; }
		if (e.state == L3AE_HURT){ e.x += e.vx; e.y += e.vy; e.vx *= 0.78f; e.vy *= 0.78f; e.stateTimer++; clampArenaEnemyInside(e); if (e.stateTimer >= 8){ e.state = L3AE_IDLE; e.stateTimer = 0; e.vx = e.vy = 0; e.decisionCooldown = 8; }return; }
		if (e.state == L3AE_BLOCK){ if (++e.stateTimer >= 11){ e.state = L3AE_IDLE; e.stateTimer = 0; e.decisionCooldown = 6; }return; }
		if (e.state == L3AE_DODGE){ e.x += e.vx; e.y += e.vy; e.vx *= 0.82f; e.vy *= 0.82f; clampArenaEnemyInside(e); if (++e.stateTimer >= 8){ e.state = L3AE_IDLE; e.stateTimer = 0; e.vx = e.vy = 0; e.decisionCooldown = 10; }return; }
		if (e.state == L3AE_LEAP){ e.x += e.vx; e.y += e.vy; clampArenaEnemyInside(e); e.stateTimer++; if (e.stateTimer == 10 && !e.attackDone){ e.attackDone = true; villainMeleeHit(e, ninja, villainLeapDamage(), false); }if (e.stateTimer >= 18){ e.state = L3AE_IDLE; e.stateTimer = 0; e.vx = e.vy = 0; e.decisionCooldown = 8; }return; }
		if (e.state == L3AE_PUNCH || e.state == L3AE_KICK || e.state == L3AE_POWER || e.state == L3AE_SPIN){
			e.stateTimer++; float step = 0; if (e.state == L3AE_PUNCH&&e.stateTimer >= 4 && e.stateTimer <= 8)step = 1.15f; else if (e.state == L3AE_KICK&&e.stateTimer >= 5 && e.stateTimer <= 11)step = 1.45f; else if (e.state == L3AE_SPIN&&e.stateTimer >= 5 && e.stateTimer <= 13)step = 1.20f; if (step>0){ float dx, dy; dirVector(e.actionDir, dx, dy); e.x += dx*step; e.y += dy*step; clampArenaEnemyInside(e); }
			if (e.state == L3AE_PUNCH&&e.stateTimer == 8 && !e.attackDone){ e.attackDone = true; villainMeleeHit(e, ninja, villainPunchDamage(), false); }if (e.state == L3AE_KICK&&e.stateTimer == 10 && !e.attackDone){ e.attackDone = true; villainMeleeHit(e, ninja, villainKickDamage(), false); }if (e.state == L3AE_SPIN&&e.stateTimer == 11 && !e.attackDone){ e.attackDone = true; villainMeleeHit(e, ninja, villainSpinDamage(), true); addArenaEffect(imgArenaCrescent, e.x - 35, e.y + 5, 175, 120, 13, e.actionDir == L3_LEFT); }if (e.state == L3AE_POWER&&e.stateTimer == 18 && !e.projectileDone){ e.projectileDone = true; spawnEnemyArenaPower(e, ninja); }
			int end = 18; if (e.state == L3AE_KICK)end = 20; else if (e.state == L3AE_POWER)end = 34; else if (e.state == L3AE_SPIN)end = 24; if (e.stateTimer >= end){ e.state = L3AE_IDLE; e.stateTimer = 0; e.vx = e.vy = 0; e.decisionCooldown = e.wave == 3 ? 5 : 9; }return;
		}
		if (!arenaFightStarted){ e.state = L3AE_IDLE; e.vx = e.vy = 0; return; }

		float dx = arenaHeroCX(ninja) - arenaEnemyCX(e), dy = arenaHeroCY(ninja) - arenaEnemyCY(e), d = sqrtf(dx*dx + dy*dy); e.dir = vectorDir(dx, dy); const float ATTACK_RANGE = 150.0f;
		if (d>ATTACK_RANGE){ e.state = L3AE_CHASE; float len = d; if (len<0.001f)len = 1; e.vx = dx / len*e.moveSpeed; e.vy = dy / len*e.moveSpeed; e.x += e.vx; e.y += e.vy; clampArenaEnemyInside(e); return; }
		e.state = L3AE_IDLE; e.vx = e.vy = 0; if (e.decisionCooldown>0 || e.attackCooldown>0)return;
		if (arenaHeroAttacking()){ e.defenseCycle++; if (e.wave >= 2 && e.defenseCycle % 5 == 0){ startEnemyDodge(e, ninja); return; }if (e.defenseCycle % 3 == 0){ startEnemyBlock(e); return; } }
		e.aiCycle++; if (e.wave == 1){ if (e.powerCooldown <= 0 && e.aiCycle % 5 == 0)startEnemyPower(e); else if (e.aiCycle % 3 == 2)startEnemyKick(e); else startEnemyPunch(e); }
		else if (e.wave == 2){ if (e.powerCooldown <= 0 && e.aiCycle % 4 == 0)startEnemyPower(e); else if (e.aiCycle % 5 == 1)startEnemySpin(e); else if (e.aiCycle % 5 == 2)startEnemyKick(e); else if (e.aiCycle % 5 == 4)startEnemyLeap(e, ninja); else startEnemyPunch(e); }
		else{ if (e.powerCooldown <= 0 && e.aiCycle % 3 == 0)startEnemyPower(e); else if (e.aiCycle % 5 == 1)startEnemySpin(e); else if (e.aiCycle % 5 == 2)startEnemyKick(e); else if (e.aiCycle % 5 == 3)startEnemyLeap(e, ninja); else startEnemyPunch(e); }
	}

	void separateArenaEnemies(){ for (int i = 0; i<ARENA_ENEMY_COUNT; i++)for (int j = i + 1; j<ARENA_ENEMY_COUNT; j++){ L3ArenaEnemy &a = arenaEnemy[i], &b = arenaEnemy[j]; if (!a.active || !a.alive || !b.active || !b.alive || a.entering || b.entering)continue; float dx = arenaEnemyCX(a) - arenaEnemyCX(b), dy = arenaEnemyCY(a) - arenaEnemyCY(b), d = sqrtf(dx*dx + dy*dy); if (d<70 && d>0.001f){ float push = (70 - d)*0.14f; dx /= d; dy /= d; a.x += dx*push; a.y += dy*push; b.x -= dx*push; b.y -= dy*push; clampArenaEnemyInside(a); clampArenaEnemyInside(b); } } }

	void updateArenaProjectiles(NinjaPlayer &ninja){
		for (int i = 0; i<ARENA_PROJECTILE_COUNT; i++){
			L3ArenaProjectile &p = arenaProjectile[i]; if (!p.active)continue; p.x += p.vx; p.y += p.vy; p.animTimer++; if (p.x<-160 || p.x>SCREEN_WIDTH + 160 || p.y<-160 || p.y>SCREEN_HEIGHT + 160){ p.active = false; continue; }
			if (p.fromHero){ for (int e = 0; e<ARENA_ENEMY_COUNT; e++){ L3ArenaEnemy &v = arenaEnemy[e]; if (!v.active || !v.alive)continue; if (absF((p.x + p.w*.5f) - arenaEnemyCX(v))<55 && absF((p.y + p.h*.5f) - arenaEnemyCY(v))<55){ if (v.state == L3AE_BLOCK){ arenaScore += 12; arenaGainPower(4); } else{ damageArenaEnemy(v, p.damage); arenaGainPower(8); arenaScore += 75; addArenaEffect(arenaPowerTimer>0 ? imgHeroPowerHit : imgHeroHit, v.x + 18, v.y + 28, arenaPowerTimer>0 ? 88 : 70, arenaPowerTimer>0 ? 88 : 70, 9); addArenaEffect(imgArenaSmoke, v.x + 28, v.y + 15, 70, 55, 10); }p.active = false; break; } } }
			else if (absF((p.x + p.w*.5f) - arenaHeroCX(ninja))<45 && absF((p.y + p.h*.5f) - arenaHeroCY(ninja))<45){ if (arenaHeroState == L3AH_BLOCK){ damageArenaHero(ninja, p.damage / 5, p.x, p.y); arenaGainPower(10); arenaScore += 25; } else{ damageArenaHero(ninja, p.damage, p.x, p.y); arenaScore -= 20; if (arenaScore<0)arenaScore = 0; }addArenaEffect(imgArenaGroundBurst, ninja.x - 5, ninja.y - 5, 110, 100, 12); p.active = false; }
		}
	}

	void finishArena(bool win){ arenaFinished = true; arenaWon = win; arenaResultTimer = 0; clearArenaProjectiles(); if (win){ finalScore = currentDisplayScore() + 1800 + arenaHeroLives * 550 + arenaHealCharges * 120; if (arenaHeroLives == 3)finalScore += 900; earnedStars = arenaHeroLives >= 3 ? 3 : (arenaHeroLives == 2 ? 2 : 1); resultReady = true; } else{ finalScore = currentDisplayScore(); earnedStars = 0; resultReady = false; } }

	void updateArena(NinjaPlayer &ninja, bool left, bool right, bool up, bool down, bool punch, bool kick, bool heavy, bool block, bool shuriken, bool dash, bool power, bool heal){
		if (arenaFinished){ arenaResultTimer++; if (arenaResultTimer>125)returnRequested = true; return; }globalTick++; if (arenaComboTimer>0 && --arenaComboTimer == 0)arenaCombo = 0; updateArenaEffects();
		if (arenaWave == 0){ startArenaWave(1); return; }
		if (!arenaFightStarted){ ninja.vx = ninja.vy = 0; arenaHeroState = L3AH_IDLE; for (int i = 0; i<ARENA_ENEMY_COUNT; i++)if (arenaEnemy[i].active)updateArenaEnemy(arenaEnemy[i], ninja); if (allEnemiesEntered()){ arenaIntroTimer++; if (arenaIntroTimer >= 22)arenaFightStarted = true; }return; }
		if (aliveArenaEnemies() == 0){ arenaWaveDelay++; ninja.vx = ninja.vy = 0; arenaHeroState = L3AH_IDLE; if (arenaWaveDelay >= 80){ if (arenaWave<3){ arenaHeroHP += 15; if (arenaHeroHP>arenaHeroMaxHP)arenaHeroHP = arenaHeroMaxHP; startArenaWave(arenaWave + 1); } else finishArena(true); }return; }

		updateArenaHero(ninja, left, right, up, down, punch, kick, heavy, block, shuriken, dash, power, heal); if (arenaFinished)return;
		for (int i = 0; i<ARENA_ENEMY_COUNT; i++)if (arenaEnemy[i].active&&arenaEnemy[i].alive)updateArenaEnemy(arenaEnemy[i], ninja); separateArenaEnemies(); updateArenaProjectiles(ninja);
	}

	void update(NinjaPlayer &ninja, bool left, bool right, bool up, bool down, bool punchPressed = false, bool kickPressed = false, bool heavyPressed = false, bool blockHeld = false, bool shurikenPressed = false, bool dashPressed = false, bool powerPressed = false, bool healPressed = false){
		if (arenaMode){ updateArena(ninja, left, right, up, down, punchPressed, kickPressed, heavyPressed, blockHeld, shurikenPressed, dashPressed, powerPressed, healPressed); return; }
		float dx = 0, dy = 0; if (left&&!right)dx = -1; else if (right&&!left)dx = 1; if (up&&!down)dy = 1; else if (down&&!up)dy = -1; if (dx != 0 && dy != 0){ dx *= 0.7071f; dy *= 0.7071f; }
		ninja.vx = dx*moveSpeed; ninja.vy = dy*moveSpeed; if (dx<0)ninja.facingRight = false; else if (dx>0)ninja.facingRight = true; ninja.state = (dx != 0 || dy != 0) ? NINJA_RUN : NINJA_IDLE; updatePlayerAnimation(dx, dy); moveAxis(ninja, ninja.vx, true); moveAxis(ninja, ninja.vy, false);
		globalTick++; if (invincibleTimer>0)invincibleTimer--; updateTraps(ninja); updatePuzzlesAndPickups(ninja); if (arenaMode)return;
		if (left&&heroInExit(exitArea[currentArea][L3_LEFT], ninja)){ changeArea(exitArea[currentArea][L3_LEFT], ninja); return; }if (right&&heroInExit(exitArea[currentArea][L3_RIGHT], ninja)){ changeArea(exitArea[currentArea][L3_RIGHT], ninja); return; }if (up&&heroInExit(exitArea[currentArea][L3_UP], ninja)){ changeArea(exitArea[currentArea][L3_UP], ninja); return; }if (down&&heroInExit(exitArea[currentArea][L3_DOWN], ninja)){ changeArea(exitArea[currentArea][L3_DOWN], ninja); return; }
	}

	bool isArenaMode(){ return arenaMode; }
	bool isPortalActive(){ return portalActive; }
	bool hasResultToSubmit(){ return resultReady&&!resultSubmitted; }
	void markResultSubmitted(){ resultSubmitted = true; }
	bool shouldReturnToLevelSelect(){ return returnRequested; }

	void showMirrored(int x, int y, int w, int h, int img, bool right){
		if (right){ iShowImage(x, y, w, h, img); return; }
		glPushMatrix(); glTranslatef((float)(x + w), 0.0f, 0.0f); glScalef(-1.0f, 1.0f, 1.0f); iShowImage(0, y, w, h, img); glPopMatrix();
	}

	void drawPlayer(NinjaPlayer &ninja){
		if (arenaMode){ drawArenaHero(ninja); return; }
		bool moving = (ninja.vx != 0 || ninja.vy != 0);
		if (ninja.vy>0.01f){ iShowImage((int)ninja.x, (int)ninja.y, NinjaPlayer::DRAW_W, NinjaPlayer::DRAW_H, imgUp[verticalFrame]); return; }
		if (ninja.vy<-0.01f){ iShowImage((int)ninja.x, (int)ninja.y, NinjaPlayer::DRAW_W, NinjaPlayer::DRAW_H, imgDown[verticalFrame]); return; }
		if (!moving&&lastMoveDir == L3_UP){ iShowImage((int)ninja.x, (int)ninja.y, NinjaPlayer::DRAW_W, NinjaPlayer::DRAW_H, imgUp[0]); return; }
		if (!moving&&lastMoveDir == L3_DOWN){ iShowImage((int)ninja.x, (int)ninja.y, NinjaPlayer::DRAW_W, NinjaPlayer::DRAW_H, imgDown[0]); return; }
		if (lastMoveDir == L3_LEFT)ninja.facingRight = false; else if (lastMoveDir == L3_RIGHT)ninja.facingRight = true; ninja.draw(0);
	}

	void drawTraps(){
		if (arenaMode)return;
		for (int i = 0; i<trapCount; i++){
			L3Trap &t = trap[i]; if (t.map != currentArea || !t.revealed)continue;
			float drawX = t.x; if (t.type == L3_TRAP_BLADE)drawX += sinf((t.cycleTimer + t.phase)*0.08f)*12.0f;
			if (t.type == L3_TRAP_SPIKE){ if (t.active)iShowImage((int)drawX, (int)t.y, (int)t.w, (int)t.h, t.img); else iShowImage((int)drawX, (int)t.y, (int)t.w, (int)(t.h*0.42f), t.img); }
			else if (t.type == L3_TRAP_BLADE)iShowImage((int)drawX, (int)t.y, (int)t.w, (int)t.h, t.img);
			else if (t.active)iShowImage((int)drawX, (int)t.y, (int)t.w, (int)t.h, t.img);
		}
	}

	void drawPuzzlesAndPickups(){
		if (arenaMode)return;
		for (int i = 0; i<PUZZLE_COUNT; i++){
			L3Puzzle &q = puzzle[i]; if (q.map != currentArea)continue; iShowImage((int)q.x - 34, (int)q.y - 42, 68, 84, imgPuzzle);
			if (q.solved){ iSetColor(120, 230, 120); iText((int)q.x - 28, (int)q.y - 56, (char*)"SOLVED", GLUT_BITMAP_HELVETICA_12); }
			else { iSetColor(245, 190, 100); iText((int)q.x - 35, (int)q.y - 56, (char*)"APPROACH", GLUT_BITMAP_HELVETICA_12); }
		}
		for (int i = 0; i<pickupCount; i++){
			L3Pickup &p = pickup[i]; if (p.taken || p.map != currentArea || p.type == L3_PICK_STAR)continue;
			if (p.type == L3_PICK_CHEST)iShowImage((int)p.x - 32, (int)p.y - 28, 64, 56, p.img); else iShowImage((int)p.x - 34, (int)p.y - 28, 68, 56, p.img);
		}

		// IMPORTANT: after all 3 puzzles, the same t-10 exit portal appears on EVERY map.
		if (portalActive){
			int x = (int)portalX[currentArea], y = (int)portalY[currentArea];
			iShowImage(x - 58, y - 68, 116, 136, imgPortal); iSetColor(185, 255, 185); iText(x - 62, y - 84, (char*)"ENTER ARENA", GLUT_BITMAP_HELVETICA_12);
		}
	}

	void drawCheck(int x, int y, bool done){
		iSetColor(230, 220, 180); iRectangle(x, y, 22, 22);
		if (done){ iSetColor(70, 220, 110); iLine(x + 4, y + 11, x + 9, y + 5); iLine(x + 9, y + 5, x + 18, y + 18); }
	}

	void drawCollision(){
		if (!showCollisionDebug || arenaMode)return;
		iSetColor(60, 220, 90); for (int i = 0; i<walkCount[currentArea]; i++){ L3Rect &r = walkArea[currentArea][i]; iRectangle((int)r.x, (int)r.y, (int)r.w, (int)r.h); }
		iSetColor(235, 65, 65); for (int i = 0; i<blockCount[currentArea]; i++){ L3Rect &r = blockArea[currentArea][i]; iRectangle((int)r.x, (int)r.y, (int)r.w, (int)r.h); }
		iSetColor(255, 220, 80); for (int d = 0; d<4; d++){ L3Exit &e = exitArea[currentArea][d]; if (e.target >= 0)iRectangle((int)e.trigger.x, (int)e.trigger.y, (int)e.trigger.w, (int)e.trigger.h); }
	}

	int verticalVillainMoveImage(int dir, int frame){ if (dir == L3_UP){ int s[4] = { 0, 1, 2, 4 }; return imgVillainVertical[s[frame % 4]]; }int s[4] = { 5, 6, 7, 8 }; return imgVillainVertical[s[frame % 4]]; }
	int verticalVillainAttackImage(int dir, int frame){ if (dir == L3_UP){ int s[3] = { 3, 9, 11 }; return imgVillainVertical[s[frame % 3]]; }int s[3] = { 10, 12, 13 }; return imgVillainVertical[s[frame % 3]]; }
	int heroTimedImage(int *arr, int count, int total, int timer){ int f = timer*count / total; if (f<0)f = 0; if (f >= count)f = count - 1; return arr[f]; }

	void drawArenaHero(NinjaPlayer &ninja){
		if (arenaHeroInvincible>0 && (arenaHeroInvincible / 2) % 2 == 0 && arenaHeroState != L3AH_KO)return; int x = (int)ninja.x, y = (int)ninja.y, dir = arenaHeroAttacking() || arenaHeroState == L3AH_DASH ? arenaHeroActionDir : arenaHeroDir;
		if (dir == L3_UP || dir == L3_DOWN){
			int img = dir == L3_UP ? imgUp[(arenaHeroState == L3AH_MOVE) ? verticalFrame : 0] : imgDown[(arenaHeroState == L3AH_MOVE) ? verticalFrame : 0]; iShowImage(x, y, NinjaPlayer::DRAW_W, NinjaPlayer::DRAW_H, img);
			if (arenaHeroState == L3AH_BLOCK){ iSetColor(150, 100, 220); iCircle(x + 45, y + 45, 50); }if (arenaPowerTimer>0){ iSetColor(190, 150, 255); iCircle(x + 45, y + 45, 58); }return;
		}
		bool right = dir == L3_RIGHT; int img = right ? ninja.imgIdleRight : ninja.imgIdleLeft;
		if (arenaHeroState == L3AH_PUNCH)img = heroTimedImage(imgArenaHeroPunch, 6, 12, arenaHeroTimer); else if (arenaHeroState == L3AH_KICK)img = heroTimedImage(imgArenaHeroKick, 5, 15, arenaHeroTimer); else if (arenaHeroState == L3AH_HEAVY)img = heroTimedImage(imgArenaHeroHeavy, 6, 21, arenaHeroTimer); else if (arenaHeroState == L3AH_BLOCK)img = imgArenaHeroBlock[(globalTick / 5) % 4]; else if (arenaHeroState == L3AH_THROW)img = heroTimedImage(imgArenaHeroThrow, 8, 18, arenaHeroTimer); else if (arenaHeroState == L3AH_HURT)img = heroTimedImage(imgArenaHeroHurt, 5, 9, arenaHeroTimer); else if (arenaHeroState == L3AH_KO)img = heroTimedImage(imgArenaHeroKO, 5, 22, arenaHeroTimer); else if (arenaHeroState == L3AH_DASH)img = heroTimedImage(imgArenaHeroDash, 6, 8, arenaHeroTimer); else if (arenaHeroState == L3AH_MOVE)img = right ? ninja.runRight[(globalTick / 3) % 2] : ninja.runLeft[(globalTick / 3) % 2];
		if (right)iShowImage(x, y, NinjaPlayer::DRAW_W, NinjaPlayer::DRAW_H, img); else if (img == ninja.imgIdleLeft || img == ninja.runLeft[0] || img == ninja.runLeft[1])iShowImage(x, y, NinjaPlayer::DRAW_W, NinjaPlayer::DRAW_H, img); else showMirrored(x, y, NinjaPlayer::DRAW_W, NinjaPlayer::DRAW_H, img, false);
		if (arenaPowerTimer>0){ iSetColor(190, 150, 255); iCircle(x + 45, y + 45, 58); }
	}

	int horizontalVillainImage(L3ArenaEnemy &e){
		if (e.state == L3AE_KO)return imgVillainKO; if (e.state == L3AE_HURT)return imgVillainHurt[(e.stateTimer / 3) % 3]; if (e.state == L3AE_BLOCK)return imgVillainBlock[(e.stateTimer / 4) % 3]; if (e.state == L3AE_DODGE)return imgVillainDodge[(e.stateTimer / 2) % 4]; if (e.state == L3AE_SPIN)return imgVillainSpin[(e.stateTimer / 4) % 3];
		if (e.state == L3AE_LEAP){ int f = e.stateTimer<4 ? 0 : (e.stateTimer<8 ? 1 : (e.stateTimer<12 ? 2 : (e.stateTimer<16 ? 3 : 4))); return imgVillainLeap[f]; }
		if (e.state == L3AE_PUNCH){ int f = e.stateTimer<4 ? 0 : (e.stateTimer<8 ? 1 : (e.stateTimer<12 ? 2 : (e.stateTimer<15 ? 3 : 4))); return imgVillainPunch[f]; }
		if (e.state == L3AE_KICK){ int f = e.stateTimer<5 ? 0 : (e.stateTimer<10 ? 1 : (e.stateTimer<15 ? 2 : 3)); return imgVillainKick[f]; }
		if (e.state == L3AE_POWER){ int f = e.stateTimer<5 ? 0 : (e.stateTimer<9 ? 1 : (e.stateTimer<13 ? 3 : (e.stateTimer<17 ? 4 : (e.stateTimer<21 ? 6 : (e.stateTimer<27 ? 2 : 7))))); return imgVillainPower[f]; }
		if (e.state == L3AE_ENTER || e.state == L3AE_CHASE)return imgVillainRun[(e.animTimer / 4) % 6]; return imgVillainIdle;
	}

	void drawArenaEnemy(L3ArenaEnemy &e){
		if (!e.active)return; int x = (int)e.x, y = (int)e.y, w = 110, h = 110, dir = (e.state == L3AE_PUNCH || e.state == L3AE_KICK || e.state == L3AE_POWER || e.state == L3AE_SPIN || e.state == L3AE_LEAP) ? e.actionDir : e.dir;
		if (e.state == L3AE_KO){ showMirrored(x, y, w, h, imgVillainKO, dir != L3_LEFT); return; }
		if (dir == L3_UP || dir == L3_DOWN){ int img = (e.state == L3AE_ENTER || e.state == L3AE_CHASE || e.state == L3AE_IDLE) ? verticalVillainMoveImage(dir, (e.animTimer / 4) % 4) : verticalVillainAttackImage(dir, e.stateTimer / 4); iShowImage(x, y, w, h, img); }
		else showMirrored(x, y, w, h, horizontalVillainImage(e), dir == L3_RIGHT);
		if (e.invincibleTimer>0){ iSetColor(245, 210, 100); iRectangle(x + 8, y + 6, w - 16, h - 12); }
	}

	void drawArenaProjectiles(){ for (int i = 0; i<ARENA_PROJECTILE_COUNT; i++){ L3ArenaProjectile &p = arenaProjectile[i]; if (!p.active)continue; int img = p.img[p.frameCount <= 1 ? 0 : (p.animTimer / 2) % p.frameCount]; iShowImage((int)p.x, (int)p.y, p.w, p.h, img); } }
	void drawArenaEffects(){ for (int i = 0; i<ARENA_EFFECT_COUNT; i++){ L3ArenaEffect &e = arenaEffect[i]; if (!e.active)continue; if (!e.mirror)iShowImage((int)e.x, (int)e.y, e.w, e.h, e.img); else{ glPushMatrix(); glTranslatef(e.x + e.w, 0, 0); glScalef(-1, 1, 1); iShowImage(0, (int)e.y, e.w, e.h, e.img); glPopMatrix(); } } }

	void drawArenaHUD(){
		int totalHP = arenaEnemyTotalHP(), totalMax = arenaEnemyTotalMaxHP(), displayScore = currentDisplayScore(); float hr = arenaHeroMaxHP>0 ? (float)arenaHeroHP / arenaHeroMaxHP : 0, er = totalMax>0 ? (float)totalHP / totalMax : 0; hr = clampF(hr, 0, 1); er = clampF(er, 0, 1);
		iSetColor(180, 38, 42); iFilledRectangle(45, 548, (int)(245 * hr), 12); iSetColor(120, 48, 185); iFilledRectangle(734, 548, (int)(245 * er), 12);
		iShowImage(18, 536, 300, 38, imgArenaHUD[1]); iShowImage(402, 538, 220, 38, imgArenaHUD[2]); iShowImage(706, 536, 300, 38, imgArenaHUD[3]); iShowImage(25, 570, 118, 26, imgArenaHUD[4]); iShowImage(806, 570, 185, 25, imgArenaHUD[6]); iShowImage(451, 570, 112, 25, imgArenaHUD[5]); iShowImage(25, 505, 145, 25, imgArenaHUD[7]); iShowImage(824, 505, 150, 25, imgArenaHUD[8]);
		char t[80]; iSetColor(250, 230, 170); sprintf_s(t, sizeof(t), "%d", displayScore); iText(575, 574, t, GLUT_BITMAP_HELVETICA_18); iSetColor(245, 235, 210); sprintf_s(t, sizeof(t), "%d", arenaHeroLives); iText(176, 510, t, GLUT_BITMAP_HELVETICA_18); sprintf_s(t, sizeof(t), "%d", aliveArenaEnemies()); iText(982, 510, t, GLUT_BITMAP_HELVETICA_18);
		iShowImage(22, 474, 122, 25, imgArenaHUD[13]); iSetColor(28, 22, 35); iFilledRectangle(148, 480, 145, 11); iSetColor(130, 55, 185); iFilledRectangle(150, 482, (int)(141 * (arenaPowerMeter / 100.0f)), 7); iShowImage(309, 474, 118, 25, imgArenaHUD[14]); sprintf_s(t, sizeof(t), "%d", arenaHealCharges); iSetColor(210, 245, 180); iText(433, 479, t, GLUT_BITMAP_HELVETICA_18);
		if (arenaWave == 1)iShowImage(455, 472, 120, 27, imgArenaHUD[15]); else if (arenaWave == 2)iShowImage(455, 472, 120, 27, imgArenaHUD[16]); else if (arenaWave >= 3)iShowImage(455, 472, 120, 27, imgArenaHUD[17]);
		if (!arenaFightStarted&&arenaWave>0 && allEnemiesEntered())iShowImage(424, 390, 176, 55, imgArenaHUD[18]); if (arenaCombo >= 2 && arenaComboTimer>0){ sprintf_s(t, sizeof(t), "%d HIT COMBO", arenaCombo); iSetColor(235, 235, 235); iText(465, 450, t, GLUT_BITMAP_HELVETICA_12); }
		if (arenaPowerTimer>0){ iSetColor(210, 170, 255); iText(455, 445, (char*)"POWER ACTIVE", GLUT_BITMAP_HELVETICA_12); }
		if (arenaFinished){ iSetColor(10, 10, 16); iFilledRectangle(320, 245, 385, 95); iSetColor(245, 220, 145); iText(arenaWon ? 390 : 445, 305, arenaWon ? (char*)"LEVEL 3 COMPLETE" : (char*)"DEFEATED", GLUT_BITMAP_TIMES_ROMAN_24); sprintf_s(t, sizeof(t), "FINAL SCORE: %d   STARS: %d", finalScore, earnedStars); iText(385, 275, t, GLUT_BITMAP_HELVETICA_18); }
	}

	void drawArena(NinjaPlayer &ninja){
		iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, arenaImage); for (int i = 0; i<ARENA_ENEMY_COUNT; i++)if (arenaEnemy[i].active&&arenaEnemy[i].y>ninja.y)drawArenaEnemy(arenaEnemy[i]); drawArenaHero(ninja); for (int i = 0; i<ARENA_ENEMY_COUNT; i++)if (arenaEnemy[i].active&&arenaEnemy[i].y <= ninja.y)drawArenaEnemy(arenaEnemy[i]); drawArenaProjectiles(); drawArenaEffects(); drawArenaHUD();
	}

	void drawHUD(){
		if (!showHUD)return;
		if (arenaMode){ drawArenaHUD(); return; }
		char line[100], hpText[30], lootText[40]; sprintf_s(line, sizeof(line), "LEVEL 3   MAP %d/10   %s", currentArea + 1, getAreaName());
		sprintf_s(hpText, sizeof(hpText), "HP: %d/5", playerHP); sprintf_s(lootText, sizeof(lootText), "TREASURE: %d", treasureCount);
		iSetColor(10, 10, 16); iFilledRectangle(16, SCREEN_HEIGHT - 68, 600, 52); iSetColor(245, 220, 155); iText(26, SCREEN_HEIGHT - 39, line, GLUT_BITMAP_HELVETICA_18);
		iSetColor(225, 225, 225); iText(26, SCREEN_HEIGHT - 57, (char*)"WASD / ARROWS MOVE   SOLVE 3 PUZZLES   R RESET   ESC BACK", GLUT_BITMAP_HELVETICA_12);
		iSetColor(245, 220, 155); iText(645, SCREEN_HEIGHT - 35, hpText, GLUT_BITMAP_HELVETICA_18); iText(735, SCREEN_HEIGHT - 35, lootText, GLUT_BITMAP_HELVETICA_18);
		iText(645, SCREEN_HEIGHT - 57, (char*)"PUZZLES", GLUT_BITMAP_HELVETICA_12); drawCheck(715, SCREEN_HEIGHT - 61, puzzle[0].solved); drawCheck(745, SCREEN_HEIGHT - 61, puzzle[1].solved); drawCheck(775, SCREEN_HEIGHT - 61, puzzle[2].solved);
		if (portalActive){ iSetColor(180, 255, 180); iText(820, SCREEN_HEIGHT - 57, (char*)"PORTAL OPEN", GLUT_BITMAP_HELVETICA_12); }
	}

	void draw(NinjaPlayer &ninja){
		if (arenaMode){ drawArena(ninja); return; }
		iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, areaImage[currentArea]); drawTraps(); drawPuzzlesAndPickups(); drawPlayer(ninja); drawCollision(); drawHUD();
	}
};

#endif
