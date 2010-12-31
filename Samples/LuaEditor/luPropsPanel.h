/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 harkon.kr

    Contributor(s): none yet.
-------------------------------------------------------------------------------
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/

#ifndef _luPropsPanel_h_
#define _luPropsPanel_h_

class luPhysicsProperty;
class luObjectProperty;
class luLightObjProperty;
class luEntityObjProperty;
class luProjectProperty;
class luProjectFile;
class luCameraObjProperty;

class luPropsPanel : public wxPanel
{
	wxPropertyGrid* m_prop;
	luProjectProperty* m_proj;
	luObjectProperty* m_obj;
	luPhysicsProperty* m_physics;
	luLightObjProperty* m_light;
	luEntityObjProperty* m_entity;
	luCameraObjProperty* m_camera;

	void OnPropertyGridChanged( wxPropertyGridEvent& event );

public:
	luPropsPanel(wxWindow* parent);
	~luPropsPanel();

	void updateProject(luProjectFile* project);
	void selectObject(gkGameObject* obj);

	DECLARE_EVENT_TABLE()
};

#endif //_luPropsPanel_h_