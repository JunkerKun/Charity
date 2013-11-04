#include "Decoration.h"
#include "Engine.h"

extern Engine* engine;

Decoration::Decoration():Block() {
	objectIndex=3;
	spriteName="none";
	SetBBox(-gridSize/2+gridSize/32*2,-gridSize/2+gridSize/32*4,gridSize-gridSize/32*4,gridSize-gridSize/32*8);
	//SetBBox(-28,-24,56,48);
	text.setColor(sf::Color::Black);
	text.setCharacterSize(14);
	text.setFont(engine->font);
};

Decoration::~Decoration() {
	
};

bool Decoration::Update() {
if (grabbed) {
		if (engine->GetInputEvent().type==sf::Event::MouseMoved) {
			if (!engine->input->GetKeyIsPressed(sf::Keyboard::LShift)) {
				x=std::floor(static_cast<float>(engine->input->GetMousePosition(true).x)/gridSize)*gridSize+(gridSize/2);
				y=std::floor(static_cast<float>(engine->input->GetMousePosition(true).y)/gridSize)*gridSize+(gridSize/2);
			} else {
				x=std::floor(static_cast<float>(engine->input->GetMousePosition(true).x)/(gridSize/2))*(gridSize/2)+(gridSize/2);
				y=std::floor(static_cast<float>(engine->input->GetMousePosition(true).y)/(gridSize/2))*(gridSize/2)+(gridSize/2);
			};
		};
		MoveToChunk();
	};
	return true;
};

bool Decoration::Draw(sf::RenderTarget& RT) {
	sf::RectangleShape RS;
	RS.setPosition(x+bBox.left,y+bBox.top);
	RS.setSize(sf::Vector2f(bBox.width,bBox.height));
	RS.setFillColor(sf::Color(200,200,0,120));
	RT.draw(RS);
	text.setString(spriteName);
	text.setColor(sf::Color::White);
	text.setPosition(floor(x-text.getLocalBounds().width/2),floor(y-10));
	RT.draw(text);
	return true;
};