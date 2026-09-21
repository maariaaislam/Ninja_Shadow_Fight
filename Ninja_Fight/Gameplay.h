#ifndef GAMEPLAY_H
#define GAMEPLAY_H


// BASIC HELPERS

struct RectF {
	float x, y, w, h;
};

bool rectOverlap(RectF a, RectF b) {
	return (a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y);
}

float absFloat(float value) {
	return value < 0.0f ? -value : value;
}

struct PlatformLine {
	float x, y, width;
};

// CAMERA

struct Camera2D {
	float x, worldWidth;
	void reset(float levelWidth) {
		x = 0.0f;
		worldWidth = levelWidth;
	}

	void follow(float playerX) {
		float target = playerX - SCREEN_WIDTH * 0.42f;
		if (target < 0.0f) target = 0.0f;
		float maxCamera = worldWidth - SCREEN_WIDTH;
		if (maxCamera < 0.0f) maxCamera = 0.0f;
		if (target > maxCamera) target = maxCamera;
		x += (target - x) * 0.16f;
	}
};

// PLAYER STATES

enum NinjaState {
	NINJA_IDLE, NINJA_RUN, NINJA_CROUCH,
	NINJA_JUMP, NINJA_DOUBLE_JUMP, NINJA_LAND,
	NINJA_DASH,
	NINJA_GROUND_THROW, NINJA_AIR_THROW,
	NINJA_HURT, NINJA_AIR_HURT, NINJA_KNOCKBACK,
	NINJA_DEAD, NINJA_RESPAWN
};

// WEAPON TYPES

enum ShurikenType {
	SHURIKEN_STANDARD = 0, SHURIKEN_FAST = 1, SHURIKEN_HEAVY = 2, SHURIKEN_CRIMSON = 3
};

// PLAYER


struct NinjaPlayer {
	// POSITION / PHYSICS
	float x, y;
	float vx, vy;
	float runAcceleration, airAcceleration;
	float runMaxSpeed, sprintMaxSpeed;
	float groundFriction, airDrag;
	float gravity;
	float jumpPower, doubleJumpPower;
	float terminalFallSpeed, fastFallAcceleration, fastFallMaxSpeed;
	float dashSpeed;
	bool grounded, facingRight, invincible;
	bool attackReleasePending;
	int state;
	// JUMP
	int jumpCount, maxJumps;
	int coyoteTimer, coyoteMax;
	int jumpBufferTimer, jumpBufferMax;
	// DASH
	int dashTimer, dashDuration;
	int dashCooldownTimer, dashCooldownMax;
	// ATTACK
	int attackTimer, attackType;
	bool attackReleased;
	int attackRecoveryTimer;
	// HEALTH / DAMAGE
	int maxHP, hp;
	int hurtTimer, invincibleTimer;
	int deathTimer, respawnTimer;
	float checkpointX, checkpointY;
	// ANIMATION
	int runFrame, runTimer;
	int jumpAnimTimer, landTimer;
	// DRAW / HITBOX
	static const int DRAW_W = 90;
	static const int DRAW_H = 90;
	static const int FOOT_OFFSET = 7;
	static const int HEAD_OFFSET = 82;
	// RIGHT IMAGES
	int imgIdleRight;
	int runRight[2];
	int imgCrouchRight;
	int imgJumpLaunchRight, imgJumpDiagonalRight, imgJumpRiseRight, imgJumpApexRight, imgJumpFallRight, imgHardFallRight;
	int imgLandRight;
	// LEFT IMAGES
	int imgIdleLeft;
	int runLeft[2];
	int imgCrouchLeft;
	int imgJumpLaunchLeft, imgJumpDiagonalLeft, imgJumpRiseLeft, imgJumpFallLeft, imgHardFallLeft;
	int imgLandLeft;
	// THROW IMAGES
	static const int RIGHT_THROW_COUNT = 9;
	int rightThrow[RIGHT_THROW_COUNT];
	int imgAirThrowWindupRight, imgAirThrowReleaseRight;
	int imgThrowWindupLeft, imgThrowReleaseLeft;
	// DAMAGE / DEATH IMAGES
	int imgHurt, imgGroundRecoil, imgAirHurt, imgKnockback;
	int imgDeath1, imgDeath2;

	// LOAD
	void load() {
		// RIGHT
		imgIdleRight = iLoadImage("Images\\Player\\c-32.png");
		runRight[0] = iLoadImage("Images\\Player\\c-01.png");
		runRight[1] = iLoadImage("Images\\Player\\c-02.png");
		imgCrouchRight = iLoadImage("Images\\Player\\c-03.png");
		imgJumpLaunchRight = iLoadImage("Images\\Player\\c-05.png");
		imgJumpDiagonalRight = iLoadImage("Images\\Player\\c-13.png");
		imgJumpRiseRight = iLoadImage("Images\\Player\\c-08.png");
		imgJumpApexRight = iLoadImage("Images\\Player\\c-06.png");
		imgJumpFallRight = iLoadImage("Images\\Player\\c-07.png");
		imgHardFallRight = iLoadImage("Images\\Player\\c-14.png");
		imgLandRight = iLoadImage("Images\\Player\\c-09.png");
		// LEFT
		imgIdleLeft = iLoadImage("Images\\Player\\c-15.png");
		runLeft[0] = iLoadImage("Images\\Player\\c-16.png");
		runLeft[1] = iLoadImage("Images\\Player\\c-17.png");
		imgCrouchLeft = iLoadImage("Images\\Player\\c-18.png");
		imgJumpLaunchLeft = iLoadImage("Images\\Player\\c-19.png");
		imgJumpDiagonalLeft = iLoadImage("Images\\Player\\c-20.png");
		imgJumpRiseLeft = iLoadImage("Images\\Player\\c-21.png");
		imgJumpFallLeft = iLoadImage("Images\\Player\\c-22.png");
		imgHardFallLeft = iLoadImage("Images\\Player\\c-23.png");
		imgLandLeft = iLoadImage("Images\\Player\\c-24.png");
		// RIGHT THROW
		rightThrow[0] = iLoadImage("Images\\Player\\c-25.png");
		rightThrow[1] = iLoadImage("Images\\Player\\c-26.png");
		rightThrow[2] = iLoadImage("Images\\Player\\c-27.png");
		rightThrow[3] = iLoadImage("Images\\Player\\c-28.png");
		rightThrow[4] = iLoadImage("Images\\Player\\c-29.png");
		rightThrow[5] = iLoadImage("Images\\Player\\c-30.png");
		rightThrow[6] = iLoadImage("Images\\Player\\c-31.png");
		rightThrow[7] = iLoadImage("Images\\Player\\c-33.png");
		rightThrow[8] = iLoadImage("Images\\Player\\c-34.png");
		// AIR THROW
		imgAirThrowWindupRight = iLoadImage("Images\\Player\\c-35.png");
		imgAirThrowReleaseRight = iLoadImage("Images\\Player\\c-36.png");
		// LEFT THROW
		imgThrowWindupLeft = iLoadImage("Images\\Player\\c-37.png");
		imgThrowReleaseLeft = iLoadImage("Images\\Player\\c-38.png");
		// DAMAGE
		imgHurt = iLoadImage("Images\\Player\\c-39.png");
		imgGroundRecoil = iLoadImage("Images\\Player\\c-40.png");
		imgAirHurt = iLoadImage("Images\\Player\\c-41.png");
		imgKnockback = iLoadImage("Images\\Player\\c-42.png");
		// DEATH
		imgDeath1 = iLoadImage("Images\\Player\\c-10.png");
		imgDeath2 = iLoadImage("Images\\Player\\c-11.png");
	}

	// RESET

	void reset() {
		// Responsive movement.
		runAcceleration = 1.25f;
		airAcceleration = 0.85f;
		runMaxSpeed = 9.0f;
		sprintMaxSpeed = 14.0f;
		groundFriction = 0.68f;
		airDrag = 0.995f;
		// Fast game feel.
		gravity = 1.65f;
		jumpPower = 19.0f;
		doubleJumpPower = 17.0f;
		terminalFallSpeed = -23.0f;
		fastFallAcceleration = 2.8f;
		fastFallMaxSpeed = -30.0f;
		dashSpeed = 23.0f;
		maxJumps = 2;
		coyoteMax = 5;
		jumpBufferMax = 5;
		dashDuration = 6;
		dashCooldownMax = 20;
		maxHP = 100;
		hp = maxHP;
		checkpointX = 80.0f;
		checkpointY = 90.0f - FOOT_OFFSET;
		respawnAtCheckpoint(false);
	}

	void respawnAtCheckpoint(bool giveInvincibility) {
		x = checkpointX;
		y = checkpointY;
		vx = 0.0f;
		vy = 0.0f;
		grounded = true;
		facingRight = true;
		state = NINJA_IDLE;
		jumpCount = 0;
		coyoteTimer = coyoteMax;
		jumpBufferTimer = 0;
		dashTimer = 0;
		dashCooldownTimer = 0;
		attackTimer = 0;
		attackType = SHURIKEN_STANDARD;
		attackReleased = false;
		attackReleasePending = false;
		attackRecoveryTimer = 0;
		hurtTimer = 0;
		deathTimer = 0;
		respawnTimer = 0;
		runFrame = 0;
		runTimer = 0;
		jumpAnimTimer = 0;
		landTimer = 0;
		if (giveInvincibility) {
			invincible = true;
			invincibleTimer = 45;
		}
		else {
			invincible = false;
			invincibleTimer = 0;
		}
	}

	// STATE HELPERS

	bool isDeadState() {
		return (state == NINJA_DEAD || state == NINJA_RESPAWN);
	}

	bool isHurtState() {
		return (state == NINJA_HURT || state == NINJA_AIR_HURT || state == NINJA_KNOCKBACK);
	}

	bool isAttackState() {
		return (state == NINJA_GROUND_THROW || state == NINJA_AIR_THROW);
	}

	bool canStartAttack() {
		if (isDeadState()) return false;
		if (isHurtState()) return false;
		if (state == NINJA_DASH) return false;
		if (isAttackState()) return false;
		return true;
	}

	// HITBOX

	RectF getHitbox() {
		RectF box;
		if (state == NINJA_CROUCH) {
			box.x = x + 20.0f;
			box.y = y + 7.0f;
			box.w = 50.0f;
			box.h = 48.0f;
		}
		else if (isHurtState()) {
			box.x = x + 14.0f;
			box.y = y + 8.0f;
			box.w = 62.0f;
			box.h = 65.0f;
		}
		else if (state == NINJA_DEAD) {
			box.x = x + 10.0f;
			box.y = y + 5.0f;
			box.w = 70.0f;
			box.h = 30.0f;
		}
		else {
			box.x = x + 18.0f;
			box.y = y + 7.0f;
			box.w = 54.0f;
			box.h = 72.0f;
		}
		return box;
	}

	float feetY() {
		return y + FOOT_OFFSET;
	}

	// JUMP

	void requestJump() {
		if (isDeadState() || isHurtState()) return;
		jumpBufferTimer = jumpBufferMax;
		if (grounded || coyoteTimer > 0) {
			performGroundJump();
			jumpBufferTimer = 0;
			return;
		}
		if (!grounded && jumpCount < maxJumps) {
			performDoubleJump();
			jumpBufferTimer = 0;
		}
	}

	void performGroundJump() {
		grounded = false;
		vy = jumpPower;
		jumpCount = 1;
		state = NINJA_JUMP;
		jumpAnimTimer = 0;
		coyoteTimer = 0;
	}

	void performDoubleJump() {
		vy = doubleJumpPower;
		jumpCount++;
		state = NINJA_DOUBLE_JUMP;
		jumpAnimTimer = 0;
	}

	// DASH

	void requestDash() {
		if (isDeadState() || isHurtState()) return;
		if (dashCooldownTimer > 0) return;
		attackReleasePending = false;
		attackReleased = false;
		attackTimer = 0;
		state = NINJA_DASH;
		dashTimer = dashDuration;
		dashCooldownTimer = dashCooldownMax;
		if (facingRight) vx = dashSpeed;
		else vx = -dashSpeed;
		if (!grounded) vy *= 0.25f;
	}

	// ATTACK

	bool requestAttack(int type) {
		if (!canStartAttack()) return false;
		attackType = type;
		attackTimer = 0;
		attackReleased = false;
		attackReleasePending = false;
		if (grounded) {
			state = NINJA_GROUND_THROW;
			vx = 0.0f;
		}
		else {
			state = NINJA_AIR_THROW;
		}
		return true;
	}

	bool consumeAttackRelease(int &releasedType) {
		if (!attackReleasePending) return false;
		attackReleasePending = false;
		releasedType = attackType;
		return true;
	}

	// DAMAGE

	void takeDamage(int damage, float sourceX, float knockbackPower) {
		if (invincible) return;
		if (isDeadState()) return;
		hp -= damage;
		if (hp <= 0) {
			hp = 0;
			die();
			return;
		}
		attackReleasePending = false;
		attackReleased = false;
		attackTimer = 0;
		dashTimer = 0;
		invincible = true;
		invincibleTimer = 38;
		hurtTimer = 14;
		if (sourceX < x) {
			vx = knockbackPower;
			facingRight = false;
		}
		else {
			vx = -knockbackPower;
			facingRight = true;
		}
		vy = 8.5f;
		grounded = false;
		if (absFloat(knockbackPower) >= 10.0f) state = NINJA_KNOCKBACK;
		else state = NINJA_AIR_HURT;
	}

	void die() {
		state = NINJA_DEAD;
		deathTimer = 0;
		vx = 0.0f;
		vy = 0.0f;
		grounded = false;
		invincible = true;
		invincibleTimer = 9999;
		attackReleasePending = false;
		attackReleased = false;
	}

	// PLATFORM SUPPORT

	bool hasPlatformSupport(PlatformLine platforms[], int platformCount) {
		RectF box = getHitbox();
		float feet = feetY();
		for (int i = 0; i < platformCount; i++) {
			bool horizontal = box.x + box.w > platforms[i].x && box.x < platforms[i].x + platforms[i].width;
			bool sameHeight = feet >= platforms[i].y - 3.0f && feet <= platforms[i].y + 3.0f;
			if (horizontal && sameHeight) return true;
		}
		return false;
	}

	// ATTACK UPDATE

	void updateAttack() {
		attackTimer++;
		if (state == NINJA_GROUND_THROW) {
			int releaseTick = 8;
			int finishTick = 18;
			if (!attackReleased && attackTimer >= releaseTick) {
				attackReleased = true;
				attackReleasePending = true;
			}
			if (attackTimer >= finishTick) {
				state = NINJA_IDLE;
				attackTimer = 0;
				attackReleased = false;
			}
		}
		else if (state == NINJA_AIR_THROW) {
			int releaseTick = 3;
			int finishTick = 7;
			if (!attackReleased && attackTimer >= releaseTick) {
				attackReleased = true;
				attackReleasePending = true;
			}
			if (attackTimer >= finishTick) {
				state = NINJA_JUMP;
				attackTimer = 0;
				attackReleased = false;
			}
		}
	}

	// MAIN UPDATE
	void update(PlatformLine platforms[], int platformCount, int moveDirection, bool sprintHeld, bool downHeld, float deathY, float worldWidth) {
		// TIMERS
		if (dashCooldownTimer > 0) dashCooldownTimer--;
		if (invincible) {
			invincibleTimer--;
			if (invincibleTimer <= 0) {
				invincible = false;
				invincibleTimer = 0;
			}
		}
		if (jumpBufferTimer > 0) jumpBufferTimer--;
		// DEAD / RESPAWN
		if (state == NINJA_DEAD) {
			deathTimer++;
			if (deathTimer >= 42) {
				state = NINJA_RESPAWN;
				respawnTimer = 8;
			}
			return;
		}
		if (state == NINJA_RESPAWN) {
			respawnTimer--;
			if (respawnTimer <= 0) {
				hp = maxHP;
				respawnAtCheckpoint(true);
			}
			return;
		}
		// HURT / KNOCKBACK
		if (isHurtState()) {
			hurtTimer--;
			x += vx;
			vx *= 0.90f;
			vy -= gravity;
			if (vy < terminalFallSpeed) vy = terminalFallSpeed;
			y += vy;
			clampWorldX(worldWidth);
			handleVerticalPlatforms(platforms, platformCount);
			if (hurtTimer <= 0) {
				if (grounded) state = NINJA_IDLE;
				else state = NINJA_JUMP;
			}
			if (y < deathY) die();
			return;
		}
		// DASH
		if (state == NINJA_DASH) {
			x += vx;
			clampWorldX(worldWidth);
			dashTimer--;
			if (!grounded) {
				vy -= gravity * 0.45f;
				if (vy < terminalFallSpeed) vy = terminalFallSpeed;
				y += vy;
				handleVerticalPlatforms(platforms, platformCount);
			}
			if (dashTimer <= 0) {
				if (grounded) state = NINJA_IDLE;
				else state = NINJA_JUMP;
			}
			if (y < deathY) die();
			return;
		}
		// ATTACK
		if (isAttackState()) {
			updateAttack();
			if (state == NINJA_AIR_THROW) {
				applyHorizontalInput(moveDirection, sprintHeld);
				x += vx;
				vx *= airDrag;
				if (downHeld) {
					vy -= fastFallAcceleration;
					if (vy < fastFallMaxSpeed) vy = fastFallMaxSpeed;
				}
				else {
					vy -= gravity;
					if (vy < terminalFallSpeed) vy = terminalFallSpeed;
				}
				y += vy;
				clampWorldX(worldWidth);
				handleVerticalPlatforms(platforms, platformCount);
			}
			if (y < deathY) die();
			return;
		}
		// COYOTE TIME
		if (grounded) {
			coyoteTimer = coyoteMax;
		}
		else if (coyoteTimer > 0) {
			coyoteTimer--;
		}
		// CROUCH OR HORIZONTAL INPUT
		if (grounded && downHeld) {
			state = NINJA_CROUCH;
			vx *= groundFriction;
			if (absFloat(vx) < 0.15f) vx = 0.0f;
		}
		else {
			applyHorizontalInput(moveDirection, sprintHeld);
		}
		// HORIZONTAL MOVEMENT
		x += vx;
		clampWorldX(worldWidth);
		// WALK OFF PLATFORM
		if (grounded && !hasPlatformSupport(platforms, platformCount)) {
			grounded = false;
			state = NINJA_JUMP;
			jumpAnimTimer = 0;
		}
		// AIR PHYSICS
		if (!grounded) {
			if (downHeld) {
				vy -= fastFallAcceleration;
				if (vy < fastFallMaxSpeed) vy = fastFallMaxSpeed;
			}
			else {
				vy -= gravity;
				if (vy < terminalFallSpeed) vy = terminalFallSpeed;
			}
			y += vy;
			handleVerticalPlatforms(platforms, platformCount);
			jumpAnimTimer++;
		}
		// JUMP BUFFER
		if (grounded && jumpBufferTimer > 0) {
			performGroundJump();
			jumpBufferTimer = 0;
		}
		// LAND ANIMATION
		if (state == NINJA_LAND) {
			landTimer++;
			if (landTimer >= 4) {
				if (moveDirection != 0) state = NINJA_RUN;
				else state = NINJA_IDLE;
			}
		}
		// IDLE / RUN
		if (grounded && state != NINJA_LAND && state != NINJA_CROUCH) {
			if (absFloat(vx) > 0.30f) state = NINJA_RUN;
			else state = NINJA_IDLE;
		}
		if (y < deathY) die();
	}

	// HORIZONTAL INPUT

	void applyHorizontalInput(int moveDirection, bool sprintHeld) {
		float maxSpeed = sprintHeld ? sprintMaxSpeed : runMaxSpeed;
		float acceleration = grounded ? runAcceleration : airAcceleration;
		if (moveDirection < 0) {
			vx -= acceleration;
			if (vx < -maxSpeed) vx = -maxSpeed;
			facingRight = false;
		}
		else if (moveDirection > 0) {
			vx += acceleration;
			if (vx > maxSpeed) vx = maxSpeed;
			facingRight = true;
		}
		else {
			if (grounded) {
				vx *= groundFriction;
				if (absFloat(vx) < 0.15f) vx = 0.0f;
			}
			else {
				vx *= airDrag;
			}
		}
	}

	// WORLD X LIMIT

	void clampWorldX(float worldWidth) {
		if (x < 0.0f) {
			x = 0.0f;
			if (vx < 0.0f) vx = 0.0f;
		}
		if (x > worldWidth - DRAW_W) {
			x = worldWidth - DRAW_W;
			if (vx > 0.0f) vx = 0.0f;
		}
	}

	// PLATFORM COLLISION

	void handleVerticalPlatforms(PlatformLine platforms[], int platformCount) {
		RectF box = getHitbox();
		// UNDERSIDE COLLISION
		if (vy > 0.0f) {
			float head = y + HEAD_OFFSET;
			float previousHead = head - vy;
			for (int i = 0; i < platformCount; i++) {
				bool horizontal = box.x + box.w > platforms[i].x && box.x < platforms[i].x + platforms[i].width;
				bool crossed = previousHead <= platforms[i].y && head >= platforms[i].y;
				if (horizontal && crossed) {
					y = platforms[i].y - HEAD_OFFSET - 1.0f;
					vy = -2.0f;
					return;
				}
			}
		}
		// LANDING
		if (vy <= 0.0f) {
			float feet = feetY();
			float previousFeet = feet - vy;
			for (int i = 0; i < platformCount; i++) {
				bool horizontal = box.x + box.w > platforms[i].x && box.x < platforms[i].x + platforms[i].width;
				bool crossed = previousFeet >= platforms[i].y && feet <= platforms[i].y;
				if (horizontal && crossed) {
					y = platforms[i].y - FOOT_OFFSET;
					vy = 0.0f;
					grounded = true;
					jumpCount = 0;
					state = NINJA_LAND;
					landTimer = 0;
					return;
				}
			}
		}
	}

	// DRAW

	void draw(float cameraX) {
		// Invincibility blink.
		if (invincible && state != NINJA_DEAD) {
			if ((invincibleTimer / 3) % 2 == 0) {
				return;
			}
		}
		int drawX = (int)(x - cameraX);
		int drawY = (int)y;
		// DEAD
		if (state == NINJA_DEAD) {
			if (deathTimer < 12) {
				iShowImage(drawX, drawY, DRAW_W, DRAW_H, imgDeath1);
			}
			else {
				iShowImage(drawX, drawY, 110, 70, imgDeath2);
			}
			return;
		}
		// HURT
		if (state == NINJA_HURT) {
			iShowImage(drawX, drawY, DRAW_W, DRAW_H, imgHurt);
			return;
		}
		if (state == NINJA_AIR_HURT) {
			iShowImage(drawX, drawY, DRAW_W, DRAW_H, imgAirHurt);
			return;
		}
		if (state == NINJA_KNOCKBACK) {
			iShowImage(drawX, drawY, DRAW_W, DRAW_H, imgKnockback);
			return;
		}
		// DASH
		if (state == NINJA_DASH) {
			int image = facingRight ? imgJumpDiagonalRight : imgJumpDiagonalLeft;
			iShowImage(drawX, drawY, DRAW_W, DRAW_H, image);
			return;
		}
		// GROUND THROW
		if (state == NINJA_GROUND_THROW) {
			if (facingRight) {
				int frame = attackTimer / 2;
				if (frame < 0) frame = 0;
				if (frame >= RIGHT_THROW_COUNT) {
					frame = RIGHT_THROW_COUNT - 1;
				}
				iShowImage(drawX, drawY, DRAW_W, DRAW_H, rightThrow[frame]);
			}
			else {
				int image = attackTimer < 7 ? imgThrowWindupLeft : imgThrowReleaseLeft;
				iShowImage(drawX, drawY, DRAW_W, DRAW_H, image);
			}
			return;
		}
		// AIR THROW
		if (state == NINJA_AIR_THROW) {
			int image;
			if (facingRight) {
				image = attackTimer < 3 ? imgAirThrowWindupRight : imgAirThrowReleaseRight;
			}
			else {
				image = attackTimer < 3 ? imgThrowWindupLeft : imgThrowReleaseLeft;
			}
			iShowImage(drawX, drawY, DRAW_W, DRAW_H, image);
			return;
		}
		// CROUCH
		if (state == NINJA_CROUCH) {
			int image = facingRight ? imgCrouchRight : imgCrouchLeft;
			iShowImage(drawX, drawY, DRAW_W, DRAW_H, image);
			return;
		}
		// LAND
		if (state == NINJA_LAND) {
			int image = facingRight ? imgLandRight : imgLandLeft;
			iShowImage(drawX, drawY, DRAW_W, DRAW_H, image);
			return;
		}
		// JUMP / DOUBLE JUMP
		if (state == NINJA_JUMP || state == NINJA_DOUBLE_JUMP) {
			int image;
			float speedX = absFloat(vx);
			if (facingRight) {
				if (jumpAnimTimer <= 2 && vy > 7.0f) {
					image = imgJumpLaunchRight;
				}
				else if (vy > 8.0f && speedX > 3.0f) {
					image = imgJumpDiagonalRight;
				}
				else if (vy > 3.0f) {
					image = imgJumpRiseRight;
				}
				else if (vy > -3.0f) {
					image = imgJumpApexRight;
				}
				else if (vy > -10.0f) {
					image = imgJumpFallRight;
				}
				else {
					image = imgHardFallRight;
				}
			}
			else {
				if (jumpAnimTimer <= 2 && vy > 7.0f) {
					image = imgJumpLaunchLeft;
				}
				else if (vy > 8.0f && speedX > 3.0f) {
					image = imgJumpDiagonalLeft;
				}
				else if (vy > 2.0f) {
					image = imgJumpRiseLeft;
				}
				else if (vy > -9.0f) {
					image = imgJumpFallLeft;
				}
				else {
					image = imgHardFallLeft;
				}
			}
			iShowImage(drawX, drawY, DRAW_W, DRAW_H, image);
			return;
		}
		// RUN
		if (state == NINJA_RUN) {
			int image = facingRight ? runRight[runFrame] : runLeft[runFrame];
			iShowImage(drawX, drawY, DRAW_W, DRAW_H, image);
			runTimer++;
			if (runTimer >= 2) {
				runFrame = 1 - runFrame;
				runTimer = 0;
			}
			return;
		}
		// IDLE
		{
			int image = facingRight ? imgIdleRight : imgIdleLeft;
			iShowImage(drawX, drawY, DRAW_W, DRAW_H, image);
		}
	}
};

// WEAPON EFFECT

struct WeaponEffect {
	bool active;
	int image;
	float x, y;
	int width, height;
	int timer, maxTimer;
};

// PROJECTILE

struct ShurikenProjectile {
	bool active;
	int type;
	float x, y;
	float vx;
	int width, height;
	int damage;
	int lifeTimer, effectTimer;
};

// SHURIKEN SYSTEM

struct ShurikenSystem {
	static const int MAX_PROJECTILES = 16;
	static const int MAX_EFFECTS = 28;
	ShurikenProjectile projectile[MAX_PROJECTILES];
	WeaponEffect effect[MAX_EFFECTS];
	int imgShuriken[4];
	int imgSlashSmall, imgImpact, imgSmoke, imgSlashLarge, imgShockRing, imgSpeedTrail;
	int cooldown[4];
	void load() {
		imgShuriken[SHURIKEN_STANDARD] = iLoadImage("Images\\Weapons\\Shuriken\\s-01.png");
		imgShuriken[SHURIKEN_FAST] = iLoadImage("Images\\Weapons\\Shuriken\\s-02.png");
		imgShuriken[SHURIKEN_HEAVY] = iLoadImage("Images\\Weapons\\Shuriken\\s-03.png");
		imgShuriken[SHURIKEN_CRIMSON] = iLoadImage("Images\\Weapons\\Shuriken\\s-04.png");
		imgSlashSmall = iLoadImage("Images\\Weapons\\Shuriken\\s-05.png");
		imgImpact = iLoadImage("Images\\Weapons\\Shuriken\\s-06.png");
		imgSmoke = iLoadImage("Images\\Weapons\\Shuriken\\s-07.png");
		imgSlashLarge = iLoadImage("Images\\Weapons\\Shuriken\\s-08.png");
		imgShockRing = iLoadImage("Images\\Weapons\\Shuriken\\s-09.png");
		imgSpeedTrail = iLoadImage("Images\\Weapons\\Shuriken\\s-10.png");
	}

	void reset() {
		int i;
		for (i = 0; i < 4; i++)
			cooldown[i] = 0;
		for (i = 0; i < MAX_PROJECTILES; i++) {
			projectile[i].active = false;
		}
		for (i = 0; i < MAX_EFFECTS; i++) {
			effect[i].active = false;
		}
	}

	bool canFire(int type) {
		if (type < 0 || type > 3) return false;
		return cooldown[type] <= 0;
	}

	bool fire(int type, NinjaPlayer &ninja) {
		if (!canFire(type)) return false;
		int slot = -1;
		for (int i = 0; i < MAX_PROJECTILES; i++) {
			if (!projectile[i].active) {
				slot = i;
				break;
			}
		}
		if (slot < 0) return false;
		ShurikenProjectile &p = projectile[slot];
		p.active = true;
		p.type = type;
		p.lifeTimer = 0;
		p.effectTimer = 0;
		if (ninja.facingRight) {
			p.x = ninja.x + 66.0f;
		}
		else {
			p.x = ninja.x - 12.0f;
		}
		p.y = ninja.y + 42.0f;
		if (type == SHURIKEN_STANDARD) {
			p.width = 34;
			p.height = 34;
			p.damage = 20;
			p.vx = ninja.facingRight ? 17.0f : -17.0f;
			cooldown[type] = 10;
		}
		else if (type == SHURIKEN_FAST) {
			p.width = 30;
			p.height = 30;
			p.damage = 13;
			p.vx = ninja.facingRight ? 26.0f : -26.0f;
			cooldown[type] = 7;
		}
		else if (type == SHURIKEN_HEAVY) {
			p.width = 46;
			p.height = 46;
			p.damage = 35;
			p.vx = ninja.facingRight ? 13.0f : -13.0f;
			cooldown[type] = 17;
		}
		else {
			p.width = 48;
			p.height = 48;
			p.damage = 50;
			p.vx = ninja.facingRight ? 21.0f : -21.0f;
			cooldown[type] = 28;
		}
		return true;
	}

	RectF getProjectileHitbox(int index) {
		RectF box;
		box.x = projectile[index].x + 4.0f;
		box.y = projectile[index].y + 4.0f;
		box.w = projectile[index].width - 8.0f;
		box.h = projectile[index].height - 8.0f;
		return box;
	}

	bool checkEnemyCollision(RectF enemyBox, int &damageOut, int &typeOut) {
		for (int i = 0; i < MAX_PROJECTILES; i++) {
			if (!projectile[i].active) continue;
			RectF projectileBox = getProjectileHitbox(i);
			if (rectOverlap(projectileBox, enemyBox)) {
				damageOut = projectile[i].damage;
				typeOut = projectile[i].type;
				float hitX = projectile[i].x + projectile[i].width / 2.0f;
				float hitY = projectile[i].y + projectile[i].height / 2.0f;
				spawnImpactForType(projectile[i].type, hitX, hitY);
				projectile[i].active = false;
				return true;
			}
		}
		return false;
	}

	void spawnEffect(int image, float x, float y, int width, int height, int life) {
		for (int i = 0; i < MAX_EFFECTS; i++) {
			if (!effect[i].active) {
				effect[i].active = true;
				effect[i].image = image;
				effect[i].x = x;
				effect[i].y = y;
				effect[i].width = width;
				effect[i].height = height;
				effect[i].timer = 0;
				effect[i].maxTimer = life;
				return;
			}
		}
	}

	void spawnImpactForType(int type, float x, float y) {
		if (type == SHURIKEN_STANDARD) {
			spawnEffect(imgImpact, x - 35, y - 35, 70, 70, 7);
		}
		else if (type == SHURIKEN_FAST) {
			spawnEffect(imgShockRing, x - 50, y - 50, 100, 100, 8);
		}
		else if (type == SHURIKEN_HEAVY) {
			spawnEffect(imgSlashLarge, x - 60, y - 60, 120, 120, 9);
			spawnEffect(imgImpact, x - 35, y - 35, 70, 70, 6);
		}
		else {
			spawnEffect(imgSmoke, x - 60, y - 60, 120, 120, 12);
			spawnEffect(imgShockRing, x - 65, y - 65, 130, 130, 9);
			spawnEffect(imgSlashLarge, x - 60, y - 60, 120, 120, 8);
		}
	}

	void update(float worldWidth) {
		int i;
		for (i = 0; i < 4; i++) {
			if (cooldown[i] > 0) cooldown[i]--;
		}
		for (i = 0; i < MAX_PROJECTILES; i++) {
			if (!projectile[i].active) continue;
			ShurikenProjectile &p = projectile[i];
			p.x += p.vx;
			p.lifeTimer++;
			p.effectTimer++;
			if (p.type == SHURIKEN_FAST && p.effectTimer >= 2) {
				p.effectTimer = 0;
				float trailX = p.vx > 0 ? p.x - 55 : p.x + 15;
				spawnEffect(imgSpeedTrail, trailX, p.y - 8, 72, 45, 3);
			}
			else if (p.type == SHURIKEN_HEAVY && p.effectTimer >= 4) {
				p.effectTimer = 0;
				spawnEffect(imgSlashSmall, p.x - 22, p.y - 22, 70, 70, 4);
			}
			else if (p.type == SHURIKEN_CRIMSON && p.effectTimer >= 2) {
				p.effectTimer = 0;
				float trailX = p.vx > 0 ? p.x - 62 : p.x + 18;
				spawnEffect(imgSpeedTrail, trailX, p.y - 12, 88, 55, 4);
			}
			if (p.x < -120.0f || p.x > worldWidth + 120.0f || p.lifeTimer > 180) {
				p.active = false;
			}
		}
		for (i = 0; i < MAX_EFFECTS; i++) {
			if (!effect[i].active) continue;
			effect[i].timer++;
			if (effect[i].timer >= effect[i].maxTimer) {
				effect[i].active = false;
			}
		}
	}

	void draw(float cameraX) {
		int i;
		for (i = 0; i < MAX_EFFECTS; i++) {
			if (!effect[i].active) continue;
			iShowImage((int)(effect[i].x - cameraX), (int)effect[i].y, effect[i].width, effect[i].height, effect[i].image);
		}
		for (i = 0; i < MAX_PROJECTILES; i++) {
			if (!projectile[i].active) continue;
			iShowImage((int)(projectile[i].x - cameraX), (int)projectile[i].y, projectile[i].width, projectile[i].height, imgShuriken[projectile[i].type]);
		}
	}
};

// PLATFORM-ONLY WORLD

struct MapGrid {
	static const int PLATFORM_COUNT = 22;
	float worldWidth, deathY;
	PlatformLine platforms[PLATFORM_COUNT];
	void reset() {
		worldWidth = 3000.0f;
		deathY = -160.0f;
		// START
		platforms[0].x = 0;
		platforms[0].y = 90;
		platforms[0].width = 340;
		platforms[1].x = 410;
		platforms[1].y = 165;
		platforms[1].width = 180;
		platforms[2].x = 660;
		platforms[2].y = 255;
		platforms[2].width = 180;
		platforms[3].x = 880;
		platforms[3].y = 125;
		platforms[3].width = 220;
		platforms[4].x = 170;
		platforms[4].y = 290;
		platforms[4].width = 170;
		platforms[5].x = 430;
		platforms[5].y = 385;
		platforms[5].width = 190;
		platforms[6].x = 720;
		platforms[6].y = 480;
		platforms[6].width = 210;
		platforms[7].x = 520;
		platforms[7].y = 225;
		platforms[7].width = 90;
		platforms[8].x = 345;
		platforms[8].y = 335;
		platforms[8].width = 90;
		// SCROLLING LEVEL
		platforms[9].x = 1090;
		platforms[9].y = 215;
		platforms[9].width = 190;
		platforms[10].x = 1350;
		platforms[10].y = 320;
		platforms[10].width = 190;
		platforms[11].x = 1600;
		platforms[11].y = 170;
		platforms[11].width = 220;
		platforms[12].x = 1880;
		platforms[12].y = 285;
		platforms[12].width = 180;
		platforms[13].x = 2130;
		platforms[13].y = 390;
		platforms[13].width = 210;
		platforms[14].x = 2410;
		platforms[14].y = 230;
		platforms[14].width = 220;
		platforms[15].x = 1160;
		platforms[15].y = 465;
		platforms[15].width = 160;
		platforms[16].x = 1660;
		platforms[16].y = 490;
		platforms[16].width = 180;
		platforms[17].x = 2070;
		platforms[17].y = 115;
		platforms[17].width = 190;
		platforms[18].x = 2650;
		platforms[18].y = 350;
		platforms[18].width = 180;
		platforms[19].x = 2800;
		platforms[19].y = 180;
		platforms[19].width = 170;
		platforms[20].x = 2550;
		platforms[20].y = 500;
		platforms[20].width = 170;
		platforms[21].x = 2870;
		platforms[21].y = 470;
		platforms[21].width = 120;
	}

	void draw(float cameraX) {
		iSetColor(190, 190, 190);
		for (int i = 0; i < PLATFORM_COUNT; i++) {
			float screenX = platforms[i].x - cameraX;
			if (screenX > -platforms[i].width && screenX < SCREEN_WIDTH + 50) {
				iFilledRectangle((int)screenX, (int)platforms[i].y - 4, (int)platforms[i].width, 8);
			}
		}
	}
};

#endif
