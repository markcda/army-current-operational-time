#include "cpewindow.h"

CPEWindow::CPEWindow(QWidget *parent) : QMainWindow(parent) {
  showDialog();
  if (cd == nullptr)
    return;
  setStyleSheet("* { background-color: \"#383c4a\"; color: \"#ffffff\"; }");
  setWindowTitle("ПОПУПД-АиОВдКШТ-" + comms[cd->cmnd] + "-А.ТКА2021");
  showFullScreen();
  auto *cw = new QWidget(this);
  auto *clt = new QGridLayout();
  lC = new QLabel(
      "Команда " + comms[cd->cmnd] + ". Время \"Ч\" - " +
          QLocale::system().toString(cd->CHplus, "hh:mm, d MMMM yyyy."),
      this);
  lC->setStyleSheet("color: \"#d0d000\";");
  lC->setFixedHeight(100);
  QFont font;
  font.setPointSize(42);
  lC->setFont(font);
  font.setPointSize(28);
  clt->addWidget(lC, 0, 0, 1, 2);
  auto *lAstro = new QLabel("АСТРОНОМИЧЕСКОЕ ВРЕМЯ", this);
  auto *lOpera = new QLabel("ОПЕРАТИВНОЕ ВРЕМЯ", this);
  lAstro->setFont(font);
  lOpera->setFont(font);
  font.setPointSize(20);
  lAstro->setAlignment(Qt::AlignHCenter);
  lOpera->setAlignment(Qt::AlignHCenter);
  lAstro->setFixedHeight(100);
  lOpera->setFixedHeight(100);
  clt->addWidget(lAstro, 1, 0);
  clt->addWidget(lOpera, 1, 1);
  cdatetime = new QDateTime(QDateTime::currentDateTime());
  lclock = new AnalogClock(cdatetime->time(), this);
  clt->addWidget(lclock, 2, 0);
  rtime = new QDateTime(cdatetime->addMSecs(cd->substract));
  rclock = new AnalogClock(rtime->time(), this);
  clt->addWidget(rclock, 2, 1);
  auto *wlclock = new QLabel(this);
  auto *wrclock = new QLabel(this);
  font.setPointSize(24);
  wlclock->setFont(font);
  wrclock->setFont(font);
  wlclock->setAlignment(Qt::AlignHCenter);
  wrclock->setAlignment(Qt::AlignHCenter);
  wlclock->setFixedHeight(80);
  wrclock->setFixedHeight(80);
  clt->addWidget(wlclock, 3, 0);
  clt->addWidget(wrclock, 3, 1);
  auto *ld = new QLabel(this);
  auto *rd = new QLabel(this);
  ld->setFont(font);
  ld->setWordWrap(true);
  ld->setAlignment(Qt::AlignHCenter);
  ld->setFixedHeight(80);
  rd->setFont(font);
  rd->setWordWrap(true);
  rd->setAlignment(Qt::AlignHCenter);
  rd->setFixedHeight(80);
  clt->addWidget(ld, 4, 0);
  clt->addWidget(rd, 4, 1);
  cw->setLayout(clt);
  setCentralWidget(cw);
  auto *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this,
          [this, timer, wlclock, wrclock, ld, rd]() {
            wlclock->setText(QLocale::system().toString(*cdatetime, "hh:mm:ss"));
            wrclock->setText(QLocale::system().toString(*rtime, "hh:mm:ss"));
            ld->setText(QLocale::system().toString(*cdatetime, "d MMMM yyyy"));
            rd->setText(QLocale::system().toString(*rtime, "d MMMM yyyy"));
            auto tc = QDateTime(cdatetime->addMSecs(1000));
            delete cdatetime;
            cdatetime = new QDateTime(tc);
            auto tr = QDateTime(rtime->addMSecs(1000));
            delete rtime;
            rtime = new QDateTime(tr);
            timer->start(1000);
          });
  timer->start();
}

CPEWindow::~CPEWindow() { delete cd; }

void CPEWindow::closeEvent(QCloseEvent *event) {
  showDialog();
  updateInfo();
  event->ignore();
}

void CPEWindow::updateInfo() {
  setWindowTitle("ПОПУПД-АиОВдКШТ-" + comms[cd->cmnd] + "-А.ТКА2021");
  lC->setText("Команда " + comms[cd->cmnd] + ". Время \"Ч\" - " +
              QLocale::system().toString(cd->CHplus, "hh:mm, d MMMM yyyy."));
  auto rt = cdatetime->addMSecs(cd->substract);
  delete rtime;
  rtime = new QDateTime(rt);
  rclock->setTime(rtime->time());
}

void CPEWindow::showDialog() {
  if (cd == nullptr)
    cd = new cdata;
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
  auto *dok = new QPushButton("Далее", d);
  connect(dok, &QPushButton::clicked, this, [d]() { d->accept(); });
  auto *dcancel = new QPushButton("Выйти из программы", d);
  connect(dcancel, &QPushButton::clicked, this, [d]() { d->reject(); });
  ddtcwlt->addWidget(dok);
  ddtcwlt->addWidget(dcancel);
  ddtcw->setLayout(ddtcwlt);
  ddtcw->setContentsMargins(0, 0, 0, 0);
  dlt->addWidget(ddtcw);
  d->setLayout(dlt);
  d->exec();
  if (d->result() == QDialog::Accepted) {
    cd->cmnd = Command(dcbCommands->currentIndex());
    cd->CHplus = ddteAstronomic->dateTime();
    cd->substract = ddteOperative->dateTime().toMSecsSinceEpoch() -
                    ddteAstronomic->dateTime().toMSecsSinceEpoch();
  } else {
    delete cd;
    QApplication::exit();
  }
}
