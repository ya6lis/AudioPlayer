#ifndef SONGWIDGET_H
#define SONGWIDGET_H

#include <QPushButton>
#include <QWidget>

namespace Ui {
class SongWidget;
}

class SongWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SongWidget(QString songName, int index, QWidget *parent = nullptr);
    ~SongWidget();

    QPushButton *getPause();
    QPushButton *getResume();

signals:
    void sendData(int index);
    void sendRemoveData(int index);
    void sendSongName(QString songName);

public slots:
    void on_play_clicked();
    void resumeSong(bool status, int index);
    void checkIndex(int index);

private slots:

    void on_remove_clicked();

private:
    Ui::SongWidget *ui;
    int index;
    QString songName;
};

#endif // SONGWIDGET_H
