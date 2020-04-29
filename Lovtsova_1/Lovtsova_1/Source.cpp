#include "EarlyDateDll.h"

void main() {
	setlocale(LC_ALL, "rus");
	vector<date>v;
	cout << "¬ведите количество дат: ";
	int n;
	date u;
	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> u;
		v.push_back(u);
	}
	cout <<"Cама€ ранн€€ дата: "<< earlyDate(v) << endl;
	
	system("pause");
}