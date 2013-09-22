#include "Engine.h"

Engine::Engine() {
	//System Stuff
	debug=false;
	delta = clock.restart().asSeconds();
	windowSize.x=800;
	windowSize.y=600;
	volumeSounds=80;
	volumeBGM=80;
	screenScale=1;
	setVsync=false;
	setBloom=true;
	setFullscreen=false;
	setTextSpeed=120;
	setOutline=true;
	setFramerateLimit=0;
	LoadSettings();
	
	if (setFullscreen) 
		renderWindow.create(sf::VideoMode(windowSize.x, windowSize.y), "Charity",sf::Style::Fullscreen);
	else
		renderWindow.create(sf::VideoMode(windowSize.x, windowSize.y), "Charity",sf::Style::Close);
	renderWindow.setVerticalSyncEnabled(setVsync);
	if (setFramerateLimit)
	renderWindow.setFramerateLimit(setFramerateLimit);
	
	commandNumber = 0;
	commandLastText = 0;
	commandPause = false;
	commandReturn = 0;

	placeIndex=0;

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
	queue=new Queue(scripting);
	scripting.ExecuteFile("Data/Resources.script");
	LoadMap("Map");

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
	queue->Update(delta);
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
	delete queue;
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
	bool done = objectsManager->LoadMap(path);
	if (done) scripting.ExecuteFunction(L"Init");
	return done;
};

std::ifstream Engine::IniOpen(std::string filename) {
	std::ifstream file(filename);
	return file;
};

void Engine::IniClose(std::ifstream &ini) {
	if (ini) {
		ini.close();
	};
};

float Engine::IniFindValue(std::ifstream &ini, std::string group, std::string key, float defaultValue) {
	ini.clear();
	ini.seekg(std::ifstream::beg);
	std::string string;
	while(!ini.eof()) {
		std::getline(ini,string);
		int pos = string.find_first_of("=");
		std::string tempKey = string.substr(0,pos);
		std::string tempValue = string.substr(pos+1,string.length()-pos);
		pos = tempKey.find_first_of(" ");
		while(pos!=std::string::npos) {
			tempKey.erase(pos);
			pos = tempKey.find_first_of(" ");
		};
		if (tempKey==key) {
			float value = std::stof(tempValue);
			return value;
		};
	};
	return defaultValue;
};

void Engine::LoadSettings() {
	std::ifstream File = IniOpen("Settings.ini");
	if (File) {
		//Sounds settings
		volumeSounds = IniFindValue(File,"Audio","SoundsVolume",80);
		volumeBGM = IniFindValue(File,"Audio","MusicVolume",80);

		//Graphics settings
		setVsync = IniFindValue(File,"Video","Vsync",0);
		setBloom = IniFindValue(File,"Video","Bloom",1);
		setFullscreen = IniFindValue(File,"Video","Fullscreen",0);
		setFramerateLimit = IniFindValue(File,"Video","FramerateLimit",0);

		//Game settings
		setTextSpeed = IniFindValue(File,"Game","TextSpeed",120);
		setOutline = IniFindValue(File,"Game","Outline",1);

		IniClose(File);
		printf("Settings loaded\n");
	};
};