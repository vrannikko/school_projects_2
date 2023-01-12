#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "gradecounter.hh"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void countAbsoluteGrade();

private:
    Ui::MainWindow *ui;
    int n = 0,
        g = 0,
        p = 0,
        e = 0;
};
#endif // MAINWINDOW_HH
