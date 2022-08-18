#include "volumeslider.h"

VolumeSlider::VolumeSlider(QAudioOutput *&audioOutput)
    : audioOutput(audioOutput)
{
    this->setOrientation(Qt::Horizontal);

    setStyleSheet("QSlider{height: 121px;width: 20px; }"
                  "QSlider::groove:horizontal { border: 1px solid rgba(100, 100, 100, 200); "
                  "background: rgba(100, 100, 100, 200); height: 5px; border-radius: 2px; } "
                  "QSlider::sub-page:horizontal { background: white; border: 1px solid white; "
                  "height: 7px; border-radius: 2px; } QSlider::handle:horizontal { background: "
                  "transparent; width: 10px; height: 10px; margin-top: -2px; margin-bottom: -2px; "
                  "border-radius: 3px; } QSlider::handle:horizontal:hover { background: white; "
                  "border: 1px solid white; border-radius:3px; }");
}

void VolumeSlider::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (orientation() == Qt::Vertical) {
            audioOutput->setVolume(
                minimum() + ((maximum() - minimum()) * (height() - event->y())) / height());

        } else {
            audioOutput->setVolume(minimum() + ((maximum() - minimum()) * event->x()) / width());
        }

        event->accept();
    }
    QSlider::mousePressEvent(event);
}
