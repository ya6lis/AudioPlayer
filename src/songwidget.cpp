#include "songwidget.h"
#include "mainwindow.h"
#include "ui_songwidget.h"

SongWidget::SongWidget(QString songName, int index, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SongWidget)
    , index(index)
    , songName(songName)

{
    ui->setupUi(this);
    ui->name->setText(songName);
    ui->pause->hide();
}

SongWidget::~SongWidget()
{
    delete ui;
}

void SongWidget::resumeSong(bool isPlaying, int index)
{
    if (this->index != index) {
        ui->name->setStyleSheet("color: white; background-color: transparent;border: none;");
        ui->play->show();
        ui->pause->hide();
        ui->resume->hide();

        return;
    }

    if (!isPlaying) {
        ui->resume->show();
        ui->pause->hide();
        ui->play->hide();

    } else {
        ui->name->setStyleSheet(
            "color: rgba(30,215,96,255); background-color: transparent;border: none;");
        emit sendSongName(songName);
        ui->pause->show();
        ui->resume->hide();
        ui->play->hide();
    }
}

void SongWidget::checkIndex(int index)
{
    if (this->index > index) {
        this->index--;
    }
}

void SongWidget::on_play_clicked()
{
    emit sendSongName(songName);
    emit sendData(index);
    ui->play->hide();
    ui->name->setStyleSheet(
        "color: rgba(30,215,96,255); background-color: transparent;border: none;");
}

void SongWidget::on_remove_clicked()
{
    emit sendRemoveData(index);
}

QPushButton *SongWidget::getPause()
{
    return ui->pause;
}
QPushButton *SongWidget::getResume()
{
    return ui->resume;
}
