#include "Player.h"
#include "Engine.h"

extern Engine* engine;

Player::Player(sf::Texture* tex):Image(tex) {
	collisionTrigger=NULL;
	maxHP=100;
	maxSP=100;
	maxMP=100;
	playerHP=&engine->playerHP;
	playerSP=&engine->playerSP;
	playerMP=&engine->playerMP;
	drawHP=engine->drawHP;
	drawSP=engine->drawSP;
	drawMP=engine->drawMP;
	speed=126;
	objectIndex=1;
	direction=0;
	directionTo=1;
	directionDelta=1;
	canMove=true;
	drawExclamation=false;
	canPlaySound=true;
	SetBBox(-engine->gridSize/2+engine->gridSize/32*4,-engine->gridSize/2+engine->gridSize/32*8,
		engine->gridSize-engine->gridSize/32*8,engine->gridSize-engine->gridSize/32*15);
	//SetBBox(-28,-18,56,36);
	SetOrigin(imageWidth/2,imageHeight-6*engine->gridSize/32);

	AddSequence(0,0,0.25);
	AddSequence(5,5,0.25);
	AddSequence(10,10,0.25);
	AddSequence(15,15,0.25);

	AddSequence(1,4,0.2);
	AddSequence(6,9,0.2);
	AddSequence(11,14,0.2);
	AddSequence(16,19,0.2);

	SetSequence(0,1);
	engine->camera->target=this;
	engine->objectsManager->SetPlayer(this);
	sprShadow.setTexture(*engine->resourcesManager->GetTexture("sprShadow"),true);
	sprShadow.setOrigin(engine->resourcesManager->GetTexture("sprShadow")->getSize().x/2,
		engine->resourcesManager->GetTexture("sprShadow")->getSize().y/2-2);
	sprExclamation.setTexture(*engine->resourcesManager->GetTexture("sprExclamation"),true);
	sprExclamation.setOrigin(engine->resourcesManager->GetTexture("sprExclamation")->getSize().x/2,
		engine->resourcesManager->GetTexture("sprExclamation")->getSize().y/2);

	sndStep[0]=engine->resourcesManager->GetSound("sndStep1")->sound;
	sndStep[1]=engine->resourcesManager->GetSound("sndStep2")->sound;
	sndStep[2]=engine->resourcesManager->GetSound("sndStep3")->sound;
	sndStep[3]=engine->resourcesManager->GetSound("sndStep4")->sound;

	overlayHP=NULL;
	overlaySP=NULL;
	overlayMP=NULL;

	directions[0]=3;
	directions[1]=0;
	directions[2]=1;
	directions[3]=2;
	directions[4]=3;
	directions[5]=0;

	directionSpin=1;

	overlayHPBar = engine->objectsManager->AddOverlay(engine->resourcesManager->GetTexture("UIHealthBarLine"));
	overlayHPBar->Lock();
	overlayHPBar->AddSequence(0,0,0);
	overlayHPBar->SetPosition(53,71-overlayHPBar->imageHeight);

	overlayHP = engine->objectsManager->AddOverlay(engine->resourcesManager->GetTexture("UIHealthBar"));
	overlayHP->Lock();
	overlayHP->AddSequence(0,0,0);
};

bool Player::Update() {
	xPrev=x;
	yPrev=y;
	drawExclamation=false;

	if (overlayHP!=NULL) {
		overlayHP->visible=drawHP;
		overlayHPBar->visible=drawHP;
		if (drawHP) {
			overlayHPBar->GetSprite().setTextureRect(sf::IntRect(0,0,
				static_cast<int>((overlayHPBar->imageWidth/100.f)**playerHP),overlayHPBar->imageHeight));
		};
	};
	if (overlaySP!=NULL) {
		overlaySP->visible=drawSP;
	};
	if (overlayMP!=NULL) {
		overlayMP->visible=drawMP;
	};
	if (!isControlled && !isBlocked) {
		isMoving=false;
		if (canMove) {
			if (engine->input->GetKeyIsPressed(sf::Keyboard::Left)) {
				isMoving=true;
				x-=speed*engine->GetDelta();
				//if (directionTo>=4) {directionDelta=0; directionSpin=-1;}
				directionTo=2;
			} else if (engine->input->GetKeyIsPressed(sf::Keyboard::Right)) {
				isMoving=true;
				x+=speed*engine->GetDelta();
				if (directionTo<=1) {directionTo=0; directionSpin=-1;}
				else
				if (directionTo==2) {directionTo=0; directionSpin=-1;}
				else directionTo=4;
			} else
				if (engine->input->GetKeyIsPressed(sf::Keyboard::Up)) {
					isMoving=true;
					y-=speed*0.8*engine->GetDelta();
					directionTo=3;
				} else if (engine->input->GetKeyIsPressed(sf::Keyboard::Down)) {
					isMoving=true;
					y+=speed*0.8*engine->GetDelta();
					if (directionTo>=4) {directionTo=5; directionSpin=1;}
					else directionTo=1;
				};

				if (isMoving) {
					if (CollisionCheckIntersect(this,0)) {
						switch(directionTo) {
						case 1:
						case 5:
							y-=speed*0.8*engine->GetDelta();
							isMoving=false;
							break;
						case 2:
							x+=speed*engine->GetDelta();
							isMoving=false;
							break;
						case 3:
							y+=speed*0.8*engine->GetDelta();
							isMoving=false;
							break;
						case 4:
						case 0:
							x-=speed*engine->GetDelta();
							isMoving=false;
							break;
						};
					};
				};
				if (isMoving) {
					if (directionTo==2) engine->camera->angleTo=-1;
					else
						if (directionTo==4 || directionTo==0) engine->camera->angleTo=1;
						else engine->camera->angleTo=0;
				} else {
					engine->camera->angleTo=0;
				};

				Object* collision=CollisionCheckRadius(68,x,y,2);
				if (collision!=NULL) {
					if (collision->active) drawExclamation=true;
					else drawExclamation=false;
				} else {
					drawExclamation=false;
				};

				if (engine->input->GetKeyPressed(sf::Keyboard::Z)) {
					Block blk;
					blk.SetBBox(-26,-14,52,28);
					blk.chunk.x=chunk.x;
					blk.chunk.y=chunk.y;
					switch(static_cast<int>(direction)) {
					case 0:{
						blk.SetBBox(-14,-22,28,44);
						blk.x=x;
						blk.y=y+20;
						break;
							};
					case 1: {
						blk.x=x-16;
						blk.y=y;
						break;
							};
					case 2: {
						blk.SetBBox(-14,-22,28,44);
						blk.x=x;
						blk.y=y-16;
						break;
							};
					case 3:{
						blk.x=x+20;
						blk.y=y;
						break;
							 };
					};
					collision=CollisionCheckIntersect(&blk,2);
					if (collision!=NULL) {
						if (collision->active) {
							Usable* use=static_cast<Usable*>(collision);
							if (use->function!="none") engine->scripting.ExecuteFunction(engine->scripting.StringToWString(use->function));
						};
					};
				};
		};
	}
	directionDelta=Increment(directionDelta,directionTo,6);
	if (directionDelta>4) {
		if (directionSpin==1) {
		directionTo=1;
		directionDelta-=4;
		};
	};
	if (directionDelta<1) {
		if (directionSpin==-1) {
		directionTo=4;
		directionDelta+=4;
		};
	};
	direction=directions[static_cast<int>(Round(directionDelta))];
	if (!isControlled && isBlocked) isMoving=false;

	MoveToChunk();

	int seq=static_cast<int>(direction);
	SetSequence(seq+4*(isMoving));
	Image::Update();
	sprShadow.setPosition(floor(x),floor(y));

	if (canPlaySound) {
		if ((imageFrame>=1+5*direction && imageFrame<2+5*direction) || 
			(imageFrame>=3+5*direction && imageFrame<4+5*direction)) {
				int sound=rand()%3;
				sndStep[sound]->play();
				canPlaySound=false;
		};
	} else {
		int temp=5*static_cast<int>(direction);
		if ((imageFrame>=2+temp && imageFrame<3+temp) || 
			imageFrame>=4+temp || imageFrame<1+temp) {
				canPlaySound=true;
		};
	};

	//if (collisionTrigger==NULL) {
	collisionTrigger=CollisionCheckIntersect(this,4);
	if (collisionTrigger!=NULL) {
		if (collisionTrigger->active) {
			Trigger* trg=static_cast<Trigger*>(collisionTrigger);
			if (trg->function!="none") {
				engine->scripting.ExecuteFunction(engine->scripting.StringToWString(trg->function));
			};
		};
	};
	//};
	return true;
};

bool Player::Draw(sf::RenderTarget &RT) {
	if (!visible) return true;
	RT.draw(sprShadow);
	Image::Draw(RT);
	if (drawExclamation) {
		sprExclamation.setPosition(x,y-72);
		RT.draw(sprExclamation);
	};
	if (engine->debug) {
		Block blk;
		blk.SetBBox(-26,-14,52,28);
		switch(static_cast<int>(direction)) {
		case 0: {
			blk.SetBBox(-14,-22,28,44);
			blk.x=x;
			blk.y=y+16;
			break;
				};
		case 1: {
			blk.x=x-20;
			blk.y=y;
			break;
				};
		case 2: {
			blk.SetBBox(-14,-22,28,44);
			blk.x=x;
			blk.y=y-16;
			break;
				};
		case 3: {
			blk.x=x+20;
			blk.y=y;
			break;
				};
		};
		sf::RectangleShape RS;
		RS.setPosition(blk.x+blk.GetBBox().left,blk.y+blk.GetBBox().top);
		RS.setSize(sf::Vector2f(blk.GetBBox().width,blk.GetBBox().height));
		RS.setFillColor(sf::Color(200,0,0,120));
		RT.draw(RS);
	}
	return true;
};

void Player::SetDirection(int dir, bool instant) {
	if (instant) {
		direction=dir;
		directionDelta=dir+1;
	};
	directionTo=dir+1;
};

int Player::GetDirection() {
	return directionTo;
};

Player::~Player() {

};