#pragma once
#include <string>
#include <stack>
#include <utility>
#include <math.h>

// Math Solver solves simple mathematical formulas using the Shunting-Yard algorithm. Operations are resolved in the middle of parsing to minimize string parsing.
class MathSolver
{
	std::string input;
	const int maxOperatorsInInput = 256;
	int operatorsInInput = 0;
	// Tracks mismatching parenthesis
	int parenthesisOnStack = 0;
	// Tracks bad syntax e.g. "1+*2".
	bool illegalOperationsInRow = false;
		
	std::stack<float> numberStack;

	// Operator + precedence of the operation
	std::stack<std::pair<char, int>> operationStack;

	float ResolveNextOperation()
	{
		float stackTop = numberStack.top();
		numberStack.pop();

		float stackSecond = numberStack.top();
		numberStack.pop();

		char operation = operationStack.top().first;
		operationStack.pop();

		switch (operation)
		{
		case '*':
			return stackSecond * stackTop;
		case '/':
			if (stackTop == 0)
				throw "Can't divide with zero!";
			return stackSecond / stackTop;
		case '%':			
			return fmod (stackSecond, stackTop);
		case '-':
			return stackSecond - stackTop;
		case '+':
			return stackSecond + stackTop;
		case '(':
			throw "Mismatchin parenthesis '('!";		
			//return 0;
		}	
	}

	void ResolveStack()
	{
		while (operationStack.empty() == false)
		{
			numberStack.push(ResolveNextOperation());			
		}

		return;
	}

	// Used to resolve operations until '(' or a '+' or '-' if a more precedent operation would be added onto the stack.
	void ResolveStackUntil(int precedence)
	{
		while (operationStack.top().second > precedence)
		{
			numberStack.push(ResolveNextOperation());

			if (operationStack.empty())
				return;		
		}

		//precedence 0 means '(', it's always popped
		if (operationStack.top().second == 0)
			operationStack.pop();

		return;
	}

	void PushToOperatorStack(char operation)
	{
		// Push operations and their precedences onto the stack
		switch (operation)
		{
		case '*':
		case '/':
		case '%':
			if (illegalOperationsInRow)
				throw ("Bad Syntax!");

			illegalOperationsInRow = true;

			//No need to check precedence because these have the highest
			operationStack.push(std::make_pair(operation, 3));			
			break;

		case '-':
		case '+': 
			if (illegalOperationsInRow)
				throw ("Bad Syntax!");
			
			illegalOperationsInRow = true;

			if (!operationStack.empty()) //if stack not empty. check if need to resolve.
				if (operationStack.top().second > 2)
					ResolveStackUntil(2);

			operationStack.push(std::make_pair(operation, 2));
			break;

		case '(': // Always pushed to stack.
			operationStack.push(std::make_pair(operation, 0));
			parenthesisOnStack++;
			return; // Return because only pairs of parenthesis are counted as operators.

		case ')': // Closed parenthesis immediately resolve the whole stack until '(' operator unless parenthesis mismatch.
			parenthesisOnStack--;
			if (parenthesisOnStack < 0) // No '(' on the stack. 
				throw("Mismatching parenthesis!");

			ResolveStackUntil(0);
			break;
		}

		operatorsInInput++;
		if (operatorsInInput >= maxOperatorsInInput)
			throw("Too many operators in input!");
	}

	void PushToNumberStack(std::string numStr)
	{
		numberStack.push(std::stof(numStr));
		illegalOperationsInRow = false;
	}

	void PopStacks()
	{
		while (!operationStack.empty())
			operationStack.pop();

		while (!numberStack.empty())
			numberStack.pop();
	}

	bool isOperator(char c)
	{
		if (c == '(' ||
			c == ')' || 
			c == '*' || 
			c == '/' || 
			c == '%' || 
			c == '+' || 
			c == '-')
			return true;

		return false;		
	}

public:

	
	float Solve(std::string input)
	{
		float answer;
		int numStartIndex = 0;
		bool operatorsInRow = false;

		//Ensure that the stacks are empty before doing anything else.
		PopStacks();
		//Reset to track 256 operator limit correctly.
		operatorsInInput = 0;
		parenthesisOnStack = 0;

		for (int i = 0; i < input.length(); i++)
		{			
			if (isOperator(input[i]))
			{
				if(i != 0 && operatorsInRow == false) //Don't push numbers to stack if the first char is operator
					PushToNumberStack(input.substr(numStartIndex, i - numStartIndex)); //Push substring the length of the number to stack

				try
				{
					PushToOperatorStack(input[i]);
				}
				catch (const char* msg)
				{
					std::cout << msg;
					return 0;
				}

				numStartIndex = i + 1;
				operatorsInRow = true;
			}
			else
				operatorsInRow = false;
			
		}

		//Add the last number of input to stack.
		if (input.back() != ')') //The last parenthesis will be pushed onto the stack in the for loop. 
			PushToNumberStack(input.substr(numStartIndex, input.back() - numStartIndex));

		//Resolve all remaining operations on the stack.
		try
		{
			ResolveStack();
		}
		catch(const char* msg)
		{
			std::cout << msg;
			return 0;
		}

		//Last number left to stack should be the answer.
		answer = numberStack.top();
			   
		return answer;
	}
};