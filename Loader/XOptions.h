#ifndef __Options__H__
#define __Options__H__

struct XOptions
{
	void ToggleMeshes() const { m_bDrawMeshes = !m_bDrawMeshes; }
	void ToggleJoints() const { m_bDrawJoints = !m_bDrawJoints; }
	void ToggleWireframe() const { m_bWireFrame = !m_bWireFrame; }
	void ToggleTextured() const { m_bTextured = !m_bTextured; }
	void ToggleCulling() const { m_bCulling = !m_bCulling; }
	void ToggleGrid() const { m_bGrid = !m_bGrid; }
	void ToggleAxes() const { m_bAxes = !m_bAxes; }
	void ToggleNormals() const { m_bNormals = !m_bNormals; }
	void ToggleAnimation() const { m_bAnimation = !m_bAnimation; }
	void ToggleSampleAlphaToCoverage() const { m_bSampleAlphaToCoverage = !m_bSampleAlphaToCoverage; }
	void ToggleAutoRotation() const { m_bAutoRotate = !m_bAutoRotate; }

	static bool m_bDrawMeshes;
	static bool m_bDrawJoints;
	static bool m_bWireFrame;
	static bool m_bTextured;
	static bool m_bCulling;
	static bool m_bAxes;
	static bool m_bGrid;
	static bool m_bNormals;
	static bool m_bAnimation;
	static bool m_bSampleAlphaToCoverage;
	static bool m_bAutoRotate;
};

#endif
