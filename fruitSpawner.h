#pragma once
#include "gFruit.h"
class fruitSpawner
{
public:
	fruitSpawner();
	fruitSpawner(const gFruit &obj);
	gFruit operator=(const gFruit &obj);
	void Append(gFruit fruit);
	gFruit* Pop();
	~fruitSpawner();
private:
	int size;
	int top;
	gFruit **fruits;
};

