#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <QPoint>
#include <optional>
#include "ray.h"

class Polygon
{
   public:
    Polygon(const std::vector<QPointF>& vertices = {});

    [[nodiscard]] const std::vector<QPointF>& getVertices() const;
    void addVertex(const QPointF& vertex);
    void updateLastVertex(const QPointF& new_vertex);

    [[nodiscard]] std::optional<QPointF> intersectRay(const Ray& ray) const;

   private:
    [[nodiscard]] bool doIntersect(const QPointF& p1, const QPointF& p2,
                     const QPointF& p3, const QPointF& p4) const;
    [[nodiscard]] std::optional<QPointF> lineIntersection(const QPointF& p1, const QPointF& p2,
                                           const QPointF& p3, const QPointF& p4) const;

    std::vector<QPointF> vertices_;
};

#endif // POLYGON_H
