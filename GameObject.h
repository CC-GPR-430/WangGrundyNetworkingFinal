#pragma once

class GameObject {
public:
	// Note that it's not easy to determine
	// how a specific memory layout within
	// a class will map to binary.

	// Importantly, especially due to alignment,
	// the same class could look different in
	// binary on two different systems.
	int x;
	int y;
	int z;
	int xVel;
	int yVel;
	int zVel;

	// Virtual functions are (basically) function pointers,
	// so they affect the size of an object and make
	// doing a simple memcpy() impossible.
	virtual void Update(float dt) {
		x++;
		y++;
	}

	int GetX() {
		return x;
	}

	void* sprite;
};