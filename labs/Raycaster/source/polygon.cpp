#include <algorithm>
#include <cmath>
#include <cstdio>
#include <limits>
#include <optional>
#include <vector>

#include <QPoint>
#include <QtTypes>

#include "../headers/polygon.h"
#include "../headers/ray.h"

Polygon::Polygon(const std::vector<QPointF>& vertices)
    : vertices_(vertices)
{
}

[[nodiscard]] const std::vector<QPointF>& Polygon::getVertices() const
{
    return vertices_;
}

void Polygon::addVertex(const QPointF& vertex)
{
    vertices_.push_back(vertex);
}

void Polygon::updateLastVertex(const QPointF& new_vertex)
{
    if (!vertices_.empty()) {
        vertices_.back() = new_vertex;
    }
}

[[nodiscard]] std::optional<QPointF> Polygon::intersectRay(const Ray& ray) const
{
    if (vertices_.size() < 2) {
        return std::nullopt;
    }

    const QPointF rayBegin = ray.getBegin();
    const QPointF rayEnd = ray.getEnd();

    std::optional<QPointF> closestIntersection;
    double minDistance = std::numeric_limits<double>::max();

    for (size_t i = 0; i < vertices_.size(); ++i) {
        const size_t next = (i + 1) % vertices_.size();
        const QPointF& p1 = vertices_[i];
        const QPointF& p2 = vertices_[next];

        if (doIntersect(rayBegin, rayEnd, p1, p2)) {
            auto intersection = lineIntersection(rayBegin, rayEnd, p1, p2);
            if (intersection) {
                const double distance = std::hypot(intersection->x() - rayBegin.x(),
                                         intersection->y() - rayBegin.y());
                if (distance < minDistance) {
                    minDistance = distance;
                    closestIntersection = intersection;
                }
            }
        }
    }

    return closestIntersection;
}

[[nodiscard]] bool Polygon::doIntersect(const QPointF& p1, const QPointF& p2,
                          const QPointF& p3, const QPointF& p4) const
{
    auto orientation = [](const QPointF& a, const QPointF& b, const QPointF& c) {
        const qreal val = ((b.y() - a.y()) * (c.x() - b.x())) -
                        ((b.x() - a.x()) * (c.y() - b.y()));
        if (val == 0) { return 0; }
        return (val > 0) ? 1 : 2;
    };

    auto onSegment = [](const QPointF& p, const QPointF& q, const QPointF& r) {
        return q.x() <= std::max(p.x(), r.x()) && q.x() >= std::min(p.x(), r.x()) &&
               q.y() <= std::max(p.y(), r.y()) && q.y() >= std::min(p.y(), r.y());
    };

    const int o1 = orientation(p1, p2, p3);
    const int o2 = orientation(p1, p2, p4);
    const int o3 = orientation(p3, p4, p1);
    const int o4 = orientation(p3, p4, p2);

    if (o1 != o2 && o3 != o4) { return true; }
    if (o1 == 0 && onSegment(p1, p3, p2)) { return true; }
    if (o2 == 0 && onSegment(p1, p4, p2)) { return true; }
    if (o3 == 0 && onSegment(p3, p1, p4)) { return true; }
    if (o4 == 0 && onSegment(p3, p2, p4)) { return true; }

    return false;
}

[[nodiscard]] std::optional<QPointF> Polygon::lineIntersection(const QPointF& p1, const QPointF& p2,
                                                const QPointF& p3, const QPointF& p4) const
{
    const double a1 = p2.y() - p1.y();
    const double b1 = p1.x() - p2.x();
    const double c1 = (a1 * p1.x()) + (b1 * p1.y());
    const double a2 = p4.y() - p3.y();
    const double b2 = p3.x() - p4.x();
    const double c2 = (a2 * p3.x()) + (b2 * p3.y());
    const double det = (a1 * b2) - (a2 * b1);
    if (det != 0) {
        const double x = (b2 * c1 - b1 * c2) / det;
        const double y = (a1 * c2 - a2 * c1) / det;
        return QPointF(static_cast<int>(x), static_cast<int>(y));
    }
    return std::nullopt;
}
