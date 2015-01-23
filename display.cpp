#include "display.h"
#include "ui_display.h"
#include <QTimer>
#include <QPixmap>
#include <QtGui/QIcon>

display::display(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::display)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/icon.png"));

    QIcon icon(":/icon.ico");
    QMainWindow *window = new QMainWindow();
    window->setWindowIcon(icon);

    estado_calentando = false;
    estado_timerOn = false;
    estado_fin = false;
    estado_avisoSacarComida = false;
    estado_pause = true;
    estado_setTemp = false;
    estado_setTimer = false;
    estado_aviso = false;
    estado_temperaturaAlcanzado = false;
    estado_avisoAgua = false;
    flag_meterComida = false;
    flag_powerOn = false;
    flag_gradoTemp = true;
    flag_timerTemp = false;
    flag_parpadeo = true;
    flag_parpadeo2 = false;
    flag_pipipi = false;
    flag_avisoAgua = false;
    flag_tempInf = false;
    button_set = 0;
    tempSet= 40;
    temp0= 25;
    timerMinSet= 0;
    timerHoraSet= 0;
    gradoTemp = 'C';
    timerSet = "00:00";

    QTimer *timer = new QTimer(this);   // 1000ms
    QTimer *timer2 = new QTimer(this);  // 500ms
    QTimer *timer3 = new QTimer(this); //2000ms
    QTimer *timer4 = new QTimer(this); //350ms

    connect(ui->button_set,SIGNAL(clicked()),SLOT(buttonSetClicked()));
    connect(ui->button_menos,SIGNAL(clicked()),SLOT(buttonMenosClicked()));
    connect(ui->button_mas,SIGNAL(clicked()),SLOT(buttonMasClicked()));
    connect(ui->button_start,SIGNAL(clicked()),SLOT(buttonStartClicked()));
    connect(ui->button_power,SIGNAL(clicked()),SLOT(buttonPowerClicked()));
    connect(ui->button_temperatura,SIGNAL(clicked()),SLOT(buttonTemperaturaCliceck()));
    connect(ui->button_agua, SIGNAL(clicked()),SLOT(buttonAguaClicked()));
    connect(timer,SIGNAL(timeout()),this,SLOT(updateTime()));
    timer->start(1000);
    connect(timer2,SIGNAL(timeout()),this,SLOT(updateTime2()));
    timer2->start(500);
    connect(timer3,SIGNAL(timeout()),this,SLOT(updateTime3()));
    timer3->start(2000);
    connect(timer4,SIGNAL(timeout()),this,SLOT(updateTime4()));
    timer4->start(350);

    ui->lcdNumber->display("");
    ui->llenarAgua->setPixmap(QString(""));



}

display::~display()
{
    delete ui;
}

//**Botón**//
void display::buttonPowerClicked()
{
    flag_powerOn= !flag_powerOn;

    if(flag_powerOn)
    {
        ui->button_power->setIcon(QPixmap(":/switch_on.png"));
        ui->texto->setText("Encendido");
        ui->ledTempC->setPixmap(QString(":/ledC_On.png"));
        ui->ledSet->setPixmap(QString(":/ledSet_Off.png"));
        ui->ledCalor->setPixmap(QString(":/ledCalor_Off.png"));
        ui->ledAviso->setPixmap(QString(":/ledAviso_Off.png"));
        ui->ledTimer->setPixmap(QString(":/ledTimer_Off.png"));
        ui->lcdNumber->display(temp0);
        estado_aviso = false;
        estado_calentando=false;
        estado_fin = false;
        flag_pipipi = false;
    }
    if(!flag_powerOn)
    {
        ui->button_power->setIcon(QPixmap(":/switch_off.png"));
        ui->texto->setText("Apagado");
        ui->ledTempC->setPixmap(QString(":/ledC_Off.png"));
        ui->ledSet->setPixmap(QString(":/ledSet_Off.png"));
        ui->ledCalor->setPixmap(QString(":/ledCalor_Off.png"));
        ui->ledAviso->setPixmap(QString(":/ledAviso_Off.png"));
        ui->ledTimer->setPixmap(QString(":/ledTimer_Off.png"));
        ui->lcdNumber->display("");
        timerMinSet= 0;
        timerHoraSet= 0;
    }
}

void display::buttonSetClicked()
{
    if(flag_powerOn)
    {

        if(estado_avisoSacarComida)
        {
            estado_avisoSacarComida = false;
        }
        else if(flag_meterComida)
        {
            flag_meterComida = false;
            flag_pipipi = false;
        }

        else if(!estado_avisoSacarComida && !flag_meterComida)
        {
            button_set = button_set+ 1;
            if(button_set>3){button_set=1;}

            menuSet(button_set);
            //1: menu temperatura
            //2: menu timer
            //3: menu calentando
        }

    }
}

void display::buttonMenosClicked()
{
    if(flag_powerOn)
    {
        if(flag_tempInf)
        {
            flag_tempInf = false;
            ui->lcdNumber->display(timerSet);
        }
        else
        {
            if(estado_avisoSacarComida)
            {
                estado_avisoSacarComida = false;
            }
            else if(flag_meterComida)
            {
                flag_meterComida = false;
                flag_pipipi = false;
            }
            else if(!estado_avisoSacarComida && !flag_meterComida)
            {
                if (button_set ==1) //menu temperatura
                {
                    tempSet = tempSet-1;
                    ui->lcdNumber->display(tempSet);
                }

                if (button_set==2)
                {
                    if(timerMinSet==0)
                    {
                        timerMinSet = 30;
                        timerHoraSet =timerHoraSet - 1;
                        if(timerHoraSet<0) timerHoraSet =0;

                    }
                    else if(timerMinSet==30) timerMinSet = 0;

                    else if (timerMinSet==0 && timerHoraSet==0)
                    {
                        timerMinSet =0;
                        timerHoraSet=0;
                    }

                    QString _horas;
                    QString _minutos;
                    if(timerHoraSet<10)
                    {
                        _horas= QString("0%1").arg(timerHoraSet);
                    }
                    else _horas = QString("%1").arg(timerHoraSet);

                    if(timerMinSet==0)
                    {
                        _minutos = QString("0%1").arg(timerMinSet);
                    }
                    else _minutos = QString("%1").arg(timerMinSet);

                    timerSet = _horas+":"+_minutos;
                    ui->lcdNumber->display(_horas+":"+_minutos);
                }
            }
        }
    }
}

void display::buttonMasClicked()
{
    if(flag_powerOn)
    {
        if(flag_tempInf)
        {
            ui->lcdNumber->display(timerSet);
            flag_tempInf = false;
        }
        else
        {
            if(estado_avisoSacarComida)
            {
                estado_avisoSacarComida = false;
            }
            else if(flag_meterComida)
            {
                flag_meterComida = false;
                flag_pipipi = false;
            }
            else if(!estado_avisoSacarComida && !flag_meterComida)
            {
                if (button_set ==1) //menu temperatura
                {
                    tempSet = tempSet+1;
                    ui->lcdNumber->display(tempSet);
                }

                if (button_set==2)
                {
                    if(timerMinSet==0) timerMinSet = 30;
                    else if(timerMinSet==30)
                    {
                        timerMinSet = 0;
                        timerHoraSet =timerHoraSet + 1;
                        if(timerHoraSet>99) timerHoraSet =99;
                    }

                    QString _horas;
                    QString _minutos;

                    if(timerHoraSet<10)
                    {
                        _horas= QString("0%1").arg(timerHoraSet);
                    }
                    else _horas = QString("%1").arg(timerHoraSet);

                    if(timerMinSet==0)
                    {
                        _minutos = QString("0%1").arg(timerMinSet);
                    }
                    else _minutos = QString("%1").arg(timerMinSet);

                    timerSet = _horas+":"+_minutos;
                    ui->lcdNumber->display(_horas+":"+_minutos);
                }
            }
        }
    }
}

void display::buttonStartClicked()
{
    if(flag_powerOn)
    {
        if(button_set!=1 && button_set!=2 && button_set!=3)
        {
            if(estado_avisoSacarComida)
            {
                estado_avisoSacarComida = false;
            }
            else if(flag_meterComida)
            {
                flag_meterComida = false;
                flag_pipipi = false;
            }
            else if(!estado_avisoSacarComida && !flag_meterComida)
            {
                flag_meterComida = true;
                if(flag_meterComida)
                {
                    estado_aviso = false;
                    flag_pipipi = false;
                    estado_temperaturaAlcanzado = true;

                    ui->ledAviso->setPixmap(QString(":/ledAviso_Off.png"));

                    if(timerMinSet==0 && timerHoraSet==0)
                    {
                        menuCoccion_inf();
                    }
                    else menuCoccion_cont();
                }
            }
        }
    }
}

void display::buttonTemperaturaCliceck()
{
    if(flag_powerOn)
    {
        if(flag_gradoTemp)
        {
            gradoTemp = 'F';
            tempSet = tempSet*9/5+32;
            temp0 = temp0*9/5+32;
            if(button_set==0)
            {
                ui->lcdNumber->display(temp0);
            }
            else if(button_set==1)
            {
                ui->lcdNumber->display(tempSet);
            }
            tempLed(true);

        }
        else
        {
            gradoTemp ='C';
            tempSet = (tempSet-32)*5/9;
            temp0 = (temp0-32)*5/9;
            if(button_set==0)
            {
                ui->lcdNumber->display(temp0);
            }
            else if(button_set==1)
            {
                ui->lcdNumber->display(tempSet);
            }
        }
        tempLed(true);
        flag_gradoTemp =!flag_gradoTemp;
    }
    else tempLed(false);
}


void display::buttonAguaClicked()
{
    //en updateTime2
    estado_avisoAgua = !estado_avisoAgua;
    if(!estado_avisoAgua)
    {
        ui->llenarAgua->setPixmap(QString(""));
        ui->ledAviso->setPixmap(QString(":/ledAviso_Off"));
    }
    //    else if(estado_avisoAgua)
    //    {
    //        ui->ledAviso->setPixmap(QString(":/ledAviso_On"));
    //    }
}

//**MENU**//
void display:: menuSet(int menu)
{
    if (menu==1)
    {
        menuTemperature();
    }
    else if (menu==2)
    {
        menuTimer();
    }
    else if(menu==3)
    {
        menuCalentando();
    }
}

void display::menuTemperature()
{
    estado_aviso = false;
    flag_meterComida = false;
    estado_timerOn = false;
    estado_pause = true; //negar

    ui->ledSet->setPixmap(QString(":/ledSet_On"));
    ui->ledCalor->setPixmap(QString(":/ledCalor_Off"));
    ui->ledAviso->setPixmap(QString(":/ledAviso_Off"));
    ui->ledTimer->setPixmap(QString(":/ledTimer_Off"));
    tempLed(true);
    ui->texto->setText("Set TEMPERATURA");

    ui->lcdNumber->display(tempSet);

    //parpadeo
    estado_setTemp = true;
    // mostrar temp -- : --
    flag_tempInf = true;

}

void display::menuTimer()
{
    ui->ledSet->setPixmap(QString(":/ledSet_On.png"));
    ui->ledCalor->setPixmap(QString(":/ledCalor_Off.png"));
    ui->ledAviso->setPixmap(QString(":/ledAviso_Off.png"));
    ui->ledTimer->setPixmap(QString(":/ledTimer_On.png"));
    ui->ledTempC->setPixmap(QString(":/ledC_Off.png"));
    ui->ledTempF->setPixmap(QString(":/ledF_Off.png"));

    ui->texto->setText("Set TIMER");
    //    if (flag_tempInf) timerSet="--:--";
    //    ui->lcdNumber->display("--:--");
    //        ui->lcdNumber->display(QString("0%1:0%2").arg(timerHoraSet).arg(timerMinSet));


    //parpadeo
    estado_setTemp = false;
    estado_setTimer = true;

}

void display::menuCalentando()
{
    timeHora = timerHoraSet;
    timeMin = timerMinSet;

    ui->texto->setText("Calendando agua...");
    ui->lcdNumber->display(temp0);

    ui->ledSet->setPixmap(QString(":/ledSet_Off.png"));
    ui->ledCalor->setPixmap(QString(":/ledCalor_On.png"));
    ui->ledAviso->setPixmap(QString(":/ledAviso_Off.png"));
    ui->ledTimer->setPixmap(QString(":/ledTimer_Off.png"));

    tempLed(true);

    estado_setTimer = false;
    estado_calentando = true;

}

void display::menuCoccion_inf()
{
    ui->texto->setText("Sous-Vide sin tiempo");
    ui->pipipi->setText(" ");
    estado_aviso = false;
    tempLed(true);
    flag_pipipi=false;
    estado_temperaturaAlcanzado = true;
}

void display::menuCoccion_cont()
{
    ui->texto->setText("Sous-Vide con tiempo");
    //    ui->lcdNumber->display(timerSet);
    ui->pipipi->setText(" ");
    estado_timerOn = true;
    flag_pipipi = false;
    estado_pause = !estado_pause; // primer estado_pause = false
    estado_temperaturaAlcanzado = true;

}

//**funciones complementarios**//
void display::tempLed(bool state)
{

    if(state)
    {
        if(gradoTemp=='C')
        {
            ui->ledTempC->setPixmap(QString(":/ledC_On.png"));
            ui->ledTempF->setPixmap(QString(":/ledF_Off.png"));
        }
        if (gradoTemp=='F')
        {
            ui->ledTempC->setPixmap(QString(":/ledC_Off.png"));
            ui->ledTempF->setPixmap(QString(":/ledF_On.png"));
        }
    }
    if(!state)
    {
        ui->ledTempC->setPixmap(QString(":/ledC_Off.png"));
        ui->ledTempF->setPixmap(QString(":/ledF_Off.png"));
    }
}

//**update**//
//subir temperatura
void display::updateTime()
{
    if(flag_powerOn)
    {
        if(estado_calentando)
        {
            if (tempSet>temp0) //Empezar a calentar
            {
                temp0 = temp0 +5;
                ui->lcdNumber->display(temp0);
            }
            else                //Temperatura alcanzada
            {
                ui->texto->setText("!Calentado! Introducir comida");
                button_set=0;
                flag_pipipi = true;
                estado_temperaturaAlcanzado =true;
                ui->lcdNumber->display(tempSet);
                estado_calentando = false; //salir del estado calentnado
                flag_meterComida = true; //temperatura alcanzada

            }
        }
    }
}

//parpadeo normal
void display::updateTime2()
{
    if(flag_powerOn)
    {
        if(estado_fin)
        {
            ui->lcdNumber->display(tempSet);
            estado_aviso = false;
            if(estado_avisoSacarComida)
            {
                estado_temperaturaAlcanzado=true;
                flag_pipipi = true;
            }
            else if(!estado_avisoSacarComida)
            {
                estado_temperaturaAlcanzado=false;
                flag_pipipi = false;
                ui->ledCalor->setPixmap(QString(":/ledCalor_Off.png"));
            }
        }


        if(estado_setTemp)
        {
            if(flag_parpadeo)
            {
                ui->lcdNumber->display(tempSet);
                //                tempLed(true);
                flag_parpadeo =!flag_parpadeo;
            }
            else
            {
                ui->lcdNumber->display("");
                //                tempLed(false);
                flag_parpadeo =!flag_parpadeo;
            }
        }
        if(estado_setTimer)
        {
            if(flag_parpadeo)
            {
                if(flag_tempInf)  ui->lcdNumber->display("--:--");
                else ui->lcdNumber->display(timerSet);
                flag_parpadeo =!flag_parpadeo;
            }
            else
            {
                ui->lcdNumber->display("");
                //                ui->ledTimer->setPixmap(QString(":/ledApagado.png"));
                flag_parpadeo =!flag_parpadeo;
            }
        }
        if(estado_aviso)
        {
            if(flag_parpadeo)
            {
                ui->ledAviso->setPixmap(QString(":/ledAviso_On.png"));
                flag_parpadeo =!flag_parpadeo;
            }
            else
            {
                ui->ledAviso->setPixmap(QString(":/ledAviso_Off.png"));
                flag_parpadeo =!flag_parpadeo;
            }
        }
        if(estado_avisoAgua)
        {
            if(flag_parpadeo2)
            {
                ui->llenarAgua->setPixmap(QString(":/aviso/agua0.png"));
                ui->ledAviso->setPixmap(QString(":/ledAviso_Off"));
                flag_parpadeo2 = !flag_parpadeo2;

            }
            else if(!flag_parpadeo2)
            {
                ui->llenarAgua->setPixmap(QString(":/aviso/agua00.png"));
                ui->ledAviso->setPixmap(QString(":/ledAviso_On"));
                flag_parpadeo2 = !flag_parpadeo2;
            }

        }
    }
}

//cuenta atras coccion
void display::updateTime3()
{
    if(flag_powerOn)
    {
        if(estado_timerOn)
        {
            ui->ledSet->setPixmap(QString(":/ledSet_Off.png"));
            int restarTiempo = 10;
            if(!estado_pause) //Si no esta en PAUSE
            {
                if(timeMin==0)
                {
                    timeMin = 60 -restarTiempo;
                    timeHora =timeHora - 1;
                    if(timeHora<0) timeHora =0;

                }
                else if(timeMin!=0) timeMin = timeMin-restarTiempo;

                if(timeMin<0) timeMin=0;
            }
            else if(estado_pause) //Si entra en PAUSE
            {
                ui->texto->setText("PAUSE");
            }

            QString _horas;
            QString _minutos;

            // mostrar alternando tiempo con temperatura
            if (flag_timerTemp) //mostrar tiempo
            {
                if(!estado_pause) ui->ledTimer->setPixmap(QString(":/ledTimer_On.png"));
                tempLed(false); //led temperatura ºC/ºF apagado

                if(timeHora<10)
                {
                    _horas= QString("0%1").arg(timeHora);
                }
                else _horas = QString("%1").arg(timeHora);

                if(timeMin==0)
                {
                    _minutos = QString("0%1").arg(timeMin);
                }
                else _minutos = QString("%1").arg(timeMin);

                ui->lcdNumber->display(_horas+":"+_minutos);
                flag_timerTemp = !flag_timerTemp;

            }
            else //mostrar temperatura
            {
                if(!estado_pause)  ui->ledTimer->setPixmap(QString(":/ledTimer_Off.png"));
                tempLed(true);
                ui->lcdNumber->display(tempSet);
                flag_timerTemp = !flag_timerTemp;
            }

            if (timeMin==0 && timeHora==0)
            {
                ui->ledCalor->setPixmap(QString(":/ledCalor_Off.png"));
                ui->ledAviso->setPixmap(QString(":/ledAviso_Off.png"));
                ui->ledTimer->setPixmap(QString(":/ledTimer_Off.png"));
                ui->ledSet->setPixmap(QString(":/ledSet_Off.png"));
                tempLed(true);
                ui->lcdNumber->display(tempSet);
                ui->texto->setText("¡FIN! Sacar comida");
                //                flag_pipipi = true;
                estado_fin = true;
                estado_timerOn = false;
                flag_meterComida = false;
                estado_avisoSacarComida = true;
                //                estado_temperaturaAlcanzado = false;
            }
        }

    }

}

//parpadeo rapido
void display::updateTime4()
{
    if(flag_powerOn)
    {
        if(estado_temperaturaAlcanzado && button_set!=1 && button_set!=2)
        {
            if(flag_parpadeo)
            {
                ui->ledCalor->setPixmap(QString(":/ledCalor_On.png"));
                flag_parpadeo= !flag_parpadeo;
            }
            else
            {
                ui->ledCalor->setPixmap(QString(":/ledCalor_Off.png"));
                flag_parpadeo= !flag_parpadeo;
            }
        }
        if(estado_pause)
        {
            if(estado_timerOn)
            {
                if(flag_parpadeo2)
                {
                    ui->ledTimer->setPixmap(QString(":/ledTimer_On.png"));
                    flag_parpadeo2= !flag_parpadeo2;
                }
                else if(!flag_parpadeo2)
                {
                    ui->ledTimer->setPixmap(QString(":/ledTimer_Off.png"));
                    flag_parpadeo2= !flag_parpadeo2;
                }
            }
        }

        if(flag_pipipi)
        {
            if(flag_parpadeo2)
            {
                ui->pipipi->setText("Pi! Pi! Pi!");
                flag_parpadeo2= !flag_parpadeo2;
            }
            else if(!flag_parpadeo2)
            {
                ui->pipipi->setText("Pi!");
                flag_parpadeo2= !flag_parpadeo2;
            }
        }
        else if(!flag_pipipi)
        {
            ui->pipipi->setText("");
        }

    }

}
