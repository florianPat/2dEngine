#include "window.h"

namespace eg
{
	LRESULT CALLBACK Window::WindowProcInit(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg == WM_NCCREATE)
		{
			// extract ptr to window class from creation data
			const CREATESTRUCTW* const create = reinterpret_cast<CREATESTRUCTW*>(lParam);
			Window* wnd = reinterpret_cast<Window*>(create->lpCreateParams);
			// sanity check
			assert(wnd != nullptr);
			// set WinAPI-managed user data to store ptr to window class
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(wnd));

			SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::WindowProc));

			return true;
		}
		else
			return DefWindowProcA(hwnd, uMsg, wParam, lParam);
	}

	LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Window* const window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

		LRESULT result = 0;

		switch (uMsg)
		{
		case WM_ACTIVATEAPP:
		{
			if (wParam)
			{
				//NOTE: Acitve
				window->clock.restart();
				//TODO: For later event queue post this as message!
			}
			else
			{
				//NOTE: Inactive
				//TODO: For later event queue post this as message!
			}
			break;
		}
		case WM_CLOSE:
		{
			window->running = false;
			window->windowHandle = 0;
			DestroyWindow(hwnd);
			break;
		}
		case WM_SETCURSOR:
		{
			if (window->hasCursor)
				result = DefWindowProcA(hwnd, uMsg, wParam, lParam);
			else
				SetCursor(0);
			break;
		}
		case WM_CHAR:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_QUIT:
		{
			InvalidCodePath;
			break;
		}
		case WM_PAINT:
		{
			window->gfx.render();
			break;
		}
		default:
		{
			result = DefWindowProcA(hwnd, uMsg, wParam, lParam);
			break;
		}
		}

		return result;
	}

	Window::~Window()
	{
		UnregisterClass(windowClass.lpszClassName, GetModuleHandle(0));
		if (windowHandle)
			DestroyWindow(windowHandle);
	}

	Window::Window(const uint32_t& width, const uint32_t& height, const std::string& name, float framerateLimit, bool32_t hasCursor)
		: framerateLimit(framerateLimit), hasCursor(hasCursor), keyboard(), mouse(), width(width), height(height), sleepIsGranulary((timeBeginPeriod(1) == TIMERR_NOERROR)),
		  clock(1), gfx(windowHandle, width, height), snd()
	{
		LARGE_INTEGER performaceCountFrequency;
		QueryPerformanceFrequency(&performaceCountFrequency);
		performanceCounertFrequency = performaceCountFrequency.QuadPart;

		windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		windowClass.hInstance = GetModuleHandle(0);
		windowClass.hCursor = LoadCursorA(0, IDC_ARROW);
		windowClass.lpszClassName = "WindowWindowClass";
		windowClass.lpfnWndProc = &WindowProcInit;

		if (RegisterClass(&windowClass))
		{
			windowHandle = CreateWindow(windowClass.lpszClassName, name.c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, GetModuleHandle(0), this);

			if (windowHandle)
			{
				snd = Audio(windowHandle);
				clock = Clock(performanceCounertFrequency);
				return;
			}
			else
			{
				//TODO: LOG!
				InvalidCodePath;
			}
		}
		else
		{
			//TODO: LOG!
			InvalidCodePath;
		}
	}

	void Window::processMessages()
	{
		MSG message;
		while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
		{
			switch (message.message)
			{
			case WM_QUIT:
			{
				running = false;
				break;
			}
			case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(windowHandle, &ps);
				gfx.render();
				EndPaint(windowHandle, &ps);
				break;
			}
			case WM_SYSKEYDOWN:
			case WM_SYSKEYUP:
			case WM_KEYDOWN:
			case WM_KEYUP:
			{
				uint32_t keyCode = (uint32_t)message.wParam;
				bool32_t wasDown = ((message.lParam & (1 << 30)) != 0);
				bool32_t isDown = ((message.lParam & (1 << 31)) == 0);

				if (wasDown != isDown)
				{
					keyboard.setKey(keyCode, isDown);
				}

				if (isDown)
				{
					bool32_t altKeyIsDown = (message.lParam & (1 << 29)); //Get back 1 if Alt is down
					if ((keyCode == VK_F4) && altKeyIsDown)
					{
						running = false;
					}
					if (keyCode == VK_RETURN && altKeyIsDown)
					{
						if (message.hwnd)
							ToggleFullscreen();
					}
				}
				break;
			}
			case WM_MOUSEMOVE:
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
			{
				POINTS p = MAKEPOINTS(message.lParam);

				if ((uint32_t)p.x <= width)
					mouse.xPos = p.x;
				if ((uint32_t)p.y <= height)
					mouse.yPos = p.y;

				mouse.setButton((uint32_t)message.wParam);
				break;
			}
			case WM_CHAR:
			{
				//TODO: Implement this, I need a queue!
				//keyboard.char((uint8_t)message.wParam);
				break;
			}
			default:
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
				break;
			}
			}
		}
	}

	void Window::limitFrames()
	{
		float secoundsElapsedForFrame = clock.getElapsedTime();
		if (secoundsElapsedForFrame < framerateLimit)
		{
			while (secoundsElapsedForFrame < framerateLimit)
			{
				if (sleepIsGranulary)
				{
					DWORD sleepMS = (DWORD)(1000.0f * (framerateLimit - secoundsElapsedForFrame));
					if (sleepMS > 0)
					{
						Sleep(sleepMS);
					}
					secoundsElapsedForFrame = clock.getElapsedTime();
				}
			}
			float totalSecoundsElapsedForFrame = clock.restart();
			std::cout << totalSecoundsElapsedForFrame << '\n';
		}
		else
		{
			float totalSecoundsElapsedForFrame = clock.restart();
			std::cout << totalSecoundsElapsedForFrame << '\n';
			//TOTO: Handle this!!
			//InvalidCodePath;
		}
	}

	Graphics2d & Window::getGfx() const
	{
		return (Graphics2d&)gfx;
	}

	Audio & Window::getAudio() const
	{
		return (Audio&)snd;
	}

	void Window::ToggleFullscreen()
	{
		DWORD dwStyle = GetWindowLong(windowHandle, GWL_STYLE);
		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			MONITORINFO mi = { sizeof(mi) };
			if (GetWindowPlacement(windowHandle, &previousWindowPos) &&
				GetMonitorInfo(MonitorFromWindow(windowHandle, MONITOR_DEFAULTTOPRIMARY),
					&mi))
			{
				SetWindowLong(windowHandle, GWL_STYLE,
					dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(windowHandle, HWND_TOP,
					mi.rcMonitor.left, mi.rcMonitor.top,
					mi.rcMonitor.right - mi.rcMonitor.left,
					mi.rcMonitor.bottom - mi.rcMonitor.top,
					SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
			}
		}
		else
		{
			SetWindowLong(windowHandle, GWL_STYLE,
				dwStyle | WS_OVERLAPPEDWINDOW);
			SetWindowPlacement(windowHandle, &previousWindowPos);
			SetWindowPos(windowHandle, NULL, 0, 0, 0, 0,
				SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
				SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		}
	}

	bool32_t Window::isOpen() const
	{
		return running;
	}

	void Window::close()
	{
		running = false;
	}


	bool32_t Window::Keyboard::isKeyPressed(uint32_t keyCode) const
	{
		return keyCode & keyCodes;
	}
}
