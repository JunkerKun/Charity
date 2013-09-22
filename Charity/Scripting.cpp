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
	bool quotes = false;
	std::wstring tempstr;
	pos=str.find_first_of(L"(");
	stringsarray.push_back(str.substr(0,pos));
	while(pos!=str.npos) {
		str=str.erase(0,pos+1);
		temp=str.find_first_of(L",");
		qpos=str.find_first_of(L'"');
		if (temp>qpos && qpos!=str.npos) {
			str.erase(0,qpos+1);
			qpos=str.find_first_of(L'"');
			/*if (qpos==str.find_first_of(L")")-1) {
			str.insert(0,L"\"");
			break;
			};*/
			tempstr=L'"';
			tempstr.append(str.substr(0,qpos+1));
			stringsarray.push_back(tempstr);
			pos=qpos+1;
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
			int pos2 = wstring.find_first_of(L"\"", pos1);
			if (pos2==wstring.npos) {
				pos2 = wstring.length();
			};
		};
		std::wstring name = wstring.substr(pos1+1,pos2-pos1-1);
		std::map<std::wstring,float>::iterator it = engine->variables->find(name);
		if (it!=engine->variables->end()) {
			stringsarray.at(i).replace(pos1,pos2-pos1,ToWString(it->second));
		} else {
			stringsarray.at(i).replace(pos1,pos2-pos1,L"0");
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
			};
		};
		//engine->textBox->toDelete=false;
		if (parameters.at(2)!=L"-1") {
			std::wstring str=L" [";
			str+=parameters.at(1).substr(1,parameters.at(1).length()-2);
			str+=L"]\n";
			str+=parameters.at(2).substr(1,parameters.at(2).length()-2);
			parameters.at(1)=str;
		} else {
			parameters.at(1)=parameters.at(1).substr(1,parameters.at(1).length()-2);
		};
		engine->textBox->AddText(WrapString(parameters.at(1),
			engine->textBox->GetBBox().width*engine->screenScale-engine->textBox->xAdv));
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
				engine->textBox->GetBBox().width*engine->screenScale-engine->textBox->xAdv));
		};
		return true;
	} else if (command==L"textCallback") {
		engine->textBox->SetCallback(parameters.at(1));
		return true;
	} else if (command==L"setTextNvl") {
		if (parameters.at(2)!=L"-1") {
			engine->textBox->SetNvl(false,engine->resourcesManager->GetTexture(wStringToString(parameters.at(2))));
		} else {
			engine->textBox->SetNvl(false);
		};
		return true;
	} else if (command==L"setEffect") {
		do {
			if (parameters.at(1)==L"noise") {
				engine->SetDrawNoise(stoi(parameters.at(2)));
				break;
			};
		} while(false);
		return true;
	} else if (command==L"moveObject") {
				if (parameters.at(5)!=L"-1") {
					if (parameters.at(1)==L"player") {
					engine->objectsManager->AddMover(engine->objectsManager->GetPlayer(),
						stoi(parameters.at(2)),stoi(parameters.at(3)),stoi(parameters.at(4)));
					} else {
						parameters.at(1)=parameters.at(1).substr(1,parameters.at(1).length()-2);
						Object* obj=engine->objectsManager->GetNpc(wStringToString(parameters.at(1)));
						engine->objectsManager->AddMover(obj,stoi(parameters.at(2)),stoi(parameters.at(3)),stoi(parameters.at(4)));
					};
				} else {
					if (parameters.at(1)==L"player") {
					engine->objectsManager->AddMover(engine->objectsManager->GetPlayer(),
						engine->objectsManager->GetPlayer()->x+stoi(parameters.at(2)),
						engine->objectsManager->GetPlayer()->y+stoi(parameters.at(3)),
						stoi(parameters.at(4)));
					} else {
						parameters.at(1)=parameters.at(1).substr(1,parameters.at(1).length()-2);
						Object* obj=engine->objectsManager->GetNpc(wStringToString(parameters.at(1)));
						engine->objectsManager->AddMover(obj,
						obj->x+stoi(parameters.at(2)),
						obj->y+stoi(parameters.at(3)),
						stoi(parameters.at(4)));
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
		str+=L");";
		ExecuteString(str);
		return true;
	} else if (command==L"addNpc") {
		parameters.at(3)=parameters.at(3).substr(1,parameters.at(3).length()-2);
		parameters.at(4)=parameters.at(4).substr(1,parameters.at(4).length()-2);
		engine->objectsManager->AddNpc(stoi(parameters.at(1)),stoi(parameters.at(2)),
			wStringToString(parameters.at(3)),engine->resourcesManager->GetTexture(wStringToString(parameters.at(4))));
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
		//varSet(variable, value);
		std::map<std::wstring,float>::iterator it = engine->variables->find(parameters.at(1));
		if (it!=engine->variables->end()) {
			if (parameters.at(2)==L"++") {it->second+=1; printf("Variable %s incremented\n",parameters.at(1).c_str()); return true;}
			else
				if (parameters.at(2)==L"--") {it->second-=1; printf("Variable %s decremented\n",parameters.at(1).c_str()); return true;}
				else
					if (parameters.at(2)==L"random") {
						std::map<std::wstring,float>::iterator it = engine->variables->find(parameters.at(1));
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
				engine->variables->insert(engine->variables->end(),
					std::pair<std::wstring,float>(parameters.at(1),rand() % stoi(parameters.at(3))));
				printf("Variable %s set to %f\n",parameters.at(1).c_str(),engine->variables->find(parameters.at(1))->second);
				return true;
			} else
				parameters.at(2)=Calculate(parameters.at(2));
			engine->variables->insert(engine->variables->end(),std::pair<std::wstring,float>(parameters.at(1),stof(parameters.at(2))));
			printf("Variable %s set to %f\n",parameters.at(1).c_str(),stof(parameters.at(2)));
			return true;
		};
		return true;
	} else if (command==L"varDel") {
		//varDel(variable);
		std::map<std::wstring,float>::iterator it = engine->variables->find(parameters.at(1));
		if (it!=engine->variables->end()) {
			engine->variables->erase(it);
		};
		printf("Variable %s was deleted\n",parameters.at(1).c_str());
		return true;
	} else if (command==L"varClear") {
		//varClear();
		engine->variables->clear();
		return true;
	} else if (command==L"setCameraTarget") {
		if (parameters.at(1)==L"-1") engine->camera->target=NULL;
		if (parameters.at(1)==L"player") engine->camera->target=engine->objectsManager->GetPlayer();
		return true;
	} else if (command==L"setCameraPosition") {
		if (parameters.at(1)==L"" && parameters.at(2)==L"-1") {
			if (engine->camera->target!=NULL) {
			engine->camera->xView=engine->camera->target->x-(engine->windowSize.x/2);
			engine->camera->yView=engine->camera->target->y-(engine->windowSize.y/2);
			engine->camera->viewTo.x=engine->camera->target->x-(engine->windowSize.x/2);
			engine->camera->viewTo.y=engine->camera->target->y-(engine->windowSize.y/2);
			return true;
			};
		};
		engine->camera->xView=stoi(parameters.at(1));
		engine->camera->yView=stoi(parameters.at(2));
		return true;
	} else if (command==L"addToQueue") {
		parameters.at(1)=parameters.at(1).substr(1,parameters.at(1).length()-2);
		engine->queue->Add(parameters.at(1),parameters.at(2));
		return true;
	} else if (command==L"call") {
		engine->scripting.ExecuteFunction(parameters.at(1));
		return true;
	} else if (command==L"debug") {
		engine->debug = true;
		return true;
	} else if (command==L"end") {
		engine->commandPause = true;
		return true;
	}
	//Resources add
	else if(command==L"addTexture") {
		parameters.at(1)=parameters.at(1).substr(1,parameters.at(1).length()-2);
		parameters.at(2)=parameters.at(2).substr(1,parameters.at(2).length()-2);
		engine->resourcesManager->AddTexture(wStringToString(parameters.at(1)),wStringToString(parameters.at(2)));
		return true;
	}else if(command==L"desaturateTexture") {
		parameters.at(1)=parameters.at(1).substr(1,parameters.at(1).length()-2);
		engine->resourcesManager->DesaturateTexture(wStringToString(parameters.at(1)));
		return true;
	} else if(command==L"addFont") {
		parameters.at(2)=parameters.at(2).substr(1,parameters.at(2).length()-2);
		if (parameters.at(3)!=L"-1") {
			engine->resourcesManager->AddFont(stoi(parameters.at(1)),wStringToString(parameters.at(2)),stoi(parameters.at(3)));
		} else {
			engine->resourcesManager->AddFont(stoi(parameters.at(1)),wStringToString(parameters.at(2)));
		};
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
		if (parameters.at(1).find(L">")!=parameters.at(1).npos) {type = 1; pos = parameters.at(1).find(L">");}
		else if (parameters.at(1).find(L"<")!=parameters.at(1).npos) {type = 2; pos = parameters.at(1).find(L"<");}
		else if (parameters.at(1).find(L">=")!=parameters.at(1).npos) {type = 3; pos = parameters.at(1).find(L">=");}
		else if (parameters.at(1).find(L"<=")!=parameters.at(1).npos) {type = 4; pos = parameters.at(1).find(L"<=");}
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