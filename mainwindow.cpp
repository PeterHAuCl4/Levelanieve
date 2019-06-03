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
       int n = ui->Grado->text().toInt(&ok,10) , i = 0 , j = 0 , p = 0;
       double faoo = ui->dataFa0->text().toDouble(&ok) , aux = 0 , sum_x = 0, sum_xy = 0 , inv , suma;
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
       for(int j = 0; j < tamano; j++){
           tamal << 0; }
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
   for (int k = 0; k < tamano; k++) {
   for (int i = k + 1; i < tamano; i++) {
   inv = ecuaciones[i][k] / ecuaciones[k][k];
   for (int j = k; j < tamano; j++) {
   ecuaciones[i][j] = ecuaciones[i][j] - inv*ecuaciones[k][j];
   }
   solucion[i] = solucion[i] - inv*solucion[k];
   }
   }
   x_vector[tamano - 1] = solucion[tamano - 1] / ecuaciones[tamano - 1][tamano - 1];
   for (int i = tamano - 2; i >= 0; i--) {
   suma = 0;
   for (int j = i + 1; j < tamano; j++) {
   suma = suma + ecuaciones[i][j] * x_vector[j];
   }
   x_vector[i] = (solucion[i] - suma) / ecuaciones[i][i];
   }
   //Obtener nuevos valores
       for(i = 0; i < lim; ++i){
           for(j = 0; j <= n; ++j){
               if(j != 0){
               aux+=x_vector[j]*pow(x[i],j);
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
QVector<QVector<double>> MainWindow::eliminacionGauss2(QVector<QVector<double>> A, QVector<double> B, int n) {
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
 return A;
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
