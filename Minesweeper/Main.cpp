#include <SFML/Graphics.hpp>
#include "Minesweeper.h"
#include <iostream>
#include <fstream>
using std::cout;
using std::map;
void Play();


int main()
{

    Play();

 
    return 0;
}

void Play()
{
 
    Minesweeper sweep("./boards/config.cfg");
    
    sf::RenderWindow window(sf::VideoMode(sweep.col *32, sweep.row*32+88), "Minesweeper");
    

    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Button::Left)
                {
                    sweep.leftClick = true;
                }
                else if (event.mouseButton.button == sf::Mouse::Button::Right)
                {
                    sweep.rightClick = true;
                }
            }
               
                
            
            for (auto iter = sweep.tiles.begin(); iter != sweep.tiles.end(); ++iter)
            {
                iter->second.LeftClicked(window);
                iter->second.RightClicked(window);
                
            }
           
        }

        window.clear();
        sweep.drawTiles(window);


        window.display();
        sweep.leftClick = false;
        //sweep.rightClick = false;
    }
}

