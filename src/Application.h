#pragma once

#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>

#include <nlohmann/json.hpp>

#include <print>
#include <fstream>
#include <format>

#include "Utils/Utils.h"
#include "Utils/Timer.h"



class Application
{
public:
	Application();
	~Application();

	void run();

private:

	void pollEvent();
	void update();

	void render();

	void handleResize();
	void toggleFullScreen();

	void initWindow();
	void createWindow(bool isfullscreen);
	void initGui();
	void initTimer();


	sf::RenderWindow window;
	tgui::Gui gui;

	nlohmann::json windowData;
};

