#ifndef _FB_SIMPLETCPIP_CLIENT_H_
#define _FB_SIMPLETCPIP_CLIENT_H_

#include <fbsdk/fbsdk.h>

/**	\file	fbsimpletcpip_client.h
*	Declaration for the data channels & network client.
*	Declarations for the DataChannel class, representing a channel
*	of information coming in from the hardware, as well as the 
*	FBSimpleNetworkClient class, representing the hardware itself.
*/

//--- Size & maximum defines
#define MAX_DATA_SCALAR_CHANNEL 500		// Maximum number of channels
#define MAX_MARKER				200		// Maximum number of markers
#define TCP_BUFFER_SIZE			10000	// Buffer size
#define MAX_DATA_ITEMS			300		//maximum number of items (float values) in incoming data stream
#define MAX_NETWORK_CONNECT		5		// Maximum number of network connections
#define MAXHANDS 4

#define HIPS 0
#define LSHOULDER 1
#define LELBOW 2
#define LWRIST 3
#define LKNUCKLE 4
#define RSHOULDER 5
#define RELBOW 6
#define RWRIST 7
#define RKNUCKLE 8
#define HEAD 9
#define LKNEE 10
#define LANKLE 11
#define LTOE 12
#define RKNEE 13
#define RANKLE 14
#define RTOE 15
#define CHEST 16
#define BUTTONS 17


#define LEFTHAND_PRESENT 1
#define RIGHTHAND_PRESENT 2
#define LEFTARM_PRESENT 4
#define RIGHTARM_PRESENT 8
#define LEFTLEG_PRESENT 16
#define RIGHTLEG_PRESENT 32
#define HEAD_PRESENT 64
#define CHEST_PRESENT 128

//--- FBX load/save tags
#define FBX_CHANNEL_TAG		"Channels"
#define FBX_VERSION_TAG		"Version"

struct STHAND_DATA {
	float base_pos[3];
	float hand_angles[3];
	float basefinger_angles[3];
	float midfinger_angles[3];
	float tipfinger_angles[3];
};

struct SHAPEHAND_DATA {
	//quaternion values for hand orientation
	float hand[4];
	//quaternion values for finger bone orientations
	float thumb[3][4];
	float index[3][4];
	float middle[3][4];
	float ring[3][4];
	float small_[3][4];
};

struct NETWORK_TIMECODE {//time code that gets sent over network connection
	unsigned char ucHour;
	unsigned char ucMinute;
	unsigned char ucSecond;
	unsigned char ucFrame;
	DWORD dwMSTimestamp;
};


//--- Data packet enumerations
enum	DataPacketInfo	{	kDataPacketInfoClose = 0,		kDataPacketInfoInfo = 1,		kDataPacketInfoData = 2,
	kDataPacketInfoStreamOn	= 3,	kDataPacketInfoStreamOff = 4	};
enum	DataPacketMode	{	kDataPacketModeRequest = 0,		kDataPacketModeReply = 1		};

//! Data channel class.
class DataChannel 
{   
private:
	float			mPos[3];				//!< Position array.
	float			mRot[3];				//!< Rotation array.

public:
	void  SetPos(float *pos) {	mPos[0] = pos[0]/10;//convert to cm
	mPos[1] = pos[1]/10;
	mPos[2] = pos[2]/10;}
	void  SetRot(float *rot) {  mRot[0] = rot[0];
	mRot[1] = rot[1];
	mRot[2] = rot[2];}

	float GetXPos() { return mPos[0];}
	float GetYPos() { return mPos[1];}
	float GetZPos() { return mPos[2];}
	float GetXRot() { return mRot[0];}
	float GetYRot() { 
		return mRot[1];
	}
	float GetZRot() {
		return mRot[2];
	}

	
	FBString			mName;				//!< Name of marker as displayed in the spreadsheet.
	HFBAnimationNode	mTAnimNode;			//!< Position animation output node. 
	HFBAnimationNode	mRAnimNode;			//!< Rotation animation output node.

	HFBModelTemplate	mModelTemplate;		//!< Marker model template driven by the data channel.
	bool				mIsUsed;			//!< Set to true to confirm presence of this sensor/marker.

	//! Constructor.
	DataChannel() 
	{ 
		mTAnimNode	= NULL;
		mRAnimNode	= NULL;
		mModelTemplate	= NULL;
		mName       = "NoName";
		mIsUsed		= false;
		for (int i=0;i<3;i++)
		{
			mPos[i]=0;
			mRot[i]=0;
		}
	}

	//! Destructor.
	~DataChannel() 
	{ 
		/*if (mRAnimNode) delete mRAnimNode;
		if (mTAnimNode) delete mTAnimNode;
		if (mModelTemplate) delete mModelTemplate;*/
	}
};



//! Data packet class
class	DataPacket	
{
public:
	long	mInfo;	//!< Information: close, info, data, streamon, streamoff
	long	mMode;	//!< Mode: request, reply

	//! Packet constructor.
	DataPacket( DataPacketInfo pInfo, DataPacketMode pMode )
	{
		mInfo	=	pInfo;
		mMode	=	pMode;
	}
};

//--- Data packets
static DataPacket	InfoRequestPacket(		kDataPacketInfoInfo,		kDataPacketModeRequest);
static DataPacket	CloseRequestPacket(		kDataPacketInfoClose,		kDataPacketModeRequest);
static DataPacket	StreamOnRequestPacket(	kDataPacketInfoStreamOn,	kDataPacketModeRequest);
static DataPacket	StreamOffRequestPacket(	kDataPacketInfoStreamOff,	kDataPacketModeRequest);

//! Different types of channels.
enum DataChannelType { 
	Data_Frame,		// Frame index (starts at zero)
	Data_RH,		
	Data_RX,		
	Data_RY,		
	Data_RZ,		
	Data_RA,		
	Data_RB,		
	Data_TX,		
	Data_TY,		
	Data_TZ,		
	Data_PX,		// X position of marker		
	Data_PY,		// Y position of marker		
	Data_PZ,		// Z position of marker		
	Data_O			// Occlusion
};


//! Convert marker information from channel data.
class MarkerToChannelIndex 
{
public:
	int x,y,z,qw,qx,qy,qz;			//!< coordinates for position and quaternion orientation

	//! Constructor.
	MarkerToChannelIndex()	{
		x=y=z=qw=qx=qy=qz=0;
	}

	//! Destructor.
	~MarkerToChannelIndex()	{;}
};


/**	Simple network client.
*	Represnets marker information from channel data.

*/

class FBSimpleNetworkClient  
{
public:
	void Initialize(int nActor_index);
	BYTE GetDataMask(int nActor_index=0);
	FBSimpleNetworkClient();	//!< Constructor.
	~FBSimpleNetworkClient();	//!< Destructor.

	//--- Client/server calls
	bool	Open(int nActor_index=0);				//!< Open connection with server.
	bool	Close(int nActor_index=0);			//!< Clost connection with server.

	/**	Obtain channel information.
	*	After the connection has been established with the Open() call
	*	this call will retreive all channel information (type, name, etc...).
	*	\return \b true if successful, \b false otherwise.
	*/
	bool	GetChannels(int nActor_index=0);

	/**	Start/Stop data stream.
	*	Once connection is established and information packets have been received
	*	these functions start and stop the data streaming
	*	\return \b true if successful, \b false otherwise.
	*/
	bool	StartDataStream(int nActor_index=0);
	/**	Start/Stop data stream.
	*	Once connection is established and information packets have been received
	*	these functions start and stops the data streaming
	*	\return \b true if successful, \b false otherwise.
	*/
	bool	StopDataStream(int nActor_index=0);

	/**	Non blocking read of data stream.
	*	Non blocking read of data stream which fills mChannel[] with a new data packet.
	*	\return \b true if a new data frame is ready, \b false otherwise.
	*/	
	bool	FetchDataPacket(FBTime &evaltime, int nActor_index=0);		 

	//!	Get the button state
	int GetButtonState(int nActor_index=0);
	//!	Get time in milliseconds.
	bool	GetTimeMilliSeconds(kLongLong *pTime, int nActor_index=0);
	//!	Get number of markers.
	int		GetMarkerCount(int nActor_index=0)						{
		return mMarkerCount[nActor_index];						}
	//!	Get the name of a marker.
	char*	GetMarkerName(int pMarkerIndex)	{ return (char *)&mMarkerName[pMarkerIndex][0]; }
	//!	Get the server port number.
	int		GetServerPort(int nActor_index=0)							{ return mServerPort[nActor_index];						}
	//!	Get the server address.
	char*	GetServerAddress(int nActor_index=0)						{ return (char *)mServerAddress[nActor_index];			}

	//! Set the server address.
	void	SetServerAddress(char *pIPAddress, int nActor_index=0);		
	//! Set the server port.
	void	SetServerPort(int pPort, int nActor_index=0)				{ mServerPort[nActor_index] = pPort;						}

	//! Set the client address.
	void	SetClientAddress(char *pIPAddress, int nActor_index=0);		
	//! Set the client port.
	void	SetClientPort(int pPort, int nActor_index=0)				{ mClientPort[nActor_index] = pPort;						}


	double	GetDataTX(int pMarker, int nActor_index=0);		//!< Get the X data
	double	GetDataTY(int pMarker, int nActor_index=0);		//!< Get the Y data
	double	GetDataTZ(int pMarker, int nActor_index=0);		//!< Get the Z data
	double  GetDataRX(int pMarker, int nActor_index=0);		//!< Get the rotation about the X-axis data
	double	GetDataRY(int pMarker, int nActor_index=0);		//!< Get the rotation about the Y-axis data
	double	GetDataRZ(int pMarker, int nActor_index=0);		//!< Get the rotation about the Z-axis data

	bool GetShapeHandData(FBTime &evaltime,SHAPEHAND_DATA *hand_data, bool bLeft, int nActor_index=0); //get shapehand data
	bool GetSTHandData(FBTime &evaltime,STHAND_DATA *hand_data, bool bLeft, int nActor_index=0); //get shapetape hand data

	//! Verify connection.
	bool	IsConnectedToServer(int nActor_index=0)					{ return mIsConnectedToServer[nActor_index];				}

	void *m_shapeHands[MAXHANDS];

private:

	/** Blocking read used to get information packets in GetSetupInfo()
	*	\param	pBuf.						Buffer to fill.
	*	\param	pNumberOfBytesToRead.		Number of bytes to read.
	*	\param	pTimeOutMilliseconds.		Timeout value.
	*	\param	pGranularityMilliseconds.	Granularity for timeout.
	*	\return \b true if successful read, \b false if timed out.
	*/

private:
	BYTE m_datamask[MAX_NETWORK_CONNECT];
	/*Critical Sections */
	CRITICAL_SECTION *m_sthand_dataSect[MAX_NETWORK_CONNECT];
	CRITICAL_SECTION *m_shapehand_dataSect[MAX_NETWORK_CONNECT];
	CRITICAL_SECTION *m_datamaskSect[MAX_NETWORK_CONNECT];

	void CalculateLHandData(int &data_index, bool bShapeHand, int nActor_index=0);
	void CalculateRHandData(int &data_index, bool bShapeHand, int nActor_index=0);
	STHAND_DATA *mLHandData[MAX_NETWORK_CONNECT];//hand data from left arm shapetape
	STHAND_DATA *mRHandData[MAX_NETWORK_CONNECT];//hand data from right arm shapetape
	SHAPEHAND_DATA *mLShapehand[MAX_NETWORK_CONNECT]; //hand data from left shapehand
	SHAPEHAND_DATA *mRShapehand[MAX_NETWORK_CONNECT]; //hand data from right shapehand
	bool m_bLSTHanddata_available[MAX_NETWORK_CONNECT];
	bool m_bRSTHanddata_available[MAX_NETWORK_CONNECT];
	int m_nLShapehand_available[MAX_NETWORK_CONNECT];
	int m_nRShapehand_available[MAX_NETWORK_CONNECT];
	float m_packetvals[MAX_NETWORK_CONNECT][MAX_DATA_ITEMS];
	bool m_bChannelsexist[MAX_NETWORK_CONNECT];

	//--- Network information
	int						mSocket[MAX_NETWORK_CONNECT];								//!< Socket number.
	FBTCPIP					*mTCP[MAX_NETWORK_CONNECT];									//!< Network interface for function calls.

	//--- Device information
	int						mButtonState[MAX_NETWORK_CONNECT];
	kLongLong				mBaseTimeMS[MAX_NETWORK_CONNECT];							//!< base time recorded in ms
	unsigned long			mFirstTimeStamp[MAX_NETWORK_CONNECT];						//!< first time stamp number received from network (in ms)
	unsigned long			mTimeStamp[MAX_NETWORK_CONNECT];							//!< Time stamp number
	NETWORK_TIMECODE        mNetTimeCode[MAX_NETWORK_CONNECT];							//!< Network timecode from synchronization device
	int						mServerPort[MAX_NETWORK_CONNECT];							//!< Server port.
	int						mClientPort[MAX_NETWORK_CONNECT];							//!< Client port
	char *				mServerAddress[MAX_NETWORK_CONNECT];							//!< Server address.
	char *				mClientAddress[MAX_NETWORK_CONNECT];							//!< Client address.

	//--- Simple static data structures
	// Note:	These structures are for example 
	//			purposes only should be dynamically 
	//			allocated.
	char 				mMarkerName[MAX_MARKER][32];				//!< Names of markers.
	int					mMarkerCount[MAX_NETWORK_CONNECT];							//!< Number of markers.

	//--- Server sends (X,Y,Z) data as separate channels of doubles (scalar)
	DataChannel *	mChannel[MAX_NETWORK_CONNECT][17];								//!< Array of channels
	long					mNumDataItems[MAX_NETWORK_CONNECT];						    //!< Number of data items per packet.
	bool					mIsConnectedToServer[MAX_NETWORK_CONNECT];					//!< Is connected to server?

	//--- Buffer
	char					mBuf[MAX_NETWORK_CONNECT][TCP_BUFFER_SIZE];					//!< Temporary storage buffer for data.
protected:
	void ConvertQuattoEuler(float *quat, float *euler);
};

#endif	/* _FB_SIMPLETCPIP_CLIENT_H_ */
