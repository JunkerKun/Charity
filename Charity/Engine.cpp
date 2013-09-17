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

	drawNoise=false;
	playerSpriteName="sprPlayer";
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
	scripting.ExecuteFile("Data/Resources.script");
	LoadMap("Map");
	scripting.ExecuteFunction(L"Init");

	if (debug) {
		debugText=new sf::Text();
		Font* fnt=resourcesManager->GetFont(1);
		debugText->setColor(fnt->color);
		debugText->setCharacterSize(fnt->size);
		debugText->setFont(fnt->font);
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

	if (drawNoise) {
	sf::Sprite spr;
	spr.setTexture(*resourcesManager->GetTexture("sprNoise"));
	spr.setTextureRect(sf::IntRect(rand()%windowSize.x,rand()%windowSize.y,windowSize.x,windowSize.y));
	spr.setPosition(camera->xView,camera->yView);
	renderWindow.draw(spr);
	};

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

void Engine::SetDrawNoise(bool mode) {
	drawNoise=mode;
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