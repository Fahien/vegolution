#include "ShotView.h"
#include "BulletView.h"

USING_NS_CC;

ShotView* ShotView::create(std::string name, MainActor* actor, float width)
{
    // Construct
    ShotView* view {new (std::nothrow) ShotView{actor, width}};
    // Initialize
    if (view && view->init(name, ui::ImageView::TextureResType::LOCAL)) {
        view->autorelease();
        return view;
    }
    // Error
    CC_SAFE_DELETE(view);
    return nullptr;
}

ShotView::ShotView(MainActor* actor, float width)
: actor_ {actor}
, width_ {width}
, open_  {false}
{}

void ShotView::toggle()
{
    log("Toggling shot view");
    if (open_) {
        // Remove all children and close
        removeAllChildrenWithCleanup(true);
        open_ = false;
    }
    else {
        createChildren();
        open_ = true;
    }
}

void ShotView::createChildren()
{
    // Create children to show
    ui::Layout* layout {ui::Layout::create()};
    Size size {getContentSize()};
    layout->setContentSize(Size{width_, size.height});
    layout->setPositionX(size.width);
    layout->setLayoutType(ui::Layout::Type::HORIZONTAL);
    for (Bullet* bullet : actor_->getBullets()) {
        log("Creating a bullet view %s", bullet->getName().c_str());
        BulletView* view = BulletView::create(bullet);
        view->setTouchEnabled(true);
        view->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
            BulletView* view {static_cast<BulletView*>(sender)};
            switch(type) {
            case ui::Widget::TouchEventType::ENDED :
                actor_->setBullet(view->getBullet());
                toggle();
                break;
            default: break;
            }
            return true;
        });        
        layout->addChild(view);
    }
    addChild(layout);
}
