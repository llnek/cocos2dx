#include "GameHUD .h"
#include "DataModel.h"

GameHUD* GameHUD::_sharHUD;

bool GameHUD::init()
{
	if (!Layer::init()) 
	{
		return false;
	}

	Size winSize = CCDirector::getInstance()->getWinSize();

	// Draw the background of the game HUD
	CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGB565);
	background = Sprite::create("hud.png");
	background->setScaleX (2);
	background->setAnchorPoint(ccp(0, 0));
	this->addChild(background);
	CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_Default);

	// Load the images of the towers we'll have and draw them to the game HUD layer
	Vector<String*> images;
	images.pushBack(StringMake("MachineGunTurret.png"));
	images.pushBack(StringMake("MachineGunTurret.png"));
	images.pushBack(StringMake("MachineGunTurret.png"));
	images.pushBack(StringMake("MachineGunTurret.png"));
	for (int i = 0; i < images.size(); ++i)
	{
		String* image = images.at(i);
		auto *sprite = Sprite::create(image->getCString());
		float offsetFraction = ((float)(i + 1)) / (images.size() + 1);
		sprite->setPosition(ccp(winSize.width*offsetFraction, 35));
		this->addChild(sprite);
		movableSprites.pushBack(sprite);
	}

	return true;
}

GameHUD* GameHUD::shareHUD()
{
	if (_sharHUD == NULL)
	{
		_sharHUD = GameHUD::create();
	}

	return _sharHUD;
}

void GameHUD::onEnter()
{
	Layer::onEnter();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(GameHUD::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameHUD::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameHUD::onTouchEnded, this);

	auto dispatcher = Director::getInstance()->getEventDispatcher();

	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	//dispatcher->addEventListenerWithFixedPriority(listener, 0);

	
}

bool GameHUD::onTouchBegan(Touch *touch, Event *event)
{
	Point touchLocation = this->convertToWorldSpace(this->convertTouchToNodeSpace(touch));

	Sprite * newSprite = NULL;
	for each(Sprite* sprite in this->movableSprites)
	{
		Rect pos_rect = Rect((sprite->getPositionX()-sprite->getContentSize().width/2), (sprite->getPositionY()-sprite->getContentSize().height/2), sprite->getContentSize().width, sprite->getContentSize().height);
		float xMin = pos_rect.getMinX();
		float xMax = pos_rect.getMaxX();
		float yMIn = pos_rect.getMinY();
		float yMax = pos_rect.getMaxY();
		if (pos_rect.containsPoint(touchLocation))
		{
			DataModel *m = DataModel::getModel();
			//m.gestureRecognizer.enabled = NO;
			selSpriteRange = Sprite::create("Range.png");
			selSpriteRange->setScale(4);
			this->addChild(selSpriteRange, -1);
			selSpriteRange->setPosition(sprite->getPosition());

			newSprite = Sprite::createWithTexture(sprite->getTexture()); //sprite;
			newSprite->setPosition(sprite->getPosition());
			selSprite = newSprite;
			this->addChild(newSprite);
		}
	}
		
	return true;
}

void GameHUD::onTouchMoved(Touch* touch,Event* event) 
{
	Point touchLocation = this->convertToWorldSpace(this->convertTouchToNodeSpace(touch));

	Point oldTouchLocation = touch->getPreviousLocationInView();
	oldTouchLocation = Director::getInstance()->convertToGL(oldTouchLocation);
	oldTouchLocation = this->convertToNodeSpace(oldTouchLocation);

	Point translation = ccpSub(touchLocation,oldTouchLocation);

	if (selSprite) 
	{
		Point newPos = selSprite->getPosition()+translation;
		selSprite->setPosition(newPos);
		selSpriteRange->setPosition(newPos);

		DataModel *m = DataModel::getModel();
		Point touchLocationInGameLayer = m->_gameLayer->convertTouchToNodeSpace(touch);

		BOOL isBuildable = m->_gameLayer->canBuildOnTilePosition(touchLocationInGameLayer);
		if (isBuildable) 
		{
			selSprite->setOpacity(200);
		}
		else 
		{
			selSprite->setOpacity(50);
		}
	}
}

void GameHUD::onTouchEnded(Touch* touch, Event* event)
{
	Point touchLocation = this->convertTouchToNodeSpace(touch);
	DataModel *m = DataModel::getModel();

	if (selSprite) 
	{
		Rect backgroundRect = Rect(background->getPositionX(),
			background->getPositionY(),
			background->getContentSize().width,
		 	background->getContentSize().height);

		if (!backgroundRect.containsPoint(touchLocation) && m->_gameLayer->canBuildOnTilePosition(touchLocation))
		{
			Point touchLocationInGameLayer = m->_gameLayer->convertTouchToNodeSpace(touch);
			m->_gameLayer->addTower(touchLocationInGameLayer);
		}

		this->removeChild(selSprite,true);
		selSprite = NULL;
		this->removeChild(selSpriteRange,true);
		selSpriteRange = NULL;
	}
}

 