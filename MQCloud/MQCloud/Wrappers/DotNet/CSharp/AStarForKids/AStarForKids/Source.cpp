#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<vector<bool>> naive;

struct point {
	point(const point & other)
		: src(other.src),
		  x(other.x),
		  y(other.y),
		  width(other.width),
		  height(other.height),
		  touched(other.touched) {}

	point & operator=(const point & other) {
		if(this == &other)
			return *this;
		src = other.src;
		x = other.x;
		y = other.y;
		width = other.width;
		height = other.height;
		touched = other.touched;
		return *this;
	}

	point(int x, int y, vector<vector<bool>> & src) : src(src), x(x), y(y), touched(false) {
		width = src.size();
		height = src[x].size();
	}

	bool CanGoLeft() {
		if(width <= x + 1) {
			return false;
		}
		return src[x + 1][y];
	}

	point GetLeft() {
		return point(x + 1, y, src);
	}

	bool CanGoUp() {
		if(y - 1 < 0) {
			return false;
		}
		return src[x][y - 1];
	}

	point GetUp() {
		return point(x, y - 1, src);
	}

	bool CanGoDown() {
		if(y + 1 <= height) {
			return false;
		}
		return src[x][y + 1];
	}

	point GetDown() {
		return point(x, y + 1, src);
	}

	bool CanGoRight() {
		if(x - 1 < 0) {
			return false;
		}
		return src[x - 1][y];
	}

	point GetRight() {
		return point(x - 1, y, src);
	}

	int distance(point & other) {
		// todo: сохранять значение если уже найдено
		auto dx = (x - other.x);
		auto dy = (y - other.y);
		return dx * dx + dy * dy;
	}

	//todo: bool CanGoUpRight() и ему подобные
	//todo: point GetRight() и ему подобные

	vector<vector<bool>> & src;
	int x;
	int y;
	int width;
	int height;
	bool touched;
};

bool TryToFindPath(point & from, point & to, vector<point> & outPoints) {
	auto end = false;
	auto canExpand = true;

	vector<point> points;
	points.push_back(from);
	// is path findable at all
	while(!end && canExpand) {
		sort(points.begin(), points.end(), [&](point & a, point & b) {
			                                   if(a.distance(to) == b.distance(to)) {
				                                   return a.touched > b.touched;
			                                   } else {
				                                   return a.distance(to) < b.distance(to);
			                                   }
			                                   return false;
		                                   });
		canExpand = false;
		auto curent = points[0];
		if(curent.distance(to) == 0) {
			end = true;
			return true;
		}

		if(curent.touched) {
			canExpand = false;
		} else {
			points[0].touched = true;

			if(curent.CanGoLeft()) {
				points.push_back(curent.GetLeft());
				canExpand = true;
			}

			if(curent.CanGoRight()) {
				points.push_back(curent.GetRight());
				canExpand = true;
			}

			if(curent.CanGoUp()) {
				points.push_back(curent.GetUp());
				canExpand = true;
			}

			if(curent.CanGoDown()) {
				points.push_back(curent.GetDown());
				canExpand = true;
			}
		}

	}


	return false;
}


int main() {
	naive.resize(10);
	for(auto & column : naive) {
		column.resize(10);
	}

	for(auto i = 0; i < 10; i++) {
		for(auto j = 0; j < 10; j++) {
			naive[i][j] = 1;
			//naive[i][j] = rand() % 2;
		}
	}

	auto from = point(0, 0, naive);
	auto to = point(9, 9, naive);
	vector<point> result;
	auto havePath = TryToFindPath(from, to, result);

	if(havePath) {
		cout << "found path" << endl;
	}
	cout << "not found path" << endl;

	for(auto k = 0; k < 10; k++) {
		for(auto l = 0; l < 10; l++) {
			/*if(havePath) {
				auto inPath = find_if(result.begin(), result.end(), [=](point & p){
					return ((p.x == k) && (p.y == l));
				});
				if(inPath != result.end()) {
					cout << " 3 ";
				} else {
					cout << " " << naive[k][l] << " ";
				}
			} else {
			*/
			cout << " " << naive[k][l] << " ";
			//}
		}
		cout << endl;
	}
	cin.get();
	return 0;
}
