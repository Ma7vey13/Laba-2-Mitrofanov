#include <stdio.h>
#include <iomanip>
#include <iostream>
#include <stdlib.h>

using namespace std;
const int Vmax = 20;
const int Emax = Vmax * (Vmax - 1) / 2;
int MatrixW[Vmax][Vmax];
int Weight[Emax], node[Vmax], view[Vmax], nextV[Emax], Mas[Emax];
int k = 0, Ecount = 0, Wcount = 0;
bool NextCount = false; // есть ли следующая вершина
int MasEdge[] = { 0, 0, 0, 0, 0, 0, 0 }; // количество овтетвлений от каждой вершины
int MasEdCount = 0;                      // счетчик овтетвлений

/*struct Vertex
{
public:
	char name;
} V;*/

class Graph {
public:
	char *V = new char[Vmax];
	int n = 0;

	void Add_V(char Name) {
		if (n < Vmax) {
			V[n] = Name;
			n++;
		}
		else if (n > Vmax){
			cout << "\n Вершину нельзя добавить";
		}
	}

	void Add_E(char Name1, char Name2, int weight, int Num1, int Num2) {
		MatrixW[VertexIndex(Name1)][VertexIndex(Name2)] = weight;
		Ecount++;
		MasEdge[Num1]++;
		k = k + 1;
		Mas[k] = Num2;
		Weight[k] = weight;
		//если вершина Name1 новая, то первая смежная ей вершина имеет номер k
		if (node[Num1] == 0) node[Num1] = k;
		//если вершина Name1 уже просматривалась, то следующая смежная с ней вершина имеет номер больший номер k
		if (view[Num1] != 0) nextV[view[Num1]] = k;
		view[Num1] = k;
	}

	void Dell_E(char Name1, char Name2) {
		MatrixW[VertexIndex(Name1)][VertexIndex(Name2)] = 0;
	}

	int VertexIndex(char Name) { // Индекс вершины
		for (int i = 0; i < n; i++) {
			if (V[i] == Name) return i;
		}
	}

	int VertexNumber(char Name) { // Номер вершины с единицы
		for (int i = 0; i < n; i++) {
			if (VertexIndex(Name) == i) return i+1;
		}
	}

	char VertexNameNumber(int i) { // Возвращает имя вершины по ее номеру
		return V[i-1];
	}

	/*
	int First(char Name) {
		for (int i = 0; i < n; i++) {
			if (MatrixW[VertexIndex(Name)][i]) return i+1;
			if (i == n - 1) return 0;
		}
	}*/
	int First(int IND) {
		for (int i = 0; i < n; i++) {
			if (MatrixW[IND][i] != 0) return i;
			if (i == n - 1) return -1;
		}
	}
	/*
	int Next(char Name, int I) { // подаем индекс, который идет после индекса из First
		for (int i = I; i < n; i++) {
			if (MatrixW[VertexIndex(Name)][i] != 0) return i+1;
			if (i == n - 1) return 0;
		}
	}*/
	int Next(int IND, int I) { // подаем индекс, который идет после индекса из First
		if (NextCount) return -1;
		if (I >= n) return -1;
		for (int i = I; i < n; i++) {
			if (MatrixW[IND][i] != 0) return i;
			if (i == n - 1) return -1;
		}
	}

	void PrintMW() {
		cout << "Дана матрица весов дуг:" << endl;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				cout << MatrixW[i][j] << setw(4);
			}
			cout << '\n';
		}
		cout << '\n';
	}

	void PrintSD() {
		cout << "Список дуг:" << endl << "Вер1  " << "Вер2  " << "Вес" << endl;
		for (int i = 0; i < Ecount; i++)
		{
			k = node[i];
			while (k > 0)
			{
				cout << VertexNameNumber(i) << setw(7) << VertexNameNumber(Mas[k]) << setw(7) << Weight[k] << endl;
				k = nextV[k];
			}
		}
	}

	int Ways(int lenght) {
		int m = 0;
		int temp = 0;                    // текущая вершина
		int count = 0;                   // счетчик дуг
		int e = 0;                       // был ли уже путь от одной и той же вершины
		bool checkParent = false;        // обнулен ли массив "родителей"
		bool miss = false;               // до конца ли посчиталось количество дуг
		int sumW = 0;                    // сумма путей
		int* ParentInd = new int[Vmax];  // массив родителей вершин
		for (int j = 0; j < n; j++) {
			temp = j;
			count = 0;
			m = 0;
			miss = false;
			Wcount = 0;
			MasEdCount = 0;                                    // запоминает количество ответвлений от вершины
		A:
			e = 0;
			for (int i = m; i < lenght; i++) {                 // Проверка первой вершины
				if (First(temp) != -1) {
					if (count == 0 && temp == j) MasEdCount++;
					Wcount++;
					count++;
					if (checkParent) ParentInd[i + 1] = temp;  // если массив родителей обнулился
					else ParentInd[i] = temp;
					temp = First(temp);
					if (temp == j) { Wcount = 0; goto C; }
					if (count == lenght) e++;
					for (int i = 0; i < count; i++) {
						if (ParentInd[i] == temp && lenght >= 4) {
							NextCount = true; goto C;
						}
					}
					if (Wcount == lenght) {
						sumW++, Wcount = 0;
						break;
					}
				}
				else Wcount = 0;
			}
			checkParent = false;

			while (count != -1) {                              // Проверка следующих вершин
				if (count == 0 && temp == j) {                 // если путь вернулся в начальную вершину
					for (int i = 0; i < n; i++) ParentInd[i] = 0;
					ParentInd[0] = temp;
					checkParent = true;
					MasEdCount++;
					NextCount = false;
					e = 0;
					if (MasEdCount > MasEdge[j + 1]) break;
					goto B;
				}
				if (temp == j || count == lenght) goto C;
			B:
				if (Next(temp, First(temp) + 1) != -1 && count < lenght) { // если существует следующая вершина
					m = lenght - count;
					if (count == 0 && j == temp) m = count;
					temp = Next(ParentInd[count], First(ParentInd[count]) + 1);
					count++;
					for (int i = 0; i < count; i++) {
						if (ParentInd[i] == temp) {
							NextCount = true; goto C;
						}
					}
					Wcount = count;
					if (miss && e == 0) NextCount = true;
					if (e >= 1) { NextCount = true; e++; }
					if (temp == j) { Wcount = 0; goto C; }
					if (Wcount == lenght) {
						sumW++;
						Wcount = 0; goto C;
					}
					else miss = true;
					goto A;
				}
			C:
				count--;
				temp = ParentInd[count];
			}
			NextCount = false;
			checkParent = false;
		}
		return sumW;
	}
};

int main()
{
	setlocale(LC_ALL, "rus");
	Graph gr;
	int x;
	int y;
	
	cout << "Граф №";
	cin >> y;
	
	if (y == 1) {
		char ver[6] = { 'A', 'B', 'C', 'D', 'E', 'F' };
		for (int i = 0; i < 6; i++) {
			gr.Add_V(ver[i]);
		}
		gr.Add_E('A', 'B', 6, gr.VertexNumber('A'), gr.VertexNumber('B'));
		gr.Add_E('A', 'C', 7, gr.VertexNumber('A'), gr.VertexNumber('C'));
		gr.Add_E('B', 'D', 9, gr.VertexNumber('B'), gr.VertexNumber('D'));
		gr.Add_E('C', 'E', 10, gr.VertexNumber('C'), gr.VertexNumber('E'));
		gr.Add_E('D', 'F', 11, gr.VertexNumber('D'), gr.VertexNumber('F'));
		gr.Add_E('D', 'C', 4, gr.VertexNumber('D'), gr.VertexNumber('C'));
		gr.Add_E('E', 'F', 3, gr.VertexNumber('E'), gr.VertexNumber('F'));
	}

	else if (y == 2) {
		char ver[5] = { 'A', 'B', 'C', 'D', 'E' };
		for (int i = 0; i < 5; i++) {
			gr.Add_V(ver[i]);
		}
		gr.Add_E('A', 'B', 6, gr.VertexNumber('A'), gr.VertexNumber('B'));
		gr.Add_E('A', 'C', 7, gr.VertexNumber('A'), gr.VertexNumber('C'));
		gr.Add_E('B', 'C', 5, gr.VertexNumber('B'), gr.VertexNumber('C'));
		gr.Add_E('C', 'D', 4, gr.VertexNumber('C'), gr.VertexNumber('D'));
		gr.Add_E('D', 'A', 9, gr.VertexNumber('D'), gr.VertexNumber('A'));
		gr.Add_E('E', 'B', 11, gr.VertexNumber('E'), gr.VertexNumber('B'));
		gr.Add_E('E', 'D', 3, gr.VertexNumber('E'), gr.VertexNumber('D'));
	}
	else {
		cout << "Ошибка ввода";
		return 0;
	}

	gr.PrintMW();
	gr.PrintSD();
	cout << endl << "Введите длину пути: ";
	cin >> x;
	cout << endl << "Найдено незамкнутых путей: " << gr.Ways(x) << endl;

	delete[] gr.V;
}
