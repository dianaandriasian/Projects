#ifndef _CONTINUUM_
#define _CONTINUUM_

#include <fstream>
#include <iostream>
#include <vector>

struct Variable
{
	std::string typeName;
	std::string variableName;
	void* value;
};

class Continuum
{
public:
	bool ReadFile();
	~Continuum();
private:
	bool Check(std::string str);
	bool SyntaxVariable(std::string str);
	bool ConvertType(std::vector<std::string> vec);
	bool IsDigit(std::string str);
	bool IsExist(std::string str, std::vector<std::string> vec);               
	int ReadLines(std::string str);
	int CheckVariable(std::string str);
	int Condition(std::vector<std::string> vec);
	void Loop(std::vector<std::string> vec);
	void big(float x, float y);
	void small(float x, float y);
	void equal(float x, float y);
	void notEqual(float x, float y);
	void ArithmeticUnit(std::vector<std::string> vec);
	void Print(std::vector<std::string> words);
	std::ifstream f_in;
	std::vector<std::string> words;
	std::vector<Variable*> vars;
	std::vector<std::string> Variables;
	std::vector<std::string> TypeName { "number", "real" };
	std::vector<std::string> Instructions { "ete", "cycle" };
	std::vector<std::string> Stack;
	int condition_flag{ 0 };
	int loop_flag{ 0 };
	float tmp1 = 0;
	float tmp2 = 0;
	float result = 0;
};

#endif // _CONTINUUM_