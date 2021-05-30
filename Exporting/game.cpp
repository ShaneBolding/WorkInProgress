//defining test conditions
#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float posx = 23, arena_halfsize = 39;
float player_posx = 80.f;
float player2_posx = -80.f;
float player_posy = 0.f;
float player2_posy = 0.f;
float arena_halfsize_x = 20, arena_halfsize_y = 45;
float player_halfsize_x = 16, player_halfsize_y = 14;
float player_halfsize = 4;
float ball_posx, ball_posy, ball_velx = 100, ball_vely, ball_halfsize = 1;

float player_vely, player2_vely;

int player_1_score = 0, player_2_score = 0;

/* modulation for adding number scoring 
	while(number)
	{
		int digit = number % 10;
		number = number/10;
		switch(digit);
	}
*/

internal void
simulate_player(float *posy, float *vely, float accely, float dt)
{
	accely -= *vely * 10.f; //add friction

	//player movement and physics
	*posy = *posy + *vely * dt + accely * dt * dt * .5f;  //implemented physics for player
	*vely = *vely + accely * dt;

	//player collision with arena
	if (*posy + player_halfsize > arena_halfsize_y)
	{
		*posy = arena_halfsize - player_halfsize_y;
		*vely = 0;
	}
	if (*posx + player_halfsize > arena_halfsize_x)
	{
		*posx = arena_halfsize - player_halfsize_x;
		*vely = 0;
	}
	if (*posx + player_halfsize < arena_halfsize_x)
	{
		*posx = arena_halfsize + player_halfsize_x;
		*vely = 0;
	}
}

internal bool //collision detection between two squares
aabb_vs_aabb(float p1x, float p1y, float hs1x, float hs1y,
	float p2x, float p2y, float hs2x, float hs2y)
{
	return(p1x + hs1x > p2x - hs2x &&
		p1x - hs1x < p2x + hs2x &&
		p1y + hs1y > p2y - hs2y &&
		p1y - hs1y < p2y + hs2y);
}

enum Gamemode
{
	GM_MENU,
	GM_GAMEPLAY,
};

Gamemode current_gamemode;
int hot_button;
bool enemy_is_ai;

internal void 
simulate(Input* input, float dt)
{
	clear_screen(0x009fdc);

	draw_tetromino(0, player_posx, player_posy, player_halfsize);
}