
#ifndef BULLET_BLEND_READER_H
#define BULLET_BLEND_READER_H



class	BulletBlendReader
{
	struct _BlendFile* m_bf;

	class btDynamicsWorld* m_destinationWorld;

public:

	BulletBlendReader(class btDynamicsWorld* destinationWorld);

	virtual ~BulletBlendReader();

	int		openFile(const char* fileName);

	void	convertAllObjects();

	///for each Blender Object, this method will be called to convert/retrieve data from the bObj
	virtual	void convertSingleObject(struct _bObj* object);

	virtual	void convertSingleMesh(struct _bMesh* mesh);

	//after each object is converter, including collision object, create a graphics object (and bind them)
	virtual void createGraphicsObject(_bObj* tmpObject, class btCollisionObject* bulletObject)=0;

	virtual	void	addCamera(_bObj* tmpObject)=0;

	virtual	void	addLight(_bObj* tmpObject)=0;

};


#endif //BULLET_BLEND_READER_H