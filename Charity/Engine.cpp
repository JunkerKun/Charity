#include "Engine.h"

Engine::Engine() {
	//System Stuff
	gridSize=64;
	debug=false;
	delta = clock.restart().asSeconds();
	windowSize.x=800;
	windowSize.y=600;
	volumeSounds=80;
	volumeBGM=80;
	setVsync=false;
	setBloom=false;
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
	fadeAlpha=0;
	fadeColor=sf::Color(0,0,0,255);
	fadeMode=0;
	fadeSpeed=255;

	gameEnd=false;
};

void Engine::Begin() {
	commands = new std::vector<std::wstring>;
	variables = new std::map<std::wstring,float>;
	variablesGlobal = new std::map<std::wstring,float>;
	commandFunctions = new std::map<std::wstring,int>;
	camera=new Camera();
	input=new Input(&inputEvent);
	renderWindow.setView(camera->view);
	objectsManager = new ObjectsManager();
	tilesManager=new TilesManager();
	textBox=NULL;
	choiceBox=NULL;
	resourcesManager=new ResourcesManager();
	scripting.ExecuteFile("Data/Resources.script");

	queue=new Queue(scripting);
	textGame=new sf::Text();
	Font* fnt=resourcesManager->GetFont(1);
	textGame->setColor(fnt->color);
	textGame->setCharacterSize(fnt->size);
	textGame->setFont(fnt->font);

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
	if (choiceBox!=NULL) choiceBox->Update();
	if (textBox!=NULL) textBox->Update();
	queue->Update(delta);
	fadeAlpha=Increment(fadeAlpha,fadeMode*255,fadeSpeed);
	return true;
};

bool Engine::Draw() {
	renderWindow.setView(camera->view);
	renderWindow.clear();
	tilesManager->Draw(renderWindow);
	objectsManager->Draw(renderWindow);
	renderWindow.setView(renderWindow.getDefaultView());
	if (drawNoise) {
		sf::Sprite spr;
		sf::Texture* tex=resourcesManager->GetTexture("sprNoise");
		sf::Vector2u vec=tex->getSize();
		spr.setTexture(*tex);
		spr.setTextureRect(sf::IntRect(rand()%vec.x,rand()%vec.y,vec.x,vec.y));
		spr.setPosition(0,0);//camera->xView,camera->yView);
		spr.setScale(2,2);
		renderWindow.draw(spr);
	};
	if (fadeAlpha!=0) {
		sf::RectangleShape rs;
		rs.setSize(sf::Vector2f(windowSize.x,windowSize.y));
		rs.setFillColor(sf::Color(fadeColor.r,fadeColor.g,fadeColor.b,fadeAlpha));
		rs.setPosition(0,0);//camera->xView,camera->yView);
		renderWindow.draw(rs);
	};
	if (textBox!=NULL) textBox->Draw(renderWindow);
	if (choiceBox!=NULL) choiceBox->Draw(renderWindow);
	if (debug) {
		debugText->setPosition(0,0);//camera->xView,camera->yView);
		std::string str;
		str+=scripting.ToString(delta);
		if (queue!=NULL) {
			str+="\n";
			str+=scripting.ToString(queue->timer.time);
			str+="|";
			str+=scripting.ToString(queue->timer.endTime);
		};
		str+="\n";
		str+=scripting.ToString(camera->xViewPrev-camera->xView);
		str+="\n";
		debugText->setString(str);
		renderWindow.draw(*debugText);
		renderWindow.setView(camera->view);
		sf::RectangleShape rs;
		rs.setSize(static_cast<sf::Vector2f>(objectsManager->chunkSize));
		rs.setFillColor(sf::Color::Transparent);
		rs.setOutlineColor(sf::Color::Cyan);
		rs.setOutlineThickness(1);
		int chunkX = floor(camera->xView/objectsManager->chunkSize.x);
		int chunkY = floor(camera->yView/objectsManager->chunkSize.y);
		int chunkXStart = std::max(0,chunkX-1);
		int chunkYStart = std::max(0,chunkY-1);
		int chunkXEnd = std::min(objectsManager->chunksNumber.x,(chunkX+3)*3);
		int chunkYEnd = std::min(objectsManager->chunksNumber.y,(chunkY+3)*3);
		for(int i=chunkXStart;i<chunkXEnd;i++) {
			for(int j=chunkYStart;j<chunkYEnd;j++) {
				rs.setPosition(rs.getSize().x*i,rs.getSize().y*j);
				renderWindow.draw(rs);
			};
		};
	};
	renderWindow.display();
	return true;
};

bool Engine::Tick() {
	while (renderWindow.isOpen())
	{
		if (gameEnd) renderWindow.close();
		while (renderWindow.pollEvent(inputEvent))
		{
			if (inputEvent.type == sf::Event::Closed) renderWindow.close();
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
	delete variablesGlobal;
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
	path="Data/Maps/";
	path+=name;
	//path+="/";
	//path+=name;
	path+=".resources";
	scripting.ExecuteFile(path);

	path="Data/Maps/";
	path+=name;
	//path+="/";
	//path+=name;
	path+=".script";
	scripting.LoadFile(path);

	path="Data/Maps/";
	path+=name;
	//path+="/";
	//path+=name;
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
		setBloom = IniFindValue(File,"Video","Bloom",0);
		setFullscreen = IniFindValue(File,"Video","Fullscreen",0);
		setFramerateLimit = IniFindValue(File,"Video","FramerateLimit",0);

		//Game settings
		setTextSpeed = IniFindValue(File,"Game","TextSpeed",120);
		setOutline = IniFindValue(File,"Game","Outline",1);

		IniClose(File);
		printf("Settings loaded\n");
	};
};

void Engine::Fade(int mode, float speed) {
	fadeMode=mode;
	if (speed==-1) fadeAlpha=fadeMode*255;
	fadeSpeed=255/speed;
};

void Engine::SetFadeColor(sf::Color color) {
	fadeColor=color;
};