ShapeRecorder can be configured to output network data and then that network data can be acquired using some 3rd party software (ex: our ShapeWrap plugin for MotionBuilder).

The preferred way to get data from the full ShapeWrap system is to configure 
the output of ShapeRecorder to output joint positions and orientations 
(expressed as quaternions) in real-time over a network socket. Page 8 of the 
attached PDF document describes how to do this. Page 20 of the attached document 
describes the output file format (listed as MotionBuilder format in the 
document, but it is also the same format that we use for our Quest3D and 
Virtools plugins).

After the client application makes an initial request for data (see Page 8 
of attached documentation for how to do this), it should start looking for 
streaming UDP data on the IP Address and port number that it specified in 
its request to the ShapeRecorder server. It should look for data in the 
following manner:

1. Look for initial mask byte. The mask byte will range from 1 to 255 
depending on which ShapeWrap elements are available:

#define LEFTHAND_AVAIL 1
#define RIGHTHAND_AVAIL 2
#define LEFTARM_AVAIL 4
#define RIGHTARM_AVAIL 8
#define LEFTLEG_AVAIL 16
#define RIGHTLEG_AVAIL 32
#define HEAD_AVAIL 64
#define CHEST_AVAIL 128

2. Look for data bytes, and final "zero" byte. A number of data bytes will 
follow, depending on which ShapeWrap devices are available. They will come 
in the order defined on Page 20 of the attached documentation (see 
MotionBuilder output format). Each position coordinate will appear as a 
4-byte floating point value, and each quaternion element will also appear as 
a 4-byte floating point value. The final timestamp is a 4-byte unsigned 
integer. All values are expressed MSB first. A final "zero" byte is included 
at the end of the sample.

Steps 1 & 2 are repeated indefinitely until ShapeRecorder is closed or the 
playback (if working from raw data) is closed.

I have included some of my source code from the MotionBuilder plugin 
(fbsimpletcpip_client.h & fbsimpletcpip_client.cxx) which should hopefully 
provide a useful example for anyone trying to get started with the network 
output. (Note that these files use UDP network connections not TCP/IP... so 
they're not really very well named!)
