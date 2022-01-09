#include "Minesweeper.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
using std::string;

Minesweeper::Minesweeper(string fileName)
{	
	flagCount = 0;
	std::ifstream filereader(fileName);
	filereader >>  col;
	filereader >> row;
	filereader >> mines;
	fieldLose = false;
	fieldWin = false;
	leftClick = false;
	//rightClick = false;

	for (int i = 1; i < 9; ++i)
	{
		numberTextures.push_back(sf::Texture());
		numberTextures[i-1].loadFromFile("./images/number_" + std::to_string(i) + ".png");
	}



	mineTexture.loadFromFile("./images/mine.png");
	flagTexture.loadFromFile("./images/flag.png");
	resetTexture.loadFromFile("./images/face_happy.png");
	resetSprite.setTexture(resetTexture);
	resetSprite.setPosition((float)col * 16, (float)row * 32);

	for (int i = 0; i < 3; ++i)
	{
		digitTexture[i].loadFromFile("./images/digits.png");
		digitSprite[i].setTexture(digitTexture[i]);
		digitSprite[i].setTextureRect(sf::IntRect(0, 0, 21, 32));
		digitSprite[i].setPosition((float)21*i, (float)row * 32);

		testTexure[i].loadFromFile("./images/test_" + std::to_string(i+1) + ".png");
		testSprite[i].setTexture(testTexure[i]);
		testSprite[i].setPosition((float)(col*32 -64 - 64*i),(float)row * 32);
	}
	debugTexture.loadFromFile("./images/debug.png");
	debugSprite.setTexture(debugTexture);
	debugSprite.setPosition((float)(col * 32 - 256), (float)row * 32);
	

	developerMode = false;
	//create points
	for (int i = 0; i < col; i++)
	{
		for (int j = 0; j < row; ++j)
		{
			tiles[point(i, j)];
		}
	}
	for (int i = 0; i < mines; i++)
	{
		tile& currTile = tiles[point(rand() % col ,rand()% row)];
		if (currTile.hasMine)
		{
			--i;
		}
		else
		{
			currTile.hasMine = true;
		}
	}

	for (auto iter = tiles.begin(); iter != tiles.end(); ++iter)
	{
		iter->second.tileSprite.setPosition(float(iter->first.x * 32), float(iter->first.y * 32));

		iter->second.mineSprite.setTexture(mineTexture);
		iter->second.flagSprite.setTexture(flagTexture);

		iter->second.mineSprite.setPosition(float(iter->first.x * 32),float(iter->first.y * 32) );
		iter->second.flagSprite.setPosition(float(iter->first.x * 32), float(iter->first.y * 32));
		iter->second.numberSprite.setPosition(float(iter->first.x * 32), float(iter->first.y * 32));
	}

	
	//populate neighbors
	for (auto iter = tiles.begin(); iter != tiles.end(); ++iter)
	{
		int x = iter->first.x;
		int y = iter->first.y;
		int k = 0;
		for (int i = -1; i < 2; ++i)
		{
			for (int j = -1; j < 2; ++j)
			{
				if (col > i + x && i + x >= 0 && row > j + y && j + y >= 0 && !(i ==0 && j==0))
				{
					iter->second.neighbors[k] = &tiles[point(i+x, j+y)];
					++k;
					
					if (tiles[point(i + x, j + y)].hasMine)
					{
						iter->second.mineCount++;
					}
				}

			}
		}
		
	}
	
	
	for (auto iter = tiles.begin(); iter != tiles.end(); ++iter)
	{
		if (iter->second.mineCount > 0)
		{
			iter->second.numberSprite.setTexture(numberTextures[iter->second.mineCount - 1]);
		}
	}
}
void Minesweeper::resetBoard()
{
	for (auto iter = tiles.begin(); iter != tiles.end(); ++iter)
	{
		tile& current = iter->second;
		current.isLeftClicked = false;
		current.hasMine = false;
		current.isRightClicked = false;
		current.tileLose = false;
		current.tileTexture.loadFromFile("./images/tile_hidden.png");
		current.tileWin = false;
		current.mineCount = 0;
	}
	resetTexture.loadFromFile("./images/face_happy.png");
	fieldLose = false;
	fieldWin = false;
}
void Minesweeper::populateBoard(string fileName)
{
	char read = 0;
	std::ifstream fileReader(fileName);
	
	for (int i = 0; i < row; i++)
	{
		for (char j = 0; j < col; j++)
		{
			fileReader >> read;
			//std::cout << (read)<<'\n';

			if ((read & 1) == 1)
			{
				tiles[point(i,j)].hasMine = true;
			}
		}
	}
	for (auto iter = tiles.begin(); iter != tiles.end(); ++iter)
	{
		tile& currentTile = iter->second;
		for (int i = 0; i < 8; i++)
		{
			if (currentTile.neighbors[i] != nullptr && currentTile.neighbors[i]->hasMine)
			{
				currentTile.mineCount++;
			}
		}
	}
	for (auto iter = tiles.begin(); iter != tiles.end(); ++iter)
	{
		if (iter->second.mineCount > 0)
		{
			iter->second.numberSprite.setTexture(numberTextures[iter->second.mineCount - 1]);
		}
	}
}
void Minesweeper::populateBoard()
{
	for (int i = 0; i < mines; i++)
	{
		tile& currTile = tiles[point(rand() % col, rand() % row)];
		if (currTile.hasMine)
		{
			--i;
		}
		else
		{
			currTile.hasMine = true;
		}
	}
	for (auto iter = tiles.begin(); iter != tiles.end(); ++iter)
	{
		tile& currentTile = iter->second;
		for (int i = 0; i < 8; i++)
		{
			if (currentTile.neighbors[i] != nullptr && currentTile.neighbors[i]->hasMine)
			{
				currentTile.mineCount++;
			}
		}
	}
	for (auto iter = tiles.begin(); iter != tiles.end(); ++iter)
	{
		if (iter->second.mineCount > 0)
		{
			iter->second.numberSprite.setTexture(numberTextures[iter->second.mineCount - 1]);
		}
	}
}

void Minesweeper::recursiveReveal(tile& selected)
{
	if (selected.mineCount == 0 && !selected.isLeftClicked &&!selected.isRightClicked)
	{
		selected.isLeftClicked = true;

		for (int i = 0; i < 8; ++i)
		{
			if (selected.neighbors[i] != nullptr && selected.neighbors[i]->mineCount == 0)
			{
				recursiveReveal(*(selected.neighbors[i]));
			}
		}
	}
}

void Minesweeper::drawTiles(sf::RenderWindow& window)
{
	int leftClickCount = 0;
	int mineCount = 0;
	for (auto iter = tiles.begin(); iter != tiles.end(); ++iter)
	{
		tile& current = iter->second;

		//Tile
		if (current.isLeftClicked && !current.isRightClicked)
		{
			current.tileTexture.loadFromFile("./images/tile_revealed.png");
			if (current.mineCount == 0)
			{
				for (int i = 0; i < 8; ++i)
				{
					if (current.neighbors[i] != nullptr && current.neighbors[i]->mineCount == 0)
					{
						recursiveReveal(*(current.neighbors[i]));
					}
				}
			}
		}
		window.draw(current.tileSprite);
		//Number
		if (current.isLeftClicked && current.mineCount > 0 && !current.hasMine)
		{
			window.draw(current.numberSprite);
		}

		//Flag
		if (current.isRightClicked)
		{
			window.draw(current.flagSprite);
			++flagCount;
		}
		//win check
		if (current.isLeftClicked)
		{
			leftClickCount++;
		}
		//Mine
		if (current.hasMine && (current.isLeftClicked || developerMode || fieldLose))
		{
			window.draw(current.mineSprite);
			if (current.isLeftClicked)
			{
				current.tileLose = true;
				fieldLose = true;
			}
		}
		if (current.hasMine)
		{
			++mineCount;
		}
		//loss propagation
		if (fieldLose)
		{
			current.tileLose = true;
		}
		//win propagation
		if (fieldWin)
		{
			current.tileWin = true;
		}

	}
	//Mine counter
	int mineAdjusted = mineCount - flagCount;
	bool isNegative = false;
	if (mineAdjusted < 0)
	{
		isNegative = true;
		mineAdjusted *= -1;
	}
	
	for (int i = 2; i >= 0; --i)
	{
		if (i == 0 && isNegative)
		{
			digitSprite[0].setTextureRect(sf::IntRect(210, 0, 21, 32));
			window.draw(digitSprite[i]);
		}
		else
		{
			digitSprite[i].setTextureRect(sf::IntRect(21 * ((mineAdjusted) % 10), 0, 21, 32));
			mineAdjusted /= 10;
			window.draw(digitSprite[i]);
		}
	}
	if (leftClickCount + mineCount == col * row)
	{
		fieldWin = true;
	}
	//loss condition
	if (fieldLose)
	{
		resetTexture.loadFromFile("./images/face_lose.png");
	}

	if (leftClicked(resetSprite, window))
	{
		resetBoard();
		populateBoard();
	}
	if (fieldWin)
	{
		resetTexture.loadFromFile("./images/face_win.png");
	}
	window.draw(resetSprite);   
	flagCount = 0;
	for (int i = 0; i < 3; ++i)
	{
		if (leftClicked(testSprite[i], window))
		{
			resetBoard();
			populateBoard("./boards/testboard"+ std::to_string(i+1)+".brd");
		}
		window.draw(testSprite[i]);
	}
	window.draw(debugSprite);
	if (leftClicked(debugSprite, window))
	{
		developerMode = !developerMode;
	}
}

Minesweeper::tile::tile()
{
	tileTexture.loadFromFile("./images/tile_hidden.png");
	tileSprite.setTexture(tileTexture);
	mineCount = 0;
	isLeftClicked = false;
	hasMine = false;
	isRightClicked = false;
	tileLose = false;
	tileWin = false;
	for (int i = 0; i < 8; ++i)
	{
		neighbors[i] = nullptr;
	}
}

//::tile& Minesweeper::tile::operator=( tile& rhs)

bool Minesweeper::leftClicked(sf::Sprite current, sf::RenderWindow& window)
{
	sf::Mouse mouse;
	if (leftClick && 
		current.getGlobalBounds().contains(float(mouse.getPosition(window).x), float(mouse.getPosition(window).y)))
	{
		return true;

	}
	return false;
}

void Minesweeper::tile::LeftClicked(sf::RenderWindow& window)
{
	sf::Mouse mouse;
	if (!tileWin && !tileLose && !isRightClicked && mouse.isButtonPressed(sf::Mouse::Button::Left) &&
		tileSprite.getGlobalBounds().contains(float(mouse.getPosition(window).x), float(mouse.getPosition(window).y)))
	{
		isLeftClicked = true;

	}
}

void Minesweeper::tile::RightClicked(sf::RenderWindow& window)
{
	sf::Mouse mouse;
	if (!tileWin && !tileLose && !isLeftClicked && mouse.isButtonPressed(sf::Mouse::Button::Right) &&
		tileSprite.getGlobalBounds().contains(float(mouse.getPosition(window).x), float(mouse.getPosition(window).y)))
	{
		isRightClicked = !isRightClicked;
	}
}



bool Minesweeper::point::operator<(const point& rhs)const
{
	if (x < rhs.x)
	{
		return true;
	}
	else if (x == rhs.x && y < rhs.y)
	{
		return true;
	}
	return false;
}
