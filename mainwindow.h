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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include "axistag.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

    void parseSerial(QByteArray data);
private slots:
    void serialRecieved();
    void timerSlot();
  
private:
  Ui::MainWindow *ui;
  QCustomPlot *mPlot;
  QCustomPlot *mPlot2;
  QPointer<QCPGraph> mGraph1;
  QPointer<QCPGraph> mGraph2;
  QPointer<QCPGraph> mGraph3;
  QPointer<QCPGraph> mGraph4;
  QPointer<QCPGraph> mGraph5;
  QPointer<QCPGraph> mGraph6;
  QPointer<QCPGraph> mGraph7;
  QPointer<QCPGraph> mGraph8;
  QPointer<QCPGraph> mGraph9;
  QPointer<QCPGraph> mGraph10;
  QPointer<QCPGraph> mGraph11;
  QPointer<QCPGraph> mGraph12;
  AxisTag *mTag1;
  AxisTag *mTag2;
  AxisTag *mTag3;
  AxisTag *mTag4;
  AxisTag *mTag5;
  AxisTag *mTag6;
  AxisTag *mTag7;
  AxisTag *mTag8;
  AxisTag *mTag9;
  AxisTag *mTag10;
  AxisTag *mTag11;
  AxisTag *mTag12;
  QTimer mDataTimer;
  double tempd;
  double humidityd;
  double pressured;
  double mag_xd;
  double mag_yd;
  double mag_zd;
  double gyro_xd;
  double gyro_yd;
  double gyro_zd;
  double acc_xd;
  double acc_yd;
  double acc_zd;

};


#endif // MAINWINDOW_H
