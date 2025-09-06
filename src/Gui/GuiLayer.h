#pragma once

#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>

class GuiLayer
{
public:
	GuiLayer() = default;

	virtual void show(bool show);
	virtual void enable(bool enable);

	const bool isShown();
	const bool isEnabled();

private:
	tgui::Group::Ptr container;
};

