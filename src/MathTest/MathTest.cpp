#include <iostream>
#include "H4x4Matrix.h"

using namespace std;

int main()
{
	H4x4Matrix test;

	test.matrix = {
		{1, 2, 3, 4},
		{5, 12, 7, 8},
		{9, 23, 4 ,54},
		{34, 34, 34, 23}
	};

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cout << test.matrix[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;

	cout << test.GetDetermination();
}