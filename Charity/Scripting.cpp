#include "Scripting.h"
#include "Image.h"
#include "Engine.h"
#include "TextBox.h"

extern Engine* engine;

const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());

Scripting::Scripting() {

};

std::string Scripting::ToString(float number) {
	std::stringstream str;
	str<<number;
	return str.str();
};

std::wstring Scripting::ToWString(float number) {
	std::wstringstream str;
	str<<number;
	return str.str();
};

std::string Scripting::wStringToString(std::wstring str) {
	char* buffer = new char[str.length()+1];
	sprintf(buffer,"%ls",str.c_str());
	std::string string;
	string = buffer;
	delete buffer;
	return string;
};

std::wstring Scripting::StringToWString(std::string str) {
	std::wstring string;
	string.assign(str.begin(),str.end());
	return string;
};

std::wstring Scripting::WrapString(std::wstring sstr, int linewidth) {
	int space=0, spaceprev=0, ret = 0, retprev = 0;
	sf::String string="", word="", str = sstr;
	sf::Text text(engine->textBox->GetTextBox());
	text.setString(L"");
	std::vector<sf::String> words;

	//Calculate space width
	text.setString("a a");
	int spacewidth=text.getLocalBounds().width;
	text.setString("a");
	spacewidth-=(text.getLocalBounds().width)*2;

	//Split string
	while (space!=sf::String::InvalidPos) {
		space=str.find(" ",spaceprev);
		word="";
		if (space==sf::String::InvalidPos) break;
		for(int i=spaceprev;i<space;i++) {
			word+=str[i];
		};
		spaceprev=space+1;
		words.push_back(word);
	};
	word="";
	for(int i=spaceprev;i<str.getSize();i++) {
		word+=str[i];
	};
	words.push_back(word);

	//Define variables
	int spaceleft=linewidth, wordwidth=0;

	//Do something with words
	for(int i=0;i<words.size();i++) {
		text.setString(words.at(i));
		wordwidth=text.getLocalBounds().width;
		int ret = words.at(i).find(L"#",0);
		if (ret==sf::String::InvalidPos) {
			if (wordwidth + spacewidth > spaceleft) {
				string+="\n";
				string+=words.at(i)+" ";
				spaceleft = linewidth - (wordwidth + spacewidth);
			} else {
				string+=words.at(i)+" ";
				spaceleft = spaceleft - (wordwidth + spacewidth);
			};
		} else {
			words.at(i).erase(ret,1);
			words.at(i).insert(ret,L"\n");
			string+=words.at(i)+" ";
			sf::String temp;
			for(int j=0;j<ret;j++) {
				temp+=words.at(i)[j];
			};
			text.setString(temp);
			wordwidth-=text.getLocalBounds().width;
			spaceleft = linewidth - (wordwidth + spacewidth);
		};
	};

	return string;
	return L"";
};

void Scripting::SplitString(std::wstring str, std::vector<std::wstring> &stringsarray) {
	//Parsing a string
	int pos,temp,qpos;
	bool screen = false;
	std::wstring tempstr;
	pos=str.find_first_of(L"(");
	stringsarray.push_back(str.substr(0,pos));
	while(pos!=str.npos) {
		str=str.erase(0,pos+1);
		temp=str.find_first_of(L",");
		qpos=str.find_first_of(L"\\");
		screen=true;
		if (qpos==str.npos) {
			qpos=str.find_first_of(L'"');
			screen=false;
		};
		if (temp>qpos && qpos!=str.npos) {
			str.erase(0,qpos+1);
			if (screen) qpos=str.find_first_of(L"\\");
			else qpos=str.find_first_of(L'"');
			/*if (qpos==str.find_first_of(L")")-1) {
			str.insert(0,L"\"");
			break;
			};*/
			if (screen) tempstr=L"\\";
				else tempstr=L'"';
			tempstr.append(str.substr(0,qpos+1));
			stringsarray.push_back(tempstr);
			pos=qpos+1;
			screen=false;
			continue;
		};
		if (temp==str.npos) break; 
		pos=temp;
		stringsarray.push_back(str.substr(0,pos));
	};
	pos=str.find_first_of(L")");
	if (pos!=-1) stringsarray.push_back(str.substr(0,pos));
	for(int i=0;i<5;i++) {
		stringsarray.push_back(L"-1");
	};

	//Check if we should swap something with a variable
	for (int i=1;i<stringsarray.size();i++) {
		std::wstring wstring = stringsarray.at(i);
		int pos1 = wstring.find_first_of(L"$");
		if (pos1==wstring.npos) continue;
		int pos2 = wstring.find_first_of(L" ", pos1);
		if (pos2==wstring.npos) {
			int pos2 = wstring.find_first_of(L'\"', pos1);
			if (pos2==wstring.npos) {
				int pos2 = wstring.find_first_of(L"'", pos1);
				if (pos2==wstring.npos) {
					pos2 = wstring.length();
				};
			};
		};
		std::wstring name = wstring.substr(pos1+1,pos2-pos1-1);
		std::map<std::wstring,float>::iterator it = engine->variables->find(name);
		if (it!=engine->variables->end()) {
			stringsarray.at(i).replace(pos1,pos2-pos1,ToWString(it->second));
		} else {
			std::map<std::wstring,float>::iterator it = engine->variablesGlobal->find(name);
			if (it!=engine->variablesGlobal->end()) {
				stringsarray.at(i).replace(pos1,pos2-pos1,ToWString(it->second));
			} else	stringsarray.at(i).replace(pos1,pos2-pos1,L"0");
		};
	};
};

bool Scripting::ExecuteCommand(std::vector<std::wstring> &parameters) {
	std::wstring command = parameters.at(0);
	if (command==L"showText") {
		bool first=false;
		if (engine->textBox==NULL) {
			if (parameters.at(3)==L"-1") engine->textBox=new TextBox(-1,-1);
			else {
				parameters.at(3)=parameters.at(3).substr(1,parameters.at(3).length()-2);
				engine->textBox=new TextBox(0,0,engine->resourcesManager->GetTexture(wStringToString(parameters.at(3))));
			};
			first=true;
		} else {
			if (parameters.at(3)!=L"-1") {
				parameters.at(3)=parameters.at(3).substr(1,parameters.at(3).length()-2);
				engine->textBox->x=engine->textBox->xNvl;
				engine->textBox->y=engine->textBox->yNvl;
				engine->textBox->SetNvl(true, engine->resourcesManager->GetTexture(wStringToString(parameters.at(3))));
			} else {
				engine->textBox->x=engine->textBox->xNvl;
				engine->textBox->y=engine->textBox->yNvl;
				engine->textBox->SetNvl(false);
			};
		};
		//engine->textBox->toDelete=false;
		if (parameters.at(2)!=L"-1") {
			std::wstring str=L" [";
			str+=parameters.at(1).substr(1,parameters.at(1).length()-2);
			str+=L"]\n";
			parameters.at(2)=parameters.at(2).substr(1,parameters.at(2).length()-2);
			str+=WrapString(parameters.at(2),engine->textBox->GetBBox().width-20);
			parameters.at(1)=str;
		} else {
			parameters.at(1)=parameters.at(1).substr(1,parameters.at(1).length()-2);
			parameters.at(1)=WrapString(parameters.at(1),engine->textBox->GetBBox().width-20);
		};
		engine->textBox->AddText(parameters.at(1));
		if (first) {
			TextBox* textBox=engine->textBox;
			textBox->SetText(
				textBox->GetStrings().at(
				textBox->GetIndex()).substr(
				0,textBox->GetStrings().at(
				textBox->GetIndex()).find_first_of(L"]")+2));
		};
		return true;
	} else if (command==L"extendText") {
		if (engine->textBox!=NULL) {
			parameters.at(1)=parameters.at(1).substr(1,parameters.at(1).length()-2);
			engine->textBox->ExtendText(WrapString(parameters.at(1),
				engine->textBox->GetBBox().width-engine->textBox->xAdv));
		};
		return true;
	} else if (command==L"textCallback") {
		engine->textBox->SetCallback(parameters.at(1));
		return true;
	} else if (command==L"setTextDrawSprite") {
		engine->textBox->drawSprite=stoi(parameters.at(1));
		return true;
	} else if (command==L"setTextCentered") {
		engine->textBox->textCenter=stoi(parameters.at(1));
		return true;
	} else if (command==L"setTextPosition") {
		if (parameters.at(1)==L"" && parameters.at(2)==L"-1") {
			engine->textBox->SetPosition(20,418);
			return true;
		};
		engine->textBox->SetPosition(stoi(parameters.at(1)),stoi(parameters.at(2)));
		return true;
	} else if (command==L"setTextNvl") {
		if (parameters.at(2)!=L"-1") {
			engine->textBox->SetNvl(stoi(parameters.at(1)),engine->resourcesManager->GetTexture(wStringToString(parameters.at(2))));
		} else {
			engine->textBox->SetNvl(stoi(parameters.at(1)));
		};
		return true;
	} else if (command==L"completeText") {
		engine->textBox->Complete();
		return true;
	} else if (command==L"showChoice") {
			engine->choiceBox=new ChoiceBox();
			parameters.at(1)=parameters.at(1).substr(1,parameters.at(1).length()-2);
			parameters.at(2)=parameters.at(2).substr(1,parameters.at(2).length()-2);
			engine->choiceBox->AddChoice(parameters.at(1),L"");
			engine->choiceBox->AddChoice(parameters.at(2),parameters.at(3));
			engine->choiceBox->index=1;
		return true;
	} else if (command==L"addChoice") {
		if(engine->choiceBox!=NULL) {
			parameters.at(1)=parameters.at(1).substr(1,parameters.at(1).length()-2);
			engine->choiceBox->AddChoice(parameters.at(1),parameters.at(2));
			return true;	
		};
		return true;
	} else if (command==L"setChoiceDrawSprite") {
		engine->choiceBox->drawSprite=stoi(parameters.at(1));
		return true;
	} else if (command==L"setChoiceDrawLine") {
		engine->choiceBox->drawLine=stoi(parameters.at(1));
		return true;
	} else if (command==L"setChoicePosition") {
		engine->textBox->SetPosition(stoi(parameters.at(1)),stoi(parameters.at(2)));
		return true;
	} else if (command==L"setChoiceBackTexture") {
		engine->choiceBox->SetBackTexture(engine->resourcesManager->GetTexture(wStringToString(parameters.at(1))));
		return true;
	} else if (command==L"setChoiceLineTexture") {
		engine->choiceBox->SetBackTexture(engine->resourcesManager->GetTexture(wStringToString(parameters.at(1))));
		return true;
	} else if (command==L"setFadeColor") {
		engine->SetFadeColor(sf::Color(stoi(parameters.at(1)),stoi(parameters.at(2)),stoi(parameters.at(3)),255));
		return true;
	} else if (command==L"fade") {
		if (parameters.at(1)==L"in") {
			engine->Fade(0,stof(parameters.at(2)));
			return true;
		} else
			if (parameters.at(1)==L"out") {
				engine->Fade(1,stof(parameters.at(2)));
				return true;
			};
	} else if (command==L"setEffect") {
			if (parameters.at(1)==L"noise") {
				engine->SetDrawNoise(stoi(parameters.at(2)));
				return true;
			} else if (parameters.at(1)==L"lamp") {
				engine->SetLamp(stoi(parameters.at(2)),stof(parameters.at(3)));
				return true;
			};
		return true;
	} else if (command==L"setTexture") {
		if (parameters.at(1)==L"player") {
			parameters.at(2)=parameters.at(2).substr(1,parameters.at(2).length()-2);
			engine->objectsManager->GetPlayer()->SetTexture(engine->resourcesManager->GetTexture(wStringToString(parameters.at(2))));
		} else {
			parameters.at(1)=parameters.at(1).substr(1,parameters.at(1).length()-2);
			parameters.at(2)=parameters.at(2).substr(1,parameters.at(2).length()-2);
			Npc* obj=static_cast<Npc*>(engine->objectsManager->GetNpc(wStringToString(parameters.at(1))));
			obj->SetTexture(engine->resourcesManager->GetTexture(wStringToString(parameters.at(2))));
		};
		return true;
	} else if (command==L"moveObject") {
		if (parameters.at(5)!=L"-1") {
			if (parameters.at(1)==L"player") {
				if (parameters.at(4)==L"-1") {
					engine->objectsManager->GetPlayer()->SetPosition(stoi(parameters.at(2)),stoi(parameters.at(3)));
					return true;
				};
				engine->objectsManager->AddMover(engine->objectsManager->GetPlayer(),
					stoi(parameters.at(2)),stoi(parameters.at(3)),stof(parameters.at(4)));
				return true;
			} else {
				parameters.at(1)=parameters.at(1).substr(1,parameters.at(1).length()-2);
				Object* obj=engine->objectsManager->GetNpc(wStringToString(parameters.at(1)));
				if (parameters.at(4)==L"-1") {
					obj->SetPosition(stoi(parameters.at(2)),stoi(parameters.at(3)));
					obj->depth=obj->y+16;
					return true;
				};
				engine->objectsManager->AddMover(obj,stoi(parameters.at(2)),stoi(parameters.at(3)),stoi(parameters.at(4)));
				return true;
			};
		} else {
			if (parameters.at(1)==L"player") {
				if (parameters.at(4)==L"-1") {
					engine->objectsManager->GetPlayer()->SetPosition(
						engine->objectsManager->GetPlayer()->x+stoi(parameters.at(2)),
						engine->objectsManager->GetPlayer()->y+stoi(parameters.at(3)));
					return true;
				};
				engine->objectsManager->AddMover(engine->objectsManager->GetPlayer(),
					engine->objectsManager->GetPlayer()->x+stoi(parameters.at(2)),
					engine->objectsManager->GetPlayer()->y+stoi(parameters.at(3)),
					stof(parameters.at(4)));
				return true;
			} else {
				parameters.at(1)=parameters.at(1).substr(1,parameters.at(1).length()-2);
				Object* obj=engine->objectsManager->GetNpc(wStringToString(parameters.at(1)));
				if (parameters.at(4)==L"-1") {
					obj->SetPosition(obj->x+stoi(parameters.at(2)),obj->y+stoi(parameters.at(3)));
					obj->depth=obj->y+16;
					return true;
				};
				engine->objectsManager->AddMover(obj,
					obj->x+stoi(parameters.at(2)),
					obj->y+stoi(parameters.at(3)),
					stof(parameters.at(4)));
				return true;
			};
		};
		return true;
	} else if (command==L"setDirection") {
		if (parameters.at(1)==L"player") engine->objectsManager->GetPlayer()->SetDirection(stoi(parameters.at(2)));
		else {
			parameters.at(1)=parameters.at(1).substr(1,parameters.at(1).length()-2);
			Npc* obj=static_cast<Npc*>(engine->objectsManager->GetNpc(wStringToString(parameters.at(1))));
			obj->SetDirection(stoi(parameters.at(2)));
		};
		return true;
	} else if (command==L"setSequence") {
			parameters.at(1)=parameters.at(1).substr(1,parameters.at(1).length()-2);
			Object* obj=engine->objectsManager->GetObjectByName(wStringToString(parameters.at(1)));
			if (obj->objectName!="none") {
				Decoration* dec=static_cast<Decoration*>(obj);
				dec->SetSequence(stoi(parameters.at(2)));
			};
		return true;
	} else if (command==L"setSolid") {
			parameters.at(1)=parameters.at(1).substr(1,parameters.at(1).length()-2);
			Object* obj=engine->objectsManager->GetObjectByName(wStringToString(parameters.at(1)));
			obj->solid = stoi(parameters.at(2));
		return true;
	} else if (command==L"setCanMove") {
		if (parameters.at(1)==L"player") engine->objectsManager->GetPlayer()->isBlocked=!(stoi(parameters.at(2)));
		else {
			parameters.at(1)=parameters.at(1).substr(1,parameters.at(1).length()-2);
			Npc* obj=static_cast<Npc*>(engine->objectsManager->GetNpc(wStringToString(parameters.at(1))));
			obj->isBlocked=!(stoi(parameters.at(2)));
		};
		return true;
	} else if (command==L"loadSequences") {
			parameters.at(1)=parameters.at(1).substr(1,parameters.at(1).length()-2);
			parameters.at(2)=parameters.at(2).substr(1,parameters.at(2).length()-2);
			Object* obj=engine->objectsManager->GetObjectByName(wStringToString(parameters.at(1)));
			if (obj->objectName!="none") {
				Image* dec=static_cast<Image*>(obj);
				dec->LoadSequences(wStringToString(parameters.at(2)));
			};
		return true;
	} else if (command==L"getDirection") {
		std::wstring str=L"varSet(";
		str+=parameters.at(2);
		str+=L",";
		if (parameters.at(1)==L"player") str+=ToWString(engine->objectsManager->GetPlayer()->GetDirection());
		else {
			parameters.at(1)=parameters.at(1).substr(1,parameters.at(1).length()-2);
			Npc* obj=static_cast<Npc*>(engine->objectsManager->GetNpc(wStringToString(parameters.at(1))));
			str+=ToWString(obj->GetDirection());
		};
		str+=L",";
		str+=L"0";
		str+=L");";
		ExecuteString(str);
		return true;
	} else if (command==L"getEnterPlace") {
		std::wstring str=L"varSet(";
		str+=parameters.at(1);
		str+=L",";
		str+=ToWString(engine->placeIndex);
		str+=L",";
		str+=L"0";
		str+=L");";
		ExecuteString(str);
		return true;
	} else if (command==L"addNpc") {
		parameters.at(3)=parameters.at(3).substr(1,parameters.at(3).length()-2);
		parameters.at(4)=parameters.at(4).substr(1,parameters.at(4).length()-2);
		Npc* npc = static_cast<Npc*>(engine->objectsManager->AddNpc(stoi(parameters.at(1)),stoi(parameters.at(2)),
			wStringToString(parameters.at(3)),engine->resourcesManager->GetTexture(wStringToString(parameters.at(4)))));
		std::vector<int>* sets=engine->resourcesManager->GetTextureSettings(wStringToString(parameters.at(4)));
		if (sets->at(0)!=-1) npc->imageWidth=sets->at(0);
		if (sets->at(1)!=-1) npc->imageHeight=sets->at(1);
		npc->SetOrigin(npc->imageWidth/2,npc->imageHeight-10*2);
		return true;
	} else if (command==L"addObject") {
		parameters.at(4)=parameters.at(4).substr(1,parameters.at(4).length()-2);
		parameters.at(5)=parameters.at(5).substr(1,parameters.at(5).length()-2);
		int index=0;
		do {
			if (parameters.at(3)==L"block") {
				index=0;
				break;
			};
			if (parameters.at(3)==L"usable") {
				index=2;
				break;
			};
			if (parameters.at(3)==L"decoration") {
				index=3;
				break;
			};
			if (parameters.at(3)==L"trigger") {
				index=4;
				break;
			};
		} while (false);
		Object* obj=engine->objectsManager->AddObject(stoi(parameters.at(1)),stoi(parameters.at(2)),index,wStringToString(parameters.at(4)));
		obj->objectName=wStringToString(parameters.at(5));
		return true;
	} else if (command==L"deleteNpc") {
		parameters.at(1)=parameters.at(1).substr(1,parameters.at(1).length()-2);
		engine->objectsManager->DeleteNpc(wStringToString(parameters.at(1)));
		return true;
	} else if (command==L"deleteLastTrigger") {
		if (engine->objectsManager->GetPlayer()->collisionTrigger!=NULL) {
			engine->objectsManager->DeleteObject(engine->objectsManager->GetPlayer()->collisionTrigger);
		};
		return true;
	} else if (command==L"varSet") {
		//varSet(variable, value, global);
		std::map<std::wstring,float>* map;
		if (parameters.at(3)==L"1") map = engine->variables;
		else map = engine->variablesGlobal;
		std::map<std::wstring,float>::iterator it = map->find(parameters.at(1));
		if (it!=map->end()) {
			if (parameters.at(2)==L"++") {it->second+=1; printf("Variable %s incremented\n",parameters.at(1).c_str()); return true;}
			else
				if (parameters.at(2)==L"--") {it->second-=1; printf("Variable %s decremented\n",parameters.at(1).c_str()); return true;}
				else
					if (parameters.at(2)==L"random") {
						std::map<std::wstring,float>::iterator it = map->find(parameters.at(1));
						if (parameters.at(3)==L"-1" || parameters.at(3)==L"0") parameters.at(3)=L"100";
						it->second = rand() % stoi(parameters.at(3));
						printf("Variable %s set to %f\n",parameters.at(1).c_str(),it->second);
						return true;
					} else
						parameters.at(2)=Calculate(parameters.at(2));
					it->second = stof(parameters.at(2));
					printf("Variable %s set to %f\n",parameters.at(1).c_str(),it->second);
					return true;
		} else {
			if (parameters.at(2)==L"random") {
				srand (time(NULL));
				if (parameters.at(3)==L"-1" || parameters.at(3)==L"0") parameters.at(3)=L"100";
				map->insert(map->end(),
					std::pair<std::wstring,float>(parameters.at(1),rand() % stoi(parameters.at(3))));
				printf("Variable %s set to %f\n",parameters.at(1).c_str(),map->find(parameters.at(1))->second);
				return true;
			} else
				parameters.at(2)=Calculate(parameters.at(2));
			map->insert(map->end(),std::pair<std::wstring,float>(parameters.at(1),stof(parameters.at(2))));
			printf("Variable %s set to %f\n",parameters.at(1).c_str(),stof(parameters.at(2)));
			return true;
		};
		return true;
	} else if (command==L"varDel") {
		//varDel(variable, global);
		std::map<std::wstring,float>* map;
		if (parameters.at(2)==L"1") map = engine->variables;
		else map = engine->variablesGlobal;
		std::map<std::wstring,float>::iterator it = map->find(parameters.at(1));
		if (it!=map->end()) {
			map->erase(it);
		};
		printf("Variable %s was deleted\n",parameters.at(1).c_str());
		return true;
	} else if (command==L"varClear") {
		//varClear(global);
		std::map<std::wstring,float>* map;
		if (parameters.at(1)==L"1") map = engine->variables;
		else map = engine->variablesGlobal;
		map->clear();
		return true;
	} else if (command==L"setCameraTarget") {
		if (parameters.at(1)==L"-1") engine->camera->target=NULL;
		if (parameters.at(1)==L"player") {
			engine->camera->target=engine->objectsManager->GetPlayer();
		} else {
			parameters.at(1)=parameters.at(1).substr(1,parameters.at(1).length()-2);
			Object* obj=engine->objectsManager->GetNpc(wStringToString(parameters.at(1)));
			engine->camera->target=obj;
		};
		return true;
	} else if (command==L"setCameraPosition") {
		if (parameters.at(1)==L"" && parameters.at(2)==L"-1") {
			if (engine->camera->target!=NULL) {
				engine->camera->xView=engine->camera->target->x-(engine->windowSize.x/2);
				engine->camera->yView=engine->camera->target->y-(engine->windowSize.y/2);
				engine->camera->viewTo.x=engine->camera->xView;
				engine->camera->viewTo.y=engine->camera->yView;
				return true;
			};
		};
		engine->camera->xView=stoi(parameters.at(1));
		engine->camera->yView=stoi(parameters.at(2));
		return true;
	} else if (command==L"setCameraBorders") {
		engine->camera->SetBorders(stoi(parameters.at(1)),stoi(parameters.at(2)),stoi(parameters.at(3)),stoi(parameters.at(4)));
		return true;
	} else if (command==L"setActive") {
		int index=0;
		if (parameters.at(1)==L"usable") index=2;
		Object* obj=engine->objectsManager->GetObjectAt(stoi(parameters.at(2)),stoi(parameters.at(3)),index);
		if (obj!=NULL) {
		obj->active=stoi(parameters.at(4));
		};
		return true;
	} else if (command==L"addToQueue") {
		parameters.at(1)=parameters.at(1).substr(1,parameters.at(1).length()-2);
		engine->queue->Add(parameters.at(1),parameters.at(2));
		return true;
	} else if (command==L"playSound") {
		parameters.at(1)=parameters.at(1).substr(1,parameters.at(1).length()-2);
		Sound* snd=engine->resourcesManager->GetSound(wStringToString(parameters.at(1)));
		snd->sound->setLoop(stoi(parameters.at(2)));
		snd->sound->setVolume(engine->volumeSounds);
		if (parameters.at(2)==L"-1") snd->sound->setLoop(false);
		snd->sound->play();
		return true;
	} else if (command==L"playMusic") {
		parameters.at(1)=parameters.at(1).substr(1,parameters.at(1).length()-2);
		if (parameters.at(1)==L"") return true;
		engine->resourcesManager->bgMusic.openFromFile(wStringToString(parameters.at(1)));
		engine->resourcesManager->bgMusic.setLoop(true);
		engine->resourcesManager->bgMusic.setVolume(engine->volumeBGM);
		engine->resourcesManager->bgMusic.play();
		engine->pathMusic = wStringToString(parameters.at(1));
		return true;
	} else if (command==L"stopMusic") {
		engine->resourcesManager->bgMusic.stop();
		return true;
	} else if (command==L"call") {
		engine->scripting.ExecuteFunction(parameters.at(1));
		return true;
	} else if (command==L"loadMap") {
		parameters.at(1)=parameters.at(1).substr(1,parameters.at(1).length()-2);
		if (parameters.at(2)>=L"-1") engine->placeIndex=stoi(parameters.at(2));
		else engine->placeIndex=-1;
		engine->LoadMap(wStringToString(parameters.at(1)));
		engine->queue->done=true;
		return true;
	} else if (command==L"clearResources") {
		if (parameters.at(1)==L"all") {
			engine->resourcesManager->ClearAll();
		};
		return true;
	} else if (command==L"debug") {
		engine->debug = true;
		return true;
	} else if (command==L"end") {
		engine->commandPause = true;
		return true;
	} else if (command==L"endGame") {
		engine->gameEnd=true;
		return true;
	} else if (command==L"save") {
		int number = stoi(parameters.at(1));
		if (number==-1) number=0;
		std::string path = "Saves/Save";
		path+=ToString(number);
		path+=".sav";
		std::ofstream file(path);
		file<<engine->mapName<<" ";
		file<<engine->pathMusic<<" ";
		//Variables
		file<<engine->variablesGlobal->size()<<" ";
		for(std::map<std::wstring,float>::iterator it=engine->variablesGlobal->begin();it!=engine->variablesGlobal->end();it++) {
			file<<wStringToString(it->first)<<" "<<it->second<<" ";
		};
		file<<engine->variables->size()<<" ";
		for(std::map<std::wstring,float>::iterator it=engine->variables->begin();it!=engine->variables->end();it++) {
			file<<wStringToString(it->first)<<" "<<it->second<<" ";
		};
		//Player coordinates
		file<<static_cast<int>(engine->objectsManager->GetPlayer()->x)<<" "<<static_cast<int>(engine->objectsManager->GetPlayer()->y)<<" ";
		file<<engine->objectsManager->GetPlayer()->GetDirection()<<" ";
		//Triggers and usables state
		std::vector<Object*> tempVec;
		for(int i=0;i<engine->objectsManager->chunksNumber.x;i++) {
			for(int j=0;j<engine->objectsManager->chunksNumber.y;j++) {
				for (int k=0;k<engine->objectsManager->chunks->at(i)->at(j)->list->size();k++) {
					Object* obj = engine->objectsManager->chunks->at(i)->at(j)->list->at(k);
					if (obj->objectIndex==2 || obj->objectIndex==4) {
						tempVec.push_back(obj);
					};
				};
			};
		};
		file<<tempVec.size()<<" ";
		for (int i=0;i<tempVec.size();i++) {
			Object* obj = tempVec.at(i);
		file<<static_cast<int>(obj->x)<<" "<<static_cast<int>(obj->y)<<" "<<static_cast<int>(obj->active)<<" ";
		};
		file.close();
		return true;
	}  else if (command==L"load") {
		int number = stoi(parameters.at(1));
		if (number==-1) number=0;
		std::string path = "Saves/Save";
		path+=ToString(number);
		path+=".sav";
		std::ifstream file(path);
		std::string mapName, musicPath;
		file>>mapName;
		file>>musicPath;
		int size;
		//Variables
		engine->variablesGlobal->clear();
		file>>size;
		for(int i=0;i<size;i++) {
			std::string name;
			float value;
			file>>name;
			file>>value;
			engine->variablesGlobal->insert(engine->variablesGlobal->end(),std::pair<std::wstring,float>(StringToWString(name),value));
		};
		engine->variables->clear();
		file>>size;
		for(int i=0;i<size;i++) {
			std::string name;
			float value;
			file>>name;
			file>>value;
			engine->variables->insert(engine->variables->end(),std::pair<std::wstring,float>(StringToWString(name),value));
		};
		//Player coordinates
		file>>engine->objectsManager->playerXStart;
		file>>engine->objectsManager->playerYStart;
		file>>engine->objectsManager->playerDirStart;
		file>>size;
		if (engine->objectsManager->playerXStart==-1 && engine->objectsManager->playerYStart==-1) {
			int a = 2;
		};
		engine->LoadMap(mapName);
		if (engine->objectsManager->playerXStart!=-1 && engine->objectsManager->playerYStart!=-1) {
		engine->objectsManager->GetPlayer()->SetPosition(engine->objectsManager->playerXStart,engine->objectsManager->playerYStart);
		engine->objectsManager->playerXStart=-1;
		engine->objectsManager->playerYStart=-1;
		engine->camera->xView=engine->objectsManager->GetPlayer()->x-(engine->windowSize.x/2);
		engine->camera->yView=engine->objectsManager->GetPlayer()->y-(engine->windowSize.y/2);
		engine->camera->viewTo.x=engine->objectsManager->GetPlayer()->x-(engine->windowSize.x/2);
		engine->camera->viewTo.y=engine->objectsManager->GetPlayer()->y-(engine->windowSize.y/2);
	};
	if (engine->objectsManager->playerDirStart!=-1) {
		engine->objectsManager->GetPlayer()->SetDirection(engine->objectsManager->playerDirStart);
	};
		for (int i=0;i<size;i++) {
			int x, y, state;
			file>>x;
			file>>y;
			file>>state;
			engine->scripting.ExecuteString(L"setActive(usable,"+ToWString(x)+L","+ToWString(y)+L","+ToWString(state)+L")");
		};
		engine->scripting.ExecuteString(L"playMusic(\""+StringToWString(musicPath)+L"\")");

		file.close();
		return true;
	}
	//Resources add
	else if(command==L"addTexture") {
		parameters.at(1)=parameters.at(1).substr(1,parameters.at(1).length()-2);
		parameters.at(2)=parameters.at(2).substr(1,parameters.at(2).length()-2);
		std::vector<int>* vec = new std::vector<int>;
		vec->push_back(stoi(parameters.at(3)));
		vec->push_back(stoi(parameters.at(4)));
		if (parameters.at(5)==L"-1") parameters.at(5)=L"0";
		engine->resourcesManager->AddTexture(wStringToString(parameters.at(1)),wStringToString(parameters.at(2)), vec, stoi(parameters.at(3)));
		return true;
	} else if(command==L"addSound") {
		parameters.at(1)=parameters.at(1).substr(1,parameters.at(1).length()-2);
		parameters.at(2)=parameters.at(2).substr(1,parameters.at(2).length()-2);
		if (parameters.at(3)==L"-1") parameters.at(3)=L"0";
		engine->resourcesManager->AddSound(wStringToString(parameters.at(1)),wStringToString(parameters.at(2)),stoi(parameters.at(3)));
		return true;
	} else if(command==L"desaturateTexture") {
		parameters.at(1)=parameters.at(1).substr(1,parameters.at(1).length()-2);
		engine->resourcesManager->DesaturateTexture(wStringToString(parameters.at(1)));
		return true;
	} else if(command==L"colorizeTexture") {
		parameters.at(1)=parameters.at(1).substr(1,parameters.at(1).length()-2);
		int mode=0;
		do {
			if (parameters.at(2)==L"multiply") {
				break;
			};
		} while(false);
		engine->resourcesManager->ColorizeTexture(wStringToString(parameters.at(1)),mode,sf::Color(stoi(parameters.at(3)),stoi(parameters.at(4)),stoi(parameters.at(5)),255));
		return true;
	} else if(command==L"addFont") {
		parameters.at(2)=parameters.at(2).substr(1,parameters.at(2).length()-2);
		if (parameters.at(3)!=L"-1") {
			engine->resourcesManager->AddFont(stoi(parameters.at(1)),wStringToString(parameters.at(2)),stoi(parameters.at(3)));
		} else {
			engine->resourcesManager->AddFont(stoi(parameters.at(1)),wStringToString(parameters.at(2)));
		};
		return true;
	}
	//Resources check
	else if(command==L"isFileExists") {
		parameters.at(2)=parameters.at(2).substr(1,parameters.at(2).length()-2);
		bool exists=false;
		std::ifstream file;
		file.open(parameters.at(2));
		if (file) {
			file.close();
			exists=true;
		};

		std::wstring str=L"varSet(";
		str+=parameters.at(1);
		str+=L",";
		str+=ToWString(exists);
		str+=L",";
		str+=L"0";
		str+=L");";
		ExecuteString(str);
		return true;
		};
	return false;
};

std::wstring Scripting::Calculate(std::wstring string) {
	//Check if we need to calculate anything
	std::vector<std::wstring> numbers;
	std::wstring str=L"";
	if (string.find_first_of(L"+-*/")!=string.npos) {
		for (int i=0;i<string.size();i++) {
			if (string.substr(i,1)==L"+" ||
				string.substr(i,1)==L"-" ||
				string.substr(i,1)==L"*" ||
				string.substr(i,1)==L"/") {
					numbers.push_back(str);
					str=L"";
					numbers.push_back(string.substr(i,1));
			} else {
				str+=string.substr(i,1);
			};
		};
		numbers.push_back(str);

		std::wstring delimeters[4]={L"/",L"*",L"+",L"-"};
		for (int j=0;j<4;j++) {
			int end=(numbers.size()-1)/2;
			//Dividing
			for(int i=0;i<end;i++) {
				std::vector<std::wstring>::iterator pos=std::find(numbers.begin(),numbers.end(),delimeters[j]);
				if (pos!=numbers.end()) {
					float temp;
					int position=pos-numbers.begin();
					temp=std::stof(numbers.at(position-1));
					switch(j) {
					case 0:
						temp/=std::stof(numbers.at(position+1));
						break;
					case 1:
						temp*=std::stof(numbers.at(position+1));
						break;
					case 2:
						temp+=std::stof(numbers.at(position+1));
						break;
					case 3:
						temp-=std::stof(numbers.at(position+1));
						break;
					};
					numbers.at(position)=ToWString(temp);
					numbers.erase(numbers.begin()+position+1);
					numbers.erase(numbers.begin()+position-1);
				};
			};
		};
		return numbers.at(0);
	};
	return string;
};


bool Scripting::ExecuteString(std::wstring string) {
	std::vector<std::wstring> parameters;
	SplitString(string, parameters);
	if (parameters.at(0)==L"if") {
		int type = 0;
		int pos = parameters.at(1).find(L"==");
		float first, second;
		if (parameters.at(1).find(L">=")!=parameters.at(1).npos) {type = 3; pos = parameters.at(1).find(L">=");}
		else if (parameters.at(1).find(L"<=")!=parameters.at(1).npos) {type = 4; pos = parameters.at(1).find(L"<=");}
		else if (parameters.at(1).find(L">")!=parameters.at(1).npos) {type = 1; pos = parameters.at(1).find(L">");}
		else if (parameters.at(1).find(L"<")!=parameters.at(1).npos) {type = 2; pos = parameters.at(1).find(L"<");}
		else if (parameters.at(1).find(L"!=")!=parameters.at(1).npos) {type = 5; pos = parameters.at(1).find(L"!=");}
		first = std::stof(parameters.at(1).substr(0,pos));

		switch (type) {
		case 0:
			//==
			second = std::stof(parameters.at(1).substr(pos+2,parameters.at(1).length()-pos-2));
			if (first!=second) break;
			return true;
		case 1:
			//>
			second = std::stof(parameters.at(1).substr(pos+2,parameters.at(1).length()-pos-2));
			if (first<=second) break;
			return true;
		case 2:
			//<
			second = std::stof(parameters.at(1).substr(pos+2,parameters.at(1).length()-pos-2));
			if (first>=second) break;
			return true;
		case 3:
			//>=
			second = std::stof(parameters.at(1).substr(pos+2,parameters.at(1).length()-pos-2));
			if (first<second) break;
			return true;
		case 4:
			//<=
			second = std::stof(parameters.at(1).substr(pos+2,parameters.at(1).length()-pos-2));
			if (first>second) break;
			return true;
		case 5:
			//!=
			second = std::stof(parameters.at(1).substr(pos+2,parameters.at(1).length()-pos-2));
			if (first==second) break;
			return true;
		};
		while(engine->commands->at(engine->commandNumber)!=L"fi" &&
			engine->commandNumber<engine->commands->size()-1) {
				engine->commandNumber++;
		};
		return true;
	} else {
		ExecuteCommand(parameters);
		return true;
	};
};

bool Scripting::ExecuteFunction(std::wstring function) {
	std::map<std::wstring,int>::iterator pos = engine->commandFunctions->find(function);
	if (pos!=engine->commandFunctions->end()) {
		engine->commandPause = false;
		engine->commandNumber = pos->second;
		Execute();
	};
	return true;
};

bool Scripting::ExecuteFile(std::string filename) {
	if (LoadFile(filename)) {
		Execute();
		return false;
	} else {
		return true;
	};
};

bool Scripting::LoadFile(std::string filename) {
	engine->commandReturn = 0;
	engine->commands->clear();
	engine->commandNumber = 0;
	engine->commandFunctions->clear();
	std::wifstream file(filename);
	if (!file) return false;
	file.imbue(utf8_locale);
	std::wstring line;
	std::wstring command;
	engine->scriptName = filename;
	int number=0;
	while (!file.eof()) {
		std::getline(file, line);
		int pos = line.find_first_of(L"(",0);
		if (pos!=line.npos) {
			if (line.substr(0,pos)==L"function") {
				int pos2 = line.find_first_of(L")",0);
				engine->commandFunctions->insert(engine->commandFunctions->end(),
					std::pair<std::wstring,int>(line.substr(pos+1,pos2-pos-1),number));
				std::cout<<"function added: "<<wStringToString(line.substr(pos+1,pos2-pos-1))<<" on the line number: "<<number<<"\n";
			};
		};
		engine->commands->push_back(line);
		number++;
	};
	file.close();
	return true;
};

void Scripting::Execute() {
	if (engine->commandNumber<engine->commands->size()) {
		if (!engine->commandPause) {
			ExecuteString(engine->commands->at(engine->commandNumber)); 
			engine->commandNumber+=1;
			Execute();
		};
	};
};