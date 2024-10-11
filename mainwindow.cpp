#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QString>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//Mio
    connect(ui->Capacidad, SIGNAL(editingFinished()), this, SLOT(capacidadDefine()));
    connect(ui->Reset, SIGNAL(clicked()), this, SLOT(reset()));
    connect(&Timer, SIGNAL(timeout()), this, SLOT(actualizarTanque()));
    connect(ui->Kin, SIGNAL(valueChanged(int)), this, SLOT(llenarTanque()));
    connect(ui->Kout, SIGNAL(valueChanged(int)), this, SLOT(vaciarTanque()));
    connect(ui->Kin, SIGNAL(valueChanged(int)), this, SLOT(porcentajeKIN()));
    connect(ui->Kout, SIGNAL(valueChanged(int)), this, SLOT(porcentajeKOUT()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
//Mio
void MainWindow::reset(){
    ui->Tanque->setValue(0);
    Timer.stop();
}

int MainWindow::tiempoSimulacionMs(){
    QString textTimeout;
    textTimeout = ui->Timeout->text();
    int timeMs = textTimeout.toInt();

    return timeMs;
}

void MainWindow::capacidadDefine(){
    QString textCapacidad;
    textCapacidad = ui->Capacidad->text();
    float valCapacidad = textCapacidad.toFloat();
    ui->Tanque->setMaximum(valCapacidad);
    float max = ui->Tanque->maximum();
    qDebug() << "Nueva capacidad maxima" << max;
}

void MainWindow::llenarTanque() {
    int valQin = ui->Kin->value();  // Valor del dial (0 - 100%)
    QString textEntrada = ui->Qin->text();
    float caudalEntrada = textEntrada.toFloat(); // Caudal en litros por segundo

    // Ajustar el caudal por el porcentaje del dial (Qin)
    caudalPorSegundoEntrada = (caudalEntrada * valQin)/100.0;

    int timeout= tiempoSimulacionMs();
    if(timeout < 100){
        timeout=100;
    }
    caudalPorSegundoEntrada = (caudalPorSegundoEntrada * timeout)/1000.0;
    // Iniciar el temporizador si no está lleno el tanque
    if (ui->Tanque->value() < ui->Tanque->maximum()) {
        Timer.start(timeout); // Intervalo de actualización de la simulacion
    } else {
        Timer.stop(); // Detener si el tanque está lleno
    }
}
void MainWindow::vaciarTanque(){
    int valQout = ui->Kout->value();
    QString textSalida = ui->Qout->text();
    float caudalSalida = textSalida.toFloat();

    caudalPorSegundoSalida = (caudalSalida * valQout ) / 100.0;
    int timeout = tiempoSimulacionMs();
    //Para evitar tiempos cortos de simulacion
    if(timeout < 100){
        timeout=100;
    }

    caudalPorSegundoSalida= (caudalPorSegundoSalida * timeout)/1000.0;
    //Se vacia el tanque si esta lleno
    if (ui->Tanque->value() > 0) {
        Timer.start(timeout); // Intervalo de actualización de la simulacion
    } else {
        Timer.stop(); // Detener si el tanque está vacio
    }
}
void MainWindow::actualizarTanque() {
    float valorActual = ui->Tanque->value();
    float variacionCaudal = caudalPorSegundoEntrada - caudalPorSegundoSalida;

    // Calcula el nuevo valor del tanque
    float nuevoValor = valorActual + variacionCaudal;

    // Evita que el valor sea negativo
    if (nuevoValor < 0) {
        nuevoValor = 0;
    }

    // Evita sobrepasar el máximo del tanque
    if (nuevoValor > ui->Tanque->maximum()) {
        nuevoValor = ui->Tanque->maximum();
    }

    // Actualizar el valor del tanque
    ui->Tanque->setValue(nuevoValor);
    qDebug() << "Llenando..." << nuevoValor << "L de " << ui->Tanque->maximum() << "L";

    // Si el tanque ya está lleno, se detiene la simulación
    if (nuevoValor >= ui->Tanque->maximum()) {
        Timer.stop();
        qDebug() << "El tanque se llenó";
        return;
    }

    // Si el tanque está vacío, se detiene la simulación
    if (nuevoValor <= 0) {
        Timer.stop();
        qDebug() << "El tanque se vació";
        return;
    }
}

void MainWindow::porcentajeKIN(){
    int porcentaje = ui->Kin->value();
    ui->PorcentajeKin->display(porcentaje);
}
void MainWindow::porcentajeKOUT(){
    int porcentaje = ui->Kout->value();
    ui->PorcentajeKout->display(porcentaje);
}
