#ifndef TEST_BOXSCENE_H
#define TEST_BOXSCENE_H

#include <box2d/box2d.h>

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QTimer>
#include <vector>

constexpr int FRAMES_PER_SECOND = 60;
constexpr float TIME_STEP = 1.0f / FRAMES_PER_SECOND;
/**
 * 像素与物理单位的比例。规定：qreal 为像素单位，float 为物理单位。
 */
constexpr int PIXEL_PER_METER = 100;

inline qreal toPixel(float meter) {
    return static_cast<qreal>(meter) * PIXEL_PER_METER;
}

inline float toMeter(qreal pixel) {
    return static_cast<float>(pixel) / PIXEL_PER_METER;
}

class BoxItem;
class BoxStaticItem;
class BoxDynamicItem;

class BoxScene : public QGraphicsScene {
    Q_OBJECT
  private:
    QTimer* timer;

    /**
     * 更新世界。在计时器中定时运行。
     * */
    void updateWorld();

  public:
    b2World world;
    BoxStaticItem* ground;
    std::vector<BoxItem> bodies;
    explicit BoxScene(QObject* parent = nullptr);
    ~BoxScene() override;

    BoxItem createBody(qreal x, qreal y, qreal w, qreal h,
                                           float density, float friction);
};

class BoxItem {
  public:
    b2Body* body;
    QAbstractGraphicsShapeItem* item;

    static bool debugMode;
    QGraphicsTextItem* text;

    explicit BoxItem(BoxScene& parent, b2Body* body,
                     QAbstractGraphicsShapeItem* item);

    void update() const;
};

class BoxStaticItem : public BoxItem {
  private:
    static b2Body* newBody(b2World* world, float x, float y, float w, float h);

  public:
    explicit BoxStaticItem(BoxScene& parent, qreal x, qreal y, qreal w,
                           qreal h);
};

class BoxDynamicItem : public BoxItem {
  private:
    static b2Body* newBody(b2World* world, float x, float y, float w, float h,
                           float density, float friction);

  public:
    explicit BoxDynamicItem(BoxScene& parent, qreal x, qreal y, qreal w,
                            qreal h, float density, float friction);
};

#endif  // TEST_BOXSCENE_H
