#ifndef __EXPLOSION_H__
#define __EXPLOSION_H__

#include "cocos2d.h"

class Explosion : public cocos2d::Sprite
{
public:
	Explosion(cocos2d::Animate* animate);
	~Explosion();
	static Explosion* create(cocos2d::Animate* animate);
	void animate();
private:
	cocos2d::Sequence* sequence_;
};

#endif // __EXPLOSION_H__
