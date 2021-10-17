#include "Living.hpp"

int Living::nb = 0;

int Living::count() { return nb;  }

void Living::display(int row) {
	string* img = getImg();
	if (!img) {
		printf("XXX");
	}
	else {
		int begin = row * img_width;
		for (int i = begin; i < begin+img_width; i++) {
			cout << img[i];
		}
	}

}