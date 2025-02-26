/*
Get user input through std::cin
5 +53

Get rid of the white spaces
5+53

Parse the string to put the constants and operators in a array
char* symbol[3] = {5, +, 53}

Create a symbol container
Symbol symbol[3] = {5, +, 53}

Loop the symbols for operators and solve the surrounding constants
56
*/

#include <iostream>
#include <string>

#ifdef _DEBUG
#define new new ( _NORMAL_BLOCK , __FILE__, __LINE__ )
#endif

enum class SymbolType
{
	None,
	Constant,
	Operator,
	LeftParenthesis,
	RightParenthesis
};

enum class Operator
{
	None,
	Plus,
	Minus,
	Divine,
	Multiply
};

struct Symbol
{
	SymbolType type;
	double constant;
	Operator op;
};

char* GetAndCreateFormulaFromUser()
{
	std::string strInput;

	std::cout << "Enter formula: ";

	// Use getline to include whitespaces
	std::getline(std::cin, strInput);

	char* input = new char[strInput.size() + 1];

	// Copy memory from strInput to input
	memcpy(input, strInput.c_str(), strInput.size());

	// Add null terminator
	input[strInput.size()] = 0;

	return input;
}

void RemoveWhiteSpaces(char* input)
{
	int j = 0;

	//    i
	//   j
	// 50 + 53
	// 50++ 53

	for (int i = 0; input[i] != 0; i++)
	{
		input[j] = input[i];
		if (input[i] != ' ')
		{
			j++;
		}
	}

	input[j] = 0;
}

Symbol* CreateSymbolsFromInput(char* input)
{
	// Give the symbols array the same size as the input array as it will never be longer than the input array
	Symbol* symbols = new Symbol[strlen(input) + 1];

	char number[64] = { 0 };

	int numberIndex = 0, symbolIndex = 0;

	// Loop through the input character array
	for (int i = 0; input[i] != 0; i++)
	{
		// Checking if the character is a part of a constant
		if (input[i] >= '0' && input[i] <= '9' || input[i] == '.')
		{
			// Add the character to our number character array
			number[numberIndex] = input[i];

			// Increase the number index so that we know where to put the next character
			numberIndex++;
		}
		else
		{
			// If the character is not a part of a constant and we have characters in the number array, then add the constant to the symbol array
			if (numberIndex > 0)
			{
				// Add null terminator to the character array
				number[numberIndex] = 0;

				// Add the symbol
				symbols[symbolIndex].type = SymbolType::Constant;

				symbols[symbolIndex].constant = std::stod(number);

				// Increase symbol index
				symbolIndex++;

				// Reset number index
				numberIndex = 0;
			}

			// Now we check if the input character is a operator or parenthesis
			if (input[i] == '*')
			{
				// Add the symbol
				symbols[symbolIndex].type = SymbolType::Operator;

				symbols[symbolIndex].op = Operator::Multiply;

				// Increase symbol index
				symbolIndex++;
			}
			else if (input[i] == '/')
			{
				symbols[symbolIndex].type = SymbolType::Operator;

				symbols[symbolIndex].op = Operator::Divine;

				symbolIndex++;
			}
			else if (input[i] == '-')
			{
				symbols[symbolIndex].type = SymbolType::Operator;

				symbols[symbolIndex].op = Operator::Minus;

				symbolIndex++;
			}
			else if (input[i] == '+')
			{
				symbols[symbolIndex].type = SymbolType::Operator;

				symbols[symbolIndex].op = Operator::Plus;

				symbolIndex++;
			}
			else if (input[i] == '(')
			{
				symbols[symbolIndex].type = SymbolType::LeftParenthesis;

				symbolIndex++;
			}
			else if (input[i] == ')')
			{
				symbols[symbolIndex].type = SymbolType::RightParenthesis;

				symbolIndex++;
			}
		}
	}

	// If we have characters in the number array, then add the constant to the symbol array
	if (numberIndex > 0)
	{
		number[numberIndex] = 0;

		symbols[symbolIndex].type = SymbolType::Constant;

		symbols[symbolIndex].constant = std::stod(number);

		symbolIndex++;
	}

	// Put a null terminator on the symbols array
	symbols[symbolIndex].type = SymbolType::None;

	return symbols;
}

void OutputSymbols(Symbol* symbols)
{
	for (int i = 0; symbols[i].type != SymbolType::None; i++)
	{
		switch (symbols[i].type)
		{
		case SymbolType::Constant:
			std::cout << symbols[i].constant;
			break;
		case SymbolType::Operator:
			switch (symbols[i].op)
			{
			case Operator::Divine:
				std::cout << "/";
				break;
			case Operator::Multiply:
				std::cout << "*";
				break;
			case Operator::Minus:
				std::cout << "-";
				break;
			case Operator::Plus:
				std::cout << "+";
				break;
			}
			break;
		case SymbolType::LeftParenthesis:
			std::cout << "(";
			break;
		case SymbolType::RightParenthesis:
			std::cout << ")";
			break;
		}
	}
}

double SolveExpression(double leftHandSide, Operator op, double rightHandSide)
{
	switch (op)
	{
		case Operator::None:
			// If we don't, then just set the constant
			leftHandSide = rightHandSide;
			break;

		// Do the operations
		case Operator::Plus:
			leftHandSide = leftHandSide + rightHandSide;
			break;

		case Operator::Minus:
			leftHandSide = leftHandSide - rightHandSide;
			break;

		case Operator::Divine:
			leftHandSide = leftHandSide / rightHandSide;
			break;

		case Operator::Multiply:
			leftHandSide = leftHandSide * rightHandSide;
			break;
	}

	return leftHandSide;
}

double SolveFormula(Symbol* symbols, int &inOutIndex)
{
	double leftHandSide = 0.0;

	Operator op = Operator::None;

	// Loop through the symbols until a null terminator is reached
	for (; symbols[inOutIndex].type != SymbolType::None; inOutIndex++)
	{
		switch (symbols[inOutIndex].type)
		{
			case SymbolType::LeftParenthesis:
			{
				inOutIndex += 1;

				// Enter a recursive formula and have the inside of the parenthasis solved seperatly
				double rightHandSide = SolveFormula(symbols, inOutIndex);

				leftHandSide = SolveExpression(leftHandSide, op, rightHandSide);

				break;
			}
			case SymbolType::RightParenthesis:
			{
				inOutIndex += 1;

				// Just leave the recursive function with the left hand side result
				return leftHandSide;
			}
			case SymbolType::Constant:
			{
				// If we reach a constant, assume it is a right hand sided constant
				leftHandSide = SolveExpression(leftHandSide, op, symbols[inOutIndex].constant);
				
				break;
			}
			// Add the operator to pending operators
			case SymbolType::Operator:
				op = symbols[inOutIndex].op;
				
				break;
		}
	}

	// Return left hand side
	return leftHandSide;
}

int main()
{
	// Locate memory leakage
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Get user input through std::cin
	char* input = GetAndCreateFormulaFromUser();

	// Get rid of the white spaces
	RemoveWhiteSpaces(input);

	//std::cout << input << std::endl;

	// Parse the input into symbols
	Symbol* symbols = CreateSymbolsFromInput(input);

	//OutputSymbols(symbols);


	// Todo
	// Either create a new function which sets parenthesis around the expressions with
	// the higher precendence. Example:
	// 5 + 2 * 4 + 2 / 4
	// 5 + (2 * 4) + (2 / 4)
	// Another example:
	// (5 + 2) * 4 + 2 / 4
	// ((5 + 2) * 4) + (2 / 4)
	// And then solve the equation

	// Or do it like this:
	// Solve the expressions with the higher precendence first
	// 5 + 2 * 4 + 2 / 4
	// 5 + 8 + 2 / 4
	// 5 + 8 + 0.5
	// 13.5


	// Solve the equation from left to right
	int index = 0;
	double result = SolveFormula(symbols, index);

	std::cout << "Result: " << result << std::endl;

	delete[] symbols;

	delete[] input;

	return 0;
}