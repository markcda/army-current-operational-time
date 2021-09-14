#ifndef CPEWINDOW_H
#define CPEWINDOW_H

#include "aclock.h"
#include "cdata.h"
#include <QApplication>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QTimer>

class CPEWindow : public QMainWindow {
  Q_OBJECT

public:
  CPEWindow(QWidget *parent = nullptr);
  ~CPEWindow();

private:
  cdata *cd = nullptr;
  QDateTime *cdatetime = nullptr, *rtime = nullptr;
  QStringList comms = QStringList({"15", "25", "35", "45", "115", "125", "135", "145"});
  
  QLabel *lC = nullptr;
  AnalogClock *lclock = nullptr, *rclock = nullptr;
  
  void showDialog();
  virtual void closeEvent(QCloseEvent *event) override;
  void updateInfo();
};
#endif // CPEWINDOW_H
