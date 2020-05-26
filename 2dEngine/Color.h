#pragma once

#include "globalDefs.h"

class Color
{
public:
	uint32_t color = 0;
public:
	constexpr Color() : color((((uchar)255) << 24) | ((uchar)255 << 16) | ((uchar)255 << 8) | (uchar)255) {};
	constexpr Color(const Color& c) : color(c.color) {};
	constexpr Color(uint32_t c) : color(c) {};
	constexpr Color(uint8_t r, uint8_t g, uint8_t b) : color((b | (g << 8) | (r << 16))) {};
	constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : color((b | (g << 8) | (r << 16) | (a << 24))) {};
	constexpr Color(Color col, char a) : color(color | a << 24) {};
	Color& operator=(Color col)
	{
		color = col.color;
		return *this;
	}
	constexpr uint8_t getA() const
	{
		return color >> 24;
	}
	constexpr uint8_t getR() const
	{
		return (color >> 16) & 0xff;
	}
	constexpr uint8_t getG() const
	{
		return (color >> 8) & 0xff;
	}
	constexpr uint8_t getB() const
	{
		return (color) & 0xff;
	}
	void setA(uint8_t a)
	{
		color = (color & 0xFFFFFF) | (a << 24);
	}
	void setR(uint8_t r)
	{
		color = (color & 0xFF00FFFF) | (r << 16);
	}
	void setG(uint8_t g)
	{
		color = (color & 0xFFFF00FF) | (g << 8);
	}
	void setB(uint8_t b)
	{
		color = (color & 0xFFFFFF00) | (b);
	}
	Color operator+(const Color& rhs) const
	{
		return Color(min((uint32_t)getR() + rhs.getR(), 255), min((uint32_t)getG() + rhs.getG(), 255), 
			min((uint32_t)getB() + rhs.getB(), 255));
	}
	Color& operator+=(const Color& rhs)
	{
		*this = *this + rhs;
		return *this;
	}
	Color operator-(const Color& rhs) const
	{
		return Color(min((uint32_t)getR() - rhs.getR(), 255), min((uint32_t)getG() - rhs.getG(), 255),
			min((uint32_t)getB() - rhs.getB(), 255));
	}
	Color& operator-=(const Color& rhs)
	{
		*this = *this - rhs;
		return *this;
	}
	Color operator*(float scalar) const
	{
		float r = getR() / 255.0f;
		float g = getG() / 255.0f;
		float b = getB() / 255.0f;
		return Color(min((uint32_t)(r * scalar * 255.0f), 255), min((uint32_t)(g * scalar * 255.0f), 255),
			min((uint32_t)(b * scalar * 255.0f), 255));
	}
	Color& operator*=(float scalar)
	{
		*this = *this * scalar;
		return *this;
	}
	Color operator*(const Color& rhs) const
	{
		float r = getR() / 255.0f;
		float g = getG() / 255.0f;
		float b = getB() / 255.0f;

		float rhsR = rhs.getR() / 255.0f;
		float rhsG = rhs.getG() / 255.0f;
		float rhsB = rhs.getB() / 255.0f;

		return Color(min((uint32_t)(r * rhsR * 255.0f), 255), min((uint32_t)(g * rhsG * 255.0f), 255),
			min((uint32_t)(b * rhsB * 255.0f), 255));
	}
	Color& operator*=(const Color& rhs)
	{
		*this = *this * rhs;
		return *this;
	}
	Color operator/(float divisor) const
	{
		return *this * (1 / divisor);
	}
};

namespace Colors
{
	static constexpr Color makeRGB(uint8_t r, uint8_t g, uint8_t b)
	{
		return (r << 16) | (g << 8) | b;
	}
	static constexpr Color White = makeRGB(255u, 255u, 255u);
	static constexpr Color Black = makeRGB(0u, 0u, 0u);
	static constexpr Color Gray = makeRGB(0x80u, 0x80u, 0x80u);
	static constexpr Color LightGray = makeRGB(0xD3u, 0xD3u, 0xD3u);
	static constexpr Color Red = makeRGB(255u, 0u, 0u);
	static constexpr Color Green = makeRGB(0u, 255u, 0u);
	static constexpr Color Blue = makeRGB(0u, 0u, 255u);
	static constexpr Color Yellow = makeRGB(255u, 255u, 0u);
	static constexpr Color Cyan = makeRGB(0u, 255u, 255u);
	static constexpr Color Magenta = makeRGB(255u, 0u, 255u);
}