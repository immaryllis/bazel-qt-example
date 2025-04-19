#include "../headers/ray.h"
#include <QPoint>
#include <cmath>

Ray::Ray(const QPointF& begin, const QPointF& end, double angle)
    : begin_(begin), end_(end), angle_(angle)
{
}

[[nodiscard]] QPointF Ray::getBegin() const {
    return begin_;
}
[[nodiscard]] QPointF Ray::getEnd() const {
    return end_;
}
[[nodiscard]] double Ray::getAngle() const {
    return angle_;
}

void Ray::setBegin(const QPointF& begin) {
    begin_ = begin;
}
void Ray::setEnd(const QPointF& end) {
    end_ = end;
}
void Ray::setAngle(double angle) {
    angle_ = angle;
}

[[nodiscard]] Ray Ray::rotate(double angle) const
{
    const double dx = end_.x() - begin_.x();
    const double dy = end_.y() - begin_.y();
    const double cos = std::cos(angle);
    const double sin = std::sin(angle);
    const double newX = (begin_.x() + dx * cos) - (dy * sin);
    const double newY = (begin_.y() + dx * sin) + (dy * cos);
    return {begin_, QPointF(newX, newY), angle_ + angle};
}
