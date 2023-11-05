#include "Data.h"

Data::Data() : value(0) { }

void Data::operator++(int d) {
	value++;
}

Data Data::operator+(const uint& v) {
	Data tmp;
	tmp.value = value + v;

	return tmp;
}

Data Data::operator=(const uint& num) {
	value = num;

	return *this;
}
void Data::reset() {
	value = 0;
}

uint Data::get() {
	return value;
}

uint Data::getTwincle() {
	/* きらきらはコンボ数の数により生成数が変わる
	 * 以下対応表
	 * combo | twincle
	 *  0,1  |    0
	 *   2   |    2
	 *   3   |	  4
	 *   4   |    5
	 *   5   |    6
	 * 6以上 |   12
	 */
	switch (value) {
		case 0:
		case 1:
			return 0;
		case 2:
			return 2;
		case 3:
			return 4;
		case 4:
			return 5;
		case 5:
			return 6;
		default:
			return 12;
	}
	
}
