#ifndef LEVEL2_H
#define LEVEL2_H

#include <cstdio>


#define L2_SHURIKEN_1 "Images\\Weapons\\Shuriken\\s-01.png"
#define L2_SHURIKEN_2 "Images\\Weapons\\Shuriken\\s-02.png"
#define L2_SHURIKEN_3 "Images\\Weapons\\Shuriken\\s-03.png"
#define L2_SHURIKEN_4 "Images\\Weapons\\Shuriken\\s-04.png"

#define L2_SHURIKEN_SLASH       "Images\\Weapons\\Shuriken\\s-05.png"
#define L2_SHURIKEN_HIT         "Images\\Weapons\\Shuriken\\s-06.png"
#define L2_SHURIKEN_SMOKE       "Images\\Weapons\\Shuriken\\s-07.png"
#define L2_SHURIKEN_POWER_SLASH "Images\\Weapons\\Shuriken\\s-08.png"
#define L2_SHURIKEN_POWER_HIT   "Images\\Weapons\\Shuriken\\s-09.png"
#define L2_SHURIKEN_FAST        "Images\\Weapons\\Shuriken\\s-10.png"

enum Level2HeroState {
	L2H_IDLE,
	L2H_RUN,
	L2H_JUMP,
	L2H_DASH,
	L2H_PUNCH,
	L2H_KICK,
	L2H_HEAVY,
	L2H_BLOCK,
	L2H_THROW,
	L2H_HURT,
	L2H_KO
};

enum Level2VillainState {
	L2V_IDLE,
	L2V_RUN,
	L2V_PUNCH,
	L2V_KICK,
	L2V_POWER,
	L2V_BLOCK,
	L2V_HURT,
	L2V_KO,
	L2V_LEAP,
	L2V_DODGE,
	L2V_SPIN
};

struct L2Effect {
	bool active;
	int image, timer, life, w, h;
	float x, y;
	bool mirror;

	void clear() {
		active = false;
		image = timer = life = w = h = 0;
		x = y = 0;
		mirror = false;
	}

	void start(int img, float px, float py, int width, int height, int duration, bool flip = false) {
		active = true;
		image = img;
		x = px;
		y = py;
		w = width;
		h = height;
		life = duration;
		timer = 0;
		mirror = flip;
	}

	void update() {
		if (!active) return;
		timer++;
		if (timer >= life) active = false;
	}

	void draw() {
		if (!active) return;

		if (!mirror) {
			iShowImage((int)x, (int)y, w, h, image);
			return;
		}

		glPushMatrix();
		glTranslatef((float)((int)x + w), 0.0f, 0.0f);
		glScalef(-1.0f, 1.0f, 1.0f);
		iShowImage(0, (int)y, w, h, image);
		glPopMatrix();
	}
};

struct L2Projectile {
	bool active, fromHero;
	float x, y, vx;

	int image[4];
	int frameCount, animTimer;
	int w, h, damage;

	void clear() {
		active = false;
		fromHero = false;
		x = y = vx = 0;

		for (int i = 0; i < 4; i++) image[i] = 0;

		frameCount = animTimer = 0;
		w = h = damage = 0;
	}

	void spawnSingle(bool heroShot, float px, float py, float speedX,
		int imageID, int width, int height, int dmg) {

		clear();

		active = true;
		fromHero = heroShot;

		x = px;
		y = py;
		vx = speedX;

		image[0] = imageID;
		frameCount = 1;

		w = width;
		h = height;
		damage = dmg;
	}

	void spawnAnimated(bool heroShot, float px, float py, float speedX,
		int img1, int img2, int img3, int img4,
		int width, int height, int dmg) {

		clear();

		active = true;
		fromHero = heroShot;

		x = px;
		y = py;
		vx = speedX;

		image[0] = img1;
		image[1] = img2;
		image[2] = img3;
		image[3] = img4;
		frameCount = 4;

		w = width;
		h = height;
		damage = dmg;
	}

	void update() {
		if (!active) return;

		x += vx;
		animTimer++;

		if (x < -160 || x > SCREEN_WIDTH + 160) active = false;
	}

	int getImage() {
		if (frameCount <= 1) return image[0];
		return image[(animTimer / 2) % frameCount];
	}

	void draw() {
		if (!active) return;

		int currentImage = getImage();

		if (vx >= 0) {
			iShowImage((int)x, (int)y, w, h, currentImage);
			return;
		}

		glPushMatrix();
		glTranslatef((float)((int)x + w), 0.0f, 0.0f);
		glScalef(-1.0f, 1.0f, 1.0f);
		iShowImage(0, (int)y, w, h, currentImage);
		glPopMatrix();
	}
};


struct Level2HeroFighter {
	static const int GROUND_Y = 145;
	static const int DRAW_W = 128;
	static const int DRAW_H = 128;

	static const int IDLE_COUNT = 1;
	static const int RUN_COUNT = 4;
	static const int JUMP_COUNT = 10;
	static const int DASH_COUNT = 6;

	static const int PUNCH_COUNT = 6;
	static const int KICK_COUNT = 5;
	static const int HEAVY_COUNT = 6;
	static const int BLOCK_COUNT = 4;

	static const int THROW_COUNT = 8;
	static const int HURT_COUNT = 5;
	static const int KO_COUNT = 5;

	float x, y, vx, vy;

	int hp, maxHP, lives;

	bool grounded;
	bool facingRight;
	bool jumpFacingRight;

	int state, stateTimer, animTimer;
	int attackCooldown, dashCooldown, throwCooldown, invincibleTimer;

	bool attackHitDone;
	bool throwReleaseDone;

	int powerMeter, powerTimer;
	int healCharges, healCooldown;

	int damageReductionPercent;
	int damageBonusPercent;

	int heroFrame[65];

	int idleSeq[IDLE_COUNT];
	int runSeq[RUN_COUNT];
	int jumpSeq[JUMP_COUNT];
	int dashSeq[DASH_COUNT];

	int punchSeq[PUNCH_COUNT];
	int kickSeq[KICK_COUNT];
	int heavySeq[HEAVY_COUNT];
	int blockSeq[BLOCK_COUNT];

	int throwSeq[THROW_COUNT];
	int hurtSeq[HURT_COUNT];
	int koSeq[KO_COUNT];

	void loadAllHeroFrames() {
		char fileName[100];

		for (int i = 1; i <= 64; i++) {
			sprintf_s(
				fileName,
				sizeof(fileName),
				"Images\\Player\\c-%02d.png",
				i
				);

			heroFrame[i] = iLoadImage(fileName);
		}
	}

	void buildAnimationGroups() {


		int idleTemp[IDLE_COUNT] = {
			45
		};

		int runTemp[RUN_COUNT] = {
			1, 2, 3, 4
		};

		int jumpTemp[JUMP_COUNT] = {
			5, 6, 7, 8, 9,
			10, 12, 13, 14, 54
		};

		int dashTemp[DASH_COUNT] = {
			33, 34, 35, 36, 43, 44
		};

		int punchTemp[PUNCH_COUNT] = {
			45, 46, 47, 48, 50, 52
		};

		int kickTemp[KICK_COUNT] = {
			49, 51, 53, 54, 55
		};

		int heavyTemp[HEAVY_COUNT] = {
			55, 56, 57, 58, 59, 60
		};

		int blockTemp[BLOCK_COUNT] = {
			61, 63, 64, 63
		};

		int throwTemp[THROW_COUNT] = {
			25, 26, 27, 28, 29, 30, 31, 34
		};

		int hurtTemp[HURT_COUNT] = {
			40, 41, 44, 62, 63
		};

		int koTemp[KO_COUNT] = {
			11, 23, 40, 42, 43
		};

		for (int i = 0; i < IDLE_COUNT; i++) idleSeq[i] = idleTemp[i];
		for (int i = 0; i < RUN_COUNT; i++) runSeq[i] = runTemp[i];
		for (int i = 0; i < JUMP_COUNT; i++) jumpSeq[i] = jumpTemp[i];
		for (int i = 0; i < DASH_COUNT; i++) dashSeq[i] = dashTemp[i];

		for (int i = 0; i < PUNCH_COUNT; i++) punchSeq[i] = punchTemp[i];
		for (int i = 0; i < KICK_COUNT; i++) kickSeq[i] = kickTemp[i];
		for (int i = 0; i < HEAVY_COUNT; i++) heavySeq[i] = heavyTemp[i];
		for (int i = 0; i < BLOCK_COUNT; i++) blockSeq[i] = blockTemp[i];

		for (int i = 0; i < THROW_COUNT; i++) throwSeq[i] = throwTemp[i];
		for (int i = 0; i < HURT_COUNT; i++) hurtSeq[i] = hurtTemp[i];
		for (int i = 0; i < KO_COUNT; i++) koSeq[i] = koTemp[i];
	}

	void load() {
		loadAllHeroFrames();
		buildAnimationGroups();
	}

	void reset() {
		x = 300.0f;
		y = (float)GROUND_Y;

		vx = vy = 0;

		maxHP = 140;
		hp = maxHP;
		lives = 3;

		grounded = true;

		facingRight = true;
		jumpFacingRight = true;

		state = L2H_IDLE;
		stateTimer = animTimer = 0;

		attackCooldown = dashCooldown = throwCooldown = invincibleTimer = 0;

		attackHitDone = false;
		throwReleaseDone = false;

		powerMeter = powerTimer = 0;

		healCharges = 3;
		healCooldown = 0;

		damageReductionPercent = 20;
		damageBonusPercent = 0;
	}

	void resetRound(bool fullHP = true) {
		x = 300.0f;
		y = (float)GROUND_Y;

		vx = vy = 0;

		if (fullHP) hp = maxHP;

		grounded = true;

		facingRight = true;
		jumpFacingRight = true;

		state = L2H_IDLE;
		stateTimer = animTimer = 0;

		attackCooldown = dashCooldown = throwCooldown = invincibleTimer = 0;

		attackHitDone = false;
		throwReleaseDone = false;

		powerTimer = 0;
		healCooldown = 0;
	}

	void applyPhaseUpgrade(int phase) {
		if (phase == 1) damageBonusPercent = 0;
		else if (phase == 2) damageBonusPercent = 8;
		else damageBonusPercent = 12;
	}

	void faceOpponent(float opponentCenterX) {
		/*
		Level 2 hero does NOT auto-turn toward the villain.
		The player sprite faces the direction the player is actually moving.

		RIGHT movement -> face RIGHT
		LEFT movement  -> face LEFT

		This function is intentionally kept for compatibility with the stage
		code, but it does not overwrite player-controlled facing.
		*/
	}

	void faceMovementDirection() {
		if (vx > 0.10f) facingRight = true;
		else if (vx < -0.10f) facingRight = false;
	}

	bool isAttacking() {
		return
			state == L2H_PUNCH ||
			state == L2H_KICK ||
			state == L2H_HEAVY ||
			state == L2H_THROW;
	}

	bool canAct() {
		return
			state != L2H_HURT &&
			state != L2H_KO &&
			state != L2H_DASH &&
			!isAttacking();
	}

	void startPunch() {
		if (!canAct() || attackCooldown > 0) return;

		state = L2H_PUNCH;
		stateTimer = 0;

		attackCooldown = 14;
		attackHitDone = false;

		vx *= 0.25f;
	}

	void startKick() {
		if (!canAct() || attackCooldown > 0) return;

		state = L2H_KICK;
		stateTimer = 0;

		attackCooldown = 19;
		attackHitDone = false;

		vx *= 0.25f;
	}

	void startHeavy() {
		if (!canAct() || attackCooldown > 0) return;

		state = L2H_HEAVY;
		stateTimer = 0;

		attackCooldown = 27;
		attackHitDone = false;

		vx *= 0.20f;
	}

	void startThrow() {
		if (!canAct() || throwCooldown > 0) return;

		state = L2H_THROW;
		stateTimer = 0;

		throwCooldown = 38;
		throwReleaseDone = false;

		vx *= 0.20f;
	}

	void startDash() {
		if (!canAct() || dashCooldown > 0 || !grounded) return;

		state = L2H_DASH;
		stateTimer = 0;
		dashCooldown = 38;

		vx = facingRight ? 7.0f : -7.0f;
		faceMovementDirection();
	}

	void gainPower(int amount) {
		if (powerTimer > 0) return;

		powerMeter += amount;

		if (powerMeter > 100)
			powerMeter = 100;
	}

	bool activatePower() {
		if (
			powerMeter < 100 ||
			powerTimer > 0 ||
			state == L2H_KO ||
			state == L2H_HURT
			)
			return false;

		powerMeter = 0;
		powerTimer = 165;

		return true;
	}

	int modifyDamage(int damage) {
		int result =
			damage +
			damage * damageBonusPercent / 100;

		if (powerTimer > 0)
			result = result * 135 / 100;

		return result;
	}

	bool heal() {
		if (
			healCharges <= 0 ||
			healCooldown > 0 ||
			hp <= 0 ||
			hp >= maxHP
			)
			return false;

		if (
			state == L2H_KO ||
			state == L2H_HURT ||
			isAttacking()
			)
			return false;

		hp += 40;

		if (hp > maxHP)
			hp = maxHP;

		healCharges--;
		healCooldown = 180;

		return true;
	}

	int reduceIncomingDamage(int damage) {
		int result =
			damage *
			(100 - damageReductionPercent) /
			100;

		if (result < 1)
			result = 1;

		return result;
	}

	void takeHit(int rawDamage) {
		if (
			invincibleTimer > 0 ||
			state == L2H_KO
			)
			return;

		int damage =
			reduceIncomingDamage(rawDamage);

		hp -= damage;

		if (hp < 0)
			hp = 0;

		gainPower(4);

		state = L2H_HURT;
		stateTimer = 0;

		invincibleTimer = 16;

		vx = facingRight ? -2.2f : 2.2f;

		if (hp <= 0) {
			state = L2H_KO;
			stateTimer = 0;

			vx = facingRight ? -4.0f : 4.0f;
			vy = 4.5f;

			grounded = false;

			powerTimer = 0;
		}
	}

	void beginJump(bool leftHeld, bool rightHeld) {
		if (!grounded) return;


		if (rightHeld && !leftHeld) facingRight = true;
		else if (leftHeld && !rightHeld) facingRight = false;

		jumpFacingRight = facingRight;

		if (rightHeld && !leftHeld) vx = 3.2f;
		else if (leftHeld && !rightHeld) vx = -3.2f;

		vy = 13.3f;
		grounded = false;

		state = L2H_JUMP;
		stateTimer = 0;
	}

	void update(
		bool leftHeld,
		bool rightHeld,
		bool jumpPressed,
		bool dashPressed,
		bool blockHeld,
		bool controlsEnabled
		) {

		if (attackCooldown > 0) attackCooldown--;
		if (dashCooldown > 0) dashCooldown--;
		if (throwCooldown > 0) throwCooldown--;
		if (invincibleTimer > 0) invincibleTimer--;
		if (powerTimer > 0) powerTimer--;
		if (healCooldown > 0) healCooldown--;

		animTimer++;

		// -------------------- KO --------------------
		if (state == L2H_KO) {
			vy -= 1.0f;

			x += vx;
			y += vy;

			vx *= 0.92f;

			if (y <= GROUND_Y) {
				y = (float)GROUND_Y;
				vy = 0;
				grounded = true;
			}

			stateTimer++;
			return;
		}

		if (state == L2H_HURT) {
			x += vx;
			vx *= 0.78f;

			stateTimer++;

			if (stateTimer >= 9) {
				state = grounded ? L2H_IDLE : L2H_JUMP;
				stateTimer = 0;
			}
		}

		else if (state == L2H_DASH) {
			x += vx;
			faceMovementDirection();
			vx *= 0.86f;

			stateTimer++;

			if (stateTimer >= 8) {
				state = grounded ? L2H_IDLE : L2H_JUMP;
				stateTimer = 0;
			}
		}

		else if (isAttacking()) {
			x += vx;
			vx *= 0.48f;

			stateTimer++;

			int endTime = 12;

			if (state == L2H_KICK) endTime = 15;
			else if (state == L2H_HEAVY) endTime = 21;
			else if (state == L2H_THROW) endTime = 18;

			if (stateTimer >= endTime) {
				state = grounded ? L2H_IDLE : L2H_JUMP;
				stateTimer = 0;
			}
		}

		// -------------------- NORMAL CONTROL --------------------
		else {
			if (!controlsEnabled) {
				// Hero remains still during villain entrance.
				vx *= 0.45f;

				if (grounded)
					state = L2H_IDLE;
			}

			else if (blockHeld && grounded) {
				state = L2H_BLOCK;
				vx *= 0.25f;
			}

			else {
				float groundAcceleration = 0.65f;
				float airAcceleration = 0.24f;

				float groundMaxSpeed = 4.0f;
				float airMaxSpeed = 3.8f;

				if (grounded) {
					if (leftHeld && !rightHeld) {
						vx -= groundAcceleration;

						if (vx < -groundMaxSpeed)
							vx = -groundMaxSpeed;

						facingRight = false;
					}

					else if (rightHeld && !leftHeld) {
						vx += groundAcceleration;

						if (vx > groundMaxSpeed)
							vx = groundMaxSpeed;

						facingRight = true;
					}

					else {
						// Strong friction removes slippery movement.
						vx *= 0.62f;

						if (vx > -0.08f && vx < 0.08f)
							vx = 0;
					}

					if (jumpPressed)
						beginJump(leftHeld, rightHeld);

					if (grounded)
						state =
						(vx > 0.30f || vx < -0.30f) ?
					L2H_RUN :
							L2H_IDLE;
				}

				else {

					if (leftHeld && !rightHeld) {
						vx -= airAcceleration;

						if (vx < -airMaxSpeed)
							vx = -airMaxSpeed;

						facingRight = false;
						jumpFacingRight = false;
					}

					else if (rightHeld && !leftHeld) {
						vx += airAcceleration;

						if (vx > airMaxSpeed)
							vx = airMaxSpeed;

						facingRight = true;
						jumpFacingRight = true;
					}

					else {
						vx *= 0.985f;
					}

					state = L2H_JUMP;
				}
			}

			x += vx;
		}

		// -------------------- JUMP / GRAVITY --------------------
		if (!grounded) {
			vy -= 0.98f;
			y += vy;

			if (y <= GROUND_Y) {
				y = (float)GROUND_Y;

				vy = 0;
				grounded = true;

				facingRight = jumpFacingRight;

				if (
					!isAttacking() &&
					state != L2H_HURT &&
					state != L2H_KO
					)
					state = L2H_IDLE;
			}

			else if (
				!isAttacking() &&
				state != L2H_HURT &&
				state != L2H_KO
				)
				state = L2H_JUMP;
		}

		// -------------------- ARENA BOUNDS --------------------
		if (x < 150) {
			x = 150;

			if (vx < 0)
				vx = 0;
		}

		if (x > 745) {
			x = 745;

			if (vx > 0)
				vx = 0;
		}


		if (grounded && state != L2H_KO) {
			y = (float)GROUND_Y;
			vy = 0;
		}
	}

	int sequenceImage(int *seq, int count, int speed) {
		if (count <= 0) return heroFrame[15];

		int index =
			(animTimer / speed) %
			count;

		return heroFrame[
			seq[index]
		];
	}

	int timedSequenceImage(int *seq, int count, int totalStateTime) {
		if (count <= 0) return heroFrame[15];

		int frame =
			stateTimer *
			count /
			totalStateTime;

		if (frame < 0) frame = 0;
		if (frame >= count) frame = count - 1;

		return heroFrame[
			seq[frame]
		];
	}

	int getImage() {
		if (state == L2H_KO)
			return timedSequenceImage(koSeq, KO_COUNT, 22);

		if (state == L2H_HURT)
			return timedSequenceImage(hurtSeq, HURT_COUNT, 9);

		if (state == L2H_BLOCK)
			return sequenceImage(blockSeq, BLOCK_COUNT, 5);

		if (state == L2H_DASH)
			return timedSequenceImage(dashSeq, DASH_COUNT, 8);

		if (state == L2H_JUMP) {

			int frame = 0;

			if (vy > 9.0f) frame = 0;
			else if (vy > 6.0f) frame = 2;
			else if (vy > 2.0f) frame = 4;
			else if (vy > -2.0f) frame = 5;
			else if (vy > -6.0f) frame = 7;
			else frame = 9;

			if (frame >= JUMP_COUNT)
				frame = JUMP_COUNT - 1;

			return heroFrame[jumpSeq[frame]];
		}

		if (state == L2H_THROW)
			return timedSequenceImage(throwSeq, THROW_COUNT, 18);

		if (state == L2H_PUNCH)
			return timedSequenceImage(punchSeq, PUNCH_COUNT, 12);

		if (state == L2H_KICK)
			return timedSequenceImage(kickSeq, KICK_COUNT, 15);

		if (state == L2H_HEAVY)
			return timedSequenceImage(heavySeq, HEAVY_COUNT, 21);

		if (state == L2H_RUN)
			return sequenceImage(runSeq, RUN_COUNT, 3);

		return heroFrame[idleSeq[0]];
	}

	bool getRenderFacing() {


		if (state == L2H_JUMP || !grounded)
			return jumpFacingRight;

		return facingRight;
	}

	void showFacingImage(int px, int py, int image) {
		bool renderRight = getRenderFacing();

		// Hero source set is treated as right-facing.
		if (renderRight) {
			iShowImage(px, py, DRAW_W, DRAW_H, image);
			return;
		}

		glPushMatrix();
		glTranslatef((float)(px + DRAW_W), 0.0f, 0.0f);
		glScalef(-1.0f, 1.0f, 1.0f);

		iShowImage(0, py, DRAW_W, DRAW_H, image);

		glPopMatrix();
	}

	void draw() {
		if (
			invincibleTimer > 0 &&
			(invincibleTimer / 2) % 2 == 0 &&
			state != L2H_KO
			)
			return;

		int drawY = (int)y;

		if (state == L2H_KICK || state == L2H_HEAVY)
			drawY -= 3;

		showFacingImage(
			(int)x,
			drawY,
			getImage()
			);

		if (powerTimer > 0) {
			iSetColor(190, 150, 255);

			iCircle(
				(int)x + DRAW_W / 2,
				drawY + DRAW_H / 2,
				58
				);
		}
	}
};

struct Level2VillainFighter {
	static const int GROUND_Y = 135;
	static const int DRAW_W = 150;
	static const int DRAW_H = 150;

	float x, y, vx, vy;

	int hp, maxHP;

	bool grounded;
	bool facingRight;
	bool actionFacingRight;
	bool enteringArena;

	int state, stateTimer, animTimer;

	int attackCooldown;
	int powerCooldown;
	int invincibleTimer;
	int decisionCooldown;

	int aiCycle;
	int defenseCycle;

	bool attackHitDone;
	bool projectileReleaseDone;

	int phase;
	float moveSpeed;

	int imgIdle[4];
	int imgRun[6];

	int imgPunch[5];
	int imgKick[4];

	int imgBlock[3];
	int imgHurt[3];
	int imgLeap[5];
	int imgDodge[4];

	int imgSpin[3];
	int imgPowerPose[8];

	int imgProjectile[5];
	int imgKO;

	void load() {
		/*
		Keep one known idle orientation and a short coherent walk/run cycle.
		The old idle sequence mixed l2-1, l2-11, l2-24 and l2-27, which made
		the villain appear to turn around while standing still.
		*/
		imgIdle[0] = iLoadImage("Images\\Enemies\\Level2\\l2-1.png");
		imgIdle[1] = imgIdle[0];
		imgIdle[2] = imgIdle[0];
		imgIdle[3] = imgIdle[0];

		imgRun[0] = iLoadImage("Images\\Enemies\\Level2\\l2-2.png");
		imgRun[1] = iLoadImage("Images\\Enemies\\Level2\\l2-3.png");
		imgRun[2] = imgRun[0];
		imgRun[3] = imgRun[1];
		imgRun[4] = imgRun[0];
		imgRun[5] = imgRun[1];

		imgPunch[0] = iLoadImage("Images\\Enemies\\Level2\\l2-4.png");
		imgPunch[1] = iLoadImage("Images\\Enemies\\Level2\\l2-13.png");
		imgPunch[2] = iLoadImage("Images\\Enemies\\Level2\\l2-25.png");
		imgPunch[3] = iLoadImage("Images\\Enemies\\Level2\\l2-29.png");
		imgPunch[4] = iLoadImage("Images\\Enemies\\Level2\\l2-38.png");

		imgKick[0] = iLoadImage("Images\\Enemies\\Level2\\l2-6.png");
		imgKick[1] = iLoadImage("Images\\Enemies\\Level2\\l2-15.png");
		imgKick[2] = iLoadImage("Images\\Enemies\\Level2\\l2-26.png");
		imgKick[3] = iLoadImage("Images\\Enemies\\Level2\\l2-34.png");

		imgBlock[0] = iLoadImage("Images\\Enemies\\Level2\\l2-8.png");
		imgBlock[1] = iLoadImage("Images\\Enemies\\Level2\\l2-18.png");
		imgBlock[2] = iLoadImage("Images\\Enemies\\Level2\\l2-30.png");

		imgHurt[0] = iLoadImage("Images\\Enemies\\Level2\\l2-17.png");
		imgHurt[1] = iLoadImage("Images\\Enemies\\Level2\\l2-24.png");
		imgHurt[2] = iLoadImage("Images\\Enemies\\Level2\\l2-32.png");

		imgDodge[0] = iLoadImage("Images\\Enemies\\Level2\\l2-19.png");
		imgDodge[1] = iLoadImage("Images\\Enemies\\Level2\\l2-20.png");
		imgDodge[2] = iLoadImage("Images\\Enemies\\Level2\\l2-28.png");
		imgDodge[3] = iLoadImage("Images\\Enemies\\Level2\\l2-33.png");

		imgLeap[0] = iLoadImage("Images\\Enemies\\Level2\\l2-21.png");
		imgLeap[1] = iLoadImage("Images\\Enemies\\Level2\\l2-22.png");
		imgLeap[2] = iLoadImage("Images\\Enemies\\Level2\\l2-23.png");
		imgLeap[3] = iLoadImage("Images\\Enemies\\Level2\\l2-26.png");
		imgLeap[4] = iLoadImage("Images\\Enemies\\Level2\\l2-40.png");

		imgSpin[0] = iLoadImage("Images\\Enemies\\Level2\\l2-35.png");
		imgSpin[1] = iLoadImage("Images\\Enemies\\Level2\\l2-39.png");
		imgSpin[2] = iLoadImage("Images\\Enemies\\Level2\\l2-40.png");

		imgPowerPose[0] = iLoadImage("Images\\Enemies\\Level2\\l2-5.png");
		imgPowerPose[1] = iLoadImage("Images\\Enemies\\Level2\\l2-7.png");
		imgPowerPose[2] = iLoadImage("Images\\Enemies\\Level2\\l2-14.png");
		imgPowerPose[3] = iLoadImage("Images\\Enemies\\Level2\\l2-36.png");
		imgPowerPose[4] = iLoadImage("Images\\Enemies\\Level2\\l2-37.png");
		imgPowerPose[5] = iLoadImage("Images\\Enemies\\Level2\\l2-38.png");
		imgPowerPose[6] = iLoadImage("Images\\Enemies\\Level2\\l2-41.png");
		imgPowerPose[7] = iLoadImage("Images\\Enemies\\Level2\\l2-30.png");

		imgProjectile[0] = iLoadImage("Images\\Enemies\\Level2\\l2-42.png");
		imgProjectile[1] = iLoadImage("Images\\Enemies\\Level2\\l2-43.png");
		imgProjectile[2] = iLoadImage("Images\\Enemies\\Level2\\l2-44.png");
		imgProjectile[3] = iLoadImage("Images\\Enemies\\Level2\\l2-45.png");
		imgProjectile[4] = iLoadImage("Images\\Enemies\\Level2\\l2-46.png");

		imgKO = iLoadImage("Images\\Enemies\\Level2\\l2-21.png");
	}

	void configurePhase(int p) {
		phase = p;


		if (phase == 1) {
			maxHP = 320;
			moveSpeed = 2.35f;
		}

		else if (phase == 2) {
			maxHP = 520;
			moveSpeed = 2.85f;
		}

		else {
			maxHP = 760;
			moveSpeed = 3.25f;
		}

		hp = maxHP;
	}

	void reset() {
		x = 720.0f;
		y = (float)GROUND_Y;

		vx = vy = 0;

		grounded = true;
		facingRight = false;
		actionFacingRight = false;
		enteringArena = true;

		state = L2V_RUN;

		stateTimer = animTimer = 0;

		attackCooldown = 0;
		powerCooldown = 90;
		invincibleTimer = 0;
		decisionCooldown = 0;

		aiCycle = defenseCycle = 0;

		attackHitDone = false;
		projectileReleaseDone = false;

		configurePhase(1);
	}

	void resetForPhase(int p) {
		x = 720.0f;
		y = (float)GROUND_Y;

		vx = vy = 0;

		grounded = true;
		facingRight = false;
		actionFacingRight = false;
		enteringArena = true;

		state = L2V_RUN;

		stateTimer = animTimer = 0;

		attackCooldown = 0;

		powerCooldown =
			p == 1 ?
			105 :
			(
			p == 2 ?
			70 :
			48
			);

		invincibleTimer = 0;
		decisionCooldown = 0;

		attackHitDone = false;
		projectileReleaseDone = false;

		configurePhase(p);
	}

	void faceOpponent(float heroCenterX) {


		if (!grounded || state == L2V_LEAP) return;

		float myCenter = x + DRAW_W * 0.5f;
		facingRight = heroCenterX > myCenter;
	}

	void faceMovementDirection() {

	}

	bool isAttacking() {
		return
			state == L2V_PUNCH ||
			state == L2V_KICK ||
			state == L2V_POWER ||
			state == L2V_LEAP ||
			state == L2V_SPIN;
	}

	void lockActionFacing() {

		actionFacingRight = facingRight;
	}

	bool getRenderFacing() {
		if (
			state == L2V_PUNCH ||
			state == L2V_KICK ||
			state == L2V_POWER ||
			state == L2V_SPIN ||
			state == L2V_LEAP
			)
			return actionFacingRight;

		return facingRight;
	}

	void startPunch() {
		state = L2V_PUNCH;
		stateTimer = 0;

		lockActionFacing();
		attackHitDone = false;

		attackCooldown =
			phase == 1 ?
			31 :
			(
			phase == 2 ?
			22 :
			16
			);

		vx = 0;
	}

	void startKick() {
		state = L2V_KICK;
		stateTimer = 0;

		lockActionFacing();
		attackHitDone = false;

		attackCooldown =
			phase == 1 ?
			36 :
			(
			phase == 2 ?
			26 :
			19
			);

		vx = 0;
	}

	void startSpin() {
		state = L2V_SPIN;
		stateTimer = 0;

		lockActionFacing();

		attackHitDone = false;

		attackCooldown =
			phase == 1 ?
			42 :
			(
			phase == 2 ?
			29 :
			21
			);

		vx = 0;
	}

	void startPower(int type) {
		if (type < 0) type = 0;
		if (type > 4) type = 4;

		state = L2V_POWER;
		stateTimer = 0;

		lockActionFacing();

		attackHitDone = false;
		projectileReleaseDone = false;

		attackCooldown =
			phase == 1 ?
			44 :
			(
			phase == 2 ?
			30 :
			22
			);

		powerCooldown =
			phase == 1 ?
			115 :
			(
			phase == 2 ?
			76 :
			52
			);

		aiCycle = type;

		vx = 0;
	}

	int powerType() {
		return aiCycle % 5;
	}

	void startBlock() {
		state = L2V_BLOCK;
		stateTimer = 0;

		vx = 0;
	}

	void startDodge(float heroCenterX) {
		state = L2V_DODGE;
		stateTimer = 0;

		float myCenter =
			x +
			DRAW_W *
			0.5f;

		vx =
			myCenter >
			heroCenterX ?
			4.0f :
			-4.0f;
	}

	void startLeap(float heroCenterX) {
		faceOpponent(heroCenterX);
		actionFacingRight = facingRight;

		state = L2V_LEAP;
		stateTimer = 0;

		grounded = false;
		vy = 10.2f;

		float myCenter =
			x +
			DRAW_W *
			0.5f;

		vx =
			heroCenterX >
			myCenter ?
			2.6f :
			-2.6f;

		attackHitDone = false;
	}

	void takeHit(int damage) {
		if (
			invincibleTimer > 0 ||
			state == L2V_KO
			)
			return;

		// Later phases resist part of the hero damage.
		// This stretches Phase 2 and especially Phase 3 without
		// making the hero feel completely ineffective.
		if (phase == 2) damage = damage * 90 / 100;
		else if (phase == 3) damage = damage * 82 / 100;

		if (damage < 1) damage = 1;

		hp -= damage;

		if (hp < 0)
			hp = 0;

		state = L2V_HURT;
		stateTimer = 0;

		invincibleTimer =
			phase == 3 ?
			7 :
			9;

		vx =
			facingRight ?
			-2.0f :
			2.0f;

		if (hp <= 0) {
			state = L2V_KO;
			stateTimer = 0;

			vy = 4.0f;
			grounded = false;
		}
	}

	void updateAI(
		Level2HeroFighter &hero,
		bool fightStarted
		) {

		if (attackCooldown > 0) attackCooldown--;
		if (powerCooldown > 0) powerCooldown--;
		if (invincibleTimer > 0) invincibleTimer--;
		if (decisionCooldown > 0) decisionCooldown--;

		animTimer++;

		float heroCenter =
			hero.x +
			Level2HeroFighter::DRAW_W *
			0.5f;

		float myCenter =
			x +
			DRAW_W *
			0.5f;

		// Do not force opponent-facing before movement.
		// Moving states will face their actual velocity direction.

		// -------------------- ENTRANCE --------------------
		if (enteringArena) {
			state = L2V_RUN;

			vx = -moveSpeed;
			x += vx;
			faceOpponent(heroCenter);

			if (x <= 675.0f) {
				x = 625.0f;

				vx = 0;

				state = L2V_IDLE;
				enteringArena = false;

				attackCooldown = 24;
			}

			y = (float)GROUND_Y;

			return;
		}

		if (state == L2V_KO) {
			vy -= 1.0f;

			x += vx;
			y += vy;

			vx *= 0.92f;

			if (y <= GROUND_Y) {
				y = (float)GROUND_Y;

				vy = 0;
				grounded = true;
			}

			stateTimer++;

			return;
		}

		// -------------------- HURT --------------------
		if (state == L2V_HURT) {
			x += vx;
			faceMovementDirection();
			vx *= 0.78f;

			stateTimer++;

			if (stateTimer >= 8) {
				state = L2V_IDLE;

				stateTimer = 0;
				vx = 0;

				decisionCooldown = 8;
			}

			y = (float)GROUND_Y;

			return;
		}

		// -------------------- BLOCK --------------------
		if (state == L2V_BLOCK) {
			stateTimer++;

			if (stateTimer >= 11) {
				state = L2V_IDLE;

				stateTimer = 0;
				decisionCooldown = 6;
			}

			y = (float)GROUND_Y;

			return;
		}

		// -------------------- DODGE --------------------
		if (state == L2V_DODGE) {
			x += vx;
			faceOpponent(heroCenter);
			vx *= 0.82f;

			stateTimer++;

			if (stateTimer >= 8) {
				state = L2V_IDLE;

				stateTimer = 0;
				vx = 0;

				decisionCooldown = 10;
			}

			if (x < 100) x = 100;
			if (x > 840) x = 840;

			y = (float)GROUND_Y;

			return;
		}

		// -------------------- LEAP --------------------
		if (state == L2V_LEAP) {
			vy -= 0.95f;

			x += vx;
			y += vy;

			stateTimer++;

			if (y <= GROUND_Y) {
				y = (float)GROUND_Y;

				vy = 0;
				vx = 0;

				grounded = true;

				state = L2V_IDLE;

				stateTimer = 0;
				decisionCooldown = 8;
			}

			return;
		}

		// -------------------- ATTACK STATE --------------------
		if (isAttacking()) {
			stateTimer++;

			/*
			Small attack lunge:
			real fighting games move the attacker slightly into contact.
			This prevents kicks/punches from looking disconnected.

			The lunge follows the LOCKED attack-facing direction.
			*/
			float attackStep = 0.0f;

			if (state == L2V_PUNCH && stateTimer >= 4 && stateTimer <= 8)
				attackStep = 1.15f;

			else if (state == L2V_KICK && stateTimer >= 5 && stateTimer <= 11)
				attackStep = 1.45f;

			else if (state == L2V_SPIN && stateTimer >= 5 && stateTimer <= 13)
				attackStep = 1.20f;

			if (attackStep > 0.0f) {
				x += actionFacingRight ? attackStep : -attackStep;

				if (x < 100) x = 100;
				if (x > 840) x = 840;
			}

			int endTime = 18;

			if (state == L2V_KICK) endTime = 20;
			else if (state == L2V_POWER) endTime = 34;
			else if (state == L2V_SPIN) endTime = 24;

			if (stateTimer >= endTime) {
				state = L2V_IDLE;

				stateTimer = 0;
				vx = 0;

				decisionCooldown =
					phase == 3 ?
					5 :
					9;
			}

			y = (float)GROUND_Y;

			return;
		}

		if (!fightStarted) {
			state = L2V_IDLE;
			vx = 0;
			return;
		}

		heroCenter =
			hero.x +
			Level2HeroFighter::DRAW_W *
			0.5f;

		myCenter =
			x +
			DRAW_W *
			0.5f;

		float distance =
			heroCenter -
			myCenter;

		float absDistance =
			distance < 0 ?
			-distance :
			distance;

		const float ATTACK_RANGE = 165.0f;


		if (absDistance > ATTACK_RANGE) {
			state = L2V_RUN;

			if (distance < 0) vx = -moveSpeed;
			else vx = moveSpeed;

			x += vx;

			if (x < 100) x = 100;
			if (x > 840) x = 840;

			faceOpponent(heroCenter);

			y = (float)GROUND_Y;
			return;
		}

		state = L2V_IDLE;
		vx = 0;

		faceOpponent(heroCenter);

		if (
			decisionCooldown > 0 ||
			attackCooldown > 0
			) {

			y = (float)GROUND_Y;
			return;
		}

		// -------------------- DEFENSE --------------------
		if (hero.isAttacking()) {
			defenseCycle++;

			if (
				phase >= 2 &&
				defenseCycle % 5 == 0
				) {

				startDodge(heroCenter);
				return;
			}

			if (defenseCycle % 3 == 0) {
				startBlock();
				return;
			}
		}

		faceOpponent(heroCenter);

		aiCycle++;

		if (phase == 1) {
			if (
				powerCooldown <= 0 &&
				aiCycle % 5 == 0
				)
				startPower(aiCycle);

			else if (aiCycle % 3 == 1)
				startPunch();

			else if (aiCycle % 3 == 2)
				startKick();

			else
				startPunch();
		}

		else if (phase == 2) {
			if (
				powerCooldown <= 0 &&
				aiCycle % 4 == 0
				)
				startPower(aiCycle);

			else if (aiCycle % 5 == 1)
				startSpin();

			else if (aiCycle % 5 == 2)
				startKick();

			else if (aiCycle % 5 == 3)
				startPunch();

			else if (aiCycle % 5 == 4)
				startLeap(heroCenter);

			else
				startPunch();
		}

		else {
			if (
				powerCooldown <= 0 &&
				aiCycle % 3 == 0
				)
				startPower(aiCycle);

			else if (aiCycle % 5 == 1)
				startSpin();

			else if (aiCycle % 5 == 2)
				startKick();

			else if (aiCycle % 5 == 3)
				startLeap(heroCenter);

			else
				startPunch();
		}

		y = (float)GROUND_Y;
	}

	int getImage() {
		if (state == L2V_KO)
			return imgKO;

		if (state == L2V_HURT)
			return imgHurt[(stateTimer / 3) % 3];

		if (state == L2V_BLOCK)
			return imgBlock[(stateTimer / 4) % 3];

		if (state == L2V_DODGE)
			return imgDodge[(stateTimer / 2) % 4];

		if (state == L2V_SPIN)
			return imgSpin[(stateTimer / 4) % 3];

		if (state == L2V_LEAP) {
			if (stateTimer < 4) return imgLeap[0];
			if (stateTimer < 8) return imgLeap[1];
			if (stateTimer < 12) return imgLeap[2];
			if (stateTimer < 16) return imgLeap[3];

			return imgLeap[4];
		}

		if (state == L2V_PUNCH) {
			if (stateTimer < 4) return imgPunch[0];
			if (stateTimer < 8) return imgPunch[1];
			if (stateTimer < 12) return imgPunch[2];
			if (stateTimer < 15) return imgPunch[3];

			return imgPunch[4];
		}

		if (state == L2V_KICK) {
			if (stateTimer < 5) return imgKick[0];
			if (stateTimer < 10) return imgKick[1];
			if (stateTimer < 15) return imgKick[2];

			return imgKick[3];
		}

		if (state == L2V_POWER) {
			if (stateTimer < 5) return imgPowerPose[0];
			if (stateTimer < 9) return imgPowerPose[1];
			if (stateTimer < 13) return imgPowerPose[3];
			if (stateTimer < 17) return imgPowerPose[4];
			if (stateTimer < 21) return imgPowerPose[6];
			if (stateTimer < 27) return imgPowerPose[2];

			return imgPowerPose[7];
		}

		if (state == L2V_RUN)
			return imgRun[(animTimer / 4) % 6];

		return imgIdle[0];
	}

	void showFacingImage(int px, int py, int image) {


		bool renderRight = getRenderFacing();

		if (renderRight) {
			iShowImage(
				px,
				py,
				DRAW_W,
				DRAW_H,
				image
				);

			return;
		}

		glPushMatrix();

		glTranslatef(
			(float)(px + DRAW_W),
			0.0f,
			0.0f
			);

		glScalef(
			-1.0f,
			1.0f,
			1.0f
			);

		iShowImage(
			0,
			py,
			DRAW_W,
			DRAW_H,
			image
			);

		glPopMatrix();
	}

	void draw() {
		if (
			invincibleTimer > 0 &&
			(invincibleTimer / 2) % 2 == 0 &&
			state != L2V_KO
			)
			return;

		int drawY = (int)y;

		if (state == L2V_RUN) drawY -= 7;
		else if (state == L2V_KICK) drawY -= 9;
		else if (state == L2V_POWER) drawY -= 7;
		else if (state == L2V_SPIN) drawY -= 10;
		else if (state == L2V_HURT) drawY -= 8;

		showFacingImage(
			(int)x,
			drawY,
			getImage()
			);
	}
};


struct Level2Stage {
	static const int PROJECTILE_COUNT = 14;
	static const int EFFECT_COUNT = 12;

	Level2HeroFighter hero;
	Level2VillainFighter villain;

	L2Projectile projectile[PROJECTILE_COUNT];
	L2Effect effect[EFFECT_COUNT];

	int imgBackground[3];

	
	int imgHUD[19];

	int imgImpact;
	int imgGroundBurst;
	int imgCrescent;
	int imgSmoke;

	int imgHeroShuriken[4];
	int imgHeroShurikenSlash;
	int imgHeroShurikenHit;
	int imgHeroShurikenSmoke;
	int imgHeroPowerSlash;
	int imgHeroPowerHit;
	int imgHeroFastShuriken;

	int score, combo, comboTimer;

	int currentPhase;

	bool fightStarted;
	int introTimer;

	int phasePause;
	int phaseResult;

	int fightTicks;

	bool finished;
	bool won;

	bool resultReady;
	bool resultSubmitted;
	bool returnRequested;

	int resultTimer;
	int finalScore;
	int earnedStars;

	void load() {
		hero.load();
		villain.load();

		imgBackground[0] =
			iLoadImage("Images\\Enemies\\Level2\\l2-51.png");

		imgBackground[1] =
			iLoadImage("Images\\Enemies\\Level2\\l2-52.png");

		imgBackground[2] =
			iLoadImage("Images\\Enemies\\Level2\\l2-53.png");

		// HUD / title / phase / fight graphics.
		for (int i = 1; i <= 18; i++) {
			char hudPath[100];
			sprintf_s(hudPath, sizeof(hudPath), "Images\\Enemies\\Level2\\%d.png", i);
			imgHUD[i] = iLoadImage(hudPath);
		}

		imgImpact =
			iLoadImage("Images\\Enemies\\Level2\\l2-47.png");

		imgGroundBurst =
			iLoadImage("Images\\Enemies\\Level2\\l2-48.png");

		imgCrescent =
			iLoadImage("Images\\Enemies\\Level2\\l2-49.png");

		imgSmoke =
			iLoadImage("Images\\Enemies\\Level2\\l2-50.png");

		imgHeroShuriken[0] =
			iLoadImage(L2_SHURIKEN_1);

		imgHeroShuriken[1] =
			iLoadImage(L2_SHURIKEN_2);

		imgHeroShuriken[2] =
			iLoadImage(L2_SHURIKEN_3);

		imgHeroShuriken[3] =
			iLoadImage(L2_SHURIKEN_4);

		imgHeroShurikenSlash =
			iLoadImage(L2_SHURIKEN_SLASH);

		imgHeroShurikenHit =
			iLoadImage(L2_SHURIKEN_HIT);

		imgHeroShurikenSmoke =
			iLoadImage(L2_SHURIKEN_SMOKE);

		imgHeroPowerSlash =
			iLoadImage(L2_SHURIKEN_POWER_SLASH);

		imgHeroPowerHit =
			iLoadImage(L2_SHURIKEN_POWER_HIT);

		imgHeroFastShuriken =
			iLoadImage(L2_SHURIKEN_FAST);
	}

	void clearProjectiles() {
		for (int i = 0; i < PROJECTILE_COUNT; i++)
			projectile[i].clear();
	}

	void clearEffects() {
		for (int i = 0; i < EFFECT_COUNT; i++)
			effect[i].clear();
	}

	void reset() {
		hero.reset();
		villain.reset();

		clearProjectiles();
		clearEffects();

		score = 0;
		combo = 0;
		comboTimer = 0;

		currentPhase = 1;

		fightStarted = false;
		introTimer = 0;

		phasePause = 0;
		phaseResult = 0;

		fightTicks = 0;

		finished = false;
		won = false;

		resultReady = false;
		resultSubmitted = false;
		returnRequested = false;

		resultTimer = 0;
		finalScore = 0;
		earnedStars = 0;

		hero.applyPhaseUpgrade(1);
	}

	float absValue(float value) {
		return value < 0 ? -value : value;
	}

	bool overlap(
		float ax,
		float ay,
		float aw,
		float ah,
		float bx,
		float by,
		float bw,
		float bh
		) {

		return
			ax < bx + bw &&
			ax + aw > bx &&
			ay < by + bh &&
			ay + ah > by;
	}

	float heroCenterX() {
		return
			hero.x +
			Level2HeroFighter::DRAW_W *
			0.5f;
	}

	float villainCenterX() {
		return
			villain.x +
			Level2VillainFighter::DRAW_W *
			0.5f;
	}

	float fighterDistance() {
		return
			absValue(
			heroCenterX() -
			villainCenterX()
			);
	}

	void addEffect(
		int image,
		float x,
		float y,
		int w,
		int h,
		int life,
		bool mirror = false
		) {

		for (int i = 0; i < EFFECT_COUNT; i++) {
			if (!effect[i].active) {
				effect[i].start(
					image,
					x,
					y,
					w,
					h,
					life,
					mirror
					);

				return;
			}
		}
	}

	void addCombo(int baseScore) {
		if (comboTimer > 0)
			combo++;

		else
			combo = 1;

		comboTimer = 48;

		score += baseScore;

		if (combo >= 3)
			score += combo * 20;
	}

	void spawnHeroShuriken() {
		for (int i = 0; i < PROJECTILE_COUNT; i++) {
			if (projectile[i].active) continue;

			bool right = hero.getRenderFacing();
			bool powered = hero.powerTimer > 0;

			float speed = right ? (powered ? 11.5f : 8.5f) : -(powered ? 11.5f : 8.5f);
			float px = right ? hero.x + Level2HeroFighter::DRAW_W - 18 : hero.x - 48;
			float py = hero.y + 53;

			
			addEffect(
				powered ? imgHeroPowerSlash : imgHeroShurikenSlash,
				right ? hero.x + 72 : hero.x - 20,
				hero.y + 32,
				95,
				80,
				9,
				!right
				);

			if (powered) {
				// s-10 is the fast powered shuriken/projectile.
				projectile[i].spawnSingle(
					true, px, py, speed, imgHeroFastShuriken,
					66, 42, hero.modifyDamage(26)
					);
			}
			else {
				// s-01 ... s-04 animate as a rotating shuriken.
				projectile[i].spawnAnimated(
					true, px, py, speed,
					imgHeroShuriken[0], imgHeroShuriken[1],
					imgHeroShuriken[2], imgHeroShuriken[3],
					50, 50, hero.modifyDamage(18)
					);
			}

			return;
		}
	}

	void spawnVillainPower() {
		for (int i = 0; i < PROJECTILE_COUNT; i++) {
			if (projectile[i].active)
				continue;

			int type =
				villain.powerType();

			int w = 108;
			int h = 64;

			if (type == 1) {
				w = 118;
				h = 75;
			}

			else if (type == 2) {
				w = 120;
				h = 90;
			}

			else if (type == 3) {
				w = 118;
				h = 82;
			}

			else if (type == 4) {
				w = 125;
				h = 64;
			}

			float speed =
				villain.facingRight ?
				(
				5.0f +
				currentPhase *
				0.7f
				) :
				-(
				5.0f +
				currentPhase *
				0.7f
				);

			float px =
				villain.facingRight ?
				villain.x +
				Level2VillainFighter::DRAW_W -
				10 :
				villain.x -
				80;

			int damage =
				18 +
				type *
				3;

			if (currentPhase == 2)
				damage += 10;

			if (currentPhase == 3)
				damage += 20;

			projectile[i].spawnSingle(
				false,
				px,
				villain.y + 45,
				speed,

				villain.imgProjectile[type],

				w,
				h,

				damage
				);

			return;
		}
	}

	void heroMeleeHit(
		int damage,
		int hitScore
		) {

		if (fighterDistance() > 175.0f)
			return;

		if (villain.state == L2V_BLOCK) {
			score += 12;

			hero.gainPower(4);

			addEffect(
				imgImpact,
				villain.x + 45,
				villain.y + 50,
				65,
				65,
				7
				);

			return;
		}

		villain.takeHit(damage);

		hero.gainPower(12);

		addCombo(hitScore);

		addEffect(
			imgImpact,
			villain.x + 45,
			villain.y + 45,
			72,
			72,
			8
			);

		if (hero.state == L2H_HEAVY) {
			addEffect(
				imgCrescent,
				villain.x - 10,
				villain.y + 20,
				130,
				95,
				10
				);
		}

		if (
			villain.hp <= 0 &&
			phasePause == 0
			) {

			score +=
				700 +
				currentPhase *
				180;

			phasePause = 80;
			phaseResult = 1;

			addEffect(
				imgSmoke,
				villain.x - 10,
				65,
				180,
				100,
				28
				);
		}
	}

	int villainPunchDamage() {
		if (currentPhase == 1) return 18;
		if (currentPhase == 2) return 27;

		return 34;
	}

	int villainKickDamage() {
		if (currentPhase == 1) return 24;
		if (currentPhase == 2) return 35;

		return 44;
	}

	int villainSpinDamage() {
		if (currentPhase == 1) return 26;
		if (currentPhase == 2) return 40;

		return 50;
	}

	int villainLeapDamage() {
		if (currentPhase == 1) return 28;
		if (currentPhase == 2) return 42;

		return 52;
	}

	void villainMeleeHit(
		int damage,
		bool powerLike
		) {

		/*
		Because the villain now approaches to ~125 px and lunges during
		attacks, use a slightly forgiving contact range.
		*/
		if (
			fighterDistance() >
			(
			powerLike ?
			155.0f :
			145.0f
			)
			)
			return;

		if (hero.state == L2H_BLOCK) {
			hero.gainPower(8);

			score +=
				powerLike ?
				25 :
				45;

			if (powerLike)
				hero.takeHit(
				damage /
				4
				);

			return;
		}

		hero.takeHit(damage);

		combo = 0;
		comboTimer = 0;

		score -= 25;

		if (score < 0)
			score = 0;

		addEffect(
			imgImpact,
			hero.x + 35,
			hero.y + 40,
			72,
			72,
			8
			);
	}

	void updateProjectiles() {
		for (int i = 0; i < PROJECTILE_COUNT; i++) {
			if (!projectile[i].active)
				continue;

			projectile[i].update();

			if (!projectile[i].active)
				continue;

			if (projectile[i].fromHero) {
				float vx =
					villain.x +
					28;

				float vy =
					villain.y +
					10;

				float vw =
					Level2VillainFighter::DRAW_W -
					55;

				float vh =
					Level2VillainFighter::DRAW_H -
					20;

				if (
					overlap(
					projectile[i].x + 6,
					projectile[i].y + 6,
					projectile[i].w - 12,
					projectile[i].h - 12,

					vx,
					vy,
					vw,
					vh
					)
					) {

					villain.takeHit(
						projectile[i].damage
						);

					hero.gainPower(8);

					score += 75;

					addEffect(
						hero.powerTimer > 0 ? imgHeroPowerHit : imgHeroShurikenHit,
						villain.x + 28, villain.y + 42,
						hero.powerTimer > 0 ? 88 : 70,
						hero.powerTimer > 0 ? 88 : 70,
						9
						);

					addEffect(
						imgHeroShurikenSmoke,
						villain.x + 35, villain.y + 25,
						70, 55, 10
						);

					projectile[i].active =
						false;

					if (
						villain.hp <= 0 &&
						phasePause == 0
						) {

						score +=
							700 +
							currentPhase *
							180;

						phasePause = 80;
						phaseResult = 1;
					}
				}
			}

			else {
				float hx =
					hero.x +
					25;

				float hy =
					hero.y +
					10;

				float hw =
					Level2HeroFighter::DRAW_W -
					48;

				float hh =
					Level2HeroFighter::DRAW_H -
					18;

				if (
					overlap(
					projectile[i].x + 8,
					projectile[i].y + 8,
					projectile[i].w - 16,
					projectile[i].h - 16,

					hx,
					hy,
					hw,
					hh
					)
					) {

					if (hero.state == L2H_BLOCK) {
						hero.takeHit(
							projectile[i].damage /
							5
							);

						hero.gainPower(10);

						score += 25;
					}

					else {
						hero.takeHit(
							projectile[i].damage
							);

						score -= 20;

						if (score < 0)
							score = 0;
					}

					addEffect(
						imgGroundBurst,
						hero.x + 10,
						hero.y + 5,
						110,
						100,
						12
						);

					projectile[i].active =
						false;
				}
			}
		}
	}

	void updateEffects() {
		for (int i = 0; i < EFFECT_COUNT; i++)
			effect[i].update();
	}

	void beginNextPhase() {
		currentPhase++;

		hero.applyPhaseUpgrade(
			currentPhase
			);

		// Small recovery only; later phases should remain demanding.
		hero.hp += 15;

		if (hero.hp > hero.maxHP)
			hero.hp =
			hero.maxHP;

		hero.resetRound(false);

		villain.resetForPhase(
			currentPhase
			);

		clearProjectiles();

		fightStarted = false;

		introTimer = 0;

		phasePause = 0;
		phaseResult = 0;
	}

	void finishWin() {
		finished = true;
		won = true;

		finalScore =
			score +
			1800 +
			hero.lives *
			550 +
			hero.healCharges *
			120;

		if (hero.lives == 3)
			finalScore += 900;

		if (hero.lives >= 3)
			earnedStars = 3;

		else if (hero.lives == 2)
			earnedStars = 2;

		else
			earnedStars = 1;

		resultReady = true;
		resultTimer = 0;
	}

	void finishLose() {
		finished = true;
		won = false;

		resultReady = false;
		resultTimer = 0;
	}

	void resetAfterHeroKO() {
		if (hero.lives <= 0) {
			finishLose();
			return;
		}

		hero.resetRound(true);

		hero.applyPhaseUpgrade(
			currentPhase
			);

		villain.resetForPhase(
			currentPhase
			);

		clearProjectiles();

		fightStarted = false;
		introTimer = 0;
	}

	void resolvePhasePause() {
		if (phaseResult == 1) {
			if (currentPhase >= 3) {
				finishWin();
				return;
			}

			beginNextPhase();
		}

		else if (phaseResult == -1)
			resetAfterHeroKO();

		phaseResult = 0;
	}

	void update(
		bool leftHeld,
		bool rightHeld,
		bool jumpPressed,
		bool dashPressed,
		bool punchPressed,
		bool kickPressed,
		bool heavyPressed,
		bool blockHeld,
		bool shurikenPressed
		) {

		if (finished) {
			resultTimer++;

			if (resultTimer > 125)
				returnRequested = true;

			return;
		}

		fightTicks++;

		if (comboTimer > 0) {
			comboTimer--;

			if (comboTimer == 0)
				combo = 0;
		}

		updateEffects();

		// -------------------- INTRO --------------------
		if (!fightStarted) {
			hero.faceOpponent(
				villainCenterX()
				);

			hero.update(
				false,
				false,
				false,
				false,
				false,
				false
				);

			villain.updateAI(
				hero,
				false
				);

			updateProjectiles();

			if (!villain.enteringArena) {
				introTimer++;

				if (introTimer >= 22)
					fightStarted = true;
			}

			return;
		}

		// -------------------- PHASE TRANSITION --------------------
		if (phasePause > 0) {
			phasePause--;

			hero.update(
				false,
				false,
				false,
				false,
				false,
				false
				);

			if (phasePause == 0)
				resolvePhasePause();

			return;
		}

		/*
		PAIR-FACING RULE

		The two fighters face each other before movement/action.
		Hero jump facing is then locked at takeoff.
		*/

		hero.faceOpponent(villainCenterX());
		villain.faceOpponent(heroCenterX());

		// -------------------- HERO INPUT --------------------
		if (punchPressed)
			hero.startPunch();

		else if (kickPressed)
			hero.startKick();

		else if (heavyPressed)
			hero.startHeavy();

		else if (shurikenPressed)
			hero.startThrow();

		if (dashPressed)
			hero.startDash();

		hero.update(
			leftHeld,
			rightHeld,
			jumpPressed,
			dashPressed,
			blockHeld,
			true
			);

		villain.updateAI(
			hero,
			true
			);

		/*
		Re-check facing after movement so that if the fighters cross sides,
		both turn toward one another immediately.

		Hero faceOpponent() ignores this while airborne, so jump facing stays
		locked until landing.
		*/

		hero.faceOpponent(villainCenterX());
		villain.faceOpponent(heroCenterX());

		// -------------------- HERO SHURIKEN RELEASE --------------------
		if (
			hero.state == L2H_THROW &&
			hero.stateTimer == 8 &&
			!hero.throwReleaseDone
			) {

			hero.throwReleaseDone = true;

			spawnHeroShuriken();
		}

		// -------------------- HERO MELEE --------------------
		if (
			hero.state == L2H_PUNCH &&
			hero.stateTimer == 5 &&
			!hero.attackHitDone
			) {

			hero.attackHitDone = true;

			heroMeleeHit(
				hero.modifyDamage(20),
				90
				);
		}

		if (
			hero.state == L2H_KICK &&
			hero.stateTimer == 7 &&
			!hero.attackHitDone
			) {

			hero.attackHitDone = true;

			heroMeleeHit(
				hero.modifyDamage(27),
				130
				);
		}

		if (
			hero.state == L2H_HEAVY &&
			hero.stateTimer == 10 &&
			!hero.attackHitDone
			) {

			hero.attackHitDone = true;

			heroMeleeHit(
				hero.modifyDamage(36),
				190
				);
		}

		// -------------------- VILLAIN MELEE --------------------
		if (
			villain.state == L2V_PUNCH &&
			villain.stateTimer == 8 &&
			!villain.attackHitDone
			) {

			villain.attackHitDone = true;

			villainMeleeHit(
				villainPunchDamage(),
				false
				);
		}

		if (
			villain.state == L2V_KICK &&
			villain.stateTimer == 10 &&
			!villain.attackHitDone
			) {

			villain.attackHitDone = true;

			villainMeleeHit(
				villainKickDamage(),
				false
				);
		}

		if (
			villain.state == L2V_SPIN &&
			villain.stateTimer == 11 &&
			!villain.attackHitDone
			) {

			villain.attackHitDone = true;

			villainMeleeHit(
				villainSpinDamage(),
				true
				);

			addEffect(
				imgCrescent,
				villain.x - 75,
				villain.y + 15,
				175,
				120,
				13,
				villain.getRenderFacing()
				);
		}

		if (
			villain.state == L2V_LEAP &&
			villain.stateTimer == 10 &&
			!villain.attackHitDone
			) {

			villain.attackHitDone = true;

			villainMeleeHit(
				villainLeapDamage(),
				false
				);
		}

		// -------------------- VILLAIN PROJECTILE --------------------
		if (
			villain.state == L2V_POWER &&
			villain.stateTimer == 18 &&
			!villain.projectileReleaseDone
			) {

			villain.projectileReleaseDone = true;

			spawnVillainPower();
		}

		updateProjectiles();

		// -------------------- HERO KO CHECK --------------------
		if (
			hero.hp <= 0 &&
			phasePause == 0
			) {

			hero.lives--;

			if (hero.lives < 0)
				hero.lives = 0;

			phasePause = 80;
			phaseResult = -1;
		}
	}

	void drawBar(
		int x,
		int y,
		int width,
		int value,
		int maximum,
		bool heroBar
		) {

		iSetColor(25, 25, 30);

		iFilledRectangle(
			x,
			y,
			width,
			18
			);

		float ratio =
			maximum > 0 ?
			(float)value /
			(float)maximum :
			0;

		if (ratio < 0) ratio = 0;
		if (ratio > 1) ratio = 1;

		if (heroBar)
			iSetColor(185, 45, 45);

		else
			iSetColor(115, 45, 170);

		iFilledRectangle(
			x + 2,
			y + 2,
			(int)(
			(width - 4) *
			ratio
			),
			14
			);

		iSetColor(230, 230, 230);

		iRectangle(
			x,
			y,
			width,
			18
			);
	}

	void drawProjectiles() {
		for (int i = 0; i < PROJECTILE_COUNT; i++)
			projectile[i].draw();
	}

	void drawEffects() {
		for (int i = 0; i < EFFECT_COUNT; i++)
			effect[i].draw();
	}

	void drawHUD() {
		float heroRatio = hero.maxHP > 0 ? (float)hero.hp / (float)hero.maxHP : 0.0f;
		float villainRatio = villain.maxHP > 0 ? (float)villain.hp / (float)villain.maxHP : 0.0f;

		if (heroRatio < 0) heroRatio = 0;
		if (heroRatio > 1) heroRatio = 1;
		if (villainRatio < 0) villainRatio = 0;
		if (villainRatio > 1) villainRatio = 1;

		iSetColor(180, 38, 42);
		iFilledRectangle(45, 548, (int)(245 * heroRatio), 12);

		iSetColor(120, 48, 185);
		iFilledRectangle(734, 548, (int)(245 * villainRatio), 12);

		// Decorative health frames and centre ornament.
		iShowImage(18, 536, 300, 38, imgHUD[1]);
		iShowImage(402, 538, 220, 38, imgHUD[2]);
		iShowImage(706, 536, 300, 38, imgHUD[3]);

		// Fighter names: replace old iText labels completely.
		iShowImage(25, 570, 118, 26, imgHUD[4]);
		iShowImage(806, 570, 185, 25, imgHUD[6]);

		// Score header + dynamic numeric value.
		iShowImage(451, 570, 112, 25, imgHUD[5]);

		char text[50];
		sprintf_s(text, sizeof(text), "%d", score);
		iSetColor(250, 230, 170);
		iText(575, 574, text, GLUT_BITMAP_HELVETICA_18);

		// Lives labels use the graphic text instead of plain GLUT text.
		iShowImage(25, 505, 145, 25, imgHUD[7]);
		sprintf_s(text, sizeof(text), "%d", hero.lives);
		iSetColor(245, 235, 210);
		iText(176, 510, text, GLUT_BITMAP_HELVETICA_18);

		iShowImage(824, 505, 150, 25, imgHUD[8]);
		sprintf_s(text, sizeof(text), "%d", 4 - currentPhase);
		iText(982, 510, text, GLUT_BITMAP_HELVETICA_18);

		// Level title in the centre.
		iShowImage(447, 505, 132, 27, imgHUD[9]);

		// Power graphic + real meter.
		iShowImage(22, 474, 122, 25, imgHUD[13]);
		iSetColor(28, 22, 35);
		iFilledRectangle(148, 480, 145, 11);
		iSetColor(130, 55, 185);
		iFilledRectangle(150, 482, (int)(141 * (hero.powerMeter / 100.0f)), 7);

		// Heal graphic + remaining charges.
		iShowImage(309, 474, 118, 25, imgHUD[14]);
		sprintf_s(text, sizeof(text), "%d", hero.healCharges);
		iSetColor(210, 245, 180);
		iText(433, 479, text, GLUT_BITMAP_HELVETICA_18);

		// Current phase uses the supplied phase image.
		if (currentPhase == 1)
			iShowImage(455, 472, 120, 27, imgHUD[15]);
		else if (currentPhase == 2)
			iShowImage(455, 472, 120, 27, imgHUD[16]);
		else
			iShowImage(455, 472, 120, 27, imgHUD[17]);

		// Optional alternate labels 10/11/12 are loaded too, so you can swap
		// them with 5/7/8 later without changing any path code.
	}

	void draw() {
		int bgIndex =
			currentPhase -
			1;

		if (bgIndex < 0)
			bgIndex = 0;

		if (bgIndex > 2)
			bgIndex = 2;

		iShowImage(
			0,
			0,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			imgBackground[bgIndex]
			);

		// Characters and combat are rendered over the arena background.
		hero.draw();
		villain.draw();

		drawProjectiles();
		drawEffects();

		// New image-based Level-2 HUD.
		drawHUD();

		// FIGHT graphic replaces the old plain "FIGHT!" text.
		if (!fightStarted && !villain.enteringArena)
			iShowImage(424, 390, 176, 55, imgHUD[18]);

		if (
			combo >= 2 &&
			comboTimer > 0
			) {

			char comboText[40];

			sprintf_s(
				comboText,
				sizeof(comboText),
				"%d HIT COMBO",
				combo
				);

			iSetColor(235, 235, 235);

			iText(
				465,
				450,
				comboText,
				GLUT_BITMAP_HELVETICA_12
				);
		}

		if (finished) {
			iSetColor(245, 220, 145);

			if (won) {
				iText(
					430,
					355,
					(char*)"LEVEL 2 CLEARED!",
					GLUT_BITMAP_HELVETICA_18
					);

				char finalText[60];

				sprintf_s(
					finalText,
					sizeof(finalText),
					"FINAL SCORE: %d",
					finalScore
					);

				iText(
					440,
					325,
					finalText,
					GLUT_BITMAP_HELVETICA_18
					);

				char starText[50];

				sprintf_s(
					starText,
					sizeof(starText),
					"STARS EARNED: %d",
					earnedStars
					);

				iText(
					442,
					295,
					starText,
					GLUT_BITMAP_HELVETICA_18
					);

				iText(
					385,
					255,
					(char*)"LEVEL 3 IS NOW UNLOCKED",
					GLUT_BITMAP_HELVETICA_18
					);
			}

			else {
				iText(
					455,
					340,
					(char*)"DEFEATED",
					GLUT_BITMAP_HELVETICA_18
					);
			}
		}
	}

	bool hasResultToSubmit() {
		return
			resultReady &&
			!resultSubmitted;
	}

	void markResultSubmitted() {
		resultSubmitted = true;
	}

	bool shouldReturnToLevelSelect() {
		return returnRequested;
	}
};

#endif
