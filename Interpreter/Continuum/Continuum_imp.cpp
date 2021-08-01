// Continuum_imp.cpp :  

#include "Continuum.h"
#include <string>

bool Continuum::ReadFile()
{
	f_in.open("Continuum.txt");
	while (!f_in.eof())
	{
		std::string str;
		std::getline(f_in, str);
		Check(str);
	}
	if (condition_flag) {
		std::cout << "Compile error" << std::endl;
		exit(1);
	}
	return f_in.is_open();
}

bool Continuum::Check(std::string str)
{
	if(str == "")
	{
		return 1;
	}
	str.push_back(' ');	
	words.clear();
	ReadLines(str);
	if (condition_flag || loop_flag) {
		if (IsExist(words[0], Instructions)) {
			std::cout << "Compile error !!!" << std::endl;
			exit(1);
		}
		if (words[0] == "}") {
			condition_flag = 0;
			return 1;
		}
	}
	if (IsExist(words[0], TypeName))
	{
		ConvertType(words);
	}
	else if (words[0] == "print")
	{
		Print(words);
	}
	else if (IsExist(words[0], Variables))
	{
		ArithmeticUnit(words);
	}
	else if (words[0]=="ete") {
		condition_flag = Condition(words);
	}
	else if (words[0] == "cycle") {
		Loop(words);
	}
	else
	{
		std::cout << "Compile error: ";
		exit(1);
	}

}

int Continuum::ReadLines(std::string str)
{
	if (str == "")
		return 0;
	std::string line = str;
	std::string word;
	for (int i = 0; i < line.size(); ++i) {
		if (line[i] != ' ') {
			word.push_back(line[i]);
		}
		else {
			if (word != "") {
				words.push_back(word);
			}
			word.clear();
		}
	}
}

bool Continuum::IsDigit(std::string str)
{
	int count = 0;
	int notDigit = 0;
	for (int i = 0; i < str.size();) {
		if (str[i] == 46 && i != 0 && i != str.size() - 1) {
			++count;
			++i;
		}
		else if (str[i] >= 48 && str[i] <= 57) {
			++i;
		}
		else {
			++notDigit;
			++i;
		}
	
	}
	if (count > 1 || notDigit > 0)
		return false;
	else return true;
}

bool Continuum::SyntaxVariable(std::string str)
{
	if ((str[0] >= 65 && str[0] <= 90) || (str[0] >= 97 && str[0] <= 122) || str[0] == 95) {}
	else return false;
		for (int i = 1; i < str.size(); ++i) {
			if ((str[i] >= 65 && str[i] <= 90) || (str[i] >= 97 && str[i] <= 122) || str[i] >= 48 && str[i] <= 57 || str[i] == 95) {}
			else return false;
		}
		return true;
}

bool Continuum::IsExist(std::string str, std::vector<std::string> vec)
{
	for (int i = 0; i < vec.size(); ++i) {
		if (str == vec[i]) {
			return true;
		}
	}
		return false;
}

bool Continuum::ConvertType(std::vector<std::string> vec)
{
	if (vec.size() != 2 && vec.size() != 4)
	{
		std::cout << "Compile error !!!";
		exit(1);
	}
	if (!SyntaxVariable(vec[1])) {
		std::cout << "Compile error !!!" << std::endl;
			exit(1);
	}
	else {
		Variables.push_back(vec[1]);
	}
	if (vec.size() == 4) {

		if (vec[2] != "=") {
			std::cout << "Compile error !!!" << std::endl;
			exit(1);
		}
		if (!IsDigit(vec[3])) {
			std::cout << "Compile error !!!" << std::endl;
			exit(1);
		}
	}

	Variable* p = new Variable;
	p->typeName = vec[0];
	p->variableName = vec[1];
	if (p->typeName == "number") {
		p->value = new int;
		if (vec.size() == 4) {
			*(int*)p->value = std::stoi(vec[3]);
		}
		else
			*(int*)p->value = 0;
	}
	else if (p->typeName == "real") {
		p->value = new float;
		if (vec.size() == 4) {
			*(float*)p->value = std::stof(vec[3]);
		}
		else
			*(float*)p->value = 0;
	}
	vars.push_back(p);
	return true;
}

void Continuum::ArithmeticUnit(std::vector<std::string> vec) 
{
	if (vec.size() == 5 || vec.size() == 3)
	{
		if (vec[1] != "=") 
		{
			std::cout << "error" << std::endl;
			exit(1);
		}
		if (IsDigit(vec[2]))
		{
			tmp1 = stof(vec[2]);
			int index = CheckVariable(vec[0]);
			if (vars[index]->typeName == "number") 
			{
				*(int*)vars[index]->value = tmp1;
			}
			else
				*(float*)vars[index]->value = tmp1;
		}
		else if (IsExist(vec[2], Variables))
		{
			int index = CheckVariable(vec[2]);
			if (vars[index]->typeName == "number") 
			{
				tmp1 = *(int*)vars[index]->value;
			}
			else
				tmp1 = *(float*)vars[index]->value;
		}
		else
		{
			std::cout << "Compile error!!!" << std::endl;
			exit(1);
		}
		if (vec.size() == 5)
		{
			if (IsDigit(vec[4]))
			{
				tmp2 = stof(vec[4]);
			}
			else if (IsExist(vec[4], Variables))
			{
				int index = CheckVariable(vec[4]);
				if (vars[index]->typeName == "number") 
				{
					tmp2 = *(int*)vars[index]->value;
				}
				else
					tmp2 = *(float*)vars[index]->value;
			}
			else
			{
				std::cout << "Compile error!!!" << std::endl;
				exit(1);
			}
			int id = CheckVariable(vec[0]);

			if (vec[3] == "+")
			{
				result = tmp1 + tmp2;
			}
			else if (vec[3] == "-") 
			{
				result = tmp1 - tmp2;
			}
			else if (vec[3] == "*")
			{
				result = tmp1 * tmp2;
			}
			else if (vec[3] == "/")
			{
				if (tmp2 == 0)
				{
					std::cout << "ZERO division error !!!" << std::endl;
					exit(1);
				}
				result = tmp1 / tmp2;
				
			}
			else
			{
				std::cout << "Compile error !!!" << std::endl;
				exit(1);
			}
			if (vars[id]->typeName == "number")
			{
				*(int*)vars[id]->value = result;
			}
			else if (vars[id]->typeName == "real") 
			{
				*(float*)vars[id]->value = result;
			}
			else 
			{
				std::cout << "Compile error !!!" << std::endl;
				exit(1);
			}
		}
	}
	else 
    {
		std::cout << "Compile error !!!" << std::endl;
		exit(1);
	}
}

Continuum::~Continuum()
{
	for (int i = 0; i < vars.size(); ++i) {
		delete vars[i];
	}
}

int Continuum::CheckVariable(std::string str)
{
	for (int i = 0; i < Variables.size(); ++i) {
		if (str == Variables[i])
			return i;
	}
	return -1;
}

int Continuum::Condition(std::vector<std::string> vec)
{
	if (vec.size() == 5) {
		float tmp1 = 0;
		float tmp2 = 0;
		if (IsDigit(vec[1])) {
			tmp1 = stof(vec[1]);
		}
		else if (IsExist(vec[1], Variables)) {
			int index = CheckVariable(vec[1]);
				if (vars[index]->typeName == "number") {
					tmp1 = *(int*)vars[index]->value;
				}
				else 
					tmp1 = *(float*)vars[index]->value;
		}
		else {
			std::cout << "Compile error !!!" << std::endl;
			return 0;
		}
		if (IsDigit(vec[3])) {
			tmp2 = stof(vec[3]);
		}
		else if (IsExist(vec[3],Variables)) {
			int index = CheckVariable(vec[3]);
				if (vars[index]->typeName == "number") {
					tmp2 = *(int*)vars[index]->value;
				}
				else
					tmp2 = *(float*)vars[index]->value;
		}
		else {
			std::cout << "Compile error !!!" << std::endl;
			return 0;
		}
		if (vec[4] != "{") {
			std::cout << "Compile error !!!" << std::endl;
			return 0;
		}
		if (vec[2] == ">") {
			if (tmp1 > tmp2) return 1;
			else return 0;
		}
		if (vec[2] == "<") {
			if (tmp1 < tmp2) return 1;
			else return 0;
		}
		if (vec[2] == "!=") {
			if (tmp1 != tmp2) return 1;
			else return 0;
		}
		if (vec[2] == "==") {
			if (tmp1 == tmp2) return 1;
			else return 0;
		}
		else {
			std::cout << "Compile error !!!" << std::endl;
			exit(1);
		}
	}
	else {
		std::cout << "Compile error !!!" << std::endl;
		exit(1);
	}
}

void Continuum::Print(std::vector<std::string> vec)
{
	for (int i = 1; i < vec.size(); ++i) {
		int flag = 0;
		for (int j = 0; j < Variables.size(); ++j) {
			if (vec[i] == Variables[j]) {
				if (vars[j]->typeName == "number") {
					std::cout << *(int*)vars[j]->value;
				}
				else if (vars[j]->typeName == "real") {
					std::cout << *(float*)vars[j]->value ;
				}
				++flag;
				break;
			}
		}
		if (flag == 0) {
			std::cout << vec[i];
		}
		std::cout << " ";
	}
	std::cout << std::endl;
}

void Continuum::Loop(std::vector<std::string> vec)
{
	if (vec.size() == 5) {
		float tmp1 = 0;
		float tmp2 = 0;
		if (IsDigit(vec[1]))
			tmp1 = stof(vec[1]);
		else if (IsExist(vec[1], Variables)) {
			int index = CheckVariable(vec[1]);
			tmp1 = index;
		}
		else {
			std::cout << "Compile error !!!" << std::endl;
			exit(1);
		}
		if (IsDigit(vec[3]))
			tmp2 = stof(vec[3]);
		else if (IsExist(vec[3], Variables)) {
			int index = CheckVariable(vec[3]);
			tmp2 = index;
		}
		else {
			std::cout << "Compile error !!!" << std::endl;
			exit(1);
		}
		if (vec[4] != "{") {
			std::cout << "Compile error !!! " << std::endl;
			exit(1);
		}
		while (!f_in.eof()) {
			std::string str;
			std::getline(f_in, str);
			loop_flag = 1;
			if (str == "}") {
				++loop_flag;
				break;
			}
			else {
				Stack.push_back(str);
			}
		}
		if (loop_flag == 1) {
			std::cout << "Compile error !!!" << std::endl;
			exit(1);
		}
		if (vec[2] == ">") {
			big(tmp1, tmp2);
		}
		else if (vec[2] == "<") {
			small(tmp1, tmp2);
		}
		else if (vec[2] == "==") {
			equal(tmp1, tmp2);
		}
		else if (vec[2] == "!=") {
			notEqual(tmp1, tmp2);
		}
		else {
			std::cout << "Compile error !!! " << std::endl;
			exit(1);
		}
	}
	else {
		std::cout << "Compile error !!!" << std::endl;
		exit(1);
	}
	loop_flag = 0;
	exit(1);
}

void Continuum::big(float x, float y)
{
	if (vars[x]->typeName == "number" && vars[y]->typeName == "number")
	{
		while (*(int*)vars[x]->value > *(int*)vars[y]->value) {
			for (int i = 0; i < Stack.size(); ++i) {
				Check(Stack[i]);
			}
		}
	}
	if (vars[x]->typeName == "number" && vars[y]->typeName == "real")
	{
		while (*(int*)vars[x]->value > *(float*)vars[y]->value) {
			for (int i = 0; i < Stack.size(); ++i) {
				Check(Stack[i]);
			}
		}
	}
	if (vars[x]->typeName == "real" && vars[y]->typeName == "number")
	{
		while (*(float*)vars[x]->value > *(int*)vars[y]->value) {
			for (int i = 0; i < Stack.size(); ++i) {
				Check(Stack[i]);
			}
		}
	}
	if (vars[x]->typeName == "real" && vars[y]->typeName == "real")
	{
		while (*(float*)vars[x]->value > *(float*)vars[y]->value) {
			for (int i = 0; i < Stack.size(); ++i) {
				Check(Stack[i]);
			}
		}
	}
}

void Continuum::small(float x, float y)
{
	if (vars[x]->typeName == "number" && vars[y]->typeName == "number")
	{
		while (*(int*)vars[x]->value < *(int*)vars[y]->value) {
			for (int i = 0; i < Stack.size(); ++i) {
				Check(Stack[i]);
			}
		}
	}
	if (vars[x]->typeName == "number" && vars[y]->typeName == "real")
	{
		while (*(int*)vars[x]->value < *(float*)vars[y]->value) {
			for (int i = 0; i < Stack.size(); ++i) {
				Check(Stack[i]);
			}
		}
	}
	if (vars[x]->typeName == "real" && vars[y]->typeName == "number")
	{
		while (*(float*)vars[x]->value < *(int*)vars[y]->value) {
			for (int i = 0; i < Stack.size(); ++i) {
				Check(Stack[i]);
			}
		}
	}
	if (vars[x]->typeName == "real" && vars[y]->typeName == "real")
	{
		while (*(float*)vars[x]->value < *(float*)vars[y]->value) {
			for (int i = 0; i < Stack.size(); ++i) {
				Check(Stack[i]);
			}
		}
	}
}

void Continuum::equal(float x, float y)
{
	if (vars[x]->typeName == "number" && vars[y]->typeName == "number")
	{
		while (*(int*)vars[x]->value == *(int*)vars[y]->value) {
			for (int i = 0; i < Stack.size(); ++i) {
				Check(Stack[i]);
			}
		}
	}
	if (vars[x]->typeName == "number" && vars[y]->typeName == "real")
	{
		while (*(int*)vars[x]->value == *(float*)vars[y]->value) {
			for (int i = 0; i < Stack.size(); ++i) {
				Check(Stack[i]);
			}
		}
	}
	if (vars[x]->typeName == "real" && vars[y]->typeName == "number")
	{
		while (*(float*)vars[x]->value == *(int*)vars[y]->value) {
			for (int i = 0; i < Stack.size(); ++i) {
				Check(Stack[i]);
			}
		}
	}
	if (vars[x]->typeName == "real" && vars[y]->typeName == "real")
	{
		while (*(float*)vars[x]->value == *(float*)vars[y]->value) {
			for (int i = 0; i < Stack.size(); ++i) {
				Check(Stack[i]);
			}
		}
	}
}

void Continuum::notEqual(float x, float y)
{
	if (vars[x]->typeName == "number" && vars[y]->typeName == "number")
	{
		while (*(int*)vars[x]->value != *(int*)vars[y]->value) {
			for (int i = 0; i < Stack.size(); ++i) {
				Check(Stack[i]);
			}
		}
	}
	if (vars[x]->typeName == "number" && vars[y]->typeName == "real")
	{
		while (*(int*)vars[x]->value != *(float*)vars[y]->value) {
			for (int i = 0; i < Stack.size(); ++i) {
				Check(Stack[i]);
			}
		}
	}
	if (vars[x]->typeName == "real" && vars[y]->typeName == "number")
	{
		while (*(float*)vars[x]->value != *(int*)vars[y]->value) {
			for (int i = 0; i < Stack.size(); ++i) {
				Check(Stack[i]);
			}
		}
	}
	if (vars[x]->typeName == "real" && vars[y]->typeName == "real")
	{
		while (*(float*)vars[x]->value != *(float*)vars[y]->value) {
			for (int i = 0; i < Stack.size(); ++i) {
				Check(Stack[i]);
			}
		}
	}
}




 