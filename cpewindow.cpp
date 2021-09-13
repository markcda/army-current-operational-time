#include "cpewindow.h"

CPEWindow::CPEWindow(QWidget *parent) : QMainWindow(parent) {
  cd = new cdata;
  auto comms =
      QStringList({"15", "25", "35", "45", "115", "125", "135", "145"});
  /************DIALOG************/
  auto *d = new QDialog(this);
  d->setWindowTitle("Первоначальная настройка");
  auto *dlt = new QVBoxLayout();
  auto *dlCommand = new QLabel("Выберите команду, доведённую по тревоге:", d);
  dlt->addWidget(dlCommand);
  auto *dcbCommands = new QComboBox(d);
  dcbCommands->addItems(comms);
  dlt->addWidget(dcbCommands);
  auto *dlDT = new QLabel("Введите астрономическое время получения "
                          "оперативного времени - и оперативное время:");
  dlt->addWidget(dlDT);
  auto *ddtcw = new QWidget(d);
  auto *ddtcwlt = new QHBoxLayout();
  ddtcwlt->setContentsMargins(0, 0, 0, 0);
  auto *ddteAstronomic = new QDateTimeEdit(d);
  ddteAstronomic->setDateTime(QDateTime::currentDateTime());
  ddtcwlt->addWidget(ddteAstronomic);
  auto *ddteOperative = new QDateTimeEdit(d);
  ddteOperative->setDateTime(QDateTime::currentDateTime());
  ddtcwlt->addWidget(ddteOperative);
  ddtcw->setLayout(ddtcwlt);
  ddtcw->setContentsMargins(0, 0, 0, 0);
  dlt->addWidget(ddtcw);
  d->setLayout(dlt);
  if (d->exec() == 0) {
    cd->cmnd = Command(dcbCommands->currentIndex());
    cd->CHplus = ddteAstronomic->dateTime();
    cd->substract = ddteOperative->dateTime().toMSecsSinceEpoch() -
                    ddteAstronomic->dateTime().toMSecsSinceEpoch();
  } else {
    delete cd;
    QApplication::exit();
    return;
  }
  /******************************/
  setWindowTitle("ПОПУПД-АиОВдКШТ-" + comms[cd->cmnd] + "-А.ТКА2021");
  showFullScreen();
  auto *cw = new QWidget(this);
  auto *clt = new QGridLayout();
  auto *lC = new QLabel(
      "Команда " + comms[cd->cmnd] + ". Время \"Ч\" - " +
          QLocale::system().toString(cd->CHplus, "hh:mm, d MMMM yyyy."),
      this);
  lC->setFixedHeight(100);
  lC->setIndent(15);
  QFont font;
  font.setPointSize(26);
  lC->setFont(font);
  font.setPointSize(20);
  clt->addWidget(lC, 0, 0, 1, 2);
  auto *lAstro = new QLabel("АСТРОНОМИЧЕСКОЕ ВРЕМЯ", this);
  auto *lOpera = new QLabel("ОПЕРАТИВНОЕ ВРЕМЯ", this);
  lAstro->setFont(font);
  lOpera->setFont(font);
  lAstro->setAlignment(Qt::AlignHCenter);
  lOpera->setAlignment(Qt::AlignHCenter);
  lAstro->setFixedHeight(120);
  lOpera->setFixedHeight(120);
  clt->addWidget(lAstro, 1, 0);
  clt->addWidget(lOpera, 1, 1);
  cdatetime = QDateTime::currentDateTime();
  auto *lclock = new AnalogClock(cdatetime.time(), this);
  clt->addWidget(lclock, 2, 0);
  rtime = cdatetime.addMSecs(cd->substract);
  auto *rclock = new AnalogClock(rtime.time(), this);
  clt->addWidget(rclock, 2, 1);
  auto *wlclock = new QLabel(this);
  auto *wrclock = new QLabel(this);
  font.setPointSize(24);
  wlclock->setFont(font);
  wrclock->setFont(font);
  wlclock->setAlignment(Qt::AlignHCenter);
  wrclock->setAlignment(Qt::AlignHCenter);
  wlclock->setFixedHeight(120);
  wrclock->setFixedHeight(120);
  clt->addWidget(wlclock, 3, 0);
  clt->addWidget(wrclock, 3, 1);
  auto *ld = new QLabel(this);
  auto *rd = new QLabel(this);
  ld->setFont(font);
  ld->setWordWrap(true);
  ld->setAlignment(Qt::AlignHCenter);
  ld->setFixedHeight(120);
  rd->setFont(font);
  rd->setWordWrap(true);
  rd->setAlignment(Qt::AlignHCenter);
  rd->setFixedHeight(120);
  clt->addWidget(ld, 4, 0);
  clt->addWidget(rd, 4, 1);
  cw->setLayout(clt);
  setCentralWidget(cw);
  auto *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this,
          [this, timer, wlclock, wrclock, ld, rd]() {
            wlclock->setText(QLocale::system().toString(cdatetime, "hh:mm:ss"));
            wrclock->setText(QLocale::system().toString(rtime, "hh:mm:ss"));
            ld->setText(QLocale::system().toString(cdatetime, "d MMMM yyyy"));
            rd->setText(QLocale::system().toString(rtime, "d MMMM yyyy"));
            cdatetime = cdatetime.addMSecs(1000);
            rtime = rtime.addMSecs(1000);
            timer->start(1000);
          });
  timer->start();
}

CPEWindow::~CPEWindow() { delete cd; }
