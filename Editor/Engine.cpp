#include "Engine.h"
#include <fstream>

Engine::Engine() {
	//System Stuff
	grab=NULL;
	delta = clock.restart().asSeconds();
	windowSize.x=640;
	windowSize.y=480;
	xView = 0;
	yView = 0;
	renderWindow.create(sf::VideoMode(windowSize.x, windowSize.y), "Charity",sf::Style::Close);
	windowView.setSize(windowSize.x, windowSize.y);
	windowView.setCenter(xView+windowSize.x/2,yView+windowSize.y/2);
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
	resourcesManager->AddTexture("TileSet","Data/Tiles/TileSet.png");
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

	windowView.setCenter(xView+windowSize.x/2,yView+windowSize.y/2);
	renderWindow.setView(windowView);

	if (marker!=NULL) {
		marker->Update();
	};

	//Editor stuff
	if (input->GetKeyIsPressed(sf::Keyboard::Left)) {
		xView-=150*delta;
	} else
		if (input->GetKeyIsPressed(sf::Keyboard::Right)) {
			xView+=150*delta;
		};
	if (input->GetKeyIsPressed(sf::Keyboard::Up)) {
		yView-=150*delta;
	} else
		if (input->GetKeyIsPressed(sf::Keyboard::Down)) {
			yView+=150*delta;
		};
	if (!tiling) {
		if (!create && grab==NULL) {

			if (!enter) {
				//Save
				if (input->GetKeyPressed(sf::Keyboard::F5)) {
					std::cout<<"\nEnter the name of the map to save: ";
					editorFuncString="Enter the name of the map to save: \n";
					path="";
					save=true;
					enter=true;
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
					std::cout<<"\nEnter the name of the map to load: \n";
					editorFuncString="Enter the name of the map to load: \n";
					path="";
					load=true;
					enter=true;
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
					std::cout<<"\nEnter new size: ";
					editorFuncString="Enter new size: \n";
					path="";
					resize=true;
					enter=true;
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
					if (marker==NULL) {
						marker = new Marker();
						marker->grabbed=true;
						marker->SetPosition(input->GetMousePosition().x,input->GetMousePosition().y);
					};
					create=!create;
				};
			};
			//Tiling
			if (!enter) {
				if (input->GetKeyPressed(sf::Keyboard::T)) {
					if (marker==NULL) {
						marker = new Marker(1);
						marker->grabbed=true;
						marker->SetPosition(input->GetMousePosition().x,input->GetMousePosition().y);
						marker->SetTexture(resourcesManager->GetTexture(editorTilesIndex));
					};
					tiling=!tiling;
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
				if (marker!=NULL) {
					delete marker;
					marker=NULL;
				};
				create=!create;
			};
			if (input->GetKeyPressed(sf::Keyboard::Numpad4)) {
				if (editorObjectIndex>0) {
					editorObjectIndex-=1;
				};
			};
			if (input->GetKeyPressed(sf::Keyboard::Numpad6)) {
				if (editorObjectIndex<5) {
					editorObjectIndex+=1;
				};
			};
		};
	} else {
		//Tiling
		if (input->GetKeyPressed(sf::Keyboard::T)) {
			if (marker!=NULL) {
				delete marker;
				marker=NULL;
			};
			tiling=!tiling;
		};
		if (input->GetKeyPressed(sf::Keyboard::Numpad7)) {
			if (editorTilesIndex>0) {
				editorTilesIndex-=1;
				marker->SetTexture(resourcesManager->GetTexture(editorTilesIndex));
				editorTileName=resourcesManager->GetTextureName(editorTilesIndex);
			};
		};
		if (input->GetKeyPressed(sf::Keyboard::Numpad1)) {
			if (editorTilesIndex<resourcesManager->GetTexturesNumber()-1) {
				editorTilesIndex+=1;
				marker->SetTexture(resourcesManager->GetTexture(editorTilesIndex));
				editorTileName=resourcesManager->GetTextureName(editorTilesIndex);
			};
		};
		if (input->GetKeyPressed(sf::Keyboard::Numpad8)) {
			if (row>0) {
				row-=1;
				marker->SetCell(col,row);
			};
		};
		if (input->GetKeyPressed(sf::Keyboard::Numpad2)) {
			if (row<floor(static_cast<float>(resourcesManager->GetTexture(editorTilesIndex)->getSize().y)/32)) {
				row+=1;
				marker->SetCell(col,row);
			};
		};
		if (input->GetKeyPressed(sf::Keyboard::Numpad4)) {
			if (col>0) {
				col-=1;
				marker->SetCell(col,row);
			};
		};
		if (input->GetKeyPressed(sf::Keyboard::Numpad6)) {
			if (col<floor(static_cast<float>(resourcesManager->GetTexture(editorTilesIndex)->getSize().x)/32)) {
				col+=1;
				marker->SetCell(col,row);
			};
		};
		if (input->GetKeyPressed(sf::Keyboard::Numpad3)) {
			if (editorTilesLayer>0) {
				editorTilesLayer-=1;
			};
		};
		if (input->GetKeyPressed(sf::Keyboard::Numpad9)) {
			editorTilesLayer+=1;
		};
		if (input->GetKeyPressed(sf::Keyboard::Numpad5)) {
			editorTilesLayer=0;
		};
	};
	return true;
};

void Engine::MouseCheck() {
	if (!save && !load) {
		if (grab!=NULL) {
			grab->depth=grab->y;
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
							grab->Grab(false);
							grab=NULL;
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
						Tile* temp = tilesManager->AddTile(editorTileName,marker->x-16,marker->y-16,col,row);
						temp->layer=editorTilesLayer;
					} else {
						if (temp->layer!=editorTilesLayer) {
							Tile* temp = tilesManager->AddTile(editorTileName,marker->x-16,marker->y-16,col,row);
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
								};
							} else if (editorCollision->objectIndex==3) {
								if (!enter) {
									path="";
									addinfo=true;
									addinfotype=3;
									std::cout<<"\nEnter the sprite's name: \n";
									editorFuncString="Enter the sprite's name: \n";
								};
							} else if (editorCollision->objectIndex==4) {
								if (!enter) {
									path="";
									addinfo=true;
									addinfotype=4;
									std::cout<<"\nEnter the function's name: \n";
									editorFuncString="Enter the function's name: \n";
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
	sf::RectangleShape rs;
	rs.setSize(sf::Vector2f(windowSize.x*objectsManager->chunksNumber.x,windowSize.y*objectsManager->chunksNumber.y));
	rs.setFillColor(sf::Color(55,95,170,0));
	rs.setOutlineColor(sf::Color(55,95,170,255));
	rs.setOutlineThickness(1);
	renderWindow.draw(rs);

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
	return objectsManager->LoadMap(path);
};