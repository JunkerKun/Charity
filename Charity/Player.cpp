#include "Player.h"
#include "Engine.h"

extern Engine* engine;

Player::Player(sf::Texture* tex):Image(tex) {
	collisionTrigger=NULL;
	maxHp=100;
	hp=maxHp;
	speed=105;
	objectIndex=1;
	direction=0;
	canMove=true;
	drawExclamation=false;
	canPlaySound=true;
	SetBBox(-engine->gridSize/2+engine->gridSize/32*2,-engine->gridSize/2+engine->gridSize/32*7,
		engine->gridSize-engine->gridSize/32*4,engine->gridSize-engine->gridSize/32*12);
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
};

bool Player::Update() {
	xPrev=x;
	yPrev=y;
	drawExclamation=false;
	if (!isControlled && !isBlocked) {
		isMoving=false;
		if (canMove) {
			if (engine->input->GetKeyIsPressed(sf::Keyboard::Left)) {
				isMoving=true;
				x-=speed*engine->GetDelta();
				direction=1;
				/*if (CollisionCheckIntersect(this,0)) {
					x+=speed*engine->GetDelta();
				};*/
			} else if (engine->input->GetKeyIsPressed(sf::Keyboard::Right)) {
				isMoving=true;
				x+=speed*engine->GetDelta();
				direction=3;
				/*if (CollisionCheckIntersect(this,0)) {
					x-=speed*engine->GetDelta();
				};*/
			} else
			if (engine->input->GetKeyIsPressed(sf::Keyboard::Up)) {
				isMoving=true;
				y-=speed*0.8*engine->GetDelta();
				direction=2;
				/*if (CollisionCheckIntersect(this,0)) {
					y+=speed*0.8*engine->GetDelta();
				};*/
			} else if (engine->input->GetKeyIsPressed(sf::Keyboard::Down)) {
				isMoving=true;
				y+=speed*0.8*engine->GetDelta();
				direction=0;
				/*if (CollisionCheckIntersect(this,0)) {
					y-=speed*0.8*engine->GetDelta();
				};*/
			};

			if (isMoving) {
				if (CollisionCheckIntersect(this,0)) {
					switch(direction) {
					case 0:
						y-=speed*0.8*engine->GetDelta();
						isMoving=false;
						break;
					case 1:
						x+=speed*engine->GetDelta();
						isMoving=false;
						break;
					case 2:
						y+=speed*0.8*engine->GetDelta();
						isMoving=false;
						break;
					case 3:
						x-=speed*engine->GetDelta();
						isMoving=false;
						break;
					};
				};
			};
			if (isMoving) {
				if (direction==1) engine->camera->angleTo=-1;
				else
				if (direction==3) engine->camera->angleTo=1;
				else engine->camera->angleTo=0;
			} else {
				engine->camera->angleTo=0;
			};

			Object* collision=CollisionCheckRadius(72,x,y,2);
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
				switch(direction) {
				case 0: {
					blk.SetBBox(-14,-26,28,52);
					blk.x=x;
					blk.y=y+20;
					break;
						};
				case 1: {
					blk.x=x-28;
					blk.y=y;
					break;
						};
				case 2: {
					blk.SetBBox(-14,-26,28,52);
					blk.x=x;
					blk.y=y-20;
					break;
						};
				case 3: {
					blk.x=x+28;
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
	};
	MoveToChunk();

	SetSequence(direction+4*(isMoving));
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
		if ((imageFrame>=2+5*direction && imageFrame<3+5*direction) || imageFrame>=4+5*direction || imageFrame<1+5*direction) {
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
		switch(direction) {
		case 0: {
			blk.SetBBox(-14,-26,28,52);
			blk.x=x;
			blk.y=y+20;
			break;
				};
		case 1: {
			blk.x=x-28;
			blk.y=y;
			break;
				};
		case 2: {
			blk.SetBBox(-14,-26,28,52);
			blk.x=x;
			blk.y=y-20;
			break;
				};
		case 3: {
			blk.x=x+28;
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

void Player::SetDirection(int dir) {
	direction=dir;
};

int Player::GetDirection() {
	return direction;
};

Player::~Player() {

};