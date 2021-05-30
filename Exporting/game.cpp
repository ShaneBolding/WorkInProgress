//defining test conditions
#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float player_posx = 80.f;
float player_posy = 80.f;
float arena_halfsize_x = 20, arena_halfsize_y = 45;
float player_halfsize = 4;


/* modulation for adding number scoring 
	while(number)
	{
		int digit = number % 10;
		number = number/10;
		switch(digit);
	}
*/

internal void
simulate_player(void)
{


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
simulate(Input* input)
{
	clear_screen(0x009fdc);

	draw_tetromino(0, player_posx, player_posy, player_halfsize);
}