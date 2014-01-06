#include "OISInputManager.h"
#include "OISFactoryCreator.h"
#include "OISKeyboard.h"
#include "OISMouse.h"
#include "OISJoyStick.h"

#ifndef OIS_NaClInputManager_H
#define OIS_NaClInputManager_H

namespace OIS
{

class NaClKeyboard : public Keyboard
{
	char mKeyBuffer[256];
public:
	NaClKeyboard(InputManager* creator);
	
	virtual void setBuffered(bool buffered) {}
	virtual void capture(){}
	virtual Interface* queryInterface(Interface::IType type) {return 0;}
	virtual void _initialize() {}
	virtual bool isKeyDown(KeyCode key) const;	
	virtual const std::string& getAsString(KeyCode kc);
	
	virtual void copyKeyStates(char keys[256]) const;

	void injectKey(int action, int uniChar, int keyCode);
	void resetBuffer();
	static KeyEvent convertKey(int action, int keyCode);
};

class NaClMouse : public Mouse
{
public:
	NaClMouse(InputManager *creator, bool buffered, bool grab, bool hide);
	virtual ~NaClMouse();

	/** @copydoc Object::setBuffered */
		void setBuffered(bool buffered);

	/** @copydoc Object::capture */
		void capture();

	/** @copydoc Object::queryInterface */
		Interface* queryInterface(Interface::IType type);

	/** @copydoc Object::_initialize */
		void _initialize();

	/** Notify that the mouse has gained/lost focus */
		void changedFocus();

		/** Process mouse events from the window system */
		//void handleMouse( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

		//void _setHandle(HWND hWnd) {mHandle = hWnd;}


private:
	int mLastX, mLastY;
	bool mMouseInit, mMouseMoved;
	bool mGrab, mDoGrab;
	bool mHide, mDoHide;

	//HWND mHandle;
	//typedef std::vector<WNMouseEvent> WNMouseEventVector;
	//WNMouseEventVector mEvents;
};


class NaClInputManager : public InputManager, public FactoryCreator
{	
	NaClKeyboard* mKeyboard;
	NaClMouse* mMouse;
public:
	NaClInputManager();
	~NaClInputManager();

	void _initialize(ParamList &paramList);

	DeviceList freeDeviceList();		
	int totalDevices(Type iType);	
	int freeDevices(Type iType);
	int getNumberOfDevices( Type iType ){ return totalDevices(iType);}

	bool vendorExist(Type iType, const std::string & vendor);

	Object* createObject(InputManager* creator, Type iType, bool bufferMode, const std::string & vendor);
	void destroyObject(Object* obj);
};
}
#endif