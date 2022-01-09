#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
using std::string;

class Minesweeper
{
public:
	struct tile
	{
		sf::Texture tileTexture;
		sf::Sprite tileSprite;

		sf::Sprite numberSprite;
		sf::Sprite mineSprite;
		sf::Sprite flagSprite;

		tile* neighbors[8];

		bool isLeftClicked;
		bool isRightClicked;
		bool hasMine;
		bool tileLose;
		bool tileWin;
		
		//Should be only used by Minesweeper class
		int mineCount;

		tile();
		void LeftClicked(sf::RenderWindow& window);
		void RightClicked(sf::RenderWindow& window);
		
		//void tileDraw(sf::RenderWindow& window);
		//tile& operator=( tile& rhs);
		
	};
	struct point
	{
		int x;
		int y;
		point(int a, int b)
		{
			x = a;
			y = b;
		}
		bool operator<(const point& rhs)const;
	};
	sf::Texture mineTexture;
	sf::Texture flagTexture;
	int col, row, mines;
	int flagCount;
	bool developerMode;
	bool fieldLose;
	bool fieldWin;
	bool leftClick;
	//bool rightClick;
	
	std::vector<sf::Texture> numberTextures;

	std::map<point, tile> tiles;
	Minesweeper(std::string fileName);
	void drawTiles(sf::RenderWindow& window);
	void recursiveReveal(tile& selected);
	void resetBoard();
	void populateBoard();
	void populateBoard(string brdName);
	bool leftClicked(sf::Sprite current, sf::RenderWindow& window);

	sf::Texture resetTexture;
	sf::Sprite resetSprite;
	sf::Texture digitTexture[3];
	sf::Sprite digitSprite[3];
	sf::Texture testTexure[3];
	sf::Sprite testSprite[3];
	sf::Texture debugTexture;
	sf::Sprite debugSprite;

};
 
