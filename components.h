#pragma once

#include "Vec2.h"
#include<SFML/Graphics.hpp>

class CTransform
{
public:
	Vec2 pos = { 0.f,0.f };
	Vec2 velocity = { 0.f,0.f };

	CTransform(const Vec2& p, const Vec2& v)
		:pos(p), velocity(v){};
};

class CShape
{
public:
	sf::RectangleShape rect;
	sf::CircleShape circle;

	CShape(float width, float height, const sf::Color& fill, const sf::Color& outline, float thickness)
		:rect({width, height})
	{
		rect.setFillColor(fill);
		rect.setOutlineThickness(thickness);
		rect.setOutlineColor(outline);
		rect.setOrigin(width/2, height/2);
	}

	CShape(float radius, const sf::Color& fill, const sf::Color& outline, float thickness)
		:circle(radius)
	{
		circle.setFillColor(fill);
		circle.setOutlineThickness(thickness);
		circle.setOutlineColor(outline);
		circle.setOrigin(radius, radius);
	};
};

class CInput
{
public:
	bool up = false;
	bool down = false;

	CInput() {};
};