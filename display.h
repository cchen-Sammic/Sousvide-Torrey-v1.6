#ifndef DISPLAY_H
#define DISPLAY_H


#include <QMainWindow>

namespace Ui {
class display;
}

class display : public QMainWindow
{
    Q_OBJECT

public:
    explicit display(QWidget *parent = 0);
    ~display();

    void menuSet(int menu);
    void menuTimer();
    void menuTemperature();
    void menuCalentando();
    void tempLed(bool state);
    void realtimeDataSlot();
    void menuCoccion_inf();
    void menuCoccion_cont();

    char gradoTemp;

    bool button_start;

    bool estado_calentando;
    bool estado_timerOn;
    bool estado_pause;
    bool estado_setTemp;
    bool estado_setTimer;
    bool estado_aviso;
    bool estado_fin;
    bool estado_temperaturaAlcanzado;
    bool estado_avisoSacarComida;
    bool estado_avisoAgua;

    bool flag_meterComida;
    bool flag_timerTemp;
    bool flag_powerOn;
    bool flag_gradoTemp;
    bool flag_parpadeo;
    bool flag_parpadeo2;
    bool flag_pipipi;
    bool flag_avisoAgua;
    bool flag_tempInf;




    int button_set;
    int temp0;
    int tempSet;
    int timerMinSet;
    int timerHoraSet;
    int timeMin;
    int timeHora;

    QString timerSet;
    QString timerCount;



private:
    Ui::display *ui;


private Q_SLOTS:
    void buttonSetClicked();
    void buttonMenosClicked();
    void buttonMasClicked();
    void buttonStartClicked();
    void buttonPowerClicked();
    void buttonTemperaturaCliceck();
    void buttonAguaClicked();
    void updateTime();
    void updateTime2();
    void updateTime3();
    void updateTime4();
};


#endif // DISPLAY_H
