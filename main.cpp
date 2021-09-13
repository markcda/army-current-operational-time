#include "cpewindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  CPEWindow w;
  w.show();
  return a.exec();
}
