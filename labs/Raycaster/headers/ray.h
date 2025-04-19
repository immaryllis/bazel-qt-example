#ifndef RAY_H
#define RAY_H

#include <QPoint>
#include <cmath>

class Ray
{
   public:
    Ray(const QPointF& begin, const QPointF& end, double angle = 0);

    [[nodiscard]] QPointF getBegin() const;
    [[nodiscard]] QPointF getEnd() const;
    [[nodiscard]] double getAngle() const;

    void setBegin(const QPointF& begin);
    void setEnd(const QPointF& end);
    void setAngle(double angle);

    [[nodiscard]] Ray rotate(double angle) const;

   private:
    QPointF begin_;
    QPointF end_;
    double angle_;
};

#endif // RAY_H
