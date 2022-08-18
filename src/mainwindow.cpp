#include "mainwindow.h"
#include <QDebug>
#include "audioslider.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);

    player = new QMediaPlayer;
    audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);


    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(on_positionChanged(qint64)));
    connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(on_durationChanged(qint64)));
    connect(player,
            SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            this,
            SLOT(on_mediaStatusChanged(QMediaPlayer::MediaStatus)));
    ui->verticalLayout->addStretch(1);
    setAcceptDrops(true);

    this->setWindowTitle("Gordon MP");
    this->setWindowIcon(QIcon(":/icons/gordon.png"));

    ui->pause->hide();
    ui->resume->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::logicToButtons()
{
    if (activeSong == 0) {
        ui->toNext->setEnabled(true);
        ui->toPrevious->setEnabled(false);
    } else if (activeSong == nameOfSong.size() - 1) {
        ui->toNext->setEnabled(false);
        ui->toPrevious->setEnabled(true);
    } else {
        ui->toNext->setEnabled(true);
        ui->toPrevious->setEnabled(true);
    }

    if (nameOfSong.size() == 1) {
        ui->toNext->setEnabled(false);
        ui->toPrevious->setEnabled(false);
    }
}

void MainWindow::setSongName(QString songName)
{
    ui->name->setText(songName);
}

void MainWindow::on_play_clicked()
{
    playSong(0);
}
void MainWindow::playSong(int index)
{
    emit songStatus(false, activeSong);
    activeSong = index;
    player->setSource(QUrl::fromLocalFile(nameOfSong[activeSong]));

    player->play();

    ui->play->hide();
    emit songStatus(true, activeSong);

    ui->resume->hide();
    ui->pause->show();

    logicToButtons();
}

void MainWindow::on_pause_clicked()
{
    player->pause();
    ui->pause->hide();
    ui->resume->show();
    ui->play->hide();
    emit songStatus(false, activeSong);
}
void MainWindow::on_resume_clicked()
{
    player->play();
    ui->pause->show();
    ui->resume->hide();
    ui->play->hide();
    emit songStatus(true, activeSong);
}

void MainWindow::on_Volume_sliderMoved(int position)
{
    rightVolume = (1.0001 * position) / 100;
    audioOutput->setVolume(rightVolume);
    ui->volumeBtn->setIcon(QIcon(":/icons/volume_white.png"));

    volume = false;
    if (!rightVolume) {
        ui->volumeBtn->setIcon(QIcon(":/icons/volume_off_white.png"));
        volume = true;
    }
}
void MainWindow::on_volumeBtn_clicked()
{
    volume = !volume;
    if (!volume) {
        ui->volumeBtn->setIcon(QIcon(":/icons/volume_white.png"));
        audioOutput->setVolume(rightVolume);
        ui->Volume->setValue((rightVolume * 100) * 1.0001);
        if (!rightVolume) {
            audioOutput->setVolume(0.1);
            ui->Volume->setValue(10);
        }

    } else {
        ui->volumeBtn->setIcon(QIcon(":/icons/volume_off_white.png"));
        audioOutput->setVolume(0);
        ui->Volume->setValue(0);
    }
}

void MainWindow::on_sliderProgress_sliderMoved(int position)
{
    if (slider) {
        player->setPosition(position);
    }
}
QString MainWindow::timeCounter(int s)
{
    QString seconds;

    if (s <= 9) {
        seconds = "0" + QString::number(s);
    } else {
        seconds = QString::number(s);
    }
    return seconds;
}
void MainWindow::on_positionChanged(qint64 position)
{
    if (slider) {
        ui->sliderProgress->setSliderPosition(position);
    }
    ui->actualTime->setText((QString::number((ui->sliderProgress->value()) / 60000)) + ":"
                            + (timeCounter((ui->sliderProgress->value() / 1000) % 60)));
}
void MainWindow::on_durationChanged(qint64 position)
{
    ui->sliderProgress->setMaximum(position);

    ui->fullTime->setText(QString::number((player->duration()) / 60000) + ":"
                          + (timeCounter((player->duration() / 1000) % 60)));
}

void MainWindow::on_sliderProgress_sliderPressed()
{
    slider = false;
}
void MainWindow::on_sliderProgress_sliderReleased()
{
    player->setPosition(ui->sliderProgress->value());
    slider = true;
}

void MainWindow::addSong(QString tempName)
{
    if (tempName.isEmpty()) {
        tempName = QFileDialog::getOpenFileName(this, "Open file", "/", ("*.mp3 *.wav"));
    }

    if (tempName != nullptr) {
        nameOfSong.push_back(tempName);
        SongWidget *songWidget = new SongWidget(QFileInfo(tempName).completeBaseName(), index, this);
        ui->verticalLayout->insertWidget(index, songWidget);
        ui->play->setEnabled(true);
        index++;

        connect(songWidget, &SongWidget::sendData, this, &MainWindow::playSong);
        connect(songWidget->getPause(), &QPushButton::clicked, this, &MainWindow::on_pause_clicked);
        connect(songWidget->getResume(),
                &QPushButton::clicked,
                this,
                &MainWindow::on_resume_clicked);
        connect(this, &MainWindow::songStatus, songWidget, &SongWidget::resumeSong);

        connect(songWidget, &SongWidget::sendRemoveData, this, [this, songWidget](int index) {
            ui->verticalLayout->removeWidget(songWidget);

            songWidget->deleteLater();
            nameOfSong.remove(index);

            if (index == activeSong) {
                if (activeSong != 0) {
                    activeSong--;
                }
                ui->fullTime->setText("0:00");
                ui->name->setText("");
                player->stop();
                ui->play->show();
            } else if (index < activeSong) {
                if (activeSong != 0) {
                    activeSong--;
                }
            }

            if (this->index > index) {
                emit songDeleted(index);
                this->index--;
            }

            if (activeSong == 0) {
                ui->toNext->setEnabled(true);
                ui->toPrevious->setEnabled(false);
            } else if (activeSong == nameOfSong.size() - 1) {
                ui->toNext->setEnabled(false);
                ui->toPrevious->setEnabled(true);
            }
            if (nameOfSong.size() == 1) {
                ui->toNext->setEnabled(false);
                ui->toPrevious->setEnabled(false);
            }
            if (nameOfSong.size() == 0) {
                ui->toNext->setEnabled(false);
                ui->toPrevious->setEnabled(false);
                ui->play->setEnabled(false);
            }
        });

        connect(this, &MainWindow::songDeleted, songWidget, &SongWidget::checkIndex);
        connect(songWidget, &SongWidget::sendSongName, this, &MainWindow::setSongName);
    }

    if (nameOfSong.size() >= 2) {
        ui->toNext->setEnabled(true);
    }
}
void MainWindow::on_add_clicked()
{
    addSong();
}

void MainWindow::on_toNext_clicked()
{
    ++activeSong;
    ui->toPrevious->setEnabled(true);

    player->setSource(QUrl::fromLocalFile(nameOfSong[activeSong]));

    player->play();

    ui->play->hide();
    ui->resume->hide();
    ui->pause->show();
    emit songStatus(false, activeSong);
    emit songStatus(true, activeSong);

    if (activeSong == nameOfSong.size() - 1) {
        ui->toNext->setEnabled(false);
    }
}
void MainWindow::on_toPrevious_clicked()
{
    ui->toNext->setEnabled(true);
    --activeSong;

    player->setSource(QUrl::fromLocalFile(nameOfSong[activeSong]));

    player->play();

    ui->play->hide();
    ui->resume->hide();
    ui->pause->show();
    emit songStatus(false, activeSong);
    emit songStatus(true, activeSong);

    if (activeSong == 0) {
        ui->toPrevious->setEnabled(false);
    }
}

void MainWindow::on_repeat_clicked()
{
    player->setLoops(QMediaPlayer::Infinite);
    ui->repeat->hide();
    ui->repeat_3->show();
}
void MainWindow::on_repeat_3_clicked()
{
    player->setLoops(QMediaPlayer::Once);
    ui->repeat_3->hide();
    ui->repeat->show();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}
void MainWindow::dropEvent(QDropEvent *event)
{
    QStringList accepted_types;
    accepted_types << "mp3";
    foreach (const QUrl &url, event->mimeData()->urls()) {
        QString fname = url.toLocalFile();
        QFileInfo info(fname);
        if (info.exists()) {
            if (accepted_types.contains(info.suffix().trimmed(), Qt::CaseInsensitive)) {
                addSong(fname);
            }
        }
    }
}

void MainWindow::on_mediaStatusChanged(QMediaPlayer::MediaStatus mediaST)
{
    if (mediaST == QMediaPlayer::MediaStatus::EndOfMedia) {
        player->stop();

        if ((nameOfSong.size() - 1) == activeSong) {
            playSong(0);
        } else {
            on_toNext_clicked();
        }
    }
}
