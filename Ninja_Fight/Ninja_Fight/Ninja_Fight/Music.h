#ifndef MUSIC_H
#define MUSIC_H

#include <windows.h>
#include <mmsystem.h>
#include <cstdio>
#include <cstring>
#pragma comment(lib,"winmm.lib")

// Add your files later
// #define MUSIC_MENU_PATH "Music\\menu.mp3"
// #define MUSIC_LEVEL2_PATH "Music\\level2.wav"
#define MUSIC_LOADING_PATH ""
#define MUSIC_MENU_PATH ""
#define MUSIC_LEVEL1_PATH ""
#define MUSIC_LEVEL2_PATH ""
#define MUSIC_LEVEL3_EXPLORE_PATH ""
#define MUSIC_LEVEL3_ARENA_PATH ""

enum NinjaMusicTrack {
	MUSIC_NONE = -1,
	MUSIC_LOADING = 0,
	MUSIC_MENU,
	MUSIC_LEVEL1,
	MUSIC_LEVEL2,
	MUSIC_LEVEL3_EXPLORE,
	MUSIC_LEVEL3_ARENA,
	MUSIC_COUNT
};

struct MusicSystem {
	char path[MUSIC_COUNT][260];
	int currentTrack, volume;
	bool enabled, opened;

	void setPath(int id, const char *p) {
		if (id<0 || id >= MUSIC_COUNT)return;
		if (!p)p = "";
		strcpy_s(path[id], sizeof(path[id]), p);
		if (currentTrack == id)currentTrack = MUSIC_NONE;
	}

	void load() {
		for (int i = 0; i<MUSIC_COUNT; i++)path[i][0] = '\0';
		setPath(MUSIC_LOADING, MUSIC_LOADING_PATH); setPath(MUSIC_MENU, MUSIC_MENU_PATH);
		setPath(MUSIC_LEVEL1, MUSIC_LEVEL1_PATH); setPath(MUSIC_LEVEL2, MUSIC_LEVEL2_PATH);
		setPath(MUSIC_LEVEL3_EXPLORE, MUSIC_LEVEL3_EXPLORE_PATH); setPath(MUSIC_LEVEL3_ARENA, MUSIC_LEVEL3_ARENA_PATH);
		currentTrack = MUSIC_NONE; volume = 70; enabled = true; opened = false;
	}

	bool fileExists(const char *p) {
		if (!p || p[0] == '\0')return false;
		DWORD a = GetFileAttributesA(p); return a != INVALID_FILE_ATTRIBUTES&&!(a&FILE_ATTRIBUTE_DIRECTORY);
	}

	void closeCurrent() {
		if (opened)mciSendStringA("stop NinjaBGM", 0, 0, 0);
		mciSendStringA("close NinjaBGM", 0, 0, 0); opened = false;
	}

	void applyVolume() {
		if (!opened)return;
		char cmd[80]; sprintf_s(cmd, sizeof(cmd), "setaudio NinjaBGM volume to %d", volume * 10); mciSendStringA(cmd, 0, 0, 0);
	}

	void setVolume(int v) {
		if (v<0)v = 0; if (v>100)v = 100;
		if (volume == v)return; volume = v; applyVolume();
	}

	void setEnabled(bool on) {
		if (enabled == on)return; enabled = on;
		if (!enabled){ closeCurrent(); currentTrack = MUSIC_NONE; }
	}

	void play(int id) {
		if (id<0 || id >= MUSIC_COUNT)return;
		if (currentTrack == id)return;
		closeCurrent(); currentTrack = id;
		if (!enabled || !fileExists(path[id]))return;

		char cmd[620], *ext = strrchr(path[id], '.');
		if (ext&&_stricmp(ext, ".mp3") == 0)sprintf_s(cmd, sizeof(cmd), "open \"%s\" type mpegvideo alias NinjaBGM", path[id]);
		else sprintf_s(cmd, sizeof(cmd), "open \"%s\" alias NinjaBGM", path[id]);

		if (mciSendStringA(cmd, 0, 0, 0) != 0){ opened = false; return; }
		opened = true; applyVolume(); mciSendStringA("play NinjaBGM repeat", 0, 0, 0);
	}

	void stop() { closeCurrent(); currentTrack = MUSIC_NONE; }
	void shutdown() { stop(); }
};

#endif
