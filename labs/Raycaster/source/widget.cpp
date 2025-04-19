#include "../headers/widget.h"
#include "../headers/polygon.h"

#include <QBrush>
#include <QColor>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QTimer>
#include <QWidget>
#include <QtGlobal>
#include <QtTypes>

const qreal kRadius = 200.;  // Radius of static light

Widget::Widget(QWidget* parent) : QWidget(parent) {
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    lightSourcePos_ = QPointF(512, 480);
    controller_.setLightSource(lightSourcePos_);
}

void Widget::setMode(Controller::Mode mode) {
    controller_.setMode(mode);
    update();
}

void Widget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor(50, 50, 50));
    drawLightMarkers(&painter);
    drawLightArea(&painter);
    drawPolygons(&painter);
}

void Widget::drawLightMarkers(QPainter* painter) {
    const std::vector<QPointF> sources = controller_.createSources(8);
    for (auto source : sources) {
        painter->setBrush(Qt::yellow);
        painter->setPen(Qt::black);
        painter->drawEllipse(source, 5, 5);
    }

    const std::vector<QPointF> static_sources = controller_.getStaticSources();
    for (auto source : static_sources) {
        painter->setBrush(Qt::white);
        painter->setPen(Qt::black);
        painter->drawEllipse(source, 6, 6);
    }
}

void Widget::drawLightArea(QPainter* painter) {
    if (controller_.getMode() == Controller::Mode::Light) {
        const std::vector<QPointF> points = controller_.createSources(8);
        for (auto point : points) {
            controller_.setLightSource(point);
            const auto rays = controller_.castRays(point);
            const Polygon lightArea = controller_.createLightArea();
            const auto& vertices = lightArea.getVertices();
            if (!vertices.empty()) {
                QRadialGradient gradient(point, 20000, point);
                gradient.setColorAt(0, QColor(255, 255, 200, 75));
                gradient.setColorAt(1, QColor(255, 255, 200, 0));
                painter->setPen(Qt::NoPen);
                painter->setBrush(gradient);
                painter->drawPolygon(
                    lightArea.getVertices().data(),
                    static_cast<int>(lightArea.getVertices().size()));
            }
        }
        QPointF point = controller_.getLightSource();
        const std::vector<QPointF> static_sources = controller_.getStaticSources();
        for (const auto& static_source : static_sources) {
            controller_.setLightSource(static_source);
            const auto rays = controller_.castRays(static_source);
            const Polygon lightArea = controller_.createLightArea();
            const auto& vertices = lightArea.getVertices();
            if (!vertices.empty()) {
                QRadialGradient gradient(static_source, 200, static_source);
                gradient.setColorAt(0, QColor(255, 255, 200, 75));
                gradient.setColorAt(1, QColor(255, 255, 200, 0));
                painter->setPen(Qt::NoPen);
                painter->setBrush(gradient);
                painter->drawPolygon(
                    lightArea.getVertices().data(),
                    static_cast<int>(lightArea.getVertices().size()));
            }
        }
        controller_.setLightSource(point);
    }
}

void Widget::drawPolygons(QPainter* painter) {
    painter->setPen(Qt::white);
    painter->setBrush(Qt::NoBrush);
    for (const auto& polygon : controller_.getPolygons()) {
        const auto& vertices = polygon.getVertices();
        if (vertices.size() > 1) {
            painter->drawPolygon(vertices.data(), static_cast<int>(vertices.size()));
        }
    }
}

void Widget::mousePressEvent(QMouseEvent* event) {
    if (controller_.getMode() == Controller::Mode::Light) {
        isDrawingPolygon_ = false;
        lightSourcePos_ = event->pos();
        controller_.setLightSource(lightSourcePos_);
    } else if (controller_.getMode() == Controller::Mode::Static_lights) {
        isDrawingPolygon_ = false;
        // static_source_ = event->pos();
        controller_.addStaticSource(event->pos());
    } else {
        if (event->button() == Qt::LeftButton) {
            if (isDrawingPolygon_) {
                controller_.addVertexToLastPolygon(event->pos());
            } else {
                controller_.addPolygon(Polygon({event->pos()}));
                controller_.addVertexToLastPolygon(event->pos());
                isDrawingPolygon_ = true;
            }
        } else if (event->button() == Qt::RightButton) {
            isDrawingPolygon_ = false;
        }
    }
    update();
}

void Widget::mouseMoveEvent(QMouseEvent* event) {
    if (controller_.getMode() == Controller::Mode::Light) {
        lightSourcePos_ = event->pos();
        controller_.setLightSource(lightSourcePos_);
    } else if (isDrawingPolygon_) {
        controller_.updateLastPolygon(event->pos());
    }
    update();
}
