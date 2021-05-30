#include <Windows.h>
#include "utilities.cpp"

global_var bool running = TRUE;

struct Render_State { // the state of the render lol
	int height, width;
	void* memory;
	BITMAPINFO bitmap_info; //information used by windows like pixel colour & compression for the video card 
};

global_var Render_State render_state;

#include "platform_common.cpp"
#include "renderer.cpp" //include after render_state so renderer.cpp can use it
#include "game.cpp"

LRESULT CALLBACK WindowCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch (uMsg) // taks msg and performs action
	{
		case WM_CLOSE:
		case WM_DESTROY: {
			running = FALSE; // If windows recieves message to end program end program
		} break;

		case WM_SIZE: {
			RECT rect;
			GetClientRect(hwnd, &rect); //gives window's left right top and bottom values to rect
			render_state.width = rect.right - rect.left;
			render_state.height = rect.bottom - rect.top;

			int buffer_size = render_state.width * render_state.height * sizeof(u32); //calcuate buffer size

			if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE); //If render_state.memory is already allocated and size changes free memory first
			render_state.memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); // allocates memory in the heap

			render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader); //same size of sindows knows what version we are using
			render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
			render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
			render_state.bitmap_info.bmiHeader.biPlanes = 1;
			render_state.bitmap_info.bmiHeader.biBitCount = 32;
			render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;

		} break;

		default: { // If windows acts normal
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
	return result;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//Create the window class
	ShowCursor(FALSE);
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW; //Redraws the win vert and horiz when needed
	window_class.lpszClassName = L"Game Window Class"; //Class need string to identify
	window_class.lpfnWndProc = WindowCallback; //Used for calling back action notifying windows actions
	
	//Register the class
	RegisterClass(&window_class); //Pass address of the window class
	
	//Create the window
	HWND window = CreateWindow(window_class.lpszClassName, L"Pong", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	{
		//Fullscreen
		SetWindowLong(window, GWL_STYLE, GetWindowLong(window, GWL_STYLE) & ~WS_OVERLAPPEDWINDOW);	// negate overlap window
		MONITORINFO mi = { sizeof(mi) };															// get monitor info
		GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY), &mi);					// get more info
		//put window on top and make window the size of the monitor read from previous 2 lines
		SetWindowPos(window, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
	HDC hdc = GetDC(window); // handle of device context

	Input input = {};

	//find the seconds per frame
	float delta_time = 0.16666f;
	LARGE_INTEGER frame_begin_time;
	QueryPerformanceCounter(&frame_begin_time);

	float performance_frequency;
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performance_frequency = (float)perf.QuadPart;
	}

	//Run window
	while (running)
	{
		//Input
		MSG message;

		//reset the whether or not the button press has been changed at every frame refresh
		for (int i = 0; i < BUTTON_COUNT; i++)
		{
			input.buttons[i].changed = false;
		}

		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) //reads meassages from the game window to windows
		{
			switch (message.message) {
				case WM_KEYUP:
				case WM_KEYDOWN: {
					u32 vk_code = (u32)message.wParam; // recieves the key the user pressed
					bool is_down = ((message.lParam & (1 << 31)) == 0); // recieves if the event was a key up or key down event using the 31st bit
			
//make macro
#define process_button(b, vk)\
case vk: {\
input.buttons[b].changed = is_down != input.buttons[b].is_down;\
input.buttons[b].is_down = is_down;\
} break;

					switch (vk_code) {
						process_button(BUTTON_UP, VK_UP);
						process_button(BUTTON_DOWN, VK_DOWN);
						process_button(BUTTON_LEFT, VK_LEFT);
						process_button(BUTTON_RIGHT, VK_RIGHT);
						process_button(BUTTON_W, 'W');
						process_button(BUTTON_S, 'S');
						process_button(BUTTON_ENTER, VK_RETURN);
						process_button(BUTTON_ESC, VK_ESCAPE);
					}
				} break;
					
				default: {
					TranslateMessage(&message);
					DispatchMessage(&message);
				}
			}
			
		}
		//Simulate
		simulate(&input, delta_time);
		
		//Render
		StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);

		LARGE_INTEGER frame_end_time;
		QueryPerformanceCounter(&frame_end_time);
		delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency;
		frame_begin_time = frame_end_time;
	}
	
	return 0;
}