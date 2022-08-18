#include "audioslider.h"

AudioSlider::AudioSlider(QMediaPlayer *&player)
    : player(player)
{
    this->setOrientation(Qt::Horizontal);

    setStyleSheet("QSlider{height: 20px;width: 441px; }"
                  "QSlider::groove:horizontal { border: 1px solid rgba(100, 100, 100, 200); "
                  "background: rgba(100, 100, 100, 200); height: 5px; border-radius: 2px; } "
                  "QSlider::sub-page:horizontal { background: white; border: 1px solid white; "
                  "height: 7px; border-radius: 2px; } QSlider::handle:horizontal { background: "
                  "transparent; width: 10px; height: 10px; margin-top: -2px; margin-bottom: -2px; "
                  "border-radius: 3px; } QSlider::handle:horizontal:hover { background: white; "
                  "border: 1px solid white; border-radius:3px; }");
}

void AudioSlider::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (orientation() == Qt::Vertical) {
            player->setPosition(minimum()
                                + ((maximum() - minimum()) * (height() - event->y())) / height());

        } else {
            player->setPosition(minimum() + ((maximum() - minimum()) * event->x()) / width());
        }

        event->accept();
    }
    QSlider::mousePressEvent(event);
}
