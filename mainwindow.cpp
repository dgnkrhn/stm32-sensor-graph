/***************************************************************************
**                                                                        **
**  QCustomPlot, an easy to use, modern plotting widget for Qt            **
**  Copyright (C) 2011-2018 Emanuel Eichhammer                            **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Emanuel Eichhammer                                   **
**  Website/Contact: http://www.qcustomplot.com/                          **
**             Date: 25.06.18                                             **
**          Version: 2.0.1                                                **
****************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QTimer>

QSerialPort *serial;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mTag1(0),
    mTag2(0),
    mTag3(0),
    m2Tag1(0),
    m2Tag2(0),
    m2Tag3(0)
{
    ui->setupUi(this);
    serial = new QSerialPort(this);
    serial->setPortName("/dev/ttyACM0");
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if (!serial->open(QIODevice::ReadWrite))
    {
        qDebug() << "opening error" << serial->errorString() << serial->error();
        return;
    }
    serial->write("ok*");
    connect(serial,SIGNAL(readyRead()),this,SLOT(serialRecieved()));

    // configure plot to have two right axes:
    ui->mPlot->yAxis->setTickLabels(false);
    connect(ui->mPlot->yAxis2, SIGNAL(rangeChanged(QCPRange)), this, SLOT(setRange0(QCPRange))); // left axis only mirrors inner right axis
    ui->mPlot->yAxis->setVisible(true);
    ui->mPlot->axisRect()->addAxis(QCPAxis::atRight);
    ui->mPlot->axisRect()->axis(QCPAxis::atRight, 1)->setPadding(50); // add some padding to have space for tags

    // configure plot to have two right axes:
    ui->m2Plot->yAxis->setTickLabels(false);
    connect(ui->m2Plot->yAxis2, SIGNAL(rangeChanged(QCPRange)), this, SLOT(setRange1(QCPRange))); // left axis only mirrors inner right axis
    ui->m2Plot->yAxis->setVisible(true);
    ui->m2Plot->axisRect()->addAxis(QCPAxis::atRight);
    ui->m2Plot->axisRect()->axis(QCPAxis::atRight, 1)->setPadding(50); // add some padding to have space for tags

    QFont legendFont = font();  // start out with MainWindow's font..
    legendFont.setPointSize(9); // and make a bit smaller for legend

    ui->mPlot->legend->setVisible(true);
    ui->mPlot->legend->setFont(legendFont);
    ui->mPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
    ui->mPlot->legend->setSelectableParts(QCPLegend::spItems);
    ui->mPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);

    ui->m2Plot->legend->setVisible(true);
    ui->m2Plot->legend->setFont(legendFont);
    ui->m2Plot->legend->setBrush(QBrush(QColor(255,255,255,230)));
    ui->m2Plot->legend->setSelectableParts(QCPLegend::spItems);
    ui->m2Plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);

    // create graphs:
    mGraph1 = ui->mPlot->addGraph(ui->mPlot->xAxis, ui->mPlot->axisRect()->axis(QCPAxis::atRight, 1));
    mGraph2 = ui->mPlot->addGraph(ui->mPlot->xAxis, ui->mPlot->axisRect()->axis(QCPAxis::atRight, 1));
    mGraph3 = ui->mPlot->addGraph(ui->mPlot->xAxis, ui->mPlot->axisRect()->axis(QCPAxis::atRight, 1));
    mGraph1->setName(QString("X"));
    mGraph2->setName(QString("Y"));
    mGraph3->setName(QString("Z"));
    mGraph1->setPen(QPen(QColor(250, 120, 0)));
    mGraph2->setPen(QPen(QColor(0, 180, 60)));
    mGraph3->setPen(QPen(QColor(0, 0, 0)));

    // create graphs:
    m2Graph1 = ui->m2Plot->addGraph(ui->m2Plot->xAxis, ui->m2Plot->axisRect()->axis(QCPAxis::atRight, 1));
    m2Graph2 = ui->m2Plot->addGraph(ui->m2Plot->xAxis, ui->m2Plot->axisRect()->axis(QCPAxis::atRight, 1));
    m2Graph3 = ui->m2Plot->addGraph(ui->m2Plot->xAxis, ui->m2Plot->axisRect()->axis(QCPAxis::atRight, 1));
    m2Graph1->setName(QString("X"));
    m2Graph2->setName(QString("Y"));
    m2Graph3->setName(QString("Z"));
    m2Graph1->setPen(QPen(QColor(250, 120, 0)));
    m2Graph2->setPen(QPen(QColor(0, 180, 60)));
    m2Graph3->setPen(QPen(QColor(0, 0, 0)));

    // create tags with newly introduced AxisTag class (see axistag.h/.cpp):
    mTag1 = new AxisTag(mGraph1->valueAxis());
    mTag1->setPen(mGraph1->pen());
    mTag2 = new AxisTag(mGraph2->valueAxis());
    mTag2->setPen(mGraph2->pen());
    mTag3 = new AxisTag(mGraph3->valueAxis());
    mTag3->setPen(mGraph3->pen());

    // create tags with newly introduced AxisTag class (see axistag.h/.cpp):
    m2Tag1 = new AxisTag(m2Graph1->valueAxis());
    m2Tag1->setPen(m2Graph1->pen());
    m2Tag2 = new AxisTag(m2Graph2->valueAxis());
    m2Tag2->setPen(m2Graph2->pen());
    m2Tag3 = new AxisTag(m2Graph3->valueAxis());
    m2Tag3->setPen(m2Graph3->pen());

    connect(&mDataTimer, SIGNAL(timeout()), this, SLOT(timerSlot()));
    mDataTimer.start(50);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QByteArray charBuffer;

void MainWindow::serialRecieved()
{
    QByteArray data = serial->readAll();
    parseSerial(data);
    return;
}

void MainWindow::parseSerial(QByteArray data)
{
    QStringList List;
    QString temp, humidity, pressure, mag_x, mag_y, mag_z, gyro_x, gyro_y, gyro_z, acc_x, acc_y, acc_z;

    charBuffer.append(data);
    if (data.contains("\n"))
    {
        QString myString(charBuffer);
        List = myString.split(':');
        qDebug() << "List = " << List << endl;
        charBuffer = "";

        if(List.length() < 14)
            return;

        temp = List[1];
        humidity = List[2];
        pressure = List[3];
        mag_x = List[4];
        mag_y = List[5];
        mag_z = List[6];
        gyro_x = List[7];
        gyro_y = List[8];
        gyro_z = List[9];
        acc_x = List[10];
        acc_y = List[11];
        acc_z = List[12];

        ui ->label_t->setText(temp);
        ui ->label_h->setText(humidity);
        ui ->label_p->setText(pressure);

        ui ->label_mx->setText(mag_x);
        ui ->label_my->setText(mag_y);
        ui ->label_mz->setText(mag_z);

    } else {
        return;
    }

    //    tempd = temp.toDouble();
    //    humidityd = humidity.toDouble();
    //    pressured = pressure.toDouble();
    //    mag_xd = mag_x.toDouble();
    //    mag_yd = mag_y.toDouble();
    //    mag_zd = mag_z.toDouble();
    gyro_xd = gyro_x.toDouble();
    gyro_yd = gyro_y.toDouble();
    gyro_zd = gyro_z.toDouble();
    acc_xd = acc_x.toDouble();
    acc_yd = acc_y.toDouble();
    acc_zd = acc_z.toDouble();


    //    qDebug() << tempd;
    //    qDebug() << humidityd;
    //    qDebug() << pressured;
    //    qDebug() << mag_xd;
    //    qDebug() << mag_yd;
    //    qDebug() << mag_zd;
    //    qDebug() << gyro_xd;
    //    qDebug() << gyro_yd;
    //    qDebug() << gyro_zd;
    //    qDebug() << acc_xd;
    //    qDebug() << acc_yd;
    //    qDebug() << acc_zd;

    //temp, humidity, pressure, mag_X, mag_Y, mag_Z, gyro_X, gyro_Y, gyro_Z, acc_X, acc_Y, acc_Z

}

void MainWindow::timerSlot()
{
    mGraph1->addData(mGraph1->dataCount(), acc_xd);
    mGraph2->addData(mGraph2->dataCount(), acc_yd);
    mGraph3->addData(mGraph3->dataCount(), acc_zd);

    m2Graph1->addData(m2Graph1->dataCount(), gyro_xd);
    m2Graph2->addData(m2Graph2->dataCount(), gyro_yd);
    m2Graph3->addData(m2Graph3->dataCount(), gyro_zd);

    //  mGraph1->addData(mGraph1->dataCount(), qSin(mGraph1->dataCount()/50.0)+qSin(mGraph1->dataCount()/50.0/0.3843)*0.25);
    //  mGraph2->addData(mGraph2->dataCount(), qCos(mGraph2->dataCount()/50.0)+qSin(mGraph2->dataCount()/50.0/0.4364)*0.15);

    // make key axis range scroll with the data:
    ui->mPlot->xAxis->rescale();
    mGraph1->rescaleValueAxis(false, true);
    mGraph2->rescaleValueAxis(false, true);
    mGraph3->rescaleValueAxis(false, true);
    ui->mPlot->xAxis->setRange(ui->mPlot->xAxis->range().upper, 200, Qt::AlignRight);

    // make key axis range scroll with the data:
    ui->m2Plot->xAxis->rescale();
    m2Graph1->rescaleValueAxis(false, true);
    m2Graph2->rescaleValueAxis(false, true);
    m2Graph3->rescaleValueAxis(false, true);
    ui->m2Plot->xAxis->setRange(ui->m2Plot->xAxis->range().upper, 200, Qt::AlignRight);

    // update the vertical axis tag positions and texts to match the rightmost data point of the graphs:
    double graph1Value = mGraph1->dataMainValue(mGraph1->dataCount()-1);
    double graph2Value = mGraph2->dataMainValue(mGraph2->dataCount()-1);
    double graph3Value = mGraph3->dataMainValue(mGraph3->dataCount()-1);
    mTag1->updatePosition(graph1Value);
    mTag2->updatePosition(graph2Value);
    mTag3->updatePosition(graph3Value);
    mTag1->setText(QString::number(graph1Value, 'f', 2));
    mTag2->setText(QString::number(graph2Value, 'f', 2));
    mTag3->setText(QString::number(graph3Value, 'f', 2));

    ui->mPlot->yAxis2->setRange(range0);
    ui->mPlot->replot();

    // update the vertical axis tag positions and texts to match the rightmost data point of the graphs:
    double graph21Value = m2Graph1->dataMainValue(m2Graph1->dataCount()-1);
    double graph22Value = m2Graph2->dataMainValue(m2Graph2->dataCount()-1);
    double graph23Value = m2Graph3->dataMainValue(m2Graph3->dataCount()-1);
    m2Tag1->updatePosition(graph21Value);
    m2Tag2->updatePosition(graph22Value);
    m2Tag3->updatePosition(graph23Value);
    m2Tag1->setText(QString::number(graph21Value, 'f', 2));
    //m2Tag1->setText("%l X",arg.(QString::number(graph21Value, 'f', 2)));
    m2Tag2->setText(QString::number(graph22Value, 'f', 2));
    m2Tag3->setText(QString::number(graph23Value, 'f', 2));

    ui->m2Plot->yAxis2->setRange(range1);
    ui->m2Plot->replot();

    static QTime time(QTime::currentTime());
    double key = time.elapsed()/1000.0;
    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 2) // average fps over 2 seconds
    {
      ui->statusBar->showMessage(
            QString("%1 FPS, Total Data points: %2")
            .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
            .arg(ui->mPlot->graph(0)->data()->size()+ui->mPlot->graph(1)->data()->size())
            , 0);
      lastFpsKey = key;
      frameCount = 0;
    }
}

void MainWindow::setRange0(QCPRange range)
{
    this->range0 = range;
}

void MainWindow::setRange1(QCPRange range)
{
    this->range1 = range;
}
