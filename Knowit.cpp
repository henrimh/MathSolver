#include <iostream>
#include <string>
#include "MathSolver.h"

int main()
{
	std::string input; // Example input 3+4*(2-1)
	float result;

	MathSolver Solver;

	while (true)
	{
		std::cin >> input;

		result = Solver.Solve(input);

		std::cout << result << '\n';
	}


	return 0;
}
