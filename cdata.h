#ifndef CDATA_H
#define CDATA_H

#include <QDateTime>

enum Command {
  C15,
  C25,
  C35,
  C45,
  Ced115,
  Ced125,
  Ced135,
  Ced145
};

struct cdata {
  Command cmnd = Ced125;
  QDateTime CHplus = QDateTime::fromMSecsSinceEpoch(0);
  qint64 substract = 0;
};

#endif // CDATA_H
