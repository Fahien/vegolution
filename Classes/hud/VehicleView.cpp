#include "VehicleView.h"

USING_NS_CC;

VehicleView::VehicleView(Vehicle* vehicle)
: vehicle_ {vehicle}
{}

VehicleView* VehicleView::create(Vehicle* vehicle)
{
    // Construct
    VehicleView* view {new (std::nothrow) VehicleView{vehicle}};
    // Initialize
    if (view && view->init(vehicle->getName(), ui::ImageView::TextureResType::LOCAL)) {
        view->autorelease();
        return view;
    }
    // Error
    CC_SAFE_DELETE(view);
    return nullptr;
}
