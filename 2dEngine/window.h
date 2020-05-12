#pragma once

#include "globalDefs.h"
#include "Clock.h"
#include "Graphics2d.h"
#include "SoundSystem.h"
#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>

namespace eg
{
	class Keyboard
	{
		bool keyCodes[255] = { 0 };
	public:
		bool isKeyPressed(char keyCode) const;
		inline void setKey(char keyCode, bool isDown);
	};

	class Mouse
	{
		uint32_t buttons = 0;
	public:
		Vector2i pos;
	public:
		enum class Button
		{
			left = MK_LBUTTON,
			middle = MK_MBUTTON,
			right = MK_RBUTTON
		};
		inline bool isButtonPressed(Button mouseButton) const;
		inline void setButton(uint32_t wParam);
	};

	class Window
	{
		static constexpr uint32_t WINDOWED_WINDOW_STYLE = WS_POPUPWINDOW | WS_CAPTION | WS_VISIBLE | WS_MINIMIZEBOX;
		static constexpr uint32_t FULLSCREEN_WINDOW_STYLE = WS_POPUP | WS_VISIBLE;
		HWND windowHandle = nullptr;
		RECT windowRect = {};
		LONGLONG performanceCounertFrequency = 0;
		bool running = true;
		bool hasCursor;
		bool sleepIsGranulary;
		bool fullscreen = false;

		Clock clock;
		Keyboard keyboard;
		Mouse mouse;

		Graphics2d gfx;
		SoundSystem snd;

		WINDOWPLACEMENT previousWindowPos = { sizeof(previousWindowPos) };
		float framerateLimit;
	public:
		Window(int32_t width, int32_t height, const std::string& name, float framerateLimit = 0.0f, bool hasCursor = true);
		Window(const Window& other) = delete;
		Window(Window&& other) = delete;
		Window& operator=(const Window& rhs) = delete;
		Window& operator=(Window&& rhs) = delete;
		bool processEvents();

		void close();
		Clock& getClock();
		void limitFrames();
		Graphics2d& getGfx();
		const Keyboard& getKeyboard() const;
		const Mouse& getMouse() const;
		SoundSystem& getSndSys();
		void setWindowText(const char* text) const;
	private:
		static LRESULT CALLBACK windowProcInit(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		void toggleFullscreen();
	};
}
