#include <iostream>
#include <vector>
#include <string>
#include <conio.h>

#ifdef _WIN32
#include <Windows.h>
inline void SetTitle(std::string title) {
	SetConsoleTitleA(title.c_str()); //windows implementation of setting console title
}
#elif __linux__
inline void SetTitle(std::string title) {
	std::cout << "\e]0;" << title << "\a\n"; //linux implementation of setting console title
}
#endif

struct Point {
	double x;
	double y;

	Point(double x, double y) {
		this->x = x;
		this->y = y;
	}
};

enum mode {
	x,
	y,
	xy,
	xs, //x^2
};
double take_mean(std::vector<Point> points, mode mode) {

	if (points.size() < 2)
		return 0.0;
	double sum = 0.0;
	for (auto point : points)
	{
		switch (mode)
		{
		case mode::x:
			sum += point.x;
			break;
		case mode::y:
			sum += point.y;
			break;
		case mode::xy:
			sum += (point.x * point.y);
			break;
		case mode::xs:
			sum += pow(point.x, 2);
			break;
		}
	}
	return sum / points.size();
}

void calculate_fit() {
	std::vector<Point> points{};
	while (!std::cin.fail()) {
		std::string temp;
		std::cin >> temp;
		if (temp == "done") {
			if (points.size() < 2) {
				printf("We need at least 2 points to calculate a line!\n");
				continue;
			}
			break;
		}

		if (temp.find(",") == -1) //couldnt find -1
		{
			printf("Input is in incorrect format!\n");
			continue;
		}
		std::string xs = temp.substr(0, temp.find(",")); //left hand side of ,
		std::string ys = temp.substr(temp.find(",") + 1); //right hand side of ,

		try {
			std::string::size_type sz{};
			double x = std::stod(xs, &sz);
			if (sz != xs.size()) //we did parse something, but there was a character in between (ex: 5758fdau28)
				throw std::invalid_argument("wrong size");
			double y = std::stod(ys, &sz);
			if (sz != ys.size()) //same as above
				throw std::invalid_argument("wrong size");

			points.push_back(Point{ x, y });
		}
		catch (std::invalid_argument ex) {
			printf("Invalid input, did not get a number!\n");
			continue;
		}
	}

	double x_mean = take_mean(points, mode::x);
	double y_mean = take_mean(points, mode::y);
	double xy_mean = take_mean(points, mode::xy);
	double xs_mean = take_mean(points, mode::xs);

	//m = (xy - x*y) / ( (x^2) - (x)^2 ) | these are all means, so they would have the line over them to indicate this
	//b = y - mx | y and x represent means here as well
	//y = mx + b | here they do not represent those anymore, but just a normal line

	double m = (xy_mean - x_mean * y_mean) / ((xs_mean)-pow(x_mean, 2));
	double b = y_mean - m * x_mean;

	printf("Least squared errors best fit line equation is\ny = %fx + %f\n", m, b);
}
int main()
{
	SetTitle("Best fit line calculator");

	char ch = ' ';
	do {
		printf("Enter each coordinate as \"x,y\" and then press enter.\r\nTo stop feeding coordinates type \"done\" and press enter.\r\n");
		calculate_fit();
		printf("press c to continue, and any other key to exit.\n");
		ch = (char)_getch();
	} while (ch == 'c');
}
