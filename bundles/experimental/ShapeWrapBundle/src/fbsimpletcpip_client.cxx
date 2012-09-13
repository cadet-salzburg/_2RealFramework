
/**	\file	fbsimpletcpip_client.cxx
*	Definition of a simple network client.
*	Definition of the FBSimpleNetworkClient class, representing a 
*	network client for the Measurand 5-Tape System
*/

//--- Class declaration
#include "math.h"
#include "winsock2.h"
#include <mmsystem.h>
#include "fbsimpletcpip_client.h"

/************************************************
*	Network client constructor.
************************************************/
FBSimpleNetworkClient::FBSimpleNetworkClient()
{
	for (int i=0;i<MAXHANDS;i++) {
		m_shapeHands[i]=NULL;
	}
	//	Network internals
	for (int i=0;i<MAX_NETWORK_CONNECT;i++) {
		m_bChannelsexist[i]=false;
		mSocket[i]				= -1;
		mServerPort[i]			= 0;
		mServerAddress[i]		= NULL;
		mClientPort[i]			= 0;
		mClientAddress[i]		= NULL;
		mTimeStamp[i] = 0;
		memset(&mNetTimeCode,0,sizeof(NETWORK_TIMECODE));
		mButtonState[i] = 0;
		mMarkerCount[i]		= 18;
		mNumDataItems[i]	= 0;
		mBaseTimeMS[i]=0;
		mFirstTimeStamp[i]=0;
		for (int j=0;j<mMarkerCount[i];j++) {
			mChannel[i][j] = new DataChannel();
		}
		m_bChannelsexist[i]=true;
		for (int j=0;j<MAX_DATA_ITEMS;j++) m_packetvals[i][j]=0.0;
		m_bLSTHanddata_available[i] = false;
		m_bRSTHanddata_available[i] = false;
		m_nLShapehand_available[i] = 0;
		m_nRShapehand_available[i] = 0;
		mLHandData[i]=NULL;
		mRHandData[i]=NULL;
		mLShapehand[i]=NULL;
		mRShapehand[i]=NULL;
		m_datamaskSect[i]=NULL;
		m_shapehand_dataSect[i]=NULL;
		m_sthand_dataSect[i]=NULL;
		m_datamask[i]=0;
	}
	strcpy(mMarkerName[HIPS],"Hips");
	strcpy(mMarkerName[LSHOULDER],"Left_Shoulder");
	strcpy(mMarkerName[LELBOW],"Left_Elbow");
	strcpy(mMarkerName[LWRIST],"Left_Wrist");
	strcpy(mMarkerName[LKNUCKLE],"Left_Knuckle");
	strcpy(mMarkerName[RSHOULDER],"Right_Shoulder");
	strcpy(mMarkerName[RELBOW],"Right_Elbow");
	strcpy(mMarkerName[RWRIST],"Right_Wrist");
	strcpy(mMarkerName[RKNUCKLE],"Right_Knuckle");
	strcpy(mMarkerName[HEAD],"Head");
	strcpy(mMarkerName[LKNEE],"Left_Knee");
	strcpy(mMarkerName[LANKLE],"Left_Ankle");
	strcpy(mMarkerName[LTOE],"Left_Toe");
	strcpy(mMarkerName[RKNEE],"Right_Knee");
	strcpy(mMarkerName[RANKLE],"Right_Ankle");
	strcpy(mMarkerName[RTOE],"Right_Toe");
	strcpy(mMarkerName[CHEST],"Chest");
	strcpy(mMarkerName[BUTTONS],"Button_Board");
}


/************************************************
*	Network client destructor.
************************************************/
FBSimpleNetworkClient::~FBSimpleNetworkClient()
{
	for (int i=0;i<MAX_NETWORK_CONNECT;i++) {
		if (m_datamaskSect[i]) {
			DeleteCriticalSection(m_datamaskSect[i]);
			delete m_datamaskSect[i];
		}
		if (m_shapehand_dataSect[i]) {
			DeleteCriticalSection(m_shapehand_dataSect[i]);
			delete m_shapehand_dataSect[i];
		}
		if (m_sthand_dataSect[i]) {
			DeleteCriticalSection(m_sthand_dataSect[i]);
			delete m_sthand_dataSect[i];
		}
		if (m_bChannelsexist[i]) {
			for (int j=0;j<18;j++) if (mChannel[i][j]) delete mChannel[i][j];
			m_bChannelsexist[i]=false;
		}
		if (mServerAddress[i]) delete [] mServerAddress[i];
		if (mClientAddress[i]) delete [] mClientAddress[i];
		if (mLHandData[i]) delete mLHandData[i];
		if (mRHandData[i]) delete mRHandData[i];
		if (mLShapehand[i]) delete mLShapehand[i];
		if (mRShapehand[i]) delete mRShapehand[i];
	}
}


/************************************************
*	Initiate device communication.
************************************************/
bool FBSimpleNetworkClient::Open(int nActor_index/*=0*/)
{
	if (!mTCP[nActor_index]) mTCP[nActor_index] = new FBTCPIP;
	// Create a non-blocking datagram socket
	bool retval	= mTCP[nActor_index]->CreateSocket(mSocket[nActor_index],kFBTCPIP_DGRAM,"ip",true);
	if (!retval) return false;
	retval=mTCP[nActor_index]->Bind(mSocket[nActor_index],inet_addr(mClientAddress[nActor_index]),
		mClientPort[nActor_index]);
	return	retval;
}


/************************************************
*	Get Channels from hardware.
************************************************/
bool FBSimpleNetworkClient::GetChannels(int nActor_index/*=0*/)
{
	//does not query the server, simply returns with fixed 
	//info specific to the 5 tape system

	//Number of markers, number of data items
	mMarkerCount[nActor_index]		= 18;
	//mNumDataItems	= 119; //and an additional channel with the time stamp

	if (!m_bChannelsexist[nActor_index]) {
		for (int i=0;i<mMarkerCount[nActor_index];i++) {
			mChannel[nActor_index][i] = new DataChannel();
		}
		m_bChannelsexist[nActor_index]=true;
	}
	//Fill in channel names
	mChannel[nActor_index][HIPS]->mName = "Hips";
	mChannel[nActor_index][LSHOULDER]->mName = "LShoulder";
	mChannel[nActor_index][LELBOW]->mName = "LElbow";
	mChannel[nActor_index][LWRIST]->mName = "LWrist";
	mChannel[nActor_index][LKNUCKLE]->mName = "LKnuckle";
	mChannel[nActor_index][RSHOULDER]->mName = "RShoulder";
	mChannel[nActor_index][RELBOW]->mName = "RElbow";
	mChannel[nActor_index][RWRIST]->mName = "RWrist";
	mChannel[nActor_index][RKNUCKLE]->mName = "RKnuckle";
	mChannel[nActor_index][HEAD]->mName = "Head";
	mChannel[nActor_index][LKNEE]->mName = "LKnee";
	mChannel[nActor_index][LANKLE]->mName = "LAnkle";
	mChannel[nActor_index][LTOE]->mName = "LToe";
	mChannel[nActor_index][RKNEE]->mName = "RKnee";
	mChannel[nActor_index][RANKLE]->mName = "RAnkle";
	mChannel[nActor_index][RTOE]->mName = "RToe";
	mChannel[nActor_index][CHEST]->mName = "Chest";
	mChannel[nActor_index][BUTTONS]->mName = "Buttons";
	return true;
}

/************************************************
*	Start sending data.
************************************************/
bool FBSimpleNetworkClient::StartDataStream(int nActor_index/*=0*/)
{
	bool	retval=false;
	unsigned char outbuffer[6];
	unsigned char ip_addr_bytes[4];
	unsigned char clientport_bytes[2];
	unsigned char serverport_bytes[2];
	unsigned long pAddr=0;
	unsigned short pPort=0;


	clientport_bytes[0]=(unsigned char)((mClientPort[nActor_index] & 0x0000ff00)>>8);
	clientport_bytes[1]=(unsigned char)(mClientPort[nActor_index] & 0x000000ff);
	serverport_bytes[0]=(unsigned char)((mServerPort[nActor_index] & 0x0000ff00)>>8);
	serverport_bytes[1]=(unsigned char)(mServerPort[nActor_index] & 0x000000ff);

	//scan in IP Address
	unsigned long temp[4]={0,0,0,0};
	sscanf(mClientAddress[nActor_index],"%u.%u.%u.%u",&temp[0],&temp[1],&temp[2],&temp[3]);
	for (int i=0;i<4;i++)
		ip_addr_bytes[i] = (unsigned char)temp[i];
	//fill in output buffer
	for (int i=0;i<4;i++) 
		outbuffer[i] = ip_addr_bytes[i];
	outbuffer[4]=clientport_bytes[0];
	outbuffer[5]=clientport_bytes[1];

	// Start Streaming
	sscanf(mServerAddress[nActor_index],"%u.%u.%u.%u",&temp[0],&temp[1],&temp[2],&temp[3]);
	pAddr = (temp[3]<<24) + (temp[2]<<16) + (temp[1]<<8) + temp[0];
	pPort = serverport_bytes[0]*256 + serverport_bytes[1];

	int bytesWritten = 0;
	mTCP[nActor_index]->WriteDatagram(mSocket[nActor_index],(void *)outbuffer,6,&bytesWritten, inet_addr(mServerAddress[nActor_index]),pPort);

	// if data would not start streaming
	if(bytesWritten>0)
	{
		retval=true;
	}

	// successful connection?
	mIsConnectedToServer[nActor_index] = retval;

	return retval;
}


/************************************************
*	Stop sending data.
************************************************/
bool FBSimpleNetworkClient::StopDataStream(int nActor_index/*=0*/)
{
	//TODO: implement means of stopping server
	return true;
}


/************************************************
*	Close network conneciton.
************************************************/
bool FBSimpleNetworkClient::Close(int nActor_index/*=0*/)
{
	bool retval;

	retval= mTCP[nActor_index]->CloseSocket(mSocket[nActor_index]);

	if(!retval){
		mSocket[nActor_index] = -1;
	}
	else{
		//unable to close socket
	}
	mIsConnectedToServer[nActor_index] = false;
	return retval;
}


/************************************************
*	Fetch a data packet.
* - is a non-blocking routine
* - fills mChannel[] with a new data packet
* - returns true if a new data frame is ready
************************************************/
bool FBSimpleNetworkClient::FetchDataPacket(FBTime &evaltime, int nActor_index/*=0*/)
{
	float*	channelPtr;


	int server_addr=0;
	unsigned long server_port=0;
	//get server_addr and server_port
	unsigned long temp[4]={0,0,0,0};
	sscanf(mServerAddress[nActor_index],"%u.%u.%u.%u",&temp[0],&temp[1],&temp[2],&temp[3]);
	server_addr = (temp[3]<<24) + (temp[2]<<16) + (temp[1]<<8) + temp[0];
	server_port = (unsigned short)mServerPort[nActor_index];

	//if (!mTCP.ReadDatagram(mSocket,(void *)&mBuf[0],481,&server_addr,&server_port)) {
	//read initial byte to determine the size of the sample to follow
	if (!mTCP[nActor_index]->ReadDatagram(mSocket[nActor_index],(void *)&mBuf[nActor_index][0],1,&server_addr,&server_port)) {
		return false;
	}

	int nBytes_to_read = 0;
	EnterCriticalSection(m_datamaskSect[nActor_index]);
	m_datamask[nActor_index] = (BYTE)mBuf[nActor_index][0];
	LeaveCriticalSection(m_datamaskSect[nActor_index]);
	mNumDataItems[nActor_index]=0;
	if (m_datamask[nActor_index]&(LEFTARM_PRESENT|RIGHTARM_PRESENT|LEFTLEG_PRESENT|RIGHTLEG_PRESENT|HEAD_PRESENT|CHEST_PRESENT)) {
		//hips data is present
		mNumDataItems[nActor_index]+=7;
		nBytes_to_read+=7*sizeof(float);
	}
	if (m_datamask[nActor_index]&LEFTARM_PRESENT) {
		mNumDataItems[nActor_index]+=32;
		nBytes_to_read+=32*sizeof(float);
	}
	if (m_datamask[nActor_index]&RIGHTARM_PRESENT) {
		mNumDataItems[nActor_index]+=32;
		nBytes_to_read+=32*sizeof(float);
	}
	if (m_datamask[nActor_index]&HEAD_PRESENT) {
		mNumDataItems[nActor_index]+=7;
		nBytes_to_read+=7*sizeof(float);
	}
	if (m_datamask[nActor_index]&LEFTLEG_PRESENT) {
		mNumDataItems[nActor_index]+=21;
		nBytes_to_read+=21*sizeof(float);
	}
	if (m_datamask[nActor_index]&RIGHTLEG_PRESENT) {
		mNumDataItems[nActor_index]+=21;
		nBytes_to_read+=21*sizeof(float);
	}
	if (m_datamask[nActor_index]&CHEST_PRESENT) {
		mNumDataItems[nActor_index]+=7;	
		nBytes_to_read+=7*sizeof(float);
	}
	if (m_datamask[nActor_index]&LEFTHAND_PRESENT) {
		mNumDataItems[nActor_index]+=64;	
		nBytes_to_read+=64*sizeof(float);
	}
	if (m_datamask[nActor_index]&RIGHTHAND_PRESENT) {
		mNumDataItems[nActor_index]+=64;	
		nBytes_to_read+=64*sizeof(float);
	}
	nBytes_to_read+=sizeof(DWORD); //add bytes for timestamp
	nBytes_to_read++;//add byte for state of button board


	if (!mTCP[nActor_index]->ReadDatagram(mSocket[nActor_index],(void *)&mBuf[nActor_index][0],nBytes_to_read,&server_addr,&server_port)) {
		return false;
	}

	//process incoming packet
	channelPtr = (float *)&(mBuf[nActor_index][0]);

	for(long i=0;i<mNumDataItems[nActor_index];i++) {
		m_packetvals[nActor_index][i] = *channelPtr;
		channelPtr++;
	}

	//fill in information for each of the markers
	int data_index=0;
	float euler_rotation[3]={0.0,0.0,0.0};
	if (m_datamask[nActor_index]&(LEFTARM_PRESENT|RIGHTARM_PRESENT|LEFTLEG_PRESENT|RIGHTLEG_PRESENT|HEAD_PRESENT|CHEST_PRESENT)) {
		//hips data is present
		mChannel[nActor_index][0]->SetPos(&m_packetvals[nActor_index][0]);
		ConvertQuattoEuler(&m_packetvals[nActor_index][3],euler_rotation);
		mChannel[nActor_index][0]->SetRot(euler_rotation);
		data_index+=7;
	}
	if (m_datamask[nActor_index]&LEFTARM_PRESENT) {
		mChannel[nActor_index][LSHOULDER]->SetPos(&m_packetvals[nActor_index][data_index]);//left shoulder position
		ConvertQuattoEuler(&m_packetvals[nActor_index][data_index+3],euler_rotation);
		mChannel[nActor_index][LSHOULDER]->SetRot(euler_rotation); //left collarbone rotation (not used)
		data_index+=7;
		mChannel[nActor_index][LELBOW]->SetPos(&m_packetvals[nActor_index][data_index]);//left elbow position
		ConvertQuattoEuler(&m_packetvals[nActor_index][data_index+3],euler_rotation);
		mChannel[nActor_index][LELBOW]->SetRot(euler_rotation);//left upper-arm rotation
		data_index+=7;
		mChannel[nActor_index][LWRIST]->SetPos(&m_packetvals[nActor_index][data_index]);//left wrist position
		ConvertQuattoEuler(&m_packetvals[nActor_index][data_index+3],euler_rotation);
		mChannel[nActor_index][LWRIST]->SetRot(euler_rotation);//left forearm rotation
		data_index+=7;
		mChannel[nActor_index][LKNUCKLE]->SetPos(&m_packetvals[nActor_index][data_index]);//left knuckle position
		ConvertQuattoEuler(&m_packetvals[nActor_index][data_index+3],euler_rotation);
		mChannel[nActor_index][LKNUCKLE]->SetRot(euler_rotation);//left hand rotation
		data_index+=7;
		if (!(m_datamask[nActor_index]&LEFTHAND_PRESENT)) CalculateLHandData(data_index,false,nActor_index);
		else data_index+=4;
	}
	if (m_datamask[nActor_index]&RIGHTARM_PRESENT) {
		mChannel[nActor_index][RSHOULDER]->SetPos(&m_packetvals[nActor_index][data_index]); //right shoulder position
		ConvertQuattoEuler(&m_packetvals[nActor_index][data_index+3],euler_rotation);
		mChannel[nActor_index][RSHOULDER]->SetRot(euler_rotation); //right collarbone rotation (not used)
		data_index+=7;
		mChannel[nActor_index][RELBOW]->SetPos(&m_packetvals[nActor_index][data_index]); //right elbow position
		ConvertQuattoEuler(&m_packetvals[nActor_index][data_index+3],euler_rotation);
		mChannel[nActor_index][RELBOW]->SetRot(euler_rotation); //right upper-arm rotation
		data_index+=7;
		mChannel[nActor_index][RWRIST]->SetPos(&m_packetvals[nActor_index][data_index]); //right wrist position
		ConvertQuattoEuler(&m_packetvals[nActor_index][data_index+3],euler_rotation);
		mChannel[nActor_index][RWRIST]->SetRot(euler_rotation); //right forearm rotation
		data_index+=7;
		mChannel[nActor_index][RKNUCKLE]->SetPos(&m_packetvals[nActor_index][data_index]); //right knuckle position
		ConvertQuattoEuler(&m_packetvals[nActor_index][data_index+3],euler_rotation);
		mChannel[nActor_index][RKNUCKLE]->SetRot(euler_rotation);//right hand rotation
		data_index+=7;
		if (!(m_datamask[nActor_index]&RIGHTHAND_PRESENT)) CalculateRHandData(data_index,false,nActor_index);
		else data_index+=4;
	}
	if (m_datamask[nActor_index]&HEAD_PRESENT) {
		mChannel[nActor_index][HEAD]->SetPos(&m_packetvals[nActor_index][data_index]); //head position
		ConvertQuattoEuler(&m_packetvals[nActor_index][data_index+3],euler_rotation);
		mChannel[nActor_index][HEAD]->SetRot(euler_rotation); //head rotation
		data_index+=7;
	}
	if (m_datamask[nActor_index]&LEFTLEG_PRESENT) {
		mChannel[nActor_index][LKNEE]->SetPos(&m_packetvals[nActor_index][data_index]); //left knee position
		ConvertQuattoEuler(&m_packetvals[nActor_index][data_index+3],euler_rotation);
		mChannel[nActor_index][LKNEE]->SetRot(euler_rotation); //left shin rotation
		data_index+=7;
		mChannel[nActor_index][LANKLE]->SetPos(&m_packetvals[nActor_index][data_index]); //left ankle position
		ConvertQuattoEuler(&m_packetvals[nActor_index][data_index+3],euler_rotation);
		mChannel[nActor_index][LANKLE]->SetRot(euler_rotation); //left foot rotation
		data_index+=7;
		mChannel[nActor_index][LTOE]->SetPos(&m_packetvals[nActor_index][data_index]); //left toe knuckle position
		ConvertQuattoEuler(&m_packetvals[nActor_index][data_index+3],euler_rotation);
		mChannel[nActor_index][LTOE]->SetRot(euler_rotation); //left toe rotation
		data_index+=7;
	}
	if (m_datamask[nActor_index]&RIGHTLEG_PRESENT) {
		mChannel[nActor_index][RKNEE]->SetPos(&m_packetvals[nActor_index][data_index]); //right knee position
		ConvertQuattoEuler(&m_packetvals[nActor_index][data_index+3],euler_rotation);
		mChannel[nActor_index][RKNEE]->SetRot(euler_rotation); //right shin rotation
		data_index+=7;
		mChannel[nActor_index][RANKLE]->SetPos(&m_packetvals[nActor_index][data_index]); //right ankle position
		ConvertQuattoEuler(&m_packetvals[nActor_index][data_index+3],euler_rotation);
		mChannel[nActor_index][RANKLE]->SetRot(euler_rotation); //right foot rotation
		data_index+=7;
		mChannel[nActor_index][RTOE]->SetPos(&m_packetvals[nActor_index][data_index]); //right toe knuckle position
		ConvertQuattoEuler(&m_packetvals[nActor_index][data_index+3],euler_rotation);
		mChannel[nActor_index][RTOE]->SetRot(euler_rotation); //right toe rotation
		data_index+=7;
	}
	if (m_datamask[nActor_index]&CHEST_PRESENT) {
		mChannel[nActor_index][CHEST]->SetPos(&m_packetvals[nActor_index][data_index]); //chest position
		ConvertQuattoEuler(&m_packetvals[nActor_index][data_index+3],euler_rotation);
		mChannel[nActor_index][CHEST]->SetRot(euler_rotation); //chest rotation
		data_index+=7;
	}
	if (m_datamask[nActor_index]&LEFTHAND_PRESENT) {
		CalculateLHandData(data_index,true,nActor_index);
	}
	if (m_datamask[nActor_index]&RIGHTHAND_PRESENT) {
		CalculateRHandData(data_index,true,nActor_index);
	}
	//get time stamp
	mTimeStamp[nActor_index] = *(DWORD *)channelPtr;
	//TRACE("timestamp = %u, current_time = %u\n",mTimeStamp[nActor_index], timeGetTime());

	//push button stuff
	mButtonState[nActor_index] = (int)mBuf[nActor_index][nBytes_to_read-1];
	float currentButtonPos[3];
	currentButtonPos[0]=mChannel[nActor_index][BUTTONS]->GetXPos();
	currentButtonPos[1]=mChannel[nActor_index][BUTTONS]->GetYPos();
	currentButtonPos[2]=0.0;//always set to zero for now
	//see if currentButtonPos[1] should be toggled
	if (mButtonState[nActor_index]>0) {
		//a button is on
		if (mButtonState[nActor_index]!=currentButtonPos[0]) {
			//a new button has just been pressed
			if (mButtonState[nActor_index]==currentButtonPos[1]) {
				currentButtonPos[1]=0.0;//toggle to zero
			}
			else currentButtonPos[1]=mButtonState[nActor_index];
		}
	}
	currentButtonPos[0]=mButtonState[nActor_index]*10.0;
	currentButtonPos[1]*=10.0;
	mChannel[nActor_index][BUTTONS]->SetPos(currentButtonPos);
	//end push button stuff
	if (mTimeStamp[nActor_index]>0) {
		evaltime.SetMilliSeconds(mTimeStamp[nActor_index]);
	}
	else {//using timecode instead
		if (!mTCP[nActor_index]->ReadDatagram(mSocket[nActor_index],(void *)&mNetTimeCode[nActor_index],sizeof(NETWORK_TIMECODE),&server_addr,&server_port)) {
			return false;
		}
		if (mBaseTimeMS[nActor_index]==0) {
			evaltime.SetTime((int)mNetTimeCode[nActor_index].ucHour,(int)mNetTimeCode[nActor_index].ucMinute,(int)mNetTimeCode[nActor_index].ucSecond,(int)mNetTimeCode[nActor_index].ucFrame);
			mBaseTimeMS[nActor_index]=evaltime.GetMilliSeconds();
			mFirstTimeStamp[nActor_index]=mNetTimeCode[nActor_index].dwMSTimestamp;
		}
		else {//compute relative ms time difference
			int nDif = mNetTimeCode[nActor_index].dwMSTimestamp - mFirstTimeStamp[nActor_index];
			kLongLong kLLNewTS = mBaseTimeMS[nActor_index] + nDif;
			evaltime.SetMilliSeconds(kLLNewTS);
		}
		//test		
		kLongLong pHour;  
		kLongLong pMinute;  
		kLongLong pSecond;  
		kLongLong pFrame;  
		kLongLong pField;  
		kLongLong pMilliSecond;  

		double  pFramerate = 60.0;
		if (evaltime.GetTime(pHour,pMinute,pSecond,pFrame,pField,pMilliSecond,kFBTimeModeDefault,pFramerate)) {
			int a=0;
		}
		//end test
	}
	return true;//a complete new data packet has been retreived and is ready to be processed
}

bool FBSimpleNetworkClient::GetSTHandData(FBTime &evaltime,STHAND_DATA *hand_data, bool bLeft, 
	int nActor_index/*=0*/) {
		//returns true if new finger data is available from shapetape hand
		EnterCriticalSection(m_sthand_dataSect[nActor_index]);
		if (bLeft&&m_bLSTHanddata_available[nActor_index]) {
			if (mTimeStamp[nActor_index]>0) {
				evaltime.SetMilliSeconds(mTimeStamp[nActor_index]);
			}
			else {//using timecode instead
				if (mBaseTimeMS[nActor_index]==0) {
					evaltime.SetTime((int)mNetTimeCode[nActor_index].ucHour,(int)mNetTimeCode[nActor_index].ucMinute,(int)mNetTimeCode[nActor_index].ucSecond,(int)mNetTimeCode[nActor_index].ucFrame);
					mBaseTimeMS[nActor_index]=evaltime.GetMilliSeconds();
					mFirstTimeStamp[nActor_index]=mNetTimeCode[nActor_index].dwMSTimestamp;
				}
				else {//compute relative ms time difference
					int nDif = mNetTimeCode[nActor_index].dwMSTimestamp - mFirstTimeStamp[nActor_index];
					kLongLong kLLNewTS = mBaseTimeMS[nActor_index] + nDif;
					evaltime.SetMilliSeconds(kLLNewTS);
				}
			}
			m_bLSTHanddata_available[nActor_index]=false;
			for (int i=0;i<3;i++) {
				hand_data->basefinger_angles[i]=mLHandData[nActor_index]->basefinger_angles[i];
				hand_data->midfinger_angles[i]=mLHandData[nActor_index]->midfinger_angles[i];
				hand_data->tipfinger_angles[i]=mLHandData[nActor_index]->tipfinger_angles[i];
				hand_data->hand_angles[i]=mLHandData[nActor_index]->hand_angles[i];
			}
			LeaveCriticalSection(m_sthand_dataSect[nActor_index]);
			return true;
		}
		else if (!bLeft&&m_bRSTHanddata_available[nActor_index]) {
			if (mTimeStamp[nActor_index]>0) {
				evaltime.SetMilliSeconds(mTimeStamp[nActor_index]);
			}
			else {//using timecode instead
				if (mBaseTimeMS[nActor_index]==0) {
					evaltime.SetTime((int)mNetTimeCode[nActor_index].ucHour,(int)mNetTimeCode[nActor_index].ucMinute,(int)mNetTimeCode[nActor_index].ucSecond,(int)mNetTimeCode[nActor_index].ucFrame);
					mBaseTimeMS[nActor_index]=evaltime.GetMilliSeconds();
					mFirstTimeStamp[nActor_index]=mNetTimeCode[nActor_index].dwMSTimestamp;
				}
				else {//compute relative ms time difference
					int nDif = mNetTimeCode[nActor_index].dwMSTimestamp - mFirstTimeStamp[nActor_index];
					kLongLong kLLNewTS = mBaseTimeMS[nActor_index] + nDif;
					evaltime.SetMilliSeconds(kLLNewTS);
				}
			}
			m_bRSTHanddata_available[nActor_index]=false;
			for (int i=0;i<3;i++) {
				hand_data->basefinger_angles[i]=mRHandData[nActor_index]->basefinger_angles[i];
				hand_data->midfinger_angles[i]=mRHandData[nActor_index]->midfinger_angles[i];
				hand_data->tipfinger_angles[i]=mRHandData[nActor_index]->tipfinger_angles[i];
				hand_data->hand_angles[i]=mRHandData[nActor_index]->hand_angles[i];
			}
			LeaveCriticalSection(m_sthand_dataSect[nActor_index]);
			return true;
		}
		LeaveCriticalSection(m_sthand_dataSect[nActor_index]);
		return false;
}


bool FBSimpleNetworkClient::GetShapeHandData(FBTime &evaltime,SHAPEHAND_DATA *hand_data, bool bLeft, 
	int nActor_index/*=0*/) {
		//returns true if shapehand data is available from shapehand
		EnterCriticalSection(m_shapehand_dataSect[nActor_index]);
		if (bLeft&&m_nLShapehand_available[nActor_index]>0) {
			if (mTimeStamp[nActor_index]>0) {
				evaltime.SetMilliSeconds(mTimeStamp[nActor_index]);
			}
			else {//using timecode instead
				if (mBaseTimeMS[nActor_index]==0) {
					evaltime.SetTime((int)mNetTimeCode[nActor_index].ucHour,(int)mNetTimeCode[nActor_index].ucMinute,(int)mNetTimeCode[nActor_index].ucSecond,(int)mNetTimeCode[nActor_index].ucFrame);
					mBaseTimeMS[nActor_index]=evaltime.GetMilliSeconds();
					mFirstTimeStamp[nActor_index]=mNetTimeCode[nActor_index].dwMSTimestamp;
				}
				else {//compute relative ms time difference
					int nDif = mNetTimeCode[nActor_index].dwMSTimestamp - mFirstTimeStamp[nActor_index];
					kLongLong kLLNewTS = mBaseTimeMS[nActor_index] + nDif;
					evaltime.SetMilliSeconds(kLLNewTS);
				}
			}
			m_nLShapehand_available[nActor_index]--;
			//get hand orientation
			hand_data->hand[0]=mLShapehand[nActor_index]->hand[0];
			hand_data->hand[1]=mLShapehand[nActor_index]->hand[1];
			hand_data->hand[2]=mLShapehand[nActor_index]->hand[2];
			hand_data->hand[3]=mLShapehand[nActor_index]->hand[3];
			//get finger orientations
			for (int i=0;i<3;i++) {
				for (int j=0;j<4;j++) {
					hand_data->thumb[i][j]=mLShapehand[nActor_index]->thumb[i][j];
					hand_data->index[i][j]=mLShapehand[nActor_index]->index[i][j];
					hand_data->middle[i][j]=mLShapehand[nActor_index]->middle[i][j];
					hand_data->ring[i][j]=mLShapehand[nActor_index]->ring[i][j];
					hand_data->small_[i][j]=mLShapehand[nActor_index]->small_[i][j];
				}
			}
			LeaveCriticalSection(m_shapehand_dataSect[nActor_index]);
			return true;
		}
		else if (!bLeft&&m_nRShapehand_available[nActor_index]>0) {
			if (mTimeStamp[nActor_index]>0) {
				evaltime.SetMilliSeconds(mTimeStamp[nActor_index]);
			}
			else {//using timecode instead
				if (mBaseTimeMS[nActor_index]==0) {
					evaltime.SetTime((int)mNetTimeCode[nActor_index].ucHour,(int)mNetTimeCode[nActor_index].ucMinute,(int)mNetTimeCode[nActor_index].ucSecond,(int)mNetTimeCode[nActor_index].ucFrame);
					mBaseTimeMS[nActor_index]=evaltime.GetMilliSeconds();
					mFirstTimeStamp[nActor_index]=mNetTimeCode[nActor_index].dwMSTimestamp;
				}
				else {//compute relative ms time difference
					int nDif = mNetTimeCode[nActor_index].dwMSTimestamp - mFirstTimeStamp[nActor_index];
					kLongLong kLLNewTS = mBaseTimeMS[nActor_index] + nDif;
					evaltime.SetMilliSeconds(kLLNewTS);
				}
			}
			m_nRShapehand_available[nActor_index]--;
			//get hand orientation
			hand_data->hand[0]=mRShapehand[nActor_index]->hand[0];
			hand_data->hand[1]=mRShapehand[nActor_index]->hand[1];
			hand_data->hand[2]=mRShapehand[nActor_index]->hand[2];
			hand_data->hand[3]=mRShapehand[nActor_index]->hand[3];
			//get finger orientations
			for (int i=0;i<3;i++) {
				for (int j=0;j<4;j++) {
					hand_data->thumb[i][j]=mRShapehand[nActor_index]->thumb[i][j];
					hand_data->index[i][j]=mRShapehand[nActor_index]->index[i][j];
					hand_data->middle[i][j]=mRShapehand[nActor_index]->middle[i][j];
					hand_data->ring[i][j]=mRShapehand[nActor_index]->ring[i][j];
					hand_data->small_[i][j]=mRShapehand[nActor_index]->small_[i][j];
				}
			}
			LeaveCriticalSection(m_shapehand_dataSect[nActor_index]);
			return true;
		}
		LeaveCriticalSection(m_shapehand_dataSect[nActor_index]);
		return false;
}


/************************************************
*	Get time in ms from client.
************************************************/
bool FBSimpleNetworkClient::GetTimeMilliSeconds(kLongLong *pTimems, int nActor_index/*=0*/)
{
	if(mTimeStamp[nActor_index] > 0)
	{
		// time is a number of frames
		*pTimems = (double)mTimeStamp[nActor_index];
		return true;
	}

	return false; // failed: no time information is sent by server
}

/************************************************
*	Get button state from client
************************************************/
int FBSimpleNetworkClient::GetButtonState(int nActor_index/*=0*/) {
	return mButtonState[nActor_index];
}



/************************************************
*	Get data from client.
************************************************/
double	FBSimpleNetworkClient::GetDataTX(int pMarker, int nActor_index/*=0*/)	{
	return mChannel[nActor_index][pMarker]->GetXPos();	
}
double	FBSimpleNetworkClient::GetDataTY(int pMarker, int nActor_index/*=0*/)	{
	return mChannel[nActor_index][pMarker]->GetYPos(); 
}
double	FBSimpleNetworkClient::GetDataTZ(int pMarker, int nActor_index/*=0*/)	{	
	return mChannel[nActor_index][pMarker]->GetZPos();	
}
double	FBSimpleNetworkClient::GetDataRX(int pMarker, int nActor_index/*=0*/)	{
	return mChannel[nActor_index][pMarker]->GetXRot(); 
}
double	FBSimpleNetworkClient::GetDataRY(int pMarker, int nActor_index/*=0*/)	{	
	return mChannel[nActor_index][pMarker]->GetYRot();
}
double	FBSimpleNetworkClient::GetDataRZ(int pMarker, int nActor_index/*=0*/)	{
	return mChannel[nActor_index][pMarker]->GetZRot(); 
}

void FBSimpleNetworkClient::ConvertQuattoEuler(float *quat, float *euler)
{
	FBRVector eulerrots(0,0,0);
	//FBQuaternion quatrot((double)quat[1],(double)quat[2],(double)quat[3],(double)quat[0]);
	double vals[4];
	vals[0]=(double)quat[1];
	vals[1]=(double)quat[2];
	vals[2]=(double)quat[3];
	vals[3]=(double)quat[0];
	FBQuaternion quatrot(vals);
	FBQuaternionToRotation	(eulerrots,quatrot);
	euler[0] = (float)eulerrots.mValue[0];
	euler[1] = (float)eulerrots.mValue[1];
	euler[2] = (float)eulerrots.mValue[2];
}

void FBSimpleNetworkClient::SetServerAddress(char *szIPAddress, int nActor_index/*=0*/) {
	if (!mServerAddress[nActor_index]) {
		mServerAddress[nActor_index] = new char[20];
	}
	strcpy(mServerAddress[nActor_index],szIPAddress);
}

void FBSimpleNetworkClient::SetClientAddress(char *szIPAddress, int nActor_index/*=0*/) {
	if (!mClientAddress[nActor_index]) {
		mClientAddress[nActor_index] = new char[20];
	}
	strcpy(mClientAddress[nActor_index],szIPAddress);
}

void FBSimpleNetworkClient::CalculateLHandData(int &data_index, bool bShapeHand, int nActor_index/*=0*/) {
	//calculate left hand data
	FBRVector lHandAngles(mChannel[nActor_index][4]->GetXRot(),mChannel[nActor_index][4]->GetYRot(),
		mChannel[nActor_index][4]->GetZRot());

	//left forearm data
	FBRVector lForeAngles(mChannel[nActor_index][3]->GetXRot(),mChannel[nActor_index][3]->GetYRot(),
		mChannel[nActor_index][3]->GetZRot());

	//matrix data for forearm and hand
	FBMatrix lForeMat, lHandMat;
	FBRotationToMatrix(lForeMat,lForeAngles);
	FBRotationToMatrix(lHandMat,lHandAngles);

	//get hand matrix local to forearm
	FBMatrix lHand_local;
	FBGetLocalMatrix(lHand_local,lForeMat,lHandMat);

	//get localized hand angles (in hand-based coordinate system z==>x, y==>y, -x==>z
	FBMatrixToRotation(lHandAngles,lHand_local);

	FBQuaternion lHandQuat;
	FBRotationToQuaternion(lHandQuat,lHandAngles);
	double tempval=lHandQuat.mValue[0];
	lHandQuat.mValue[0]=lHandQuat.mValue[2];
	lHandQuat.mValue[2]=-tempval;

	FBQuaternionToRotation(lHandAngles,lHandQuat);
	//left and right hand data
	mLHandData[nActor_index]->hand_angles[0]=0.0; //disallow roll of hand
	mLHandData[nActor_index]->hand_angles[1]=-lHandAngles.mValue[1];
	mLHandData[nActor_index]->hand_angles[2]=-lHandAngles.mValue[2];

	//finger data
	if (!bShapeHand) {
		//using shapetape to infer finger movement
		int lfinger_index = data_index;
		//convert quaternion finger data to Euler angles
		float lfinger_eulers[3]={0.0,0.0,0.0};
		ConvertQuattoEuler(&m_packetvals[nActor_index][lfinger_index],lfinger_eulers);
		//interpolate to get incremental orientation angles of each finger segment

		FBRVector lHandBase(0.0,0.0,0.0);
		FBRVector lHandTip(lfinger_eulers[0],lfinger_eulers[1],lfinger_eulers[2]);
		FBInterpolateRotation(lHandAngles,lHandBase,lHandTip,((double)1.0)/3.0);

		//copy to hand structures
		//disable roll for fingers
		mLHandData[nActor_index]->basefinger_angles[0]=0.0;
		mLHandData[nActor_index]->midfinger_angles[0]=0.0;
		mLHandData[nActor_index]->tipfinger_angles[0]=0.0;

		//enable yaw for base of finger only 
		mLHandData[nActor_index]->basefinger_angles[1]=-3*(float)lHandAngles.mValue[1];
		mLHandData[nActor_index]->midfinger_angles[1]=0.0;
		mLHandData[nActor_index]->tipfinger_angles[1]=0.0;

		//enable pitch for all finger segments
		mLHandData[nActor_index]->basefinger_angles[2]=-(float)lHandAngles.mValue[2];
		mLHandData[nActor_index]->midfinger_angles[2]=-(float)lHandAngles.mValue[2];
		mLHandData[nActor_index]->tipfinger_angles[2]=-(float)lHandAngles.mValue[2];

		EnterCriticalSection(m_sthand_dataSect[nActor_index]);
		m_bLSTHanddata_available[nActor_index]=true;
		LeaveCriticalSection(m_sthand_dataSect[nActor_index]);
		data_index+=4;
	}
	else {
		//using shapehand to infer finger movement
		memcpy(mLShapehand[nActor_index]->hand,&m_packetvals[nActor_index][data_index],4*sizeof(float));
		memcpy(mLShapehand[nActor_index]->thumb,&m_packetvals[nActor_index][data_index+4],12*sizeof(float));
		memcpy(mLShapehand[nActor_index]->thumb,&m_packetvals[nActor_index][data_index+4],12*sizeof(float));
		memcpy(mLShapehand[nActor_index]->index,&m_packetvals[nActor_index][data_index+16],12*sizeof(float));
		memcpy(mLShapehand[nActor_index]->middle,&m_packetvals[nActor_index][data_index+28],12*sizeof(float));
		memcpy(mLShapehand[nActor_index]->ring,&m_packetvals[nActor_index][data_index+40],12*sizeof(float));
		memcpy(mLShapehand[nActor_index]->small_,&m_packetvals[nActor_index][data_index+52],12*sizeof(float));
		EnterCriticalSection(m_shapehand_dataSect[nActor_index]);
		m_nLShapehand_available[nActor_index]++;
		LeaveCriticalSection(m_shapehand_dataSect[nActor_index]);
		data_index+=64;
	}				
}

void FBSimpleNetworkClient::CalculateRHandData(int &data_index, bool bShapeHand, int nActor_index) {
	//calculate right hand data
	FBRVector rHandAngles(mChannel[nActor_index][8]->GetXRot(),mChannel[nActor_index][8]->GetYRot(),
		mChannel[nActor_index][8]->GetZRot());

	//right forearm data
	FBRVector rForeAngles(mChannel[nActor_index][7]->GetXRot(),mChannel[nActor_index][7]->GetYRot(),
		mChannel[nActor_index][7]->GetZRot());

	//matrix data for forearm and hand
	FBMatrix rForeMat, rHandMat;
	FBRotationToMatrix(rForeMat,rForeAngles);
	FBRotationToMatrix(rHandMat,rHandAngles);

	//get hand matrix local to forearm
	FBMatrix rHand_local;
	FBGetLocalMatrix(rHand_local,rForeMat,rHandMat);

	//get localized hand angles (in hand-based coordinate system z==>x, y==>y, -x==>z
	FBMatrixToRotation(rHandAngles,rHand_local);
	FBQuaternion rHandQuat;
	FBRotationToQuaternion(rHandQuat,rHandAngles);
	double tempval=rHandQuat.mValue[0];
	rHandQuat.mValue[0]=rHandQuat.mValue[2];
	rHandQuat.mValue[2]=-tempval;

	FBQuaternionToRotation(rHandAngles,rHandQuat);
	//right hand data
	mRHandData[nActor_index]->hand_angles[0]=0.0; //disallow roll of hand
	mRHandData[nActor_index]->hand_angles[1]=-rHandAngles.mValue[1];
	mRHandData[nActor_index]->hand_angles[2]=-rHandAngles.mValue[2];

	//finger data
	if (!bShapeHand) {
		int rfinger_index = data_index;
		//convert quaternion finger data to Euler angles
		float rfinger_eulers[3]={0.0,0.0,0.0};
		ConvertQuattoEuler(&m_packetvals[nActor_index][rfinger_index],rfinger_eulers);
		//interpolate to get incremental orientation angles of each finger segment
		FBRVector rHandBase(0.0,0.0,0.0);
		FBRVector rHandTip(rfinger_eulers[0],rfinger_eulers[1],rfinger_eulers[2]);
		FBInterpolateRotation(rHandAngles,rHandBase,rHandTip,((double)1.0)/3.0);

		//copy to hand structures
		//disable roll for fingers
		mRHandData[nActor_index]->basefinger_angles[0]=0.0;
		mRHandData[nActor_index]->midfinger_angles[0]=0.0;
		mRHandData[nActor_index]->tipfinger_angles[0]=0.0;

		//enable yaw for base of finger only 
		mRHandData[nActor_index]->basefinger_angles[1]=-3*(float)rHandAngles.mValue[1];
		mRHandData[nActor_index]->midfinger_angles[1]=0.0;
		mRHandData[nActor_index]->tipfinger_angles[1]=0.0;

		//enable pitch for all finger segments
		mRHandData[nActor_index]->basefinger_angles[2]=-(float)rHandAngles.mValue[2];
		mRHandData[nActor_index]->midfinger_angles[2]=-(float)rHandAngles.mValue[2];
		mRHandData[nActor_index]->tipfinger_angles[2]=-(float)rHandAngles.mValue[2];

		EnterCriticalSection(m_sthand_dataSect[nActor_index]);
		m_bRSTHanddata_available[nActor_index]=true;
		LeaveCriticalSection(m_sthand_dataSect[nActor_index]);

		data_index+=4;
	}
	else {
		//using shapehand to infer finger movement
		memcpy(mRShapehand[nActor_index]->hand,&m_packetvals[nActor_index][data_index],4*sizeof(float));
		memcpy(mRShapehand[nActor_index]->thumb,&m_packetvals[nActor_index][data_index+4],12*sizeof(float));
		memcpy(mRShapehand[nActor_index]->index,&m_packetvals[nActor_index][data_index+16],12*sizeof(float));
		memcpy(mRShapehand[nActor_index]->middle,&m_packetvals[nActor_index][data_index+28],12*sizeof(float));
		memcpy(mRShapehand[nActor_index]->ring,&m_packetvals[nActor_index][data_index+40],12*sizeof(float));
		memcpy(mRShapehand[nActor_index]->small_,&m_packetvals[nActor_index][data_index+52],12*sizeof(float));
		EnterCriticalSection(m_shapehand_dataSect[nActor_index]);
		m_nRShapehand_available[nActor_index]++;
		LeaveCriticalSection(m_shapehand_dataSect[nActor_index]);
		data_index+=64;
	}
}

BYTE FBSimpleNetworkClient::GetDataMask(int nActor_index/*=0*/) {
	return m_datamask[nActor_index];
}



void FBSimpleNetworkClient::Initialize(int nActor_index) {
	//allocates memory for objects needed for this actor
	if (!m_datamaskSect[nActor_index]) {
		m_datamaskSect[nActor_index] = new CRITICAL_SECTION;
		InitializeCriticalSection(m_datamaskSect[nActor_index]);
	}
	if (!m_shapehand_dataSect[nActor_index]) {
		m_shapehand_dataSect[nActor_index] = new CRITICAL_SECTION;
		InitializeCriticalSection(m_shapehand_dataSect[nActor_index]);
	}
	if (!m_sthand_dataSect[nActor_index]) {
		m_sthand_dataSect[nActor_index] = new CRITICAL_SECTION;
		InitializeCriticalSection(m_sthand_dataSect[nActor_index]);
	}
	if (!mLHandData[nActor_index]) mLHandData[nActor_index] = new STHAND_DATA;
	if (!mRHandData[nActor_index]) mRHandData[nActor_index] = new STHAND_DATA;
	if (!mLShapehand[nActor_index]) mLShapehand[nActor_index] = new SHAPEHAND_DATA;
	if (!mRShapehand[nActor_index]) mRShapehand[nActor_index] = new SHAPEHAND_DATA;
}
