#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <math.h>

QString archivoX , archivoY;
double lesarchivoX = 0, lesarchivoY = 0 , maxX = 0, maxY = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QCPGraph *graph2 = ui->customPlot->addGraph();
    graph2->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(rand()%20+1)));
    graph2->setPen(QPen(QColor(80,250,123)));
    // create graph and assign data to it:
    QCPGraph *graph1 = ui->customPlot->addGraph();
    graph1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(QColor(139,233,253), 1.5), QBrush(Qt::black), 5));
    graph1->setPen(QPen(QColor(200,240,17)));

    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("Fa0/ra");
    ui->customPlot->xAxis->setBasePen(QPen(QColor(255,85,85)));
    ui->customPlot->yAxis->setBasePen(QPen(QColor(255,85,85)));
    ui->customPlot->xAxis->setTickPen(QPen(QColor(255,85,85)));
    ui->customPlot->yAxis->setTickPen(QPen(QColor(255,85,85)));
    ui->customPlot->xAxis->setSubTickPen(QPen(QColor(255,85,85)));
    ui->customPlot->yAxis->setSubTickPen(QPen(QColor(255,85,85)));
    ui->customPlot->xAxis->setTickLabelColor(QColor(255,85,85));
    ui->customPlot->yAxis->setTickLabelColor(QColor(255,85,85));
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
    plotGradient.setColorAt(0, QColor(60, 65, 90));
    plotGradient.setColorAt(1, QColor(60, 80, 90));
    ui->customPlot->setBackground(plotGradient);
    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(58, 71, 90));
    axisRectGradient.setColorAt(1, QColor(68, 71, 90));
    ui->customPlot->axisRect()->setBackground(axisRectGradient);
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
    QVector<double> ever, x , y , extra;
    QString archivo;
    QVector<QString> Complete , Y;
    QFile file(archivoX) , fileY(archivoY);

    if(!file.open(QIODevice::ReadOnly))
        QMessageBox::information(0,"Info",file.errorString());
    QTextStream in(&file);

    while(!in.atEnd()){
        archivo = in.readLine();
        Complete += archivo;
    }
    x = MainWindow::toFloatVector(Complete);

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
    ui->customPlot->graph(0)->setData(x, y);

    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(lesarchivoX, maxX);
    ui->customPlot->yAxis->setRange(lesarchivoY, maxY);
    ui->customPlot->replot();
}
void MainWindow::on_Ecu_clicked(){

    bool ok;
       int n = ui->Grado->text().toInt(&ok,10) , i = 0 , j = 0;
       double faoo = ui->dataFa0->text().toDouble(&ok) , aux = 0 , sum_x = 0, sum_xy = 0 , inv , suma;
       QVector<double> x , y , extra, solucion(0), x_vector(0);
       QString archivo;
       QVector<QString> Complete , Y;
       QFile file(archivoX) , fileY(archivoY);

       if(!file.open(QIODevice::ReadOnly))
          QMessageBox::information(0,"Info",file.errorString());
       QTextStream in(&file);

       while(!in.atEnd()){
           archivo = in.readLine();
           Complete += archivo;
       }
       x = MainWindow::toFloatVector(Complete);

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

//Sustitución hacia atrás
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
           extra << aux;
           aux = 0;
       }
        ui->customPlot->graph(1)->setData(x, extra);
        // give the axes some labels:
            // set axes ranges, so we see all data:
        ui->customPlot->xAxis->setRange(lesarchivoX, maxX);
        ui->customPlot->yAxis->setRange(lesarchivoY, maxY);
        ui->customPlot->replot();
        QString printt = "La ecuación del polinomio es f(x) = ";
        for (int i = 0; i < tamano; i++) {
        printt+= QString::number(x_vector[i]);
        if (i != 0) {
        printt+= "x^";
        printt+= QString::number(i);
        }
        if (i != tamano - 1) {
        printt+= " + ";
        }
        }
        ui->output->setText(printt);
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
    archivoY =
            QFileDialog::getOpenFileName(this, "Open a file", "directoryToOpen",
                "Text files (*.txt);;Images (*.png *.xpm *.jpg);;XML files (*.xml)");
}
void MainWindow::on_inDataX_clicked()
{
    archivoX =
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
        lesarchivoX = val;
        auxLess = lesarchivoX;
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
        lesarchivoY = val;
        auxLess = lesarchivoY;
    }
    }
}
