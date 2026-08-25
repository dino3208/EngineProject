#include <cmath>
#include <iostream>

int main()
{
	float playerAngle = 0.0f;

	float dx = cosf(playerAngle);
	float dy = sinf(playerAngle);

	std::cout << "dx: " << dx << ", dy: " << dy << std::endl;
}