#pragma once

#include <Windows.h>
#include <string>
#include "Clock.h"
#include <iostream>

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
			inline bool32_t isKeyPressed(uint32_t keyCode) const;
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
			//TODO: Make this a Vec2?
			uint16_t xPos, yPos;
		public:
			enum class Button
			{
				left = MK_LBUTTON,
				middle = MK_MBUTTON,
				right = MK_RBUTTON
			};
			//Vec2 getPos() const;
			inline bool32_t isButtonPressed(Button mouseButton) const
			{
				return (buttons & (uint32_t)mouseButton);
			}

			//inline Vec2 getPos() const;
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
		WNDCLASS windowClass = {};
		HWND windowHandle;
		bool32_t running = true;
		float framerateLimit;
		bool32_t hasCursor;
		WINDOWPLACEMENT previousWindowPos = { sizeof(previousWindowPos) };
		uint32_t width, height;
		LONGLONG performanceCounertFrequency;
		Clock clock;
		bool sleepIsGranulary = false;
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
	private:
		void ToggleFullscreen();
		static LRESULT CALLBACK WindowProcInit(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};
}
