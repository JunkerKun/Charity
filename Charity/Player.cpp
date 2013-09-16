#include "Player.h"
#include "Engine.h"

extern Engine* engine;

Player::Player(sf::Texture* tex):Image(tex) {
	speed=64;
	objectIndex=1;
	direction=0;
	canMove=true;
	SetBBox(-14,-9,28,18);
	SetOrigin(imageWidth/2,imageHeight-7);

	AddSequence(0,0,0.25);
	AddSequence(1,1,0.25);
	AddSequence(2,2,0.25);
	AddSequence(3,3,0.25);
	AddSequence(0,0,0.25);
	AddSequence(1,1,0.25);
	AddSequence(2,2,0.25);
	AddSequence(3,3,0.25);

	AddSequence(0,0,0.25);
	AddSequence(5,5,0.25);
	AddSequence(10,10,0.25);
	AddSequence(15,15,0.25);

	AddSequence(1,4,0.25);
	AddSequence(6,9,0.25);
	AddSequence(11,14,0.25);
	AddSequence(16,19,0.25);

	SetSequence(0,1);
	engine->camera->target=this;
	engine->objectsManager->SetPlayer(this);
	sprShadow.setTexture(*engine->resourcesManager->GetTexture("sprShadow"),true);
	sprShadow.setOrigin(engine->resourcesManager->GetTexture("sprShadow")->getSize().x/2,
		engine->resourcesManager->GetTexture("sprShadow")->getSize().y/2-1);
};

bool Player::Update() {
	bool move=false;
	if (canMove) {
		if (engine->input->GetKeyIsPressed(sf::Keyboard::Left)) {
			move=true;
			x-=speed*engine->GetDelta();
			direction=1;
			if (CollisionCheckIntersect(this,0)) {
				x+=speed*engine->GetDelta();
			};
		} else if (engine->input->GetKeyIsPressed(sf::Keyboard::Right)) {
			move=true;
			x+=speed*engine->GetDelta();
			direction=3;
			if (CollisionCheckIntersect(this,0)) {
				x-=speed*engine->GetDelta();
			};
		};
		if (engine->input->GetKeyIsPressed(sf::Keyboard::Up)) {
			move=true;
			y-=speed*0.8*engine->GetDelta();
			direction=2;
			if (CollisionCheckIntersect(this,0)) {
				y+=speed*0.8*engine->GetDelta();
			};
		} else if (engine->input->GetKeyIsPressed(sf::Keyboard::Down)) {
			move=true;
			y+=speed*0.8*engine->GetDelta();
			direction=0;
			if (CollisionCheckIntersect(this,0)) {
				y-=speed*0.8*engine->GetDelta();
			};
		};

		if (engine->input->GetKeyPressed(sf::Keyboard::Z)) {
			/*Object* collision=NULL;
			switch(static_cast<int>(direction)) {
			case 0:
			collision=CollisionCheckPoint(x,y+22,2);
			break;
			case 1:
			collision=CollisionCheckPoint(x-22,y,2);
			break;
			case 2:
			collision=CollisionCheckPoint(x,y-22,2);
			break;
			case 3:
			collision=CollisionCheckPoint(x+22,y,2);
			break;
			};*/

			Block blk;
			blk.SetBBox(-13,-7,26,14);
			switch(static_cast<int>(direction)) {
			case 0: {
				blk.SetBBox(-7,-13,14,26);
				blk.x=x;
				blk.y=y+10;
				break;
					};
			case 1: {
				blk.x=x-14;
				blk.y=y;
				break;
					};
			case 2: {
				blk.SetBBox(-7,-13,14,26);
				blk.x=x;
				blk.y=y-10;
				break;
					};
			case 3: {
				blk.x=x+14;
				blk.y=y;
				break;
					};
			};
			Object* collision=CollisionCheckIntersect(&blk,2);
			if (collision!=NULL) {
				Usable* use=static_cast<Usable*>(collision);
				if (use->function!="none") engine->scripting.ExecuteFunction(engine->scripting.StringToWString(use->function));
			};
		};
	};

	MoveToChunk();

	SetSequence(floor(direction)+4*(move));
	Image::Update();
	sprShadow.setPosition(floor(x),floor(y));

	xPrev=x;
	xPrev=y;
	return true;
};

bool Player::Draw(sf::RenderTarget &RT) {
	if (!visible) return true;
	RT.draw(sprShadow);
	Image::Draw(RT);

	if (engine->debug) {
		Block blk;
		blk.SetBBox(-13,-7,26,14);
		switch(static_cast<int>(direction)) {
		case 0: {
			blk.SetBBox(-7,-13,14,26);
			blk.x=x;
			blk.y=y+10;
			break;
				};
		case 1: {
			blk.x=x-14;
			blk.y=y;
			break;
				};
		case 2: {
			blk.SetBBox(-7,-13,14,26);
			blk.x=x;
			blk.y=y-10;
			break;
				};
		case 3: {
			blk.x=x+14;
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

Player::~Player() {

};