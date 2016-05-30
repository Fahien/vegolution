#include "BodyView.h"
#include "VehicleView.h"

USING_NS_CC;

BodyView* BodyView::create(std::string name, MainActor* actor, float width)
{
    // Construct
    BodyView* view {new (std::nothrow) BodyView{actor, width}};
    // Initialize
    if (view && view->init(name, ui::ImageView::TextureResType::LOCAL)) {
        view->autorelease();
        return view;
    }
    // Error
    CC_SAFE_DELETE(view);
    return nullptr;
}

BodyView::BodyView(MainActor* actor, float width)
: actor_ {actor}
, width_ {width}
, open_  {false}
{}

void BodyView::toggle()
{
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

void BodyView::createChildren()
{
    // Create children to show
    ui::Layout* layout {ui::Layout::create()};
    Size size {getContentSize()};
    layout->setContentSize(Size{width_, size.height});
    layout->setPositionX(size.width);
    layout->setLayoutType(ui::Layout::Type::HORIZONTAL);
    for (Vehicle* vehicle : actor_->getVehicles()) {
        VehicleView* view = VehicleView::create(vehicle);
        view->setTouchEnabled(true);
        view->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type) {
            VehicleView* view {static_cast<VehicleView*>(sender)};
            switch(type) {
            case ui::Widget::TouchEventType::ENDED :
                actor_->switchVehicle();
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
