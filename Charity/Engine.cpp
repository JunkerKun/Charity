#include "Engine.h"

Engine::Engine() {
	//System Stuff
	debug=false;
	delta = clock.restart().asSeconds();
	windowSize.x=640;
	windowSize.y=480;
	
	renderWindow.create(sf::VideoMode(windowSize.x, windowSize.y), "Charity",sf::Style::Close);
	//renderWindow.create(sf::VideoMode(windowSize.x, windowSize.y), "Charity",sf::Style::Fullscreen);
	renderWindow.setVerticalSyncEnabled(false);
	renderWindow.setFramerateLimit(9999);
	
	commandNumber = 0;
	commandLastText = 0;
	commandPause = false;
	commandReturn = 0;

	sizeText=16;
	colorText=sf::Color::White;

	pathPlayerSprite = "Data/Sprites/Charity.png";
};

void Engine::Begin() {
	commands = new std::vector<std::wstring>;
	variables = new std::map<std::wstring,float>;
	commandFunctions = new std::map<std::wstring,int>;
	input=new Input(&inputEvent);
	camera=new Camera();
	renderWindow.setView(camera->view);
	objectsManager = new ObjectsManager();
	tilesManager=new TilesManager();
	textBox=NULL;
	resourcesManager=new ResourcesManager();
	resourcesManager->AddTexture("UITextBox","Data/UI/TextBox.png");
	resourcesManager->AddTexture("TileSet","Data/Tiles/TileSet.png");
	resourcesManager->AddTexture("sprPlayer",pathPlayerSprite);
	resourcesManager->AddTexture("sprTable","Data/Sprites/Table.png");
	resourcesManager->AddTexture("sprShadow","Data/Sprites/Shadow.png");
	resourcesManager->AddFont(1,"Data/Fonts/STREET.ttf");
	LoadMap("Map");
	scripting.ExecuteFunction(L"Init");

	if (debug) {
		debugText=new sf::Text();
		debugText->setColor(colorText);
		debugText->setCharacterSize(sizeText);
		debugText->setFont(*resourcesManager->GetFont(1));
	};
};

void Engine::End() {
	delete this;
};

bool Engine::GetIsFocused() {
		return isFocused;
};

bool Engine::Update() {
	delta = clock.restart().asSeconds();

	isFocused=true;
	if (inputEvent.type==sf::Event::LostFocus) {
		isFocused=false;
	};

	if (textBox!=NULL) textBox->Unlock();
	input->Update();
	objectsManager->Update();
	camera->Update();
	if (textBox!=NULL) textBox->Update();
	return true;
};

bool Engine::Draw() {
	renderWindow.setView(camera->view);
	renderWindow.clear();
	tilesManager->Draw(renderWindow);
	objectsManager->Draw(renderWindow);
	if (textBox!=NULL) textBox->Draw(renderWindow);
	if (debug) {
		debugText->setPosition(camera->xView,camera->yView);
		debugText->setString(scripting.ToString(delta));
		renderWindow.draw(*debugText);
	};
	renderWindow.display();
	return true;
};

bool Engine::Tick() {
	while (renderWindow.isOpen())
	{
		while (renderWindow.pollEvent(inputEvent))
		{
			if (inputEvent.type == sf::Event::Closed)
				renderWindow.close();
			switch (inputEvent.type) {
			case sf::Event::KeyPressed:
				if (inputEvent.key.code==sf::Keyboard::Escape) {
					renderWindow.close();
				};
			};
		};

		Update();
		Draw();
	};

	return true;
};

float Engine::GetDelta() {
	return delta;
};

sf::Event Engine::GetInputEvent() {
	return inputEvent;
};

Engine::~Engine() {
	delete commands;
	delete variables;
	delete commandFunctions;
	delete input;
	delete camera;
	if (textBox!=NULL) delete textBox;
	delete objectsManager;
	delete tilesManager;
	delete resourcesManager;
	if (debug) delete debugText;
};

bool Engine::LoadMap(std::string name) {
	std::string path;
	path="Data/Scripts/";
	path+=name;
	path+=".script";
	scripting.LoadFile(path);
	path="Data/Maps/";
	path+=name;
	path+=".map";
	return objectsManager->LoadMap(path);
};