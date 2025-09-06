#pragma once
#include "pch.h"

#include "GuiLayer.h"


class GuiManager
{
public:
	GuiManager()
private:
	using Layers = std::shared_ptr<GuiLayer>;

	std::queue<Layers> deleteLayers;
	std::stack<Layers> layerStack;
};

