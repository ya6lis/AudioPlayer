#ifndef AUDIOSLIDER_H
#define AUDIOSLIDER_H

#include <QMediaPlayer>
#include <QMouseEvent>
#include <QSlider>

class AudioSlider : public QSlider
{
    Q_OBJECT
    QMediaPlayer *&player;

protected:
    void mousePressEvent(QMouseEvent *event);

public:
    AudioSlider(QMediaPlayer *&player);
};

#endif // AUDIOSLIDER_H
