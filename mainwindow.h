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
    void setRange0(QCPRange range);
    void setRange1(QCPRange range);
  
private:
  Ui::MainWindow *ui;

  QCPRange range0;
  QCPRange range1;

  QPointer<QCPGraph> mGraph1;
  QPointer<QCPGraph> mGraph2;
  QPointer<QCPGraph> mGraph3;
  QPointer<QCPGraph> m2Graph1;
  QPointer<QCPGraph> m2Graph2;
  QPointer<QCPGraph> m2Graph3;

  AxisTag *mTag1;
  AxisTag *mTag2;
  AxisTag *mTag3;
  AxisTag *m2Tag1;
  AxisTag *m2Tag2;
  AxisTag *m2Tag3;

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
