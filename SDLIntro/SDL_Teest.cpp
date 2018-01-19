//-------------------------------------------------------------------------------------------------------------------------------
//Pass the ball back and forth between players (aka pucks), and try to score onto the other player's...
//side by making the soccerball slip past them.
//Contains a clear button to erase the score, an easy and hard mode, and a start button to play another round

//For future reference this program contains use of structs, random number generators...
//implementing pictures, text, examples of passing pointers
//-------------------------------------------------------------------------------------------------------------------------------



#include <SDL.h>        
#include <SDL_image.h>
#include <iostream>
#include <math.h>
#include <SDL_ttf.h>
#include <string.h>
#include <cstdlib> 
#include <time.h>
//-----------------------------------------Getting Started with some intial Parameters--------------------------------------------


//20 distance frome the side
extern bool start = false;
using namespace std;
extern bool quit = false;
extern bool goal = false;
//difficulty 0 is easy and 1 is hard   AND SPEED CONRTROLLER AKA DELAY
extern int difficulty = 0;
extern int overAllspeed = 5;
//intial positions of pucks/ball
const int x_InitPosLeft = 20; const int y_InitPosLeft = 150; 
const int x_InitPosRight = 560; const int y_InitPosRight = 150;
const int x_InitPosBall = 280; const int y_InitPosBall = 180;
//current positions of pucks/ball
extern float y_currentLeft = y_InitPosLeft;
extern float y_currentRight = y_InitPosRight;
extern float x_currentBall = x_InitPosBall;extern float y_currentBall = y_InitPosBall;
//Change in y to determine current direction
//dySpeed is the incremental change for the position aswell as the ball
extern int dyLeft = 0; extern int dyRight = 0; extern int dyDisplacement = 2;
extern float dxBall = -2; extern float dyBall= 2;
//Keeps track of angle to ensure accuracy of ball bouncing
extern int currentAngle = 45; 
//Control
extern bool showControls = false;
//Structures---------------------------------------------------------------

typedef struct score
{
	int playerLeft;
	int playerRight;
}score;
score scoreBoard = { 0,0 };
typedef struct BoundingBox
{
	float x1; float y1;
	float x2; float y2;
}BoundingBox;
BoundingBox puckLeft = {20,0,40,0};
BoundingBox puckRight = {560,0,580,0};
typedef struct Mouse
{
	int x;
	int y;
	bool state;
}Mouse;
Mouse myMouse = { 0,0,false };
BoundingBox ball = { x_currentBall,y_currentBall,x_currentBall + 40,y_currentBall + 40 };


//-----------------------------------------------Functions-----------------------------------------------------------------------
void randomBallStart(void)
{
	srand((unsigned)time(0));
	static int i;
	i = (rand() % 4) + 1;
	cout << "Random Num: " << i << endl;
	switch (i) {
		case 1:
			dxBall = -2;//Top left Quad
			dyBall = -2;
			break;
		case 2:
			dxBall = 2;//Top right Quad
			dyBall = -2;
			break;
		case 3:
			dxBall = -2;//Bottom left Quad
			dyBall = 2;
			break;
		case 4:
			dxBall = 2;//Bottom right Quad
			dyBall = 2;
			break;
	}

}
void destroy(SDL_Surface * Backgroundimg, SDL_Surface * PuckLeftimg, SDL_Surface * PuckRightimg,
	SDL_Surface * Soccerballimg, SDL_Surface *StartButtonimg, SDL_Surface *Goalimg, 
	SDL_Surface *EasyorHardimg, SDL_Surface *Clearimg,
	SDL_Texture * BackgroundTex, SDL_Texture * PuckLeftTex, SDL_Texture * PuckRightTex, 
	SDL_Texture * SoccerballTex, SDL_Texture * StartButtonTex, SDL_Texture * GoalTex, SDL_Texture * EasyorHardTex, 
	SDL_Texture * ClearTex, SDL_Renderer * renderer, SDL_Window * window)
{
	SDL_DestroyTexture(BackgroundTex); SDL_DestroyTexture(PuckLeftTex);
	SDL_DestroyTexture(PuckRightTex); SDL_DestroyTexture(SoccerballTex);
	SDL_DestroyTexture(StartButtonTex); SDL_DestroyTexture(GoalTex);
	SDL_DestroyTexture(EasyorHardTex); SDL_DestroyTexture(ClearTex);
	SDL_FreeSurface(Backgroundimg); SDL_FreeSurface(PuckLeftimg);
	SDL_FreeSurface(PuckRightimg); SDL_FreeSurface(Soccerballimg);
	SDL_FreeSurface(StartButtonimg); SDL_FreeSurface(Goalimg);
	SDL_FreeSurface(EasyorHardimg); SDL_FreeSurface(Clearimg);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}
bool resetRound(void)
{
	//Resets the positions of the ball, the puck, the angle of the ball, the way the ball starts off moving..
	//showControls,and scoring
	if (ball.x1 <= 0 || ball.x2 >= 600) {
		cout << "reset" << endl;
		start = false;
		int leftTest = ball.x1;
		int rightTest = ball.x2;
		x_currentBall = x_InitPosBall;
		y_currentBall = y_InitPosBall;
		randomBallStart();
		currentAngle = 45;
		showControls = false;

		y_currentLeft = y_InitPosLeft;
		y_currentRight = y_InitPosRight;

		ball.x1 = x_currentBall; ball.y1 = y_currentBall;
		ball.x2 = x_currentBall + 40; ball.y2 = y_currentBall + 40;

		puckLeft.y1 = y_currentLeft; puckLeft.y2 = y_currentLeft + 100;
		puckRight.y1 = y_currentRight; puckRight.y2 = y_currentRight + 100;
		if (leftTest <= 1)
			scoreBoard.playerRight += 1;
		if(rightTest >= 599)
			scoreBoard.playerLeft += 1;
		return true;
	}
	else
		return false;
}
void buttonHandler(void)
{
	int x = myMouse.x;
	int y = myMouse.y;
	if (myMouse.state) {
		if (y < (pow((1) - (pow((x - 300), 2) / pow(100, 2)), 0.5) * 40.0 + 200)) //Elipse Function For Start
		{																		//Functions FLip top is now bottom
			if (y > (-pow((1) - (pow((x - 300), 2) / pow(100, 2)), 0.5) * 40.0 + 200))
			{
				//cout << "\nHIT\n" << endl;
				start = true;
			}
		}
		if (x > 250 && x < 300 && y>100 && y < 140) //-------------EASY MODE-------------
		{
			//cout << "EASY" << endl;
			overAllspeed = 3;
		}
		if (x > 300 && x < 350 && y>100 && y < 140)//---------------HARD MODE--------------
		{
			//cout << "HARD" << endl;
			overAllspeed = 1;
		}
		if (y < pow(pow(25, 2) - pow(x - 45, 2), .5) + 45) //Sphere Function For CLEAR 20 + 25 Function Flips
		{
			if (y > -pow(pow(25, 2) - pow(x - 45, 2), .5) + 45)
			{
				//cout << "CLEAR" << endl;
				scoreBoard.playerLeft = 0; scoreBoard.playerRight = 0;
			}
		}
	}
	if (x > 465 && x <575  && y>20 && y < 65)
	{

		if (myMouse.state)
			showControls = true;
		else
			showControls = false;
	}//465,20,110,45
}
SDL_Point mouseHandler(void)
{
	//current position is gathered of mouse during click or a general call
	static SDL_Point mouse_position;
	static bool state;
	SDL_GetMouseState(&(mouse_position.x),&(mouse_position.y));
	if (SDL_GetMouseState(&(mouse_position.x), &(mouse_position.y)) & SDL_BUTTON(SDL_BUTTON_LEFT))
		state = true;
	else
		state = false;
	if (state) {
		cout << "Mouse position: x: " <<  //    Print mouse position
			mouse_position.x << "   y:   " << mouse_position.y << endl;
	}
	myMouse.state = state;
	myMouse.x = mouse_position.x;
	myMouse.y = mouse_position.y;
	buttonHandler();
	return mouse_position;
}
void ballBounce(void) //Make dx and dy floats and then convert them to ints moving pic
{
	ball.x1 = x_currentBall; ball.y1 = y_currentBall;
	ball.x2 = x_currentBall + 40; ball.y2 = y_currentBall + 40;

	int x1 = ball.x1; int y1 = ball.y1;             //bounding box of ball, use in separate variables so it's..
	int x2 = ball.x2; int y2 = ball.y2;             //easier to reference
	int x1LP = puckLeft.x1; int y1LP = puckLeft.y1;//leftpuck bounding box
	int x2LP = puckLeft.x2; int y2LP = puckLeft.y2;
	int x1RP = puckRight.x1; int y1RP = puckRight.y1;//Right puck bounding box
	int x2RP = puckRight.x2; int y2RP = puckRight.y2;


	//floor bounce
	if ((x2 < 600 && x1>0) && (y2 >= 400) 
		&& (dxBall == abs(dxBall))) {
		dxBall = abs(dxBall);
		dyBall = -abs(dyBall);
	}
	if ((x2 < 600 && x1>0) && (y2 >= 400)
		&& (dxBall == -abs(dxBall))) {
		dxBall = -abs(dxBall);
		dyBall = -abs(dyBall);
	}
	//cieling bounce
	if ((x1 < 600 && x2> 0) && (y1 <= 0)
		&& (dxBall == abs(dxBall))) {
		dxBall = abs(dxBall);
		dyBall = abs(dyBall);
		cout << "dx is " << dxBall << endl;
		cout << "dy is " << dyBall << endl;

	}
	if ((x1 < 600 && x2>0) && (y1 <= 0)
		&& (dxBall == -abs(dxBall))) {
		dxBall = -abs(dxBall);
		dyBall = abs(dyBall);
		cout << "dx is " << dxBall << endl;
		cout << "dy is " << dyBall << endl;


	}
	//The pucks alter the angle of the ball depending on their current movement 
	//leftpuck bounce large side
	if (((y1 >= y1LP && y1 <= y2LP) || (y2 >= y1LP && y2 <= y2LP)) && (x1 <= x2LP)
		&& (dyBall == abs(dyBall)))
	{
		switch (dyLeft)
		{
			case 1:
				switch(currentAngle)
				{
					case 30:
						currentAngle = 30;
						dyBall = pow(8, .5) * sin(30 * M_PI / 180);
						dxBall = pow(8, .5) * cos(30 * M_PI / 180);
						break;
					case 45:
						currentAngle = 30;
						dyBall = pow(8, .5) * sin(30 * M_PI / 180);
						dxBall = pow(8, .5) * cos(30 * M_PI / 180);
						break;
					case 60:
						currentAngle = 45;
						dyBall = pow(8, .5) * sin(45 * M_PI / 180);
						dxBall = pow(8, .5) * cos(45 * M_PI / 180);
						break;
				}
				break;
			case -1:
				switch (currentAngle)
				{
				case 30:
					currentAngle = 45;
					dyBall = pow(8, .5) * sin(45 * M_PI / 180);
					dxBall = pow(8, .5) * cos(45 * M_PI / 180);
					break;
				case 45:
					currentAngle = 60;
					dyBall = pow(8, .5) * sin(45 * M_PI / 180);
					dxBall = pow(8, .5) * cos(45 * M_PI / 180);
					break;
				case 60:
					currentAngle = 60;
					dyBall = pow(8, .5) * sin(60 * M_PI / 180);
					dxBall = pow(8, .5) * cos(60 * M_PI / 180);
					break;
				}
				break;
			default:
				dxBall = abs(dxBall);
				dyBall = abs(dyBall);
				break;
		}
		//dxBall = abs(dxBall);
		//dyBall = abs(dyBall);
		cout << "Current Angle: " << currentAngle << endl;


	}
	if (((y1 >= y1LP && y1 <= y2LP) || (y2 >= y1LP && y2 <= y2LP)) && (x1 <= x2LP)
		&& (dyBall == -abs(dyBall)))
	{
		switch (dyLeft)
		{
		case 1:
			switch (currentAngle)
			{
			case 30:
				currentAngle = 45;
				dyBall = -pow(8, .5) * sin(30 * M_PI / 180);
				dxBall = pow(8, .5) * cos(30 * M_PI / 180);
				break;
			case 45:
				currentAngle = 60;
				dyBall = -pow(8, .5) * sin(30 * M_PI / 180);
				dxBall = pow(8, .5) * cos(30 * M_PI / 180);
				break;
			case 60:
				currentAngle = 60;
				dyBall = -pow(8, .5) * sin(45 * M_PI / 180);
				dxBall = pow(8, .5) * cos(45 * M_PI / 180);
				break;
			}
			break;
		case -1:
			switch (currentAngle)
			{
			case 30:
				currentAngle = 30;
				dyBall = -pow(8, .5) * sin(45 * M_PI / 180);
				dxBall = pow(8, .5) * cos(45 * M_PI / 180);
				break;
			case 45:
				currentAngle = 30;
				dyBall = -pow(8, .5) * sin(60 * M_PI / 180);
				dxBall = pow(8, .5) * cos(60 * M_PI / 180);
				break;
			case 60:
				currentAngle = 45;
				dyBall = -pow(8, .5) * sin(60 * M_PI/180);
				dxBall = pow(8, .5) * cos(60 * M_PI / 180);
				break;
			}
			break;
		default:
			dxBall = abs(dxBall);
			dyBall = -abs(dyBall);
			break;
		}
		//dxBall = abs(dxBall);
		//dyBall = -abs(dyBall);
		cout <<"Current Angle: "<< currentAngle << endl;


	}

	//rightpuck bounce large side
	if (((y1 >= y1RP && y1 <= y2RP) || (y2 >= y1RP && y2 <= y2RP)) && (x2 >= x1RP)
		&& (dyBall == abs(dyBall)))
	{
		switch (dyRight)
		{
		case 1:
			switch (currentAngle)
			{
			case 30:
				currentAngle = 30;
				dyBall = pow(8, .5) * sin(30 * M_PI / 180);
				dxBall = -pow(8, .5) * cos(30 * M_PI / 180);
				break;
			case 45:
				currentAngle = 30;
				dyBall = pow(8, .5) * sin(30 * M_PI / 180);
				dxBall = -pow(8, .5) * cos(30 * M_PI / 180);
				break;
			case 60:
				currentAngle = 45;
				dyBall = pow(8, .5) * sin(45 * M_PI / 180);
				dxBall = -pow(8, .5) * cos(45 * M_PI / 180);
				break;
			}
			break;
		case -1:
			switch (currentAngle)
			{
			case 30:
				currentAngle = 45;
				dyBall = pow(8, .5) * sin(45 * M_PI / 180);
				dxBall = -pow(8, .5) * cos(45 * M_PI / 180);
				break;
			case 45:
				currentAngle = 60;
				dyBall = pow(8, .5) * sin(60 * M_PI / 180);
				dxBall = -pow(8, .5) * cos(60 * M_PI / 180);
				break;
			case 60:
				currentAngle = 60;
				dyBall = pow(8, .5) * sin(60 * M_PI / 180);
				dxBall = -pow(8, .5) * cos(60 * M_PI / 180);
				break;
			}
			break;
		default:
			dxBall = -abs(dxBall);
			dyBall = abs(dyBall);
			break;
		}

		cout << "Current Angle: " << currentAngle << endl;
		//dxBall = -abs(dxBall);
		//dyBall = abs(dyBall);
	}
	if (((y1 >= y1RP && y1 <= y2RP) || (y2 >= y1RP && y2 <= y2RP)) && (x2 >= x1RP)
		&& (dyBall == -abs(dyBall)))
	{
		switch (dyRight)
		{
		case 1:
			switch (currentAngle)
			{
			case 30:
				currentAngle = 45;
				dyBall = -pow(8, .5) * sin(30 * M_PI / 180);
				dxBall = -pow(8, .5) * cos(30 * M_PI / 180);
				break;
			case 45:
				currentAngle = 60;
				dyBall = -pow(8, .5) * sin(30 * M_PI / 180);
				dxBall = -pow(8, .5) * cos(30 * M_PI / 180);
				break;
			case 60:
				currentAngle = 60;
				dyBall = -pow(8, .5) * sin(45 * M_PI / 180);
				dxBall = -pow(8, .5) * cos(45 * M_PI / 180);
				break;
			}
			break;
		case -1:
			switch (currentAngle)
			{
			case 30:
				currentAngle = 30;
				dyBall = -pow(8, .5) * sin(45 * M_PI / 180);
				dxBall = -pow(8, .5) * cos(45 * M_PI / 180);
				break;
			case 45:
				currentAngle = 30;
				dyBall = -pow(8, .5) * sin(60 * M_PI / 180);
				dxBall = -pow(8, .5) * cos(60 * M_PI / 180);
				break;
			case 60:
				currentAngle = 45;
				dyBall = -pow(8, .5) * sin(60 * M_PI / 180);
				dxBall = -pow(8, .5) * cos(60 * M_PI / 180);
				break;
			}
			break;
		default:
			dxBall = -abs(dxBall);
			dyBall = -abs(dyBall);
			break;
		}

		cout << "Current Angle: " << currentAngle << endl;
		//dxBall = -abs(dxBall);
		//dyBall = -abs(dyBall);
	}

	x_currentBall += dxBall;
	y_currentBall += dyBall;




}
void puckMovement(void)
{
	//the value of dyleft and dyRight is dependent on the key pressed, handled in keyboardFunc
	if (dyLeft == 1) {

		if ((puckLeft.y1 - dyDisplacement) >= 0)
		{
			y_currentLeft -= dyDisplacement;
		}
	}
	if (dyLeft == -1) {
		if ((puckLeft.y2 + dyDisplacement) <= 400) {
			y_currentLeft += dyDisplacement;
		}
	}
	if (dyRight == 1) {
		if ((puckRight.y1 - dyDisplacement) >= 0) {
			y_currentRight -= dyDisplacement;
		}
	}
	if (dyRight == -1) {
		if ((puckRight.y2 + dyDisplacement) <= 400) {
			y_currentRight += dyDisplacement;
		}
	}
	puckLeft.y1 = y_currentLeft; puckLeft.y2 = y_currentLeft + 100;
	puckRight.y1 = y_currentRight; puckRight.y2 = y_currentRight + 100;

}
void display(SDL_Renderer * renderer, SDL_Texture * BackgroundTex, SDL_Texture * PuckLeftTex, 
	SDL_Texture * PuckRightTex,SDL_Texture * SoccerballTex, TTF_Font * MainFont,SDL_Color * White)
{
	//displays the pucks, the ball, the background and the text
	//the rest is handled by the main
	char player1[20]; char player2[20];
	sprintf_s(player1, "%ld", scoreBoard.playerLeft);
	sprintf_s(player2, "%ld", scoreBoard.playerRight);

	SDL_RenderClear(renderer);
	SDL_Rect LeftPuckrect = {x_InitPosLeft, y_currentLeft, 20, 100 }; //x,y,w,h
	SDL_Rect RightPuckrect = {x_InitPosRight, y_currentRight, 20, 100 };
	SDL_Rect Ballrect = {x_currentBall, y_currentBall, 40, 40 };
	SDL_Surface * leftMessageimg = TTF_RenderText_Solid(MainFont, player1, *White);
	SDL_Surface * rightMessageimg = TTF_RenderText_Solid(MainFont, player2, *White);

	SDL_Texture * leftMessageTex = SDL_CreateTextureFromSurface(renderer, leftMessageimg);;
	SDL_Texture * rightMessageTex = SDL_CreateTextureFromSurface(renderer, rightMessageimg);;
	SDL_Rect leftrect = {220,350,80,30}; SDL_Rect rightrect = {300,350,80,30 };
	SDL_RenderCopy(renderer, BackgroundTex, NULL, NULL);
	SDL_RenderCopy(renderer, PuckLeftTex, NULL, &LeftPuckrect);
	SDL_RenderCopy(renderer, PuckRightTex, NULL, &RightPuckrect);
	SDL_RenderCopy(renderer, SoccerballTex, NULL, &Ballrect);
	SDL_RenderCopy(renderer, leftMessageTex, NULL, &leftrect);
	SDL_RenderCopy(renderer, rightMessageTex, NULL, &rightrect);

	if(start)
		SDL_RenderPresent(renderer);
}
bool keyboardFunc(SDL_Event  *eventTest,bool quit)
{
	SDL_PollEvent(eventTest);
	switch ((*eventTest).type)
	{
	case SDL_QUIT:
		quit = true;
		return true;
		break;
	case SDL_MOUSEBUTTONDOWN:
		mouseHandler();
		return false;
		break;
	case SDL_MOUSEBUTTONUP:
		mouseHandler();
		return false;
		break;
	case SDL_KEYDOWN:

		switch ((*eventTest).key.keysym.sym)
		{
		case SDLK_w:
			dyLeft = 1;
			break;
		case SDLK_s:
			dyLeft = -1;
			break;
		case SDLK_i:
			dyRight = 1;
			break;
		case SDLK_k:
			dyRight = -1;
			break;
		default:
			dyRight = 0;
			dyLeft = 0;
			break;
		}
		return false;
		break;

	case SDL_KEYUP:

		switch ((*eventTest).key.keysym.sym) 
		{
			case SDLK_w:
				dyLeft = 0;
				break;
			case SDLK_s:
				dyLeft = 0;
				break;
			case SDLK_i:
				dyRight = 0;
				break;
			case SDLK_k:
				dyRight = 0;
				break;
			default:
				dyRight = 0;
				dyLeft = 0;
				break;
		}
		return false;
		break;
	default :
		return false;
		break;
	}
}
int main(int argc, char ** argv)
{
	bool quit = false;
	SDL_Point mouse_position;             //    Mouse position coords
	SDL_Event eventTest;

	SDL_Init(SDL_INIT_VIDEO);            //---------Intialization of Init
	IMG_Init(IMG_INIT_PNG);
	if (TTF_Init() == -1)
		cout << "TTF_Init: " << TTF_GetError() << endl;
	else
		cout << "works" << endl;
	cout << "TTF_Init: " << TTF_GetError() << endl;

	SDL_Window * window = SDL_CreateWindow("SDL2 Displaying Image",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 400, 0);
	TTF_Font* MainFont = TTF_OpenFont("Arial.ttf", 24);
	SDL_Color White = { 255, 255, 255 };

	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0); //Loading Images
	SDL_Surface * Backgroundimg = IMG_Load("Background.png");
	SDL_Surface * PuckLeftimg = IMG_Load("PuckLeft.png");
	SDL_Surface * PuckRightimg = IMG_Load("PuckRight.png");
	SDL_Surface * Soccerballimg = IMG_Load("Soccerball.png");
	SDL_Surface * StartButtonimg = IMG_Load("StartButton.png");
	SDL_Surface * Goalimg = IMG_Load("goal.png");
	SDL_Surface * EasyorHardimg = IMG_Load("EasyorHard.png");
	SDL_Surface * Clearimg = IMG_Load("Clear.png");
	SDL_Surface * Instructionsimg = IMG_Load("Instructions.png");
	SDL_Surface * Controlsimg = IMG_Load("Controls.png");


	SDL_Texture * BackgroundTex = SDL_CreateTextureFromSurface(renderer, Backgroundimg); //Creating Textures
	SDL_Texture * PuckLeftTex = SDL_CreateTextureFromSurface(renderer, PuckLeftimg);
	SDL_Texture * PuckRightTex = SDL_CreateTextureFromSurface(renderer, PuckRightimg);
	SDL_Texture * SoccerballTex = SDL_CreateTextureFromSurface(renderer, Soccerballimg);	
	SDL_Texture * StartButtonTex = SDL_CreateTextureFromSurface(renderer, StartButtonimg);
	SDL_Texture * GoalTex = SDL_CreateTextureFromSurface(renderer, Goalimg);
	SDL_Texture * EasyorHardTex = SDL_CreateTextureFromSurface(renderer, EasyorHardimg);
	SDL_Texture * ClearTex = SDL_CreateTextureFromSurface(renderer, Clearimg);
	SDL_Texture * InstructionsTex = SDL_CreateTextureFromSurface(renderer, Instructionsimg);
	SDL_Texture * ControlsTex = SDL_CreateTextureFromSurface(renderer, Controlsimg);




	//----------------------------------MAIN LOOP-----------------------------------------------
	while (!quit)                                              //MAIN LOOP
	{
		quit = keyboardFunc(&eventTest,quit);
		if (start) {
			ballBounce();
			puckMovement();
			goal = resetRound();
			
		}
		display(renderer,BackgroundTex,PuckLeftTex,PuckRightTex,SoccerballTex,MainFont,&White);
		if (!start){
			SDL_Rect startrect = { 200, 160,200, 80 }; //x,y,w,h
			SDL_Rect goalrect = { 100, 80,400, 240 }; 
			SDL_Rect diffrect = { 250, 100,100, 40 }; //difficulty
			SDL_Rect clearrect = { 20,20,50,50 };
			SDL_Rect instructrect = {465,20,110,45};
			SDL_Rect controlsrect = { 100,200,400,200 };

			if (goal) {
				SDL_RenderCopy(renderer, GoalTex, NULL, &goalrect);
			}
			else {

				SDL_RenderCopy(renderer, StartButtonTex, NULL, &startrect);
				SDL_RenderCopy(renderer, EasyorHardTex, NULL, &diffrect);
				SDL_RenderCopy(renderer, ClearTex, NULL, &clearrect);
				SDL_RenderCopy(renderer, InstructionsTex, NULL, &instructrect);
				if(showControls)
					SDL_RenderCopy(renderer, ControlsTex, NULL, &controlsrect);


				int x = myMouse.x;
				int y = myMouse.y;
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);//red
				for (int i = 0; i <= 600; i++)
					SDL_RenderDrawPoint(renderer, i, (pow((1) - (pow((i - 300), 2) / pow(100, 2)), 0.5) * 40.0 + 200));

				SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);//blue
				for (int j = 0; j <= 600; j++)
					SDL_RenderDrawPoint(renderer, j, (-pow((1) - (pow((j - 300), 2) / pow(100, 2)), 0.5) * 40.0 + 200));
			}
			SDL_RenderPresent(renderer);
			if (goal) {
				SDL_Delay(1000);
				goal = false;
			}

		}
		SDL_Delay(overAllspeed); //---------------Creates Slow Down for entire game
	} 
	destroy(Backgroundimg,PuckLeftimg,PuckRightimg,Soccerballimg,StartButtonimg,Goalimg,EasyorHardimg,Clearimg,
				BackgroundTex,PuckLeftTex,PuckRightTex,SoccerballTex,StartButtonTex,GoalTex,EasyorHardTex,ClearTex,renderer,window);
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();

	return 0;
}
