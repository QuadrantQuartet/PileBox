#include "BoxScene.h"

BoxScene::BoxScene(QObject* parent)
    : QGraphicsScene(parent), world(b2Vec2(0.f, 10.f)) {
    // 初始化地面
    ground = new BoxStaticItem(*this, 0., 250., 5000., 500.);
    ground->item->setBrush(Qt::green);

    // 初始化计时器
    timer = new QTimer(this);
    timer->start(1000 / FRAMES_PER_SECOND);
    connect(timer, &QTimer::timeout, this, &BoxScene::updateWorld);
}

BoxItem BoxScene::createBody(qreal x, qreal y, qreal w, qreal h, float density,
                             float friction) {
    auto body = BoxDynamicItem(*this, x, y, w, h, density, friction);
    bodies.push_back(body);
    return body;
}

void BoxScene::updateWorld() {
    world.Step(TIME_STEP, 10, 5);
    world.ClearForces();

    for (auto& body : bodies) {
        body.update();
    }

    this->update();
}
BoxScene::~BoxScene() {
    delete timer;
    delete ground;
}

#ifdef QT_DEBUG
bool BoxItem::debugMode = true;
#else
bool BoxItem::debugMode = false;
#endif

BoxItem::BoxItem(BoxScene& parent, b2Body* body,
                 QAbstractGraphicsShapeItem* item)
    : body(body), item(item) {
    parent.addItem(item);

    text = new QGraphicsTextItem(item);

    this->update();
}

void BoxItem::update() const {
    auto x = toPixel(body->GetPosition().x);
    auto y = toPixel(body->GetPosition().y);

    if (debugMode)
        text->setPlainText(QString("(%1, %2)").arg(x).arg(y));
    else
        text->hide();

    item->setPos(x, y);
    item->setRotation(body->GetAngle() * 180 / b2_pi);
}

BoxStaticItem::BoxStaticItem(BoxScene& parent, qreal x, qreal y, qreal w,
                             qreal h)
    : BoxItem(parent,
              newBody(&parent.world, toMeter(x), toMeter(y), toMeter(w),
                      toMeter(h)),
              new QGraphicsRectItem(-w / 2, -h / 2, w, h)) {}

b2Body* BoxStaticItem::newBody(b2World* world, float x, float y, float w,
                               float h) {
    b2BodyDef bodyDef;
    bodyDef.position.Set(x, y);

    auto* body = world->CreateBody(&bodyDef);

    b2PolygonShape staticBox;
    staticBox.SetAsBox(w / 2, h / 2);
    body->CreateFixture(&staticBox, 0.0f);
    return body;
}

BoxDynamicItem::BoxDynamicItem(BoxScene& parent, qreal x, qreal y, qreal w,
                               qreal h, float density, float friction)
    : BoxItem(parent,
              newBody(&parent.world, toMeter(x), toMeter(y), toMeter(w),
                      toMeter(h), density, friction),
              new QGraphicsRectItem(-w / 2, -h / 2, w, h)) {}

b2Body* BoxDynamicItem::newBody(b2World* world, float x, float y, float w,
                                float h, float density, float friction) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);
    auto* body = world->CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(w / 2, h / 2);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = density;
    fixtureDef.friction = friction;

    body->CreateFixture(&fixtureDef);

    return body;
}
