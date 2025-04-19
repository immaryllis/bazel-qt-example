#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <vector>
#include <QPointF>
#include "polygon.h"
#include "ray.h"


class Controller
{
   public:
    Controller();

    const std::vector<Polygon>& getPolygons() const;
    void addPolygon(const Polygon& polygon);
    void addVertexToLastPolygon(const QPointF& new_vertex);
    void updateLastPolygon(const QPointF& new_vertex);

    std::vector<Ray> castRays() const;
    std::vector<Ray> castRays(const QPointF& light_source_) const;
    void intersectRays(std::vector<Ray>* rays) const;
    void removeAdjacentRays(std::vector<Ray>* rays) const;
    Polygon createLightArea() const;
    Polygon createLightArea(const std::vector<Ray>& rays) const;
    Polygon createLightArea(const QPointF& light_source) const;

    std::vector<QPointF> createSources(const QPointF& light_source,
                                                  int quality) const;
    std::vector<QPointF> createSources(int quality) const;

    std::vector<QPointF> getLightSources() const;
    void setLightSources(const std::vector<QPointF>& sources);
    void updateLightSourcePosition(const QPointF& offset);

    enum class Mode : uint8_t {Light, Polygons, Static_lights};
    void setMode(Mode mode);
    Mode getMode() const;

    QPointF getLightSource() const;
    void setLightSource(const QPointF& light_source);


    std::vector<QPointF> getStaticSources() const;
    void addStaticSource(QPointF source);

   private:
    std::vector<Polygon> polygons_;
    QPointF light_source_;
    std::vector<QPointF> light_sources_;
    std::vector<QPointF> static_sources_;
    Mode mode_ = Mode::Light;
};

#endif // CONTROLLER_H

