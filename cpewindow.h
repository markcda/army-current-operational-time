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
  QDateTime cdatetime, rtime;
};
#endif // CPEWINDOW_H
