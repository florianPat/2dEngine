#pragma once

class Color
{
public:
	unsigned int color;
public:
	constexpr Color() : color((((uchar)255) << 24) | ((uchar)255 << 16) | ((uchar)255 << 8) | (uchar)255) {};
	constexpr Color(const Color& c) : color(c.color) {};
	constexpr Color(unsigned int c) : color(c) {};
	constexpr Color(unsigned char r, unsigned char g, unsigned char b) : color((r | (g << 8) | (b << 16))) {};
	constexpr Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : color((r | (g << 8) | (b << 16) | (a << 24))) {};
	constexpr Color(Color col, char a) : color(color | a << 24) {};
	Color& operator=(Color col)
	{
		color = col.color;
		return *this;
	}
	constexpr unsigned char GetA() const
	{
		return color >> 24;
	}
	constexpr unsigned char GetR() const
	{
		return (color) & 0xff;
	}
	constexpr unsigned char GetG() const
	{
		return (color >> 8) & 0xff;
	}
	constexpr unsigned char GetB() const
	{
		return (color >> 16) & 0xff;
	}
	void SetA(unsigned char a)
	{
		color = (color & 0xFFFFFF) | (a << 24);
	}
	void SetR(unsigned char r)
	{
		color = (color & 0xFFFFFF00) | (r);
	}
	void SetG(unsigned char g)
	{
		color = (color & 0xFFFF00FF) | (g << 8);
	}
	void SetB(unsigned char b)
	{
		color = (color & 0xFF00FFFF) | (b << 16);
	}
};

namespace Colors
{
	static constexpr Color MakeRGB(unsigned char r, unsigned char g, unsigned char b)
	{
		return (((uchar)255) << 24) | (r << 16) | (g << 8) | b;
	}
	static constexpr Color White = MakeRGB(255u, 255u, 255u);
	static constexpr Color Black = MakeRGB(0u, 0u, 0u);
	static constexpr Color Gray = MakeRGB(0x80u, 0x80u, 0x80u);
	static constexpr Color LightGray = MakeRGB(0xD3u, 0xD3u, 0xD3u);
	static constexpr Color Red = MakeRGB(255u, 0u, 0u);
	static constexpr Color Green = MakeRGB(0u, 255u, 0u);
	static constexpr Color Blue = MakeRGB(0u, 0u, 255u);
	static constexpr Color Yellow = MakeRGB(255u, 255u, 0u);
	static constexpr Color Cyan = MakeRGB(0u, 255u, 255u);
	static constexpr Color Magenta = MakeRGB(255u, 0u, 255u);
}