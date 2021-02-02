//Author: Leah Moser
//Date : 10/21/20
//Homework 2 - CS300 - Fall 2020
//This is the implentation file for the the game Conways Game of Life 
//This program will implor the use of ncurses library to create this game
//using the glider method.



#include <iostream>
#include <ncurses.h>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <vector>
#include <thread>
#define ALIVE "X"
#define DEAD " "
#define QUIT (int)'q'
#define PANDP (int) 'p'
#define STEP (int)'n'

using namespace std;

void  search(vector<vector<int>>&);
void draw(vector<vector<int>>&);
void init_world(vector<vector<int>>&);
int pause(vector<vector<int>>&, int &, int &, int &);
int play(vector<vector<int>>&);
const int SIZE = COLS * LINES;
const float FPS  = 0.5;


int main()
{
	initscr();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	int x = 0;
	int y = 0;
	int c;
	vector<vector<int>> world1; //original world

	init_world(world1);//initalize screen to all zeros

	c = getch(); //user input
	//game loop
	do{
		c = pause(world1, x, y, c);
		if(c == PANDP)
			c = play(world1);
	}while(c != QUIT && c == PANDP);
	endwin();
	return 0;
}

//pause screen where the user inputs x's and can toggle the buttons
int pause(vector<vector<int>>& world1, int &x, int &y, int &c)
{
	nodelay(stdscr, FALSE);

	do //user input pause loop
	{	switch(c)
		{	
			case KEY_UP: //move up
				if(y == 0)
				{
					y= 1;

				}
				else
					--y;
				break;
			case KEY_DOWN: //move down
				if(y == LINES) {
					y = LINES -1;
				}
				else
					++y;
				break;
			case KEY_LEFT: //move left
				if(x == 0)
				{
					x = 1;
				}
				else 
					--x;
				break;
			case KEY_RIGHT: //move right
				if(x == COLS)
				{
					x = COLS -1;
				}
				else
					++x;
				break;
			case (int)'x': //adding the x's and placing into vector in real time
				if(world1[y][x] == 0)
					world1[y][x] = 1; 
				else
					world1[y][x] = 0;
				c = 0;
				break;
			case QUIT: //quit the game
				break;
			case STEP: //one step through the vector at a time
				search(world1);
				break;

		}
		draw(world1); //draw the new world
		move(y,x);//move the cursor
		c = mvgetch(y,x); //grab the new spot
		curs_set(1);//place cursor in prior postition
	}while(c != QUIT && c != PANDP);
	return c;	
}
//this is the loop that continuously will play
int play(vector<vector<int>> &world1)
{
	nodelay(stdscr, TRUE);
	int c;
	do{
		curs_set(0); //set cursor to previous place
		search(world1);
		int seconds = 1000 * FPS;
		this_thread::sleep_for(chrono::milliseconds(seconds)); //timing for the game
		draw(world1);
		curs_set(0); //set cursor to previous place
		refresh(); 
		c = getch();
	}while(c != PANDP && c != QUIT);
	return c;
}
//this is the function in charge of the game rules
void search(vector<vector<int>> & old_world)
{
	curs_set(0);
	//blank vector to use for updates
	vector<vector<int>> future;
	for (int l = 0; l < LINES- 1; ++l) 
	{
		//temp vector to use for updates
		vector<int> temp;

		for (int m = 0; m < COLS - 1;++m) 
		{ 
			// finding number of neighbors that are alive 
			int alivefriends = 0; 
			if(l == 0 || l == LINES-1 || m == 0 || m == COLS-1)
			{
				temp.push_back(0);
			}
			else
			{
				for (int i = -1; i < 2; ++i) 
					for (int j = -1; j < 2; ++j) 
						alivefriends+= old_world[l+i][m+j]; 

				// The cell needs to be subtracted from 
				// its neighbors as it was counted before 
				alivefriends -= old_world[l][m]; 
				/*Implementing the Rules of Life*/

				// Cell is lonely and dies 
				if ((old_world[l][m] == 1) && (alivefriends < 2)) 
					temp.push_back(0); 

				// Cell dies due to over population 
				else if ((old_world[l][m] == 1) && (alivefriends > 3)) 
					temp.push_back(0); 

				// A new cell is born 
				else if ((old_world[l][m] == 0) && (alivefriends == 3)) 
					temp.push_back(1); 

				// Remains the same 
				else
					//place changes into temp vector
					temp.push_back(old_world[l][m]); 
			}
		} 
		//place into new blank 2d vector
		future.push_back(temp);
	} 
	//update the old vector to new
	old_world = future;


}
//initialize vectors to zero
void init_world(vector<vector<int>> &start_world)
{
	int x, y;
	for(y = 0; y < LINES; ++y)
	{
		vector<int> temp;
		for(x = 0; x< COLS; ++x)
		{
			temp.push_back(0);
		}

		start_world.push_back(temp);
	}
}
//draw the worlds
void draw(vector<vector<int>> &world1)
{
	for(int y = 0; y < LINES; ++y)
	{
		for(int x = 0; x < COLS; ++x)
		{
			if(world1[y][x] ==1)
			{
				mvprintw(y,x,ALIVE);
			}
			else
			{
				mvprintw(y,x,DEAD);
			}
		}

	}
}

