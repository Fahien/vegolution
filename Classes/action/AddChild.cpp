#include "AddChild.h"

USING_NS_CC;

AddChild::AddChild()
        : addedNode_{ nullptr }
{ }

AddChild::~AddChild()
{
    CC_SAFE_RELEASE(addedNode_);
}


AddChild* AddChild::create(Node* addedNode)
{
    AddChild* addChild{ new(std::nothrow) AddChild{ }};
    if (addChild && addChild->initWithTarget(addedNode)) {
        addChild->autorelease();
        return addChild;
    }
    CC_SAFE_DELETE(addChild);
    return nullptr;
}

bool AddChild::initWithTarget(Node* addedNode)
{
    CCASSERT(addedNode != nullptr, "Added Node can't be NULL");
    addedNode->retain();
    addedNode_ = addedNode;
    return true;
}

void AddChild::update(float time)
{
    CC_UNUSED_PARAM(time);
    _target->addChild(addedNode_);
}