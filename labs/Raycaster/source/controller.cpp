#include <cmath>
#include <iterator>
#include <algorithm>
#include <QtMath>

#include "../headers/controller.h"

Controller::Controller()
{
    const Polygon boundingPolygon({
      QPointF(0., 0.),
      QPointF(3000., 0.),
      QPointF(3000., 1024.),
      QPointF(0., 1024.)
    });
    polygons_.push_back(boundingPolygon);
}

const std::vector<Polygon>& Controller::getPolygons() const
{
    return polygons_;
}

void Controller::addPolygon(const Polygon& polygon)
{
    polygons_.push_back(polygon);
}

void Controller::addVertexToLastPolygon(const QPointF& new_vertex)
{
    if (!polygons_.empty()) {
        polygons_.back().addVertex(new_vertex);
    }
}

void Controller::updateLastPolygon(const QPointF& new_vertex)
{
    if (!polygons_.empty()) {
        polygons_.back().updateLastVertex(new_vertex);
    }
}

QPointF Controller::getLightSource() const
{
    return light_source_;
}

void Controller::setLightSource(const QPointF& light_source)
{
    light_source_ = light_source;
}

std::vector<Ray> Controller::castRays(const QPointF& light_source_) const
{
    std::vector<Ray> rays;
    const double smallAngle = 0.00001;
    for (const auto& polygon : polygons_) {
        for (const auto& vertex : polygon.getVertices()) {
                const Ray mainRay(light_source_, vertex);
                rays.push_back(mainRay);
                rays.push_back(mainRay.rotate(smallAngle));
                rays.push_back(mainRay.rotate(-smallAngle));
        }
    }
    return rays;
}

std::vector<Ray> Controller::castRays() const {
    return castRays(light_source_);
}

void Controller::intersectRays(std::vector<Ray>* rays) const
{
    for (auto& ray : *rays) {
        const QPointF rayBegin = ray.getBegin();
        const QPointF originalEnd = ray.getEnd();
        double minDistance = std::hypot(originalEnd.x() - rayBegin.x(),
                                        originalEnd.y() - rayBegin.y());

        for (const auto& polygon : polygons_) {
            auto intersection = polygon.intersectRay(ray);
            if (intersection) {
                const double distance = std::hypot(intersection->x() - rayBegin.x(),
                                         intersection->y() - rayBegin.y());
                if (distance < minDistance) {
                    minDistance = distance;
                    ray.setEnd(*intersection);
                }
            }
        }
    }
}

void Controller::removeAdjacentRays(std::vector<Ray>* rays) const //NOLINT
{
    if (rays->size() <= 1) { return; }
    std::ranges::sort(
        *rays, [this](const Ray& a, const Ray& b) { return a.getAngle() < b.getAngle(); });
    const double distanceThreshold = 0.00001;
    auto it = rays->begin();
    while (it != rays->end()) {
        auto next = std::next(it);
        if (next != rays->end()) {
            const QPointF p1 = it->getEnd();
            const QPointF p2 = next->getEnd();
            const double distance = std::hypot(p1.x() - p2.x(), p1.y() - p2.y());
            if (distance < distanceThreshold) {
                it = rays->erase(next);
                continue;
            }
        }
         std::advance(it, 1);
    }
}

Polygon Controller::createLightArea() const {
    std::vector<QPointF> lightAreaVertices;
    const int rayCount = 360;
    for (int i = 0; i < rayCount; ++i) {
        const double angle = 2 * M_PI * i / rayCount;
        const QPointF direction(cos(angle), sin(angle));
        const Ray ray(light_source_, light_source_ + direction * 1000.0);
        QPointF endPoint = light_source_ + direction * 1000.0;
        for (const auto& polygon : polygons_) {
            auto intersection = polygon.intersectRay(ray);
            if (intersection) {
                const double newDist = QPointF::dotProduct(*intersection - light_source_,
                                                     *intersection - light_source_);
                const double currentDist = QPointF::dotProduct(endPoint - light_source_,
                                                         endPoint - light_source_);
                if (newDist < currentDist) {
                    endPoint = *intersection;
                }
            }
        }
        lightAreaVertices.push_back(endPoint);
    }
    return {lightAreaVertices};
}

Polygon Controller::createLightArea(const std::vector<Ray>& rays) const { //NOLINT
    std::vector<QPointF> vertices;
    for (const auto& ray : rays) {
        vertices.push_back(ray.getEnd()); //NOLINT
    }
    return {vertices};
}

Polygon Controller::createLightArea(const QPointF& light_source) const {
    auto rays = castRays(light_source);
    intersectRays(&rays);
    removeAdjacentRays(&rays);
    return createLightArea(rays);
}

std::vector<QPointF> Controller::createSources(const QPointF& light_source,
                                               int quality) const {
    std::vector<QPointF> result;
    result.push_back(light_source_);
    const double kAngleStep = 2 * M_PI / quality;
    const double offsetAmount = 18;

    for (int i = 0; i < quality; ++i) {
        const double angle = kAngleStep * i;
        const QPointF offset(offsetAmount * std::cos(angle),
                       offsetAmount * std::sin(angle));
        result.push_back(offset + light_source);
    }
    return result;
}

std::vector<QPointF> Controller::createSources(int quality) const {
    return createSources(light_source_, quality);
}


std::vector<QPointF> Controller::getLightSources() const
{
    return light_sources_;
}

void Controller::setLightSources(const std::vector<QPointF>& sources)
{
    light_sources_ = sources;
}

void Controller::updateLightSourcePosition(const QPointF& offset)
{
    for (auto& light : light_sources_) {
        light += offset;
    }
}

void Controller::setMode(Mode mode) {
    mode_ = mode;
}

Controller::Mode Controller::getMode() const {
    return mode_;
}

std::vector<QPointF> Controller::getStaticSources() const
{
    return static_sources_;
}

void Controller::addStaticSource(QPointF source)
{
    static_sources_.push_back(source);
}
