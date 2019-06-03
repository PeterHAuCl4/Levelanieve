#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <math.h>

QString sX , sY;
double lessX = 0, lessY = 0 , maxX = 0, maxY = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    MainWindow::makePlot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Solver_clicked()
{
    bool ok;
    int i = 0 , lim = 0;
    double faoo = ui->dataFa0->text().toDouble(&ok);
    QVector<double> todo, x , y , y1;
    QString archivo;
    QVector<QString> fagocitos , Y;
    QFile file(sX) , fileY(sY);

    if(!file.open(QIODevice::ReadOnly))
        QMessageBox::information(0,"Info",file.errorString());
    QTextStream in(&file);

    while(!in.atEnd()){
        archivo = in.readLine();
        fagocitos += archivo;
    }
    x = MainWindow::toFloatVector(fagocitos);

    if(!fileY.open(QIODevice::ReadOnly))
        QMessageBox::information(0,"Info",file.errorString());
     QTextStream inY(&fileY);
    while(!inY.atEnd()){
        archivo = inY.readLine();
       Y += archivo;
    }
    y = MainWindow::toFloatVector(Y);
    lim = x.length();


    for(i = 0; i < lim; ++i){
        y[i] = faoo/y[i];
    }
    updateValues(lim, x, y);

    // create graph and assign data to it:
    QCPGraph *graph2 = ui->customPlot->addGraph();
    graph2->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 9));
    graph2->setPen(QPen(QColor(0,191,17)));
    ui->customPlot->graph(0)->setData(x, y);
//    QCPGraph *graph1 = ui->customPlot->addGraph();
//    graph1->setData(x, y1);
//    graph1->setPen(QPen(QColor(255, 243, 0)));
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
    ui->customPlot->xAxis->setBasePen(QPen(Qt::white, 1));
    ui->customPlot->yAxis->setBasePen(QPen(Qt::white, 1));
    ui->customPlot->xAxis->setTickPen(QPen(Qt::white, 1));
    ui->customPlot->yAxis->setTickPen(QPen(Qt::white, 1));
    ui->customPlot->xAxis->setSubTickPen(QPen(Qt::white, 1));
    ui->customPlot->yAxis->setSubTickPen(QPen(Qt::white, 1));
    ui->customPlot->xAxis->setTickLabelColor(Qt::white);
    ui->customPlot->yAxis->setTickLabelColor(Qt::white);
    ui->customPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    ui->customPlot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    ui->customPlot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    ui->customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    ui->customPlot->xAxis->grid()->setSubGridVisible(true);
    ui->customPlot->yAxis->grid()->setSubGridVisible(true);
    ui->customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    ui->customPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    ui->customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    ui->customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    ui->customPlot->setBackground(plotGradient);
    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
    ui->customPlot->axisRect()->setBackground(axisRectGradient);
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(lessX, maxX);
    ui->customPlot->yAxis->setRange(lessY, maxY);
    ui->customPlot->replot();
}
void MainWindow::on_Ecu_clicked(){

    bool ok;
       int n = ui->Grado->text().toInt(&ok,10) , i = 0 , j = 0;
       double faoo = ui->dataFa0->text().toDouble(&ok) , aux = 0 , sum_x = 0, sum_xy = 0;
       QVector<double> x , y , y1, solucion(0), x_vector(0);
       QString archivo;
       QVector<QString> fagocitos , Y;
       QFile file(sX) , fileY(sY);

       if(!file.open(QIODevice::ReadOnly))
          QMessageBox::information(0,"Info",file.errorString());
       QTextStream in(&file);

       while(!in.atEnd()){
           archivo = in.readLine();
           fagocitos += archivo;
       }
       x = MainWindow::toFloatVector(fagocitos);

       if(!fileY.open(QIODevice::ReadOnly))
           QMessageBox::information(0,"Info",file.errorString());
        QTextStream inY(&fileY);
       while(!inY.atEnd()){
           archivo = inY.readLine();
          Y += archivo;
       }
       y = MainWindow::toFloatVector(Y);
       int lim = x.length();

       for(i = 0; i < lim; ++i){
           y[i] = faoo/y[i];
       }
   /* ================= INICIA SOLUCIÓN ========================*/
       int tamano = n+1;
   QVector<QVector<double>> ecuaciones(0);

   for(int i = 0; i < tamano; i++){
       QVector<double> tamal;
       for(int j = 0; j < tamano; j++)
           tamal << 0;
       ecuaciones.push_back(tamal);
       x_vector << 0;
   }

   for (int i = 0; i < tamano; i++) {
       sum_xy = 0;

       for (int j = 0; j < lim; j++)
       sum_xy += pow(x[j], i)*y[j];
       solucion << sum_xy;

       for (int j = 0; j < tamano; j++) {
       sum_x = 0;
       if (i == 0 && j == 0)
       ecuaciones[i][j] = lim;
       else {
       for (int h = 0; h < lim; h++)
       sum_x += pow(x[h], (j + i));
       ecuaciones[i][j] = sum_x;
       }
       }
       }
       //Resolucion de sistemas de ecuaciones.
       solucion = eliminacionGauss(ecuaciones, solucion, tamano);

       x_vector = sustitucionAtras(ecuaciones, solucion, tamano, x_vector);

       for(i = 0; i < lim; ++i){
           for(j = 0; j < n; ++j){
               if(j != 0){
               aux+=x[i]*pow(x_vector[j],j);
               }else{
               aux+=x_vector[j];
               }
           }
           y1 << aux;
           aux = 0;
       }
        // create graph and assign data to it:
        QCPGraph *graph1 = ui->customPlot->addGraph();
        graph1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 9));
        graph1->setPen(QPen(QColor(200,240,17)));
        ui->customPlot->graph(1)->setData(x, y1);
        // give the axes some labels:
        ui->customPlot->xAxis->setLabel("x");
        ui->customPlot->yAxis->setLabel("y");
        ui->customPlot->xAxis->setBasePen(QPen(Qt::white, 1));
        ui->customPlot->yAxis->setBasePen(QPen(Qt::white, 1));
        ui->customPlot->xAxis->setTickPen(QPen(Qt::white, 1));
        ui->customPlot->yAxis->setTickPen(QPen(Qt::white, 1));
        ui->customPlot->xAxis->setSubTickPen(QPen(Qt::white, 1));
        ui->customPlot->yAxis->setSubTickPen(QPen(Qt::white, 1));
        ui->customPlot->xAxis->setTickLabelColor(Qt::white);
        ui->customPlot->yAxis->setTickLabelColor(Qt::white);
        ui->customPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
        ui->customPlot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
        ui->customPlot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
        ui->customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
        ui->customPlot->xAxis->grid()->setSubGridVisible(true);
        ui->customPlot->yAxis->grid()->setSubGridVisible(true);
        ui->customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
        ui->customPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
        ui->customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
        ui->customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
        QLinearGradient plotGradient;
        plotGradient.setStart(0, 0);
        plotGradient.setFinalStop(0, 350);
        plotGradient.setColorAt(0, QColor(80, 80, 80));
        plotGradient.setColorAt(1, QColor(50, 50, 50));
        ui->customPlot->setBackground(plotGradient);
        QLinearGradient axisRectGradient;
        axisRectGradient.setStart(0, 0);
        axisRectGradient.setFinalStop(0, 350);
        axisRectGradient.setColorAt(0, QColor(80, 80, 80));
        axisRectGradient.setColorAt(1, QColor(30, 30, 30));
        ui->customPlot->axisRect()->setBackground(axisRectGradient);
        // set axes ranges, so we see all data:
        ui->customPlot->xAxis->setRange(lessX, maxX);
        ui->customPlot->yAxis->setRange(lessY, maxY);
        ui->customPlot->replot();
}
QVector<double> MainWindow::sustitucionAtras(QVector<QVector<double>> A, QVector<double> B, int n, QVector<double> C) {
 double suma;
 C[n - 1] = B[n - 1] / A[n - 1][n - 1];
 for (int i = n - 2; i >= 0; i--) {
 suma = 0;
 for (int j = i + 1; j < n; j++) {
 suma = suma + A[i][j] * C[j];
 }
 C[i] = (B[i] - suma) / A[i][i];
 }
 return C;
}
QVector<double> MainWindow::eliminacionGauss(QVector<QVector<double>> A, QVector<double> B, int n) {
 double inv;
 for (int k = 0; k < n; k++) {
 for (int i = k + 1; i < n; i++) {
 inv = A[i][k] / A[k][k];
 for (int j = k; j < n; j++) {
 A[i][j] = A[i][j] - inv*A[k][j];
 }
 B[i] = B[i] - inv*B[k];
 }
 }
 return B;
}
QVector<double> MainWindow::toFloatVector(QVector<QString> &aVector){
    QVector<double> vector;

    for(const auto& item : aVector){
        bool ok = true;
        const double value = item.toDouble(&ok);
        if(ok)
            vector << value;
    }
    return vector;
}
void MainWindow::on_inDataY_clicked()
{
    sY =
            QFileDialog::getOpenFileName(this, "Open a file", "directoryToOpen",
                "Text files (*.txt);;Images (*.png *.xpm *.jpg);;XML files (*.xml)");
}
void MainWindow::on_inDataX_clicked()
{
    sX =
            QFileDialog::getOpenFileName(this, "Open a file", "directoryToOpen",
                "Text files (*.txt);;Images (*.png *.xpm *.jpg);;XML files (*.xml)");
}
void MainWindow::updateValues(int lim, QVector<double> x , QVector<double> y){

    double auxLess = 100 , auxMax = 0 , val = 0;

    for(int i = 0; i < lim; ++i){
    val = x[i];
    if(val > auxMax){
        maxX = val;
        auxMax = maxX;
    }
    if(val < auxLess){
        lessX = val;
        auxLess = lessX;
    }
    }
    //Inicia para Y
    auxLess = 100; auxMax = 0;
    for(int i = 0; i < lim; ++i){
    val = y[i];
    if(val > auxMax){
        maxY = val;
        auxMax = maxY;
    }
    if(val < auxLess){
        lessY = val;
        auxLess = lessY;
    }
    }
}
/*
{
    bool ok;
    int n = ui->Grado->text().toInt(&ok,10), k = 0, i = 0, j = 0;
    double faoo = ui->dataFa0->text().toDouble(&ok) , producto = 0 , aux = 0;
    QVector<double> x , y , y1 , L , D , P , xp , E;
    QString archivo;
    QVector<QString> fagocitos , Y;
    QFile file(sX) , fileY(sY);

    for(i  = 0; i < n; ++i){
    L << 0;
    P << 0;
    xp << 0;
    D << 0;
    E << 0;
        if(i == n-1){
        D << 0;
        }
    }
    if(!file.open(QIODevice::ReadOnly))
       QMessageBox::information(0,"Info",file.errorString());
    QTextStream in(&file);

    while(!in.atEnd()){
        archivo = in.readLine();
        fagocitos += archivo;
    }
    x = MainWindow::toFloatVector(fagocitos);

    if(!fileY.open(QIODevice::ReadOnly))
        QMessageBox::information(0,"Info",file.errorString());
     QTextStream inY(&fileY);
    while(!inY.atEnd()){
        archivo = inY.readLine();
       Y += archivo;
    }
    y = MainWindow::toFloatVector(Y);
    int lim = x.length();

    for(i = 0; i < lim; ++i){
        y[i] = faoo/y[i];
    }

    for (k = 0; k < n; k++){
    producto = 1;
        for (int i = 0; i < n; i++)
            if (i != k) producto = producto * (x[k] - x[i]);
    L[k] = y[k]/producto;
    }
    for (k = 0; k < n; k++){
        j = 0;
        for (i = 0; i < n; i++){
            if (i != k) {xp[j] = x[i]; j += 1;}
        }
         D = monomio2(n,xp,D);
        for (i= 0; i < n; i++)
            P[i] = P[i] + L[k]*D[i];
    }

    for(i = 0; i < lim; ++i){
        for(j = n-1; j >= 0; --j){
            if(j != n-1){
            aux+=x[i]*pow(P[j],n-j-1);
            }else{
            aux+=P[j];
            }
        }
        y1 << aux;
        aux = 0;
    }
        // create graph and assign data to it:
        QCPGraph *graph1 = ui->customPlot->addGraph();
        graph1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 9));
        graph1->setPen(QPen(QColor(200,240,17)));
        ui->customPlot->graph(1)->setData(x, y1);
        // give the axes some labels:
        ui->customPlot->xAxis->setLabel("x");
        ui->customPlot->yAxis->setLabel("y");
        ui->customPlot->xAxis->setBasePen(QPen(Qt::white, 1));
        ui->customPlot->yAxis->setBasePen(QPen(Qt::white, 1));
        ui->customPlot->xAxis->setTickPen(QPen(Qt::white, 1));
        ui->customPlot->yAxis->setTickPen(QPen(Qt::white, 1));
        ui->customPlot->xAxis->setSubTickPen(QPen(Qt::white, 1));
        ui->customPlot->yAxis->setSubTickPen(QPen(Qt::white, 1));
        ui->customPlot->xAxis->setTickLabelColor(Qt::white);
        ui->customPlot->yAxis->setTickLabelColor(Qt::white);
        ui->customPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
        ui->customPlot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
        ui->customPlot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
        ui->customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
        ui->customPlot->xAxis->grid()->setSubGridVisible(true);
        ui->customPlot->yAxis->grid()->setSubGridVisible(true);
        ui->customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
        ui->customPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
        ui->customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
        ui->customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
        QLinearGradient plotGradient;
        plotGradient.setStart(0, 0);
        plotGradient.setFinalStop(0, 350);
        plotGradient.setColorAt(0, QColor(80, 80, 80));
        plotGradient.setColorAt(1, QColor(50, 50, 50));
        ui->customPlot->setBackground(plotGradient);
        QLinearGradient axisRectGradient;
        axisRectGradient.setStart(0, 0);
        axisRectGradient.setFinalStop(0, 350);
        axisRectGradient.setColorAt(0, QColor(80, 80, 80));
        axisRectGradient.setColorAt(1, QColor(30, 30, 30));
        ui->customPlot->axisRect()->setBackground(axisRectGradient);
        // set axes ranges, so we see all data:
        ui->customPlot->xAxis->setRange(lessX, maxX);
        ui->customPlot->yAxis->setRange(lessY, maxY);
        ui->customPlot->replot();

}
QVector<double> MainWindow::monomio2(int n, QVector<double>x, QVector<double> D){

    QVector<double> E;
    for(int i = 0; i < n; i++)
        E << 0;
    D[0] = 1;
    D[1] = -x[0];
    for (int i = 1; i < n; i++) {
        for (int k =1; k < i+1; k++)
            E[k] = D[k] + D[k-1]*(-x[i]);
        D[i+1] = D[i]*(-x[i]);

        for (int j = 1; j < i+1; j++)
            D[j] = E[j];
    }
    return D;
}

*/
