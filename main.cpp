#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>


enum Direction { UP, DOWN, LEFT, RIGHT };


int point2index(int x, int y, int width = 20) {
	return x * width + y;
}

std::vector<int> index2point(int z, int width = 20) {
	std::vector<int> point;
	point.push_back(z / width);
	point.push_back(z % width);
	return point;
}

std::vector<int> update_snake(std::vector<int>& snake_vec, const std::vector<Direction>& snake_directions) {
	for (int i = 0; i < snake_vec.size(); i++) {
		std::vector<int> point = index2point(snake_vec.at(i));
		Direction snake_dir = snake_directions.at(i);
		if (snake_dir == UP) {
			point.at(0) -= 1;
		}
		else if (snake_dir == DOWN) {
			point.at(0) += 1;
		}
		else if (snake_dir == LEFT) {
			point.at(1) -= 1;
		}
		else if (snake_dir == RIGHT) {
			point.at(1) += 1;
		}
		snake_vec.at(i) = point2index(point.at(0), point.at(1));
	}
	return snake_vec;
}


std::vector<Direction> update_snake_directions(const std::vector<Direction>& snake_directions) {
	std::vector<Direction> new_directions;
	new_directions.push_back(snake_directions.at(0));
	for (int i = 0; i < snake_directions.size() - 1; i++) {
		new_directions.push_back(snake_directions.at(i));
	}
	return new_directions;
}


bool check_if_index_in_vector(int index, std::vector<int> vec) {
	for (int i = 0; i < vec.size(); i++) {
		if (index == vec.at(i)) {
			return true;
		}
	}
	return false;
}


bool check_duplicate(std::vector<int> snake_vec) {
	for (int i = 1; i < snake_vec.size(); i++) {
		if (snake_vec.at(i) == snake_vec.at(0)) {
			return true;
		}
	}
	return false;
}


bool check_if_out_of_bounds(std::vector<int> snake_vec) {
	std::vector<int> point = index2point(snake_vec.at(0));
	int y = point.at(0);
	int x = point.at(1);
	if (x > 0 && x < 19 && y > 0 && y < 19) {
		return false;
	}
	return true;
}


void temp(std::vector<int>& snake_vec, std::vector<Direction>& snake_directions, std::vector<int>& reward_positions) {
	for (int i = 0; i < reward_positions.size(); i++) {
		if (snake_vec.at(0) == reward_positions.at(i)) {
			reward_positions.erase(reward_positions.begin() + i);
			int tail_idx = snake_vec.back();
			std::vector<int> tail_point = index2point(tail_idx);
			if (snake_directions.back() == UP) {
				snake_vec.push_back(point2index(tail_point.at(0) + 1, tail_point.at(1)));
				snake_directions.push_back(UP);
			}
			else if (snake_directions.back() == DOWN) {
				snake_vec.push_back(point2index(tail_point.at(0) - 1, tail_point.at(1)));
				snake_directions.push_back(DOWN);
			}
			else if (snake_directions.back() == LEFT) {
				snake_vec.push_back(point2index(tail_point.at(0), tail_point.at(1) + 1));
				snake_directions.push_back(LEFT);
			}
			else if (snake_directions.back() == RIGHT) {
				snake_vec.push_back(point2index(tail_point.at(0), tail_point.at(1) - 1));
				snake_directions.push_back(RIGHT);
			}

			return;
		}
	}
}


std::vector<int> get_reward_positions(int nrows, int ncols) {
	std::srand(time(0));
	std::vector<int> positions;
	while (true) {
		if (rand() % 1000 < 10) {
			positions.push_back(rand() % 400);
		}
		if (positions.size() == 3) {
			return positions;
		}
	}
}


void draw(int nrows, int ncols, const std::vector<int>& snake_vec, std::vector<int>& reward_positions) {
	int x, y;
	std::cout << "_";
	for (x = 0; x < ncols; x++) {
		std::cout << "_";
	}
	std::cout << "_" << std::endl;
	for (x = 0; x < nrows; x++) {
		std::cout << "|";
		for (y = 0; y < ncols; y++) {
			if (check_if_index_in_vector(point2index(x, y), snake_vec)) {
				std::cout << "o";
			}
			else if (check_if_index_in_vector(point2index(x, y), reward_positions)) {
				std::cout << "$";
			}
			else if (reward_positions.size() < 3) {
				reward_positions.push_back(rand() % 400);
				std::cout << " ";
			}
			else {
				std::cout << " ";
			}
		}
		std::cout << "|" << std::endl;
	}
	std::cout << "|";
	for (x = 0; x < ncols; x++) {
		std::cout << "_";
	}
	std::cout << "|" << std::endl;
}


int main() {
	int snake_idx = 0;
	std::vector<int> snake_vec;
	std::vector<Direction> snake_directions;
	std::vector<int> reward_positions = get_reward_positions(20, 20);

	snake_vec.push_back(point2index(10, 10));
	snake_directions.push_back(UP);

	bool done = false;
	while (!done) {
		system("cls");

		update_snake(snake_vec, snake_directions);
		snake_directions = update_snake_directions(snake_directions);
		temp(snake_vec, snake_directions, reward_positions);
		draw(20, 20, snake_vec, reward_positions);
		if (_kbhit()) {
			char ch = getch();
			if (ch == 'W' || ch == 'w') {
				snake_directions.at(0) = UP;
			}
			if (ch == 'S' || ch == 's') {
				snake_directions.at(0) = DOWN;
			}
			if (ch == 'A' || ch == 'a') {
				snake_directions.at(0) = LEFT;
			}
			if (ch == 'D' || ch == 'd') {
				snake_directions.at(0) = RIGHT;
			}
		}
		done = check_duplicate(snake_vec) || check_if_out_of_bounds(snake_vec);
		if (done) {
			std::cout << "Snake ate itself!" << std::endl;
		}

		Sleep(150);
	}
	return 0;
}
