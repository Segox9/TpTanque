#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
/* Para valores muy chicos de caudal, de entrada o salida, y porcentaje chico en los QDial, la simulacion no avanza,
 *  por eso puse los valores de caudal como floats, aun asi puede ser que en valores chicos no simule bien y
 *  no aumente o decrezca la barra de progreso
*/
//Mio
public:
    //Puntero Timer para actualizar le tiempo de la simulacion
    QTimer Timer;
    //Caudal que ingresará al tanque
    float caudalPorSegundoEntrada=0.0;
    //Caudal que saldrá del tanque
    float caudalPorSegundoSalida=0.0;
    //Funcion que devuelve el tiempo de simulacion en milisegundos, del Line Edit Timeout
    int tiempoSimulacionMs();
public slots:
    //Slot para definir la capacidad maxima del tanque (ProgressBar->maximum())
    void capacidadDefine();
    /*Slots para vaciar y llenar los tanques, en estos se convierte el caudal por segundo de los Line Edit,
     * y se calcula cuanto se deberia llenar teniendo en cuenta el tiempo de simulacion y el porcentaje del QDial*/
    void llenarTanque();
    void vaciarTanque();
    /*Slot para actualizar el estado del tanque, se calcula cuanta es la variacion entre la entrada y salida, y se actualiza
     * el value() de la ProgressBar, si este se llena se detiene, y si se vacia tambien se detiene, esperando algun cambio en el QDial,
     * y los Line Edit de entrada y salida (Qin y Qout)
    */
    void actualizarTanque();
    //Slot para reinciar la simulacion, pone el tanque en un valor de cero y detiene el Timer
    void reset();
    //Slots para mostrar en los LCD el porcentaje de cada QDial
    void porcentajeKIN();
    void porcentajeKOUT();
};
#endif // MAINWINDOW_H
