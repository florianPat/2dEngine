#pragma once

#include "globalDefs.h"
#include <Windows.h>
#include "Clock.h"
#include "Graphics2d.h"
#include "SoundSystem.h"

namespace eg
{
	class Window
	{
	public:
		friend class Graphics2d;

		//TODO: Add queue to get chars, in the order they are pressed...
		//(Know what I mean?? ;) )
		class Keyboard
		{
			friend class Window;

			uint32_t keyCodes = 0;
		public:
			bool32_t isKeyPressed(uint32_t keyCode) const;
		private:
			Keyboard() = default;
			inline void setKey(uint32_t keyCode, bool32_t isDown)
			{
				if (isDown)
				{
					keyCodes |= keyCode;
				}
				else
				{
					keyCodes &= (!keyCodes);
				}
			}
		};

		//TODO: Implement this!
		class Mouse
		{
			friend class Window;

			uint32_t buttons = 0;
			//TODO: Make this a Vector2
			uint16_t xPos, yPos;
		public:
			enum class Button
			{
				left = MK_LBUTTON,
				middle = MK_MBUTTON,
				right = MK_RBUTTON
			};
			//Vector2 getPos() const;
			inline bool32_t isButtonPressed(Button mouseButton) const
			{
				return (buttons & (uint32_t)mouseButton);
			}

			//inline Vector2 getPos() const;
		private:
			Mouse() = default;
			inline void setButton(uint32_t wParam)
			{
				//TODO: Bulletproof!
				buttons = 0;
				buttons |= wParam;
			}
			//NOTE: Window accesses pos directly!
		};
	private:
		static constexpr uint32_t WINDOWED_WINDOW_STYLE = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_VISIBLE | WS_MINIMIZEBOX;
		static constexpr uint32_t FULLSCREEN_WINDOW_STYLE = WS_POPUP | WS_VISIBLE;
		WNDCLASS windowClass = {};
		HWND windowHandle;
		bool32_t running = true;
		bool32_t fullscreen = false;
		float framerateLimit;
		bool32_t hasCursor;
		uint32_t width, height;
		RECT windowRect;
		WINDOWPLACEMENT previousWindowPos = { sizeof(previousWindowPos) };
		LONGLONG performanceCounertFrequency;
		Clock clock;
		bool sleepIsGranulary = false;
		Graphics2d gfx;
		SoundSystem snd;
	public:
		Keyboard keyboard;
		Mouse mouse;
	public:
		Window(const uint32_t& width, const uint32_t& height, const std::string& name, float framerateLimit = 0, bool32_t hasCursor = true);
		~Window();
		Window(const Window&) = delete;
		bool32_t isOpen() const;
		void close();
		void processMessages();
		void limitFrames();
		Graphics2d& getGfx() const;
		SoundSystem& getSndSys() const;
	private:
		void ToggleFullscreen();
		static LRESULT CALLBACK WindowProcInit(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};
}
