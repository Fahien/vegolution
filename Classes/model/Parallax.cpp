#include "Parallax.h"

USING_NS_CC;

// Constructor
Parallax::Parallax(MainActor* actor)
: actor_{ actor }
{}

// Create method
Parallax* Parallax::create(const std::string name, MainActor* actor)
{
	Parallax* parallax{ new (std::nothrow) Parallax{ actor } };

    // Initialize
    if (parallax && parallax->init()) {
        parallax->autorelease();
        parallax->createBackgrounds(name);
        return parallax;
    }

    CC_SAFE_DELETE(parallax);
    return nullptr;
}

// Create method for backgorunds
void Parallax::createBackgrounds(const std::string foldername)
{
	Director* director{ Director::getInstance() };
	Vec2 origin{ director->getVisibleOrigin() };
	Size visibleSize{ director->getVisibleSize() };
	float centerX{ origin.x + visibleSize.width / 2 };
    // Get the scale factor
    scale_ = director->getContentScaleFactor();

    // Background anchor point
	Vec2 anchor{ 0.5f, 0.0f };

    // Get file utils
	FileUtils* fileUtils{ FileUtils::getInstance() };

    // Load backgrounds
    for (unsigned i {0}; i < backgroundMax; ++i) {
		std::vector<Actor*> row{};

        for (unsigned j {0}; j < backgroundMax; j++) {
            // Compose a file name
            std::string name = StringUtils::format("%sback%d-%d.png", foldername.c_str(), i, j);
            // Check if file exists
            if (!fileUtils->isFileExist(name)) break;

            // Create a background
			Actor* background{ Actor::create(name) };
            log("\t%s", name.c_str());

            // Set the right position
            background->setAnchorPoint(anchor);
            float width = background->getContentSize().width;
            float x = centerX + j * width - j * 2;
            background->setPositionX(x);

            // Do not collide
			PhysicsBody* body{ background->getPhysicsBody() };
            body->setCategoryBitmask(0x00);
            body->setCollisionBitmask(0x00);
            body->setGravityEnable(false);

            // Insert in a row
            row.push_back(background);
            // Add to the parallax node
            addChild(background);

            // If it's the first row, go ahead
            if (i == 0) {
                float scaleX = visibleSize.width / width;
                background->setScaleX(scaleX);
                break;
            }

            // If it's near update the Z
            if (i > backgroundMax - 2) background->setGlobalZOrder(3);
        }

        // Insert the created row
        if (row.size() > 0) backgrounds_.push_back(row);
    }
}

// Update method
void Parallax::update(float delta)
{
	float velFactor{ 1.0f };
	for (unsigned i{ 0 }; i < backgrounds_.size(); ++i) {
        // Get a row
        std::vector<Actor*> row = backgrounds_[i];
        // Get the row size
        unsigned rowSize = row.size();
        for (unsigned j {0}; j < rowSize; ++j) {
            // Get a background
            Actor* background = row[j];
            // Set first background position
            if (velFactor == 1.0f) {
                float newX = actor_->getPositionX() + actor_->getOffsetX() + actor_->getVehicle()->getPositionX();
                background->setPositionX(newX);
                break;
            }
            // Set other backgrounds velocity
            else if (velFactor != 0.0f) {
				Vec2 velocity{ actor_->getPhysicsBody()->getVelocity() };
                velocity *= velFactor;
                velocity.y = 0.0f;
                background->getPhysicsBody()->setVelocity(velocity);
            }
            // Infinite scrolling logic
            float x = background->getPositionX();
            float width = background->getContentSize().width;
            float newX = actor_->getPositionX() + actor_->getOffsetX() + actor_->getVehicle()->getPositionX();
            // If it is going off the screen
            if (newX - x >= width - 1.0f) {
                background->setPositionX(x + (width - 0.5f * rowSize) * rowSize);
            }
        }
        velFactor -= 0.25f;
    }
}
