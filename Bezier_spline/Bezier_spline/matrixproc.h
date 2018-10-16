#ifndef MATRIXPROC_H
#define MATRIXPROC_H
#include <vector>

//Умножение квадратных матриц
template <class T>
vector < vector < T > > composition(vector < vector < T > > a, vector < vector < T > > b) {
    int n = a.size();
    vector < vector < T > > res;
    res.resize(n);
    for (int i = 0; i < n; i++){
        res[i].resize(n);
        for (int j = 0; j < n; j++)  // Выбираем элемент res
            for (int k = 0; k < n; k++) //суммируем произведения
                res[i][j] += a[i][k] * b[k][j];
    }
    return res;
}
                                    // Матрица на столбец
vector < vector < T > > composition(vector < vector < T > > a, vector < T > b) {
    int n = a.size();
    vector < vector < T > > res;
    res.resize(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            res[i] += a[i][j] * b[j];
    return res;
}
                                // Строка на матрицу
vector < vector < T > > composition(vector < T > a, vector < vector < T > > b) {
    int n = a.size();
    vector < T  > res;
    res.resize(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)  // Выбираем элемент res
                res[i] += a[j] * b[j][i];

    return res;
}

#endif // MATRIXPROC_H
