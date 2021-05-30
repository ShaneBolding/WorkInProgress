bool tetromino[7][16];
tetromino[0] =
{
	false, false, true, false,
	false, false, true, false,
	false, false, true, false,
	false, false, true, false
};
tetromino[1] =
{
	false, false, true, false,
	false, true, true, false,
	false, true, false, false,
	false, false, false, false
};
tetromino[2] =
{
	false, true, false, false,
	false, true, true, false,
	false, false, true, false,
	false, false, false, false
};
tetromino[3] =
{
	false, false, false, false,
	false, true, true, false,
	false, true, true, false,
	false, false, false, false
};
tetromino[3] =
{
	false, false, false, false,
	false, true, true, false,
	false, true, true, false,
	false, false, false, false
};
tetromino[4] =
{
	false, false, true, false,
	false, true, true, false,
	false, false, true, false,
	false, false, false, false
};
tetromino[5] =
{
	false, false, false, false,
	false, true, true, false,
	false, false, true, false,
	false, false, true, false
};
tetromino[6] =
{
	false, false, false, false,
	false, true, true, false,
	false, true, false, false,
	false, true, false, false
};
internal void
clear_screen(u32 color)
{
	u32* pixel = (u32*)render_state.memory;
	for (int x = 0; x < render_state.height; x++)
	{
		for (int y = 0; y < render_state.width; y++)
		{
			*pixel++ = color;
		}
	}
}

internal void
draw_rect_in_pixels(int x0, int y0, int x1, int y1, u32 color)
{
	x0 = clamp(0, x0, render_state.width);
	x1= clamp(0, x1, render_state.width);
	y0 = clamp(0, y0, render_state.height);
	y1 = clamp(0, y1, render_state.height);

	for (int y = y0; y < y1; y++)
	{
		u32* pixel = (u32*)render_state.memory + x0 + y * render_state.width; //offset rect
		for (int x = x0; x < x1; x++)
		{
			*pixel++ = color;
		}
	}
}

global_var float render_scale = .01f; // allows the renders to be on a scale of 1-100 for ease of coding

internal void
draw_rect(float x, float y, float half_size_x, float half_size_y, u32 color)
{
	//ensures block is proportional
	x *= render_state.height * render_scale;
	y *= render_state.height * render_scale;
	half_size_x *= render_state.height * render_scale;
	half_size_y *= render_state.height * render_scale;

	x += render_state.width / 2.f;
	y += render_state.height / 2.f;


	//change to pixels
	int x0 = x - half_size_x;
	int x1 = x + half_size_x;
	int y0 = y - half_size_y;
	int y1 = y + half_size_y;
	
	draw_rect_in_pixels(x0, y0, x1, y1, color);
}

internal void
draw_tetromino(int tetroType, float x, float y, float half_size)
{
	float x0, x1, y0, y1;
	u32 color = 0x444444;
	//ensures rect is proportional
	x *= render_state.height * render_scale;
	y *= render_state.height * render_scale;
	half_size_x *= render_state.height * render_scale;

	x += render_state.width / 2.f;
	y += render_state.height / 2.f;

	for (int p; p < 16; p++)
	{
		if (tetromino[tetroType][p])
		{
			if (p % 4 == 1)
			{
				x0 = x - half_size / 2;
				x1 = x;
			}
			if (p % 4 == 2)
			{
				x1 = x + half_size / 2;
				x0 = x;
			}
			if (p < 4)
			{
				y0 = y - half_size;
				y1 = y - half_size / 2;
			}
			else if (p < 8)
			{
				y0 = y - half_size/2;
				y1 = y;
			}
			else if (p < 12)
			{
				y0 = y;
				y1 = y + half_size / 2;
			}
			else
			{
				y0 = y + half_size / 2;
				y1 = y + half_size;
			}
			draw_rect_in_pixels(x0, y0, x1, y1, color + (u32)(4444*tetroType));
		}
	}
}