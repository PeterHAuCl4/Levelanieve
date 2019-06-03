#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Solver_clicked(); //
    void on_inData_clicked(); //
    QVector<double> toFloatVector(QVector<QString> &aVector); //
    void on_inDataY_clicked(); //
    void on_inDataX_clicked();
    void updateValues(int, QVector<double>, QVector<double>);
    void on_Ecu_clicked();
    QVector<double> eliminacionGauss(QVector<QVector<double>> A, QVector<double> B, int n);
    QVector<QVector<double>> eliminacionGauss2(QVector<QVector<double>> A, QVector<double> B, int n);
    QVector<double> sustitucionAtras(QVector<QVector<double>> A, QVector<double> B, int n, QVector<double> C);

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
