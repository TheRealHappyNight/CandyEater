#include "fruitSpawner.h"
#include "Utilities.h"
#include <random>


fruitSpawner::fruitSpawner()
{
	this->top = -1;
	this->size = 0;
	this->fruits = NULL;
}

fruitSpawner::fruitSpawner(const gFruit &obj) {

}

void fruitSpawner::Append(gFruit fruit) {
	int x = rand() % (wWidth - fruit.getTextureWitdh());
	int y = -fruit.getTextureHeight();
	if (size == 0) {
		top = 0;
		size = 5;
		fruits = new gFruit*[size];
		fruits[top] = new gFruit(fruit.getTexture(), x, y, fruit.getTextureWitdh(), fruit.getTextureHeight());
	}
	else if (top + 1 > size) {
		size *= 2;
		top++;
		gFruit **newQueue = NULL;
		newQueue = new gFruit*[size];
		for (int i = 0; i < top - 1; i++)
			(*newQueue)[i] = (*fruits)[i];
		(newQueue)[top] = new gFruit(fruit.getTexture(), x, y, fruit.getTextureWitdh(), fruit.getTextureHeight());
		
		for(int i = 0; i < top - 1; i++)
			delete fruits[i];
		delete[]fruits;

		(*fruits) = (*newQueue);
	}
	else {
		top++;
		fruits[top] = new gFruit(fruit.getTexture(), x, y, fruit.getTextureWitdh(), fruit.getTextureHeight());
	}
}

gFruit* fruitSpawner::Pop() {
	if (size == 0)
		return NULL;
	if (top - 1 < size / 2 && size >= 10) {
		gFruit *fruit = fruits[top];
		top--;
		size /= 2;

		gFruit **newQueue = NULL;
		newQueue = new gFruit*[size];
		for (int i = 0; i < top; i++)
			newQueue[i] = new gFruit(*fruits[i]);
		
		for (int i = 0; i < size * 2; i++)
			delete fruits[i];
		delete[]fruits;

		(*fruits) = (*newQueue);
		return fruit;
	}
	else {
		gFruit *fruit = new gFruit((*fruits[top]));
		(*fruits)[top] = NULL;
		top--;
		return fruit;
	}
}


fruitSpawner::~fruitSpawner()
{
	for (int i = 0; i < size; i++)
		delete fruits[i];
	delete[]fruits;
}
