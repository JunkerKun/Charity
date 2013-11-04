#include "Engine.h"
#include <fstream>

Engine::Engine() {
	//System Stuff
	grab=NULL;
	editorGridSize=64;
	delta = clock.restart().asSeconds();
	windowSize.x=800;
	windowSize.y=600;
	xView = 0;
	yView = 0;
	renderWindow.create(sf::VideoMode(windowSize.x+224, windowSize.y), "Charity",sf::Style::Close);
	renderWindow.setFramerateLimit(60);
	windowView.setSize(windowSize.x, windowSize.y);
	windowView.setCenter(xView+windowSize.x/2,yView+windowSize.y/2);
	windowView.setViewport(sf::FloatRect(0,0,0.78125,1));

	miniView.setSize(windowSize.x, windowSize.y);
	miniView.setCenter(xView+windowSize.x/2,yView+windowSize.y/2);
	miniView.setViewport(sf::FloatRect(0.80468,0.033333,0.98046,0.25833));

	renderWindow.setView(windowView);
	pathPlayerSprite="Data/Sprites/Charity.png";
	save=false;
	load=false;
	create=false;
	resize=false;
	tiling=false;
	enter=false;
	addinfo=false;
	marker=NULL;
	tileMarker=NULL;
	editorObjectIndex=0;
	editorTilesIndex=0;
	editorTilesLayer=0;
	addinfotype=0;
	editorTileName="";
	editorUsablesNumber=0;
	editorPlaceIndex=0;
	col=0;
	row=0;
	path="";
	isFocused=false;
	drawChunks=true;
	drawGrid=true;

	font.loadFromFile("Data/Fonts/STREET.ttf");
	text.setColor(sf::Color::White);
	text.setCharacterSize(14);
	text.setFont(font);

};

void Engine::Begin() {
	input=new Input(&inputEvent);
	objectsManager = new ObjectsManager();
	tilesManager=new TilesManager();
	resourcesManager=new ResourcesManager();

	std::ifstream load("Data/ResourcesEditor.script");
	if (load) {
		std::string str;
		std::string str2;
		while(load) {
			load>>str;
			load>>str2;
			resourcesManager->AddTexture(str,str2);
		};
		load.close();
	}

	//resourcesManager->AddTexture("TileSet","Data/Tiles/TileSet.png");
	editorTileName=resourcesManager->GetTextureName(editorTilesIndex);
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

	input->Update();
	MouseCheck();

	objectsManager->Update();

	if (xView<0) xView=0;
	if (yView<0) yView=0;
	if (xView+windowSize.x>objectsManager->chunkSize.x*objectsManager->chunksNumber.x) xView=objectsManager->chunkSize.x*objectsManager->chunksNumber.x-windowSize.x;
	if (yView+windowSize.y>objectsManager->chunkSize.y*objectsManager->chunksNumber.y) yView=objectsManager->chunkSize.y*objectsManager->chunksNumber.y-windowSize.y;
	windowView.setCenter(xView+windowSize.x/2,yView+windowSize.y/2);
	miniView.setCenter(xView+windowSize.x/2,yView+windowSize.y/2);
	renderWindow.setView(windowView);

	if (marker!=NULL) {
		marker->Update();
	};

	//Editor stuff
	if (!enter) {
	if (input->GetKeyPressed(sf::Keyboard::J)) {
		EditorMoveDown();
	};
	};

	if (input->GetKeyIsPressed(sf::Keyboard::Left)) {
		if (!input->GetKeyIsPressed(sf::Keyboard::LShift)) xView-=150*delta;
		else xView-=300*delta;
	} else
		if (input->GetKeyIsPressed(sf::Keyboard::Right)) {
			if (!input->GetKeyIsPressed(sf::Keyboard::LShift)) xView+=150*delta;
			else xView+=300*delta;
		};
	if (input->GetKeyIsPressed(sf::Keyboard::Up)) {
		if (!input->GetKeyIsPressed(sf::Keyboard::LShift)) yView-=150*delta;
		else yView-=300*delta;
	} else
		if (input->GetKeyIsPressed(sf::Keyboard::Down)) {
			if (!input->GetKeyIsPressed(sf::Keyboard::LShift)) yView+=150*delta;
			else yView+=300*delta;
		};
	if (!tiling) {
		if (!create && grab==NULL) {

			if (!enter) {
				//Save
				if (input->GetKeyPressed(sf::Keyboard::F5)) {
					EditorF5();
				};
			};

			if (save) {
				if (input->GetAnyKeyPressed(true)) {
					char letter = input->GetLetterPressed();
					path+=letter;
					//std::cout<<letter;
				};
				if (input->GetKeyPressed(sf::Keyboard::BackSpace)) {
					path=path.substr(0,path.size()-1);
				};
				if (input->GetKeyPressed(sf::Keyboard::Return)) {
					std::string fullPath="Data/Maps/";
					fullPath+=path;
					fullPath+=".map";
					SaveMap(fullPath);
					std::cout<<"\nMap saved\n";
					save=false;
					enter=false;
				};
				if (input->GetKeyPressed(sf::Keyboard::Escape)) {
					std::cout<<"\nSaving canceled\n";
					save=false;
					enter=false;
				};
			};
			//Load
			if (!enter) {
				if (input->GetKeyPressed(sf::Keyboard::F9)) {
					EditorF9();
				};
			};

			if (load) {
				if (input->GetAnyKeyPressed(true)) {
					char letter = input->GetLetterPressed();
					path+=letter;
					//std::cout<<letter;
				};
				if (input->GetKeyPressed(sf::Keyboard::BackSpace)) {
					path=path.substr(0,path.size()-1);
				};
				if (input->GetKeyPressed(sf::Keyboard::Return)) {
					std::string fullPath="Data/Maps/";
					fullPath+=path;
					fullPath+=".map";
					LoadMap(fullPath);
					std::cout<<"\nMap loaded\n";
					load=false;
					enter=false;
				};
				if (input->GetKeyPressed(sf::Keyboard::Escape)) {
					std::cout<<"\nLoading canceled\n";
					load=false;
					enter=false;
				};
			};
			//Resize
			if (!enter) {
				if (input->GetKeyPressed(sf::Keyboard::F2)) {
					EditorF2();
				};
			};

			if (resize) {
				if (input->GetAnyKeyPressed(true)) {
					char letter = input->GetLetterPressed();
					path+=letter;
					//std::cout<<letter;
					enter=true;
				};
				if (input->GetKeyPressed(sf::Keyboard::BackSpace)) {
					path=path.substr(0,path.size()-1);
				};
				if (input->GetKeyPressed(sf::Keyboard::Return)) {
					objectsManager->Resize(stoi(path),stoi(path));
					tilesManager->Resize(stoi(path),stoi(path));
					resize=false;
					enter=false;
				};
				if (input->GetKeyPressed(sf::Keyboard::Escape)) {
					std::cout<<"\nResizing canceled\n";
					resize=false;
					enter=false;
				};
			};
			//Create
			if (!enter) {
				if (input->GetKeyPressed(sf::Keyboard::Space)) {
					EditorSpace();
				};
			};
			//Tiling
			if (!enter) {
				if (input->GetKeyPressed(sf::Keyboard::T)) {
					EditorT();
				};
			};
		} else {
			if (addinfo) {
				if (input->GetAnyKeyPressed(true)) {
					char letter = input->GetLetterPressed();
					path+=letter;
					//std::cout<<letter;
					enter=true;
				};
				if (input->GetKeyPressed(sf::Keyboard::BackSpace)) {
					path=path.substr(0,path.size()-1);
				};
				if (input->GetKeyPressed(sf::Keyboard::Return)) {
					addinfo=false;
					enter=false;
					switch(addinfotype) {
					case 2: {
						Usable* use=static_cast<Usable*>(editorCollision);
						use->function=path;
						break;
							};
					case 3: {
						Decoration* dec=static_cast<Decoration*>(editorCollision);
						dec->spriteName=path;
						break;
							};
					case 4: {
						Trigger* trg=static_cast<Trigger*>(editorCollision);
						trg->function=path;
						break;
							};
					default:
						break;
					};
					std::cout<<"\nAdditional info added\n";
				};
				if (input->GetKeyPressed(sf::Keyboard::Escape)) {
					std::cout<<"\nAdditional info adding canceled\n";
					addinfo=false;
					enter=false;
				};
			};
			if (input->GetKeyPressed(sf::Keyboard::Space)) {
				EditorSpace();
			};
			if (input->GetKeyPressed(sf::Keyboard::Numpad4)) {
				EditorNum4();
			};
			if (input->GetKeyPressed(sf::Keyboard::Numpad6)) {
				EditorNum6();
			};
		};
	} else {
		//Tiling
		if (input->GetKeyPressed(sf::Keyboard::T)) {
			EditorT();
		};
		if (input->GetKeyPressed(sf::Keyboard::Numpad7)) {
			EditorNum7();
		};
		if (input->GetKeyPressed(sf::Keyboard::Numpad1)) {
			EditorNum1();
		};
		if (input->GetKeyPressed(sf::Keyboard::Numpad8)) {
			EditorNum8();
		};
		if (input->GetKeyPressed(sf::Keyboard::Numpad2)) {
			EditorNum2();
		};
		if (input->GetKeyPressed(sf::Keyboard::Numpad4)) {
			EditorNum4();
		};
		if (input->GetKeyPressed(sf::Keyboard::Numpad6)) {
			EditorNum6();
		};
		if (input->GetKeyPressed(sf::Keyboard::Numpad3)) {
			EditorNum3();
		};
		if (input->GetKeyPressed(sf::Keyboard::Numpad9)) {
			EditorNum9();
		};
		if (input->GetKeyPressed(sf::Keyboard::Numpad5)) {
			EditorNum5();
		};
	};
	return true;
};

void Engine::EditorSpace() {
	if (marker!=NULL) {
		delete marker;
		marker=NULL;
	} else
		if (marker==NULL) {
			marker = new Marker();
			marker->grabbed=true;
			marker->SetPosition(input->GetMousePosition().x,input->GetMousePosition().y);
		};
	create=!create;
};

void Engine::EditorF2() {
	std::cout<<"\nEnter new size: ";
	editorFuncString="Enter new size: \n";
	path="";
	resize=true;
	enter=true;
};

void Engine::EditorF5() {
	std::cout<<"\nEnter the name of the map to save: ";
	editorFuncString="Enter the name of the map to save: \n";
	path="";
	save=true;
	enter=true;
};

void Engine::EditorF9() {
	std::cout<<"\nEnter the name of the map to load: \n";
	editorFuncString="Enter the name of the map to load: \n";
	path="";
	load=true;
	enter=true;
};

void Engine::EditorT() {
	if (marker==NULL) {
		marker = new Marker(1);
		marker->grabbed=true;
		marker->SetPosition(input->GetMousePosition().x,input->GetMousePosition().y);
		marker->SetTexture(resourcesManager->GetTexture(editorTilesIndex));
	} else
		if (marker!=NULL) {
			delete marker;
			marker=NULL;
		};
	tiling=!tiling;
};

void Engine::EditorNum1() {
	if (editorTilesIndex<resourcesManager->GetTexturesNumber()-1) {
		editorTilesIndex+=1;
		marker->SetTexture(resourcesManager->GetTexture(editorTilesIndex));
		editorTileName=resourcesManager->GetTextureName(editorTilesIndex);
	};
};

void Engine::EditorNum2() {
	if (row<floor(static_cast<float>(resourcesManager->GetTexture(editorTilesIndex)->getSize().y)/editorGridSize)) {
		row+=1;
		marker->SetCell(col,row);
	};
};

void Engine::EditorNum3() {
	if (editorTilesLayer>0) {
		editorTilesLayer-=1;
	};
};


void Engine::EditorNum4() {
	if (create) {
		if (editorObjectIndex>0) {
			editorObjectIndex-=1;
		};
	} else
		if (tiling) {
			if (col>0) {
				col-=1;
				marker->SetCell(col,row);
			};
		};
};

void Engine::EditorNum5() {
	editorTilesLayer=0;
};

void Engine::EditorNum6() {
	if (create) {
		if (editorObjectIndex<5) {
			editorObjectIndex+=1;
		};
	} else
		if (tiling) {
			if (col<floor(static_cast<float>(resourcesManager->GetTexture(editorTilesIndex)->getSize().x)/editorGridSize)) {
				col+=1;
				marker->SetCell(col,row);
			};
		};
};

void Engine::EditorNum7() {
	if (editorTilesIndex>0) {
		editorTilesIndex-=1;
		marker->SetTexture(resourcesManager->GetTexture(editorTilesIndex));
		editorTileName=resourcesManager->GetTextureName(editorTilesIndex);
	};
};

void Engine::EditorNum8() {
	if (row>0) {
		row-=1;
		marker->SetCell(col,row);
	};
};

void Engine::EditorNum9() {
	editorTilesLayer+=1;
};

void Engine::EditorMoveDown() {
	for(int i=0;i<objectsManager->chunksNumber.x;i++) {
			for(int j=0;j<objectsManager->chunksNumber.y;j++) {
				for (int k=0;k<objectsManager->chunks->at(i)->at(j)->list->size();k++) {
					Object* obj = objectsManager->chunks->at(i)->at(j)->list->at(k);
					obj->SetPosition(obj->x,obj->y+64);
					//obj->MoveToChunk();
				};
			};
		};
};


void Engine::MouseCheck() {
	if (input->GetMousePosition().x>800-editorGridSize/2) {
		if (input->GetMouseIsPressed(sf::Mouse::Left)) {
			if (input->GetMousePosition().x>824) {
				if (input->GetMousePosition().y>20) {
					if (input->GetMousePosition().x<824+180) {
						if (input->GetMousePosition().y<20+135) {
							float xS=(objectsManager->chunkSize.x*objectsManager->chunksNumber.x);
							float yS=(objectsManager->chunkSize.y*objectsManager->chunksNumber.y);
							int xx, yy;
							xx=input->GetMousePosition().x-824;
							xView=xx*(xS/180)-windowSize.x/2;
							yy=input->GetMousePosition().y-20;
							yView=yy*(yS/135)-windowSize.y/2;
						};
					};
				};
			};
		};
		return;
	};
	if (input->GetMousePosition().y>600-editorGridSize/2) return;
	if (!save && !load) {
		if (grab!=NULL) {
			grab->depth=grab->y;
		};
		if (input->GetMouseIsPressed(sf::Mouse::Middle)) {
			xView+=input->GetLastMousePosition().x-input->GetMousePosition().x;
			yView+=input->GetLastMousePosition().y-input->GetMousePosition().y;
			return;
		};
		if ((input->GetMousePressed(sf::Mouse::Left)) ||
			(input->GetMouseIsPressed(sf::Mouse::Left) && input->GetKeyIsPressed(sf::Keyboard::LControl))
			) {
				if (!tiling) {
					if (!create) {
						if (grab==NULL) {
							Object* collision=CollisionCheckPoint(input->GetMousePosition(true).x,input->GetMousePosition(true).y,-1);
							if (collision!=NULL) {
								collision->Grab(); 
								grab=collision;
							};

						} else {
							if (!input->GetKeyIsPressed(sf::Keyboard::LAlt)) {
							grab->Grab(false);
							grab=NULL;
							} else {
								Object* obj = objectsManager->AddObject(grab->x,grab->y,grab->objectIndex);
								if (obj->objectIndex==3) {
									Decoration* dec = static_cast<Decoration*>(obj);
									Decoration* dec2 = static_cast<Decoration*>(grab);
									dec->spriteName=dec2->spriteName;
								} else if (obj->objectIndex==4) {
									Trigger* dec = static_cast<Trigger*>(obj);
									Trigger* dec2 = static_cast<Trigger*>(grab);
									dec->function=dec2->function;
								};
							};
						};
					} else {
						if (objectsManager->GetObjectAt(marker->x,marker->y)==NULL) {
							if (editorObjectIndex==5) {
								objectsManager->AddObject(marker->x,marker->y,editorObjectIndex,ToString(editorPlaceIndex));
								editorPlaceIndex++;
							} else objectsManager->AddObject(marker->x,marker->y,editorObjectIndex);
						};
					};
				} else {
					Tile* temp = tilesManager->GetTileAt(marker->x,marker->y);
					if (temp==NULL) {
						Tile* temp = tilesManager->AddTile(editorTileName,marker->x-(editorGridSize/2),marker->y-(editorGridSize/2),col,row);
						temp->layer=editorTilesLayer;
					} else {
						if (temp->layer!=editorTilesLayer) {
							Tile* temp = tilesManager->AddTile(editorTileName,marker->x-(editorGridSize/2),marker->y-(editorGridSize/2),col,row);
							temp->layer=editorTilesLayer;
						};
					};
				};
		} else if ((input->GetMousePressed(sf::Mouse::Right)) ||
			(input->GetMouseIsPressed(sf::Mouse::Right) && input->GetKeyIsPressed(sf::Keyboard::LControl))
			) {
				if (!tiling) {
					if (!create) {
						objectsManager->DeleteAt(input->GetMousePosition(true).x,input->GetMousePosition(true).y);
					} else {
						editorCollision = objectsManager->GetObjectAt(input->GetMousePosition(true).x,input->GetMousePosition(true).y);
						if (editorCollision) {
							if (editorCollision->objectIndex==2) {
								if (!enter) {
									path="";
									addinfo=true;
									addinfotype=2;
									std::cout<<"\nEnter the function's name: \n";
									editorFuncString="Enter the function's name\n";
									enter=true;
								};
							} else if (editorCollision->objectIndex==3) {
								if (!enter) {
									path="";
									addinfo=true;
									addinfotype=3;
									std::cout<<"\nEnter the sprite's name: \n";
									editorFuncString="Enter the sprite's name: \n";
									enter=true;
								};
							} else if (editorCollision->objectIndex==4) {
								if (!enter) {
									path="";
									addinfo=true;
									addinfotype=4;
									std::cout<<"\nEnter the function's name: \n";
									editorFuncString="Enter the function's name: \n";
									enter=true;
								};
							};
						};
					};
				} else {
					Tile* temp = tilesManager->GetTileAt(marker->x,marker->y);
					if (temp!=NULL) {
						//if (temp->depth==editorTilesDepth) 
						tilesManager->DeleteAt(marker->x,marker->y);
					}
				};
		};
	};
};

bool Engine::Draw() {
	renderWindow.clear();

	tilesManager->Draw(renderWindow);
	objectsManager->Draw(renderWindow);
	if (marker!=NULL) {
		marker->Draw(renderWindow);
	};
	renderWindow.setView(renderWindow.getDefaultView());
	std::string str="Sizes [x: ";
	str+=ToString(objectsManager->chunksNumber.x);
	str+="| y: ";
	str+=ToString(objectsManager->chunksNumber.y);
	str+="]\n";
	str+=ToString(xView);
	str+="|";
	str+=ToString(yView);
	str+="\n";
	if (marker!=NULL) {
		str+="Texture name: ";
		str+=resourcesManager->GetTextureName(marker->tex);
		str+="\n";
	};
	if (marker!=NULL) {
		str+=ToString(marker->x);
		str+="|";
		str+=ToString(marker->y);
	};
	if (tiling) {
		str+="\nTiles layer: ";
		str+=ToString(editorTilesLayer);
	};
	if (enter) {
		str+="\n";
		str+=editorFuncString;
		str+=path;
	};
	text.setString(str);
	renderWindow.draw(text);

	renderWindow.setView(windowView);

	sf::RectangleShape rs;
	if(drawChunks) {
	rs.setSize(sf::Vector2f(objectsManager->chunkSize));
	rs.setFillColor(sf::Color::Transparent);
	rs.setOutlineColor(sf::Color::Cyan);
	rs.setOutlineThickness(1);
	int chunkX = floor(xView/objectsManager->chunkSize.x);
	int chunkY = floor(yView/objectsManager->chunkSize.y);
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

	if (drawGrid) {
	rs.setSize(sf::Vector2f(editorGridSize,editorGridSize));
	rs.setFillColor(sf::Color::Transparent);
	rs.setOutlineColor(sf::Color(255,255,255,64));
	rs.setOutlineThickness(1);

	int xF=floor(xView/editorGridSize)*editorGridSize;
	int yF=floor(yView/editorGridSize)*editorGridSize;
	int xC=windowSize.x/editorGridSize+2;
	int yC=windowSize.y/editorGridSize+2;
	for(int i=0;i<xC;i++) {
		for(int j=0;j<yC;j++) {
			rs.setPosition(xF+editorGridSize*i,yF+editorGridSize*j);
			renderWindow.draw(rs);
		};
	};
	}

	//Toolbar
	renderWindow.setView(renderWindow.getDefaultView());
	rs.setSize(sf::Vector2f(224,600));
	rs.setFillColor(sf::Color(1,28,97,255));
	rs.setOutlineThickness(0);
	rs.setPosition(800,0);
	renderWindow.draw(rs);

	rs.setSize(sf::Vector2f(180,135));
	rs.setFillColor(sf::Color::Black);
	rs.setOutlineColor(sf::Color::White);
	rs.setOutlineThickness(1);
	rs.setPosition(824,20);
	renderWindow.draw(rs);

	float xS=(objectsManager->chunkSize.x*objectsManager->chunksNumber.x)/800.;
	if (xS==0) xS=1;
	float yS=(objectsManager->chunkSize.y*objectsManager->chunksNumber.y)/600.;
	if (yS==0) yS=1;

	rs.setSize(sf::Vector2f(180/xS,135/yS));
	rs.setFillColor(sf::Color(255,255,255,64));
	rs.setPosition(824+xView/((xS*800)/180),20+yView/((yS*600)/135));
	rs.setOutlineColor(sf::Color::Cyan);
	renderWindow.draw(rs);

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
					if (!save && !load && !resize && !enter && !addinfo) {
						renderWindow.close();
					};
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
	delete tilesManager;
	delete objectsManager;
	delete resourcesManager;
	delete input;
};

bool Engine::SaveMap(std::string path) {
	return objectsManager->SaveMap(path);
};

bool Engine::LoadMap(std::string path) {
	editorPlaceIndex=0;
	return objectsManager->LoadMap(path);
};