#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString mRessourceDir;
    QImage *mImage;
    QImage *mBlurredImage;

    QImage transformHelper(int);
private slots:
    void load();
    void clear();
    void display();
    void transform();

};
#endif // MAINWINDOW_H
