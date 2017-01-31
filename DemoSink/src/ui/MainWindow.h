

#ifndef EFFECTIVE_PARAKEET_MAINWINDOW_H
#define EFFECTIVE_PARAKEET_MAINWINDOW_H

#include <QMainWindow>
#include <QListView>
#include <QtCore/QStringListModel>

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr, const Qt::WindowFlags &flags = Qt::WindowFlags());

    void NewSsrc(const uint32_t unknownSsrc);

private:
    QStringList stringList;
};


#endif //EFFECTIVE_PARAKEET_MAINWINDOW_H
