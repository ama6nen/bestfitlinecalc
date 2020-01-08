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

	//note: _ does not indicate subscript here, but for other variables like SS_ret it does
	auto mean_x = take_mean(points, mode::x);
	auto mean_y = take_mean(points, mode::y);
	auto mean_xy = take_mean(points, mode::xy);
	auto mean_xs = take_mean(points, mode::xs);

	//m = (xy - x*y) / ( (x^2) - (x)^2 ) | these are all means, so they would have the line over them to indicate this
	auto m = (mean_xy - mean_x * mean_y) / ((mean_xs)-pow(mean_x, 2));
	
	//b = y - mx | y and x represent means here as well
	auto b = mean_y - m * mean_x;

	//y = mx + b | here they do not represent means anymore, but just normal variables for the standard linear line equation
	printf("Least squared errors best fit line equation is\ny = %fx + %f\n", m, b);

	//using the equation y = mx + b that we got, calculate residuals and mean errors
	auto SS_res = 0.0;
	auto SS_tot = 0.0;
	for (auto point : points) {
		auto predicted_y = m * point.x + b; //using best fit line, predict y
		SS_res += pow(point.y - predicted_y, 2);
		SS_tot += pow(point.y - mean_y, 2);
	}

	//also known as SE_line and SE_y, but SS_res and SS_tot are more commonly used
	printf("SS_res: %f, SS_tot: %f\n", SS_res, SS_tot);
	auto r2 = 1.0 - (SS_res / SS_tot);
	auto r = sqrt(r2);
	printf("r-squared (coefficient of determination) is: %f\n", r2);
	printf("correlation coefficient (r) is: %f\n", r);

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
