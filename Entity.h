#pragma once

#include "Components.h"
#include<memory>
#include<string>

class Entity {
	friend class Game;

	size_t m_id = 0;
	std::string m_tag = "default";
	bool m_active = true;

	Entity(const size_t id, const std::string& tag);

public:

	std::shared_ptr<CTransform> cTransform;
	std::shared_ptr<CShape> cShape;
	std::shared_ptr<CInput> cInput;

	const size_t id() const;
	const std::string& tag() const;
};