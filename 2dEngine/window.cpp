#include "window.h"

namespace eg
{
	LRESULT CALLBACK Window::windowProcInit(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg == WM_NCCREATE)
		{
			// extract ptr to window class from creation data
			const CREATESTRUCTW* const create = reinterpret_cast<CREATESTRUCTW*>(lParam);
			Window* wnd = reinterpret_cast<Window*>(create->lpCreateParams);
			// sanity check
			assert(wnd != nullptr);
			// set WinAPI-managed user data to store ptr to window class
			SetWindowLongPtrA(hwnd, GWLP_USERDATA, (LONG_PTR)wnd);

			SetWindowLongPtrA(hwnd, GWLP_WNDPROC, (LONG_PTR)&Window::windowProc);

			return true;
		}
		else
			return DefWindowProcA(hwnd, uMsg, wParam, lParam);
	}

	LRESULT CALLBACK Window::windowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		Window* const window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

		LRESULT result = 0;

		switch (message)
		{
			case WM_ACTIVATEAPP:
			{
				if (wParam)
				{
					//NOTE: Acitve
					window->clock.restart();
				}
				else
				{
					//NOTE: Inactive
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
			case WM_DESTROY:
			{
				PostQuitMessage(0);
			}
			case WM_QUIT:
			{
				break;
			}
			case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hwnd, &ps);
				window->gfx.render();
				EndPaint(hwnd, &ps);
				break;
			}
			case WM_SETCURSOR:
			{
				if (window->hasCursor)
					result = DefWindowProcA(hwnd, message, wParam, lParam);
				else
					SetCursor(0);
				break;
			}
			case WM_SYSKEYDOWN:
			case WM_SYSKEYUP:
			case WM_KEYDOWN:
			case WM_KEYUP:
			{
				char keyCode = (char)wParam;
				bool wasDown = ((lParam & (1 << 30)) != 0);
				bool isDown = ((lParam & (1 << 31)) == 0);

				if (wasDown != isDown)
				{
					window->keyboard.setKey(keyCode, isDown);
				}

				if (isDown)
				{
					bool altKeyIsDown = (lParam & (1 << 29)); //Get back 1 if Alt is down
					if ((keyCode == VK_F4) && altKeyIsDown)
					{
						window->running = false;
					}
					if (keyCode == VK_RETURN && altKeyIsDown)
					{
						if (hwnd)
							window->toggleFullscreen();
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
				POINTS p = MAKEPOINTS(lParam);

				if ((uint32_t)p.x <= window->gfx.width)
					window->mouse.pos.x = p.x;
				if ((uint32_t)p.y <= window->gfx.height)
					window->mouse.pos.y = p.y;

				window->mouse.setButton((uint32_t)wParam);
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
				result = DefWindowProcA(hwnd, message, wParam, lParam);
				break;
			}
		}

		return result;
	}

	Window::Window(int32_t width, int32_t height, const std::string& name, float framerateLimit, bool hasCursor)
		: hasCursor(hasCursor), sleepIsGranulary((timeBeginPeriod(1) == TIMERR_NOERROR)),
		clock(1), gfx(windowHandle, width, height), framerateLimit(framerateLimit)
	{
		LARGE_INTEGER performaceCountFrequency;
		QueryPerformanceFrequency(&performaceCountFrequency);
		performanceCounertFrequency = performaceCountFrequency.QuadPart;

		WNDCLASSA windowClass = { 0 };
		windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		windowClass.hInstance = GetModuleHandle(0);
		windowClass.hIcon = LoadIconA(0, IDI_APPLICATION);
		windowClass.hCursor = LoadCursorA(0, IDC_ARROW);
		windowClass.lpszClassName = "WindowWindowClass";
		windowClass.lpfnWndProc = &windowProcInit;

		if (RegisterClassA(&windowClass))
		{
			windowRect = { 0, 0, (LONG)width - 1, (LONG)height - 1 };
			AdjustWindowRect(&windowRect, WINDOWED_WINDOW_STYLE, false);
			windowHandle = CreateWindowA(windowClass.lpszClassName, name.c_str(), WINDOWED_WINDOW_STYLE, CW_USEDEFAULT, CW_USEDEFAULT,
				windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, 0, 0, GetModuleHandle(0), this);
			ShowWindow(windowHandle, SW_SHOW);
			SetFocus(windowHandle);
			SetForegroundWindow(windowHandle);
			SetActiveWindow(windowHandle);

			if (windowHandle)
			{
				snd = SoundSystem(windowHandle);
				clock = Clock(performanceCounertFrequency);
				return;
			}
			else
			{
				InvalidCodePath;
			}
		}
		else
		{
			InvalidCodePath;
		}
	}

	bool Window::processEvents()
	{
		MSG message;

		while (PeekMessageA(&message, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessageA(&message);

			if (message.message == WM_CLOSE)
			{
				return false;
			}
			else if (!running)
			{
				PostMessageA(windowHandle, WM_CLOSE, 0, 0);
			}
		}

		return true;
	}

	void Window::close()
	{
		PostQuitMessage(0);
	}

	Clock& Window::getClock()
	{
		return clock;
	}

	Graphics2d& Window::getGfx()
	{
		return gfx;
	}

	const Keyboard& Window::getKeyboard() const
	{
		return keyboard;
	}

	const Mouse& Window::getMouse() const
	{
		return mouse;
	}

	SoundSystem& Window::getSndSys()
	{
		return snd;
	}

	void Window::setWindowText(const char* text) const
	{
		SetWindowTextA(windowHandle, text);
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
		}
		else
		{
			utilsLog("Missed frame time!");
		}
		float totalSecoundsElapsedForFrame = clock.restart();
		std::cout << totalSecoundsElapsedForFrame << '\n';
	}

	void Window::toggleFullscreen()
	{
		if (fullscreen)
		{
			fullscreen = false;
			SetWindowLong(windowHandle, GWL_STYLE, WINDOWED_WINDOW_STYLE);
			SetWindowPlacement(windowHandle, &previousWindowPos);
			SetWindowPos(windowHandle, HWND_TOP, 0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
				SWP_NOMOVE | SWP_FRAMECHANGED);
		}
		else
		{
			fullscreen = true;
			MONITORINFO mi = { sizeof(mi) };
			if (GetWindowPlacement(windowHandle, &previousWindowPos) &&
				GetMonitorInfo(MonitorFromWindow(windowHandle, MONITOR_DEFAULTTOPRIMARY), &mi))
			{
				SetWindowLong(windowHandle, GWL_STYLE, FULLSCREEN_WINDOW_STYLE);
				SetWindowPos(windowHandle, HWND_TOP,
					mi.rcMonitor.left, mi.rcMonitor.top,
					mi.rcMonitor.right - mi.rcMonitor.left,
					mi.rcMonitor.bottom - mi.rcMonitor.top,
					SWP_FRAMECHANGED);
			}
		}
	}

	bool Keyboard::isKeyPressed(char keyCode) const
	{
		return keyCodes[keyCode];
	}

	inline void Keyboard::setKey(char keyCode, bool isDown)
	{
		if (isDown)
		{
			keyCodes[keyCode] = true;
		}
		else
		{
			keyCodes[keyCode] = false;
		}
	}

	inline bool Mouse::isButtonPressed(Button mouseButton) const
	{
		return (buttons & (uint32_t)mouseButton);
	}

	inline void Mouse::setButton(uint32_t wParam)
	{
		buttons = 0;
		buttons |= wParam;
	}
}
