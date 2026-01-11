#include "mainwindow.h"

#include <QApplication>

// Entry point for the graphical application. It constructs a QApplication
// instance, creates our MainWindow and runs the event loop. Without a
// QApplication (or QCoreApplication for non‑GUI programs) most Qt
// functionality will not work properly. The argc/argv parameters are
// passed through so that Qt can process any command line options.
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MainWindow win;
    win.show();
    return app.exec();
}