#ifndef WIDGET_H
#define WIDGET_H

#include "controller.h"
#include <QWidget>
#include <QTimer>
#include <QPixmap>
#include <QMouseEvent>
#include <QPainter>

class Widget : public QWidget {
    Q_OBJECT
   public:
    explicit Widget(QWidget* parent = nullptr);
    void setMode(Controller::Mode mode);

   protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

   private:
    void drawLightArea(QPainter* painter);
    void drawPolygons(QPainter* painter);
    void drawStaticSource(QPainter* painter);
    void drawLightMarkers(QPainter* painter);

    Controller controller_;
    QPointF lightSourcePos_;
    QPointF static_source_;
    //std::vector<QPointF> static_sources;
    bool isDrawingPolygon_ = false;
    QTimer refreshTimer_;
};
#endif // WIDGET_H
