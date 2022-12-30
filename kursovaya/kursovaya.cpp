#include "pch.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <fstream>
using namespace std;  

#define INF 99999
ofstream fout("answer.txt");

void toString(int** Matrix, int n);

void floyd(int** Matrix, int n)
{
    // dist[][] будет выходной матрицей, которая в конечном итоге будет иметь кратчайшие
    // расстояния между каждой парой вершин

    //матрица предков Next[][], которая для каждой пары вершин будет содержать номер фазы, 
    //на которой было получено кратчайшее расстояние между ними.
    int** Next = new int* [n];
    int** dis = new int* [n];
    for (int i = 0; i < n; i++) {
        Next[i] = new int[n];
        dis[i] = new int[n];
        for (int j = 0; j < n; j++)
        {
            dis[i][j] = Matrix[i][j];
            // Инициализирую матрицу решения так же, как входную матрицу графика.

            // Нет ребра между узлами
            // i и j
            if (Matrix[i][j] == INF)
                Next[i][j] = -1;
            else
                Next[i][j] = j;
        } 
    }

    // алгоритм Флойда
    // если мы найдем что dis[i][j] > dis[i][k] + dis[k][j]
    // затем модифицируем next[i][j] = next[i][k]

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
               
                if (dis[i][j] > dis[i][k] + dis[k][j]) {
                    dis[i][j] = dis[i][k] + dis[k][j];
                    
                    Next[i][j] = Next[i][k];
                    // в конце цикла предпоследняя вершина в кратчайшем  маршруте из вершины i в j
                }
            }
        }
    }

    //востановление пути
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int u = i, v = j;       // Если нет пути между
            if (Next[u][v] == -1)   // узлами u и v, просто возврат
                continue;           // пустой массив

            // Сохранение пути в векторе
            vector<int> path = { u };
            while (u != v) { // аналогичен (тому, что выше)
                u = Next[u][v];
                path.push_back(u); // записываем пусть в вектор
            }

            for (int k = 0; k < path.size() - 1; k++) {  // вывод пути
                cout << path[k] << " -> ";
                fout << path[k] << " -> ";
            }
            if (path.size() != 1) { // если путь из одной вершины, то это не имеет смысла
                cout << path[path.size() - 1] << endl;
                fout << path[path.size() - 1] << endl;
            }
        }
    }

    cout << "\nSleduyushchaya matritsa pokazyvaet kratchaishie rasstoyaniya mezhdu kazhdoi paroi vershin \n";
    fout << "\nSleduyushchaya matritsa pokazyvaet kratchaishie rasstoyaniya mezhdu kazhdoi paroi vershin \n";
    toString(dis, n);
}

// функция вывода матрицы
void toString(int** Matrix, int n) {
    for (int i = 0; i < n; i++)
    {
        cout << i << " : ";
        fout << i << " : ";
        for (int j = 0; j < n; j++)
        {
            if (Matrix[i][j] == INF) {
                cout << setw(6) << "INF";
                fout << setw(6) << "INF";
            }
            else {
                cout << setw(6) << Matrix[i][j];
                fout << setw(6) << Matrix[i][j];
            }
        }
        cout << endl;
        fout << endl;
    }
}


int main()
{
    srand(time(NULL));
    setlocale(LC_ALL, "RUS");

    ifstream in("matrix.txt");
    int** Matrix;
    int n;
    while (true) { // пока не выйдем сами

        system("cls");// очистка консоли
        cout << " Vvedite kol-vo vershin grafa: "; cin >> n;

        string s; // используем стринг для защиты от (дурака)
        cout << endl << " Vyberete sposob postroeniya grafa : \n 1) Sluchainoe zapolnenie \n"
            << " 2) Schitat graf iz faila \n 3) Vihod\n Vvedite: "; cin >> s;

        switch (atoi(s.c_str()))
        {
        case 1:
            system("cls");
            Matrix = new int* [n]; ;
            for (int i = 0; i < n; i++) {
                Matrix[i] = new int[n];
                for (int j = 0; j < n; j++)
                {
                    Matrix[i][j] = INF; // создание матрицы смежности( INF-бесконечность )
                }
            }

            int r;
            cout << "Vvedite veroyatnost prisutstviya rebra (chem bolshe, tem vyshe veroyatnost): ";
            cin >> r;
            for (int i = 0; i < n; i++)
            {
                for (int j = i + 1; j < n; j++)
                {
                    if (1 <= (rand() % r)) {
                        int u = rand() % 3;
                        u == 0 ? Matrix[i][j] = 1 + rand() % 50 : u == 1 ? Matrix[j][i] = 1+ rand() % 50 : Matrix[i][j] = Matrix[j][i] = 1 + rand() % 50;
                        // случайное заполнение графа(взвешенного ориентированного)
                        // u - служит для способа добавления в граф значения, будет ли она обоюдной или нет
                    }
                }
            }
            cout << "----------------- \n Matritsa smezhnosti: \n";
            fout << "----------------- \n Matritsa smezhnosti \n";
            toString(Matrix, n); // вывод получившейся матрицы
            cout << "\n-----------------\n Otvet: \n";
            fout << "\n-----------------\n Otvet: \n";

            floyd(Matrix,n);

            for (int i = 0; i < n; i++)
                delete[] Matrix[i];
            delete[] Matrix;
            system("pause");
            break;
        case 2:

            if (in.is_open()) {//Если открытие файла прошло успешно
                string p;
                in >> p;
                n = stoi(p);
                int** Matrix1 = new int* [n]; ;
                for (int i = 0; i < n; i++)
                    Matrix1[i] = new int[n];

                for (int i = 0; i < n; i++)
                    for (int j = 0; j < n; j++) {
                        in >> p; // считывание из файла поэлементно
                        if (p == "INF" || p == "inf" || 0 > stoi(p))
                            Matrix1[i][j] = INF;
                        else
                            Matrix1[i][j] = stoi(p);
                    }


                cout << "----------------- \n Matritsa smezhnosti: \n";
                fout << "----------------- \n Matritsa smezhnosti: \n";
                toString(Matrix1, n); // вывод получившейся матрицы
                cout << "\n-----------------\n Otvet: \n";
                fout << "\n-----------------\n Otvet: \n";

                floyd(Matrix1,n);

                for (int i = 0; i < n; i++)
                    delete[] Matrix1[i];
                delete[] Matrix1;
            }
            system("pause");
            break;
        case 3:
            exit(0);
            break;
        }
    }
    in.close();
    fout.close();
    return 0;
}