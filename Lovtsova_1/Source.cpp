#include "EarlyDateDll.h"

void main() {
	setlocale(LC_ALL, "rus");
	vector<date>v;
	cout << "������� ���������� ���: ";
	int n;
	date u;
	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> u;
		v.push_back(u);
	}
	cout <<"C���� ������ ����: "<< earlyDate(v) << endl;
	
	system("pause");
}