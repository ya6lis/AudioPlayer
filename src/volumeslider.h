#ifndef VOLUMESLIDER_H
#define VOLUMESLIDER_H

#include <QAudioOutput>

#include <QMouseEvent>
#include <QSlider>

class VolumeSlider : public QSlider
{
    Q_OBJECT
    QAudioOutput *&audioOutput;

protected:
    void mousePressEvent(QMouseEvent *event);

public:
    VolumeSlider(QAudioOutput *&audioOutput);
};

#endif // VOLUMESLIDER_H
