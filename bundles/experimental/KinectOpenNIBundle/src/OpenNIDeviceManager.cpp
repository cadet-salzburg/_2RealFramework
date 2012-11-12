#include "OpenNIDeviceManager.h"

#include <iostream>
#include <string>

using _2Real::bundle::Block;
using _2Real::bundle::BlockHandle;
using _2Real::Exception;
using std::cout;
using std::endl;
using std::string;
using namespace _2RealKinectWrapper;


OpenNIDeviceManager::OpenNIDeviceManager() : ContextBlock()
{
}

OpenNIDeviceManager::~OpenNIDeviceManager()
{
}

void OpenNIDeviceManager::setup( BlockHandle &context )
{
	Poco::Mutex::ScopedLock lock(m_Mutex);
	try
	{
		m_iNumDevices=0;
		m_2RealKinect = _2RealKinect::getInstance();
		std::cout << "_2RealKinectWrapper Version: " << m_2RealKinect->getVersion() << std::endl;
		if(m_2RealKinect!=nullptr)
			m_iNumDevices = m_2RealKinect->getNumberOfDevices();
		
		for(int i=0; i<m_iNumDevices; i++)
		{
			// you can't add infrared generator here, as it cannot be added together with the color generator
			m_2RealKinect->configure( i,  _2RealKinectWrapper::COLORIMAGE | _2RealKinectWrapper::DEPTHIMAGE | _2RealKinectWrapper::USERIMAGE );
		}
		initDeviceList();
	}
	catch ( _2RealKinectWrapper::_2RealException &e )
	{
		m_2RealKinect = nullptr;
		cout << e.what() << endl;
	}
}

void OpenNIDeviceManager::update()
{
	try
	{
		rescanDeviceList();
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void OpenNIDeviceManager::shutdown()
{
	Poco::Mutex::ScopedLock lock(m_Mutex);
	
	try
	{
		if(m_2RealKinect != nullptr)
		{
			m_2RealKinect->shutdown();
			m_2RealKinect->destroyInstance();
			m_2RealKinect = nullptr;
		}
	}
	catch ( _2RealKinectWrapper::_2RealException &e )
	{
		cout << e.what() << endl;
	}
}

void OpenNIDeviceManager::initDeviceList()
{
	Poco::Mutex::ScopedLock lock(m_Mutex);
	
	m_DevicesInUse.clear();
	for(unsigned int i=0; i<m_iNumDevices; i++)
	{
		m_DevicesInUse.push_back(DeviceItem());
	}
}

void OpenNIDeviceManager::rescanDeviceList()
{
	Poco::Mutex::ScopedLock lock(m_Mutex);

	try
	{
		if(m_2RealKinect==nullptr)
			return;						// no driver found
		int numDevices = m_2RealKinect->getNumberOfDevices();
		if(numDevices != m_iNumDevices)
		{
			// reinit devices
			m_iNumDevices = numDevices;
		}
	}
	catch ( _2RealKinectWrapper::_2RealException &e )
	{
		cout << e.what() << endl;
	}
}

bool OpenNIDeviceManager::isDeviceRunning(const unsigned int deviceIdx)
{
	Poco::Mutex::ScopedLock lock(m_Mutex);

	return true;
}

bool OpenNIDeviceManager::isDeviceAvailable(const unsigned int deviceIdx)
{
	return  (deviceIdx < m_DevicesInUse.size());
}

bool OpenNIDeviceManager::isGeneratorFree(const unsigned int deviceIdx, _2RealGenerator generatorType)
{
	if(isDeviceAvailable(deviceIdx))
		return  !m_DevicesInUse[deviceIdx].m_bGeneratorIsUsed[generatorType];
	else
	{
		return false;
	}
}

bool OpenNIDeviceManager::bindGenerator(const unsigned int deviceIdx, _2RealGenerator generatorType, int w, int h, int fps)
{
	Poco::Mutex::ScopedLock lock(m_Mutex);
	try
	{
		if(isGeneratorFree(deviceIdx, generatorType))
		{
			// setup device
			bool bResult = false;

			uint32_t resolution;
			if(generatorType == _2RealKinectWrapper::COLORIMAGE)
			{
				resolution = IMAGE_COLOR_640X480;
				if(w == 640 && h == 480)
				{
					 resolution = IMAGE_COLOR_640X480;
				}
			}
			else if(generatorType == _2RealKinectWrapper::DEPTHIMAGE || generatorType == _2RealKinectWrapper::USERIMAGE)
			{
				resolution = IMAGE_USER_DEPTH_640X480;
				if(w == 640 && h == 480)
				{
					resolution = IMAGE_USER_DEPTH_640X480;
				}
			}
			else if(generatorType == _2RealKinectWrapper::INFRAREDIMAGE)
			{
				resolution = IMAGE_INFRARED_640X480;
				if(w == 640 && h == 480)
				{
					resolution = IMAGE_INFRARED_640X480;
				}
			}

			bResult = m_2RealKinect->configure( deviceIdx,  generatorType, resolution  );

			if( bResult )
			{
				std::cout << "_2RealKinectWrapper Device " << deviceIdx << " started successfully!..." << std::endl;
			}
			m_2RealKinect->startGenerator( deviceIdx, generatorType );

			return m_DevicesInUse[deviceIdx].m_bGeneratorIsUsed[generatorType] = true;
		}
	}
	catch ( _2RealKinectWrapper::_2RealException &e )
	{
		cout << e.what() << endl;
	}
	catch(...)
	{
		printf("exception");
	}

	return false;
}

void OpenNIDeviceManager::unbindGenerator(const unsigned int deviceIdx, _2RealGenerator generatorType)
{
	Poco::Mutex::ScopedLock lock(m_Mutex);

	try
	{
		if(!isGeneratorFree(deviceIdx, generatorType))
		{
			// stop device
			m_2RealKinect->stopGenerator( deviceIdx, generatorType );
			m_DevicesInUse[deviceIdx].m_bGeneratorIsUsed[generatorType] = false;
		}
	}
	catch ( _2RealKinectWrapper::_2RealException &e )
	{
		cout << e.what() << endl;
	}
}


bool OpenNIDeviceManager::setGeneratorParams(const unsigned int deviceIdx, _2RealGenerator generatorType, int w, int h, int fps)
{
	Poco::Mutex::ScopedLock lock(m_Mutex);

	if(!isGeneratorFree(deviceIdx, generatorType))
	{
		return true;
	}
	return false;
}

unsigned int OpenNIDeviceManager::getNumberOfConnectedDevices() 
{
	Poco::Mutex::ScopedLock lock(m_Mutex);
	return m_iNumDevices;
}


_2Real::Image& OpenNIDeviceManager::getImage( const unsigned int deviceIdx, _2RealKinectWrapper::_2RealGenerator generatorType, bool bIs16Bit) 
{
	Poco::Mutex::ScopedLock lock(m_Mutex);
	try
	{
		// this makes serious problems
		/*if(!m_2RealKinect->isNewData(deviceIdx, generatorType))
			return  m_DevicesInUse[deviceIdx].m_Image;*/
		int imageWidth = m_2RealKinect->getImageWidth( deviceIdx, generatorType );		
		int imageHeight = m_2RealKinect->getImageHeight( deviceIdx, generatorType );

		if(generatorType == _2RealKinectWrapper::DEPTHIMAGE && bIs16Bit)	// get 16bit depth image
		{
			unsigned short* pixels = m_2RealKinect->getImageDataDepth16Bit( deviceIdx ).get();
			m_DevicesInUse[deviceIdx].m_ImageDepth16Bit = _2Real::Image( pixels, false, imageWidth, imageHeight, _2Real::ImageChannelOrder::A );
			return m_DevicesInUse[deviceIdx].m_ImageDepth16Bit;
		}
		else if(generatorType == _2RealKinectWrapper::DEPTHIMAGE)
		{
			unsigned char* pixels = m_2RealKinect->getImageData( deviceIdx, generatorType ).get();
			m_DevicesInUse[deviceIdx].m_ImageDepth8Bit = _2Real::Image( pixels, false, imageWidth, imageHeight, _2Real::ImageChannelOrder::A );
			return m_DevicesInUse[deviceIdx].m_ImageDepth8Bit;
		}
		else if(generatorType == _2RealKinectWrapper::COLORIMAGE)
		{
			unsigned char* pixels = m_2RealKinect->getImageData( deviceIdx, generatorType ).get();
			m_DevicesInUse[deviceIdx].m_ImageRgb = _2Real::Image( pixels, false, imageWidth, imageHeight, _2Real::ImageChannelOrder::RGB );
			return m_DevicesInUse[deviceIdx].m_ImageRgb;
		}
		else if(generatorType == _2RealKinectWrapper::USERIMAGE)
		{
			unsigned char* pixels = m_2RealKinect->getImageData( deviceIdx, generatorType ).get();
			m_DevicesInUse[deviceIdx].m_ImageUser = _2Real::Image( pixels, false, imageWidth, imageHeight, _2Real::ImageChannelOrder::RGB );
			return m_DevicesInUse[deviceIdx].m_ImageUser;
		}
		else if(generatorType == _2RealKinectWrapper::INFRAREDIMAGE)
		{
			unsigned char* pixels = m_2RealKinect->getImageData( deviceIdx, generatorType ).get();
			m_DevicesInUse[deviceIdx].m_ImageIr = _2Real::Image( pixels, false, imageWidth, imageHeight, _2Real::ImageChannelOrder::RGB );
			return m_DevicesInUse[deviceIdx].m_ImageIr;
		}
	}
	catch ( _2RealKinectWrapper::_2RealException &e )
	{
		cout << e.what() << endl;
		
		if(generatorType == _2RealKinectWrapper::DEPTHIMAGE && bIs16Bit)	// get 16bit depth image
		{
			return m_DevicesInUse[deviceIdx].m_ImageDepth16Bit;
		}
		else if(generatorType == _2RealKinectWrapper::DEPTHIMAGE)
		{
			return m_DevicesInUse[deviceIdx].m_ImageDepth8Bit;
		}
		else if(generatorType == _2RealKinectWrapper::COLORIMAGE)
		{
			return m_DevicesInUse[deviceIdx].m_ImageRgb;
		}
		else if(generatorType == _2RealKinectWrapper::USERIMAGE)
		{
			return m_DevicesInUse[deviceIdx].m_ImageUser;
		}
		else if(generatorType == _2RealKinectWrapper::INFRAREDIMAGE)
		{
			return m_DevicesInUse[deviceIdx].m_ImageIr;
		}
	}
}

int OpenNIDeviceManager::getWidth( const unsigned int deviceIdx, _2RealGenerator generatorType )
{
	Poco::Mutex::ScopedLock lock(m_Mutex);
	try
	{
		return m_2RealKinect->getImageWidth( deviceIdx, generatorType );		
	}
	catch ( _2RealKinectWrapper::_2RealException &e )
	{
		cout << e.what() << endl;
	}
	return 0;
}

int OpenNIDeviceManager::getHeight( const unsigned int deviceIdx, _2RealGenerator generatorType )
{
	Poco::Mutex::ScopedLock lock(m_Mutex);
	try
	{
		return m_2RealKinect->getImageHeight( deviceIdx, generatorType );
	}
	catch ( _2RealKinectWrapper::_2RealException &e )
	{
		cout << e.what() << endl;
	}
	return 0;
}

void OpenNIDeviceManager::setMirrored(const unsigned int deviceIdx, _2RealGenerator generatorType, bool bIsMirrored)
{
	Poco::Mutex::ScopedLock lock(m_Mutex);
	try
	{
		if( generatorType == USERIMAGE)		// mirroring for user only works for mirroring the depth with openni
			generatorType = DEPTHIMAGE;
		m_2RealKinect->setMirrored( deviceIdx, generatorType, bIsMirrored );
	}
	catch ( _2RealKinectWrapper::_2RealException &e )
	{
		cout << e.what() << endl;
	}
}

void OpenNIDeviceManager::setAlignToColor(const unsigned int deviceIdx, bool bIsAligned)
{
	Poco::Mutex::ScopedLock lock(m_Mutex);
	try
	{
		m_2RealKinect->alignDepthToColor( deviceIdx, bIsAligned );
	}
	catch ( _2RealKinectWrapper::_2RealException &e )
	{
		cout << e.what() << endl;
	}
}

int	OpenNIDeviceManager::getNumberOfUsers( const unsigned int deviceIdx )
{
	Poco::Mutex::ScopedLock lock(m_Mutex);
	return m_2RealKinect->getNumberOfUsers(deviceIdx);
}

int	OpenNIDeviceManager::getNumberOfSkeletons( const unsigned int deviceIdx )
{
	Poco::Mutex::ScopedLock lock(m_Mutex);
	return m_2RealKinect->getNumberOfSkeletons(deviceIdx);
}

std::vector<_2Real::Skeleton> OpenNIDeviceManager::getSkeletons(const unsigned int deviceIdx, bool bIsWorldCoordinates )
{
	Poco::Mutex::ScopedLock lock(m_Mutex);
	try
	{
		int iNrOfSkeletons = m_2RealKinect->getNumberOfSkeletons(deviceIdx);
		std::vector<_2Real::Skeleton> skeletons;
		for(int i=0; i<iNrOfSkeletons; i++)
		{
			skeletons.push_back( getSkeleton(deviceIdx, i, bIsWorldCoordinates ) );
			skeletons.back().setGlobal(bIsWorldCoordinates);
			if(bIsWorldCoordinates)
			{
				skeletons.back().setLimit(_2Real::BoundingBox(_2Real::Point(-3000,-3000,-3000), _2Real::Point(3000,3000,3000))); 
			}
			else
			{
				skeletons.back().setLimit(_2Real::BoundingBox(_2Real::Point(-1,-1,-1), _2Real::Point(1,1,1)));
			}
		}
		
		return skeletons; 
	}
	catch ( _2RealKinectWrapper::_2RealException &e )
	{
		cout << e.what() << endl;
		return std::vector<_2Real::Skeleton>(); 
	}
}

_2Real::Skeleton OpenNIDeviceManager::getSkeleton(const unsigned int deviceIdx, int userId, bool bIsWorldCoordinates )
{
	Poco::Mutex::ScopedLock lock(m_Mutex);
	try
	{
		_2Real::Skeleton skeleton;
		skeleton.setGlobal(true);

		_2RealKinectWrapper::_2RealPositionsVector3f positions;
		std::vector<_2Real::RigidBody> jointPositions;
		// don't regard confidence values for now, otherwise joints disappear and appear due to this faulty implementation of the confidence values, better to show them and the are not moving
		//_2RealKinectWrapper::_2RealJointConfidences confidences = m_2RealKinect->getSkeletonJointConfidences(deviceIdx, userId);

		if(bIsWorldCoordinates)
		{
			positions = m_2RealKinect->getSkeletonWorldPositions(deviceIdx, userId);
		}
		else	// normalized screen coords range [0..1]
		{
			int w = getWidth(deviceIdx, USERIMAGE);
			int h = getHeight(deviceIdx, USERIMAGE);
			positions = m_2RealKinect->getSkeletonScreenPositions(deviceIdx, userId);
			for(int i=0; i < positions.size(); i++)
			{
				positions[i].x/=(float)w;
				positions[i].y/=(float)h;
				positions[i].y = 1.0f - positions[i].y;
				positions[i].z=0;				// useless for screen coords
			}
		}

		for(int i=0; i < positions.size(); i++)
		{
			if( m_2RealKinect->isJointAvailable( (_2RealJointType)i ))// && confidences[i].positionConfidence>0)		// only add if joint is available no confidence for now implemented see comment above
			{
				_2Real::RigidBody rigidBody;
				rigidBody.setId(i);
				rigidBody.setLabel(getLabelForJoint((_2RealJointType)i));
				rigidBody.setPosition(_2Real::Number(positions[i].x), _2Real::Number(positions[i].y), _2Real::Number(positions[i].z));

				jointPositions.push_back(rigidBody);
			}
		}
		
		skeleton.setRigidBodies(jointPositions);
		return skeleton;
	}
	catch ( _2RealKinectWrapper::_2RealException &e )
	{
		cout << e.what() << endl;
		return _2Real::Skeleton(); 
	}
}

void OpenNIDeviceManager::projectiveToReal( const unsigned int deviceIdx, const unsigned int count, _2RealKinectWrapper::_2RealVector3f const* in, _2RealKinectWrapper::_2RealVector3f *out )
{
	m_2RealKinect->convertProjectiveToWorld( deviceIdx, count, in, out );
}

std::string OpenNIDeviceManager::getLabelForJoint(_2RealJointType joint)
{
	std::string label("unknown");

	switch (joint)
	{
	case _2RealJointType::JOINT_HEAD:
		label = "head";
		break;
	case _2RealJointType::JOINT_NECK:
		label = "neck";
		break;
	case _2RealJointType::JOINT_TORSO:
		label = "torso";
		break;
	case _2RealJointType::JOINT_WAIST:
		label = "waist";
		break;
	case _2RealJointType::JOINT_LEFT_COLLAR:
		label = "left collar";
		break;
	case _2RealJointType::JOINT_LEFT_SHOULDER:
		label = "left shoulder";
		break;
	case _2RealJointType::JOINT_LEFT_ELBOW:
		label = "left ellbow";
		break;
	case _2RealJointType::JOINT_LEFT_WRIST:
		label = "left wrist";
		break;
	case _2RealJointType::JOINT_LEFT_HAND:
		label = "left hand";
		break;
	case _2RealJointType::JOINT_LEFT_FINGERTIP:
		label = "left fingertip";
		break;
	case _2RealJointType::JOINT_RIGHT_COLLAR:
		label = "right collar";
		break;
	case _2RealJointType::JOINT_RIGHT_SHOULDER:
		label = "right shoulder";
		break;
	case _2RealJointType::JOINT_RIGHT_ELBOW:
		label = "right ellbow";
		break;
	case _2RealJointType::JOINT_RIGHT_WRIST:
		label = "right wrist";
		break;
	case _2RealJointType::JOINT_RIGHT_HAND:
		label = "right hand";
		break;
	case _2RealJointType::JOINT_RIGHT_FINGERTIP:
		label = "right fingertip";
		break;
	case _2RealJointType::JOINT_LEFT_HIP:
		label = "left hip";
		break;
	case _2RealJointType::JOINT_LEFT_KNEE:
		label = "left knee";
		break;
	case _2RealJointType::JOINT_LEFT_ANKLE:
		label = "left ankle";
		break;
	case _2RealJointType::JOINT_LEFT_FOOT:
		label = "left foot";
		break;
	case _2RealJointType::JOINT_RIGHT_HIP:
		label = "right hip";
		break;
	case _2RealJointType::JOINT_RIGHT_KNEE:
		label = "right knee";
		break;
	case _2RealJointType::JOINT_RIGHT_ANKLE:
		label = "right ankle";
		break;
	case _2RealJointType::JOINT_RIGHT_FOOT:
		label = "right foot";
		break;
	}

	return label;
}

 void OpenNIDeviceManager::setMotorAngle(int deviceIdx, int angle)
 {
	 Poco::Mutex::ScopedLock lock(m_Mutex);
	 m_2RealKinect->setMotorAngle(deviceIdx, angle);
 }

 int OpenNIDeviceManager::getMotorAngle(int deviceIdx)
 {
	 Poco::Mutex::ScopedLock lock(m_Mutex);
	 return m_2RealKinect->getMotorAngle(deviceIdx);
 }

double OpenNIDeviceManager::getFovH(int deviceIdx)
{
	Poco::Mutex::ScopedLock lock(m_Mutex);
	return m_2RealKinect->getFieldOfView(deviceIdx).horizontalFov;
}

double OpenNIDeviceManager::getFovV(int deviceIdx)
{
	Poco::Mutex::ScopedLock lock(m_Mutex);
	return m_2RealKinect->getFieldOfView(deviceIdx).verticalFov;
}

_2Real::Point OpenNIDeviceManager::getUsersCenterOfMass(int deviceIdx, int userId, bool bIsWorldCoordinates)
 {
	try
	{
		 Poco::Mutex::ScopedLock lock(m_Mutex);
		 _2Real::Point point;
		 _2RealVector3f tmp;
		 if(bIsWorldCoordinates)
		 {
			tmp = m_2RealKinect->getUsersWorldCenterOfMass(deviceIdx, userId);
		 }
		 else
		 {
			 tmp = m_2RealKinect->getUsersScreenCenterOfMass(deviceIdx, userId);
		 }
		 point.setX(tmp.x);
		 point.setY(tmp.y);
		 point.setZ(tmp.z);
		 point.setId(userId);
		 return point;
	}
	catch(_2RealKinectWrapper::_2RealException &e)
	{
		cout << e.what() << endl;
		return _2Real::Point(); 
	}
 }
