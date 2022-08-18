#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAudioOutput>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QMimeData>

#include <QScrollBar>
#include <QShortcut>

#include <QVector>

#include <QLabel>
#include <QTimer>
#include <iostream>
#include <string.h>

#include "songwidget.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_play_clicked();
    void on_pause_clicked();
    void on_resume_clicked();

    void on_Volume_sliderMoved(int position);
    void on_volumeBtn_clicked();

    void on_sliderProgress_sliderMoved(int position);
    void on_positionChanged(qint64 position);
    void on_durationChanged(qint64 position);

    void on_add_clicked();

    void on_toNext_clicked();
    void on_toPrevious_clicked();

    void on_mediaStatusChanged(QMediaPlayer::MediaStatus mediaST);

    void on_repeat_clicked();
    void on_repeat_3_clicked();

    QString timeCounter(int s);
    void logicToButtons();

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    void addSong(QString tempName = "");

    void on_sliderProgress_sliderPressed();

    void on_sliderProgress_sliderReleased();

signals:
    void songStatus(bool status, int index);
    void songDeleted(int index);

private:
    Ui::MainWindow *ui;

    bool playResume = false;
    bool volume = false;
    double rightVolume = 1;

    int activeSong = 0;
    int index = 0;
    int rightIndex = 0;

    bool slider = true;

    QMediaPlayer *player;
    QAudioOutput *audioOutput;

    QVector<QString> nameOfSong;
    QVector<QString>::Iterator it;
    void playSong(int index);
    void setSongName(QString songName);

    QTimer *timer;
    QLabel *text;
};
#endif // MAINWINDOW_H
