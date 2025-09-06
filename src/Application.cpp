#include "Application.h"

Application::Application()
{
	initWindow();
	initGui();
	initTimer();
}

Application::~Application()
{

}

void Application::run()
{
	while (window.isOpen())
	{
		pollEvent();
		update();
		render();
	}
}

void Application::pollEvent()
{
	while (auto event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			window.close();

		if (event->is<sf::Event::Resized>())
			handleResize();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F11) && Timer::getInstance().delayFinished("fullscreentoggle"))
		{
			toggleFullScreen();
			Timer::getInstance().resetDelay("fullscreentoggle");
		}

		gui.handleEvent(*event);
	}
}

void Application::update()
{
	if (Timer::getInstance().delayFinished("test"))
	{
		std::println("Delay finished");
		Timer::getInstance().resetDelay("test");
	}
}

void Application::render()
{
	window.clear();
	window.display();
}

void Application::handleResize()
{
	sf::Vector2u newSize = window.getSize();

	//If window is changing to fullscreen discard the new window size
	if (windowData["isfullscreen"].get<bool>())
	{
		newSize =
		{
			windowData["windowsize"][0].get<unsigned int>(),
			windowData["windowsize"][1].get<unsigned int>()
		};

	}

	gui.setRelativeView(tgui::FloatRect(0.f ,0.f,(float)newSize.x / 1920.f , (float)newSize.y / 1080.f ));

	//Format the new information to save
	std::string rawData = std::format("{{\n \"windowsize\" : [ {0} , {1} ],\n \"fps\" : {2},\n \"isfullscreen\" : {3},\n \"windowname\" : \"{4}\" }}", 
		newSize.x, newSize.y, 
		30, 
		windowData["isfullscreen"].get<bool>(),windowData["windowname"].get<std::string>()
	);

	//Debug print should remove
	std::println("{}",rawData);

	windowData= nlohmann::json::parse(rawData);

	Utils::saveStringToFile("config/window.json", windowData.dump());
}

void Application::toggleFullScreen()
{
	//Toggle fullscreen
	windowData["isfullscreen"] = windowData["isfullscreen"].get<bool>() ? false : true;

	createWindow(windowData["isfullscreen"].get<bool>());
	
	window.setFramerateLimit(windowData["fps"].get<int>());

	handleResize();

}

void Application::initWindow()
{
	std::string windowDataRaw = Utils::readWholeFile("config/window.json");

	if (windowDataRaw.empty())
	{
		windowDataRaw = std::format("{{\n \"windowsize\" : [ {0} , {1} ],\n \"fps\" : {2},\n \"isfullscreen\" : {3},\n \"windowname\" : \"{4}\" }}",1600,900,30,false,"Journal");
	}

	windowData = nlohmann::json::parse(windowDataRaw);
	
	createWindow(windowData["isfullscreen"].get<bool>());
}

void Application::createWindow(bool isfullscreen)
{
	if (isfullscreen)
	{
		window.create(
			sf::VideoMode::getFullscreenModes().front(),
			windowData["windowname"].get<std::string>(),
			sf::Style::Default,
			sf::State::Fullscreen
		);
	}
	else
	{
		window.create(
			sf::VideoMode({ 
				windowData["windowsize"][0].get<unsigned int>(), 
				windowData["windowsize"][1].get<unsigned int>()
				}),
			windowData["windowname"].get<std::string>(),
			sf::Style::Default,
			sf::State::Windowed
		);
	}
}

void Application::initGui()
{
	gui.setTarget(window);
	handleResize();
}

void Application::initTimer()
{
	Timer::getInstance().setDelay("fullscreentoggle", 1s);
	Timer::getInstance().setDelay("test", 4s);
}
