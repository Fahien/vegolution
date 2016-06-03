#ifndef  __VEGOLUTION_H__
#define  __VEGOLUTION_H__

#include "cocos2d.h"
#include "data/DataManager.h"

/**
 * @brief	The Vegolution Application
 * The reason for implement as private inheritance is to hide some interface call by Director.
 */
class Vegolution : private cocos2d::Application
{
public:
    Vegolution();
    virtual ~Vegolution();

    virtual void initGLContextAttrs();

    /**
     * @brief  Implement Director and Scene init code here.
     * @return true   Initialize success, app continue.
     * @return false  Initialize failed, app terminate.
     */
    virtual bool applicationDidFinishLaunching();

    /**
     * @brief  The function be called when the application enter background
     * @param  the pointer of the application
     */
    virtual void applicationDidEnterBackground();

    /**
     * @brief  The function be called when the application enter foreground
     * @param  the pointer of the application
     */
    virtual void applicationWillEnterForeground();

	/**
	 * @return  The application version
	 */
	virtual std::string getVersion() override { return std::string{"0.1"}; }

	/**
	 * @return	The Director instance pointer
	 */
    inline cocos2d::Director*  getDirector() const  { return director_; }

	/**
	 * @return  The FileUtils instance pointer
	 */
    inline cocos2d::FileUtils* getFileUtils() const { return fileUtils_; }

	/**
	 * @return	The DataManager instance pointer
	 */
	DataManager* getDataManager() { return &dataManager_; }

private:
    cocos2d::Director*  director_;
    cocos2d::FileUtils* fileUtils_;
    DataManager         dataManager_;
};

#endif // __VEGOLUTION_H__
