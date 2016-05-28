#include "BulletView.h"

USING_NS_CC;

BulletView::BulletView(Bullet* bullet)
: bullet_ {bullet}
{}

BulletView* BulletView::create(Bullet* bullet)
{
    // Construct
    BulletView* view {new (std::nothrow) BulletView{bullet}};
    // Initialize
    if (view && view->init(bullet->getName(), ui::ImageView::TextureResType::LOCAL)) {
        view->autorelease();
        return view;
    }
    // Error
    CC_SAFE_DELETE(view);
    return nullptr;
}
