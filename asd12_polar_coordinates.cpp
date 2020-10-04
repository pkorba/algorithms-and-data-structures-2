#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct Coordinates {
	long long x, y, radius;
	Coordinates() {}
	Coordinates(long long x, long long y, long long radius) {
		this->x = x;
		this->y = y;
		this->radius = radius;
	}
};


inline long long defineQuadrant(long long x, long long y) {
	/*
	 * Zdefiniuj, w której ćwiartce znajduje się dany punkt. Granice ćwiartek określone są
	 * w następujący sposób [oś stanowiąca prawą granicę, oś stanowiaca lewą granicę).
	 */
	long long quadrant = 0;
	if (x > 0 && y >= 0) {
		quadrant = 1;
	}
	else if (x <= 0 && y > 0) {
		quadrant = 2;
	}
	else if (x < 0 && y <= 0) {
		quadrant = 3;
	}
	else if (x >= 0 && y < 0) {
		quadrant = 4;
	}
	return quadrant;
}

bool compare(const Coordinates &c1, const Coordinates &c2) {
	// Funkcja porównująca kolejność punktów względem ich amplitudy i długości promienia.
	int p1Quadrant = defineQuadrant(c1.x, c1.y);
	int p2Quadrant = defineQuadrant(c2.x, c2.y);
	long long r1 = c1.x * c2.y;
	long long r2 = c2.x * c1.y;
	if (p1Quadrant != p2Quadrant) {
		// Jeśli nie leżą w tej samej ćwiartce układu kartezjańskiego określenie kolejności jest łatwe.
		return p1Quadrant < p2Quadrant;
	}
	else if (r1 == r2) {
		// Jeśli leżą na tej samej półprostej porównanie opiera się o długość promienia wodzącego.
		return c1.radius > c2.radius ;
	}
	else {
		// Jeśli leżą w tej samej ćwiartce i nie leżą na jednej półprostej.
		return r1 > r2;
	}
}

int main() {
	unsigned pointsNo;
	long long x, y, radius;
	cin >> pointsNo;
	vector<Coordinates> points;
	for (unsigned i = 0; i < pointsNo; ++i) {
		cin >> x >> y;
		if (x != 0 || y != 0) {
			radius = x * x + y * y;
			points.push_back({x, y, radius});
		}
	}

	sort(points.begin(), points.end(), compare);

	long long rx, ry, q1, q2;
	bool first = true;
	for (Coordinates &c: points) {
		if (first) {
			first = false;
			cout << c.x << " " << c.y << "\n";
		}
		else {
			q1 = defineQuadrant(c.x, c.y);
			q2 = defineQuadrant(rx, ry);
			if (q1 != q2 || c.x * ry != rx * c.y) {
				cout << c.x << " " << c.y << "\n";
			}
		}
		rx = c.x;
		ry = c.y;
	}

	return 0;
}
