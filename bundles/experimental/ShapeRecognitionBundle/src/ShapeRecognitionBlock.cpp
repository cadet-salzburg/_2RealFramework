#include "_2RealDatatypes.h"
#include "ShapeRecognitionBlock.h"

#include "highgui/highgui.hpp"
#include "BlobResult.h"
#include "dirent.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>


using namespace _2Real::bundle;
using namespace _2Real;
using namespace std;


class ShapeRecognitionBlockImpl
{
public:
	ShapeRecognitionBlockImpl() : 
		m_isShutDown( false ),
		m_image( 0 ),
		m_comparison( 0 ),
		m_oldFrame( 0 ),
		m_minBlobSize( 10000 ),
		m_counter( 0 ),
		m_numberOfFiles( 0 )
	{}

	~ShapeRecognitionBlockImpl()
	{
		if( !m_isShutDown )
			this->shutdown();
	}
	
	void setup(const _2Real::Image &img, const string &path)
	{
		m_image = cvCreateImage(cvSize( img.getWidth(), img.getHeight() ),IPL_DEPTH_8U, 1);
		m_comparison = cvCreateImage(cvSize( img.getWidth(), img.getHeight() ),IPL_DEPTH_8U, 1);
		m_oldFrame = cvCreateImage(cvSize( img.getWidth(), img.getHeight() ),IPL_DEPTH_8U, 1);

		if(loadImages(path))
			cout << "CAUTION! No poses for comparison found at \n" << path << endl;
	}

	int update(const _2Real::Image &img, const char* path, int maxDistance, int minProbability, double &probability)
	{
		if(m_numberOfFiles != getNumberOfFiles(path, "png"))
			loadImages(path);
		
		int imageNr = -1;

		IplImage* one = cvCreateImage(cvSize(img.getWidth(), img.getHeight()), img.getBitsPerChannel(), img.getNumberOfChannels());
		IplImage* two = cvCreateImage(cvSize(img.getWidth(), img.getHeight()), img.getBitsPerChannel(), img.getNumberOfChannels());

		one->imageData = (char*)img.getData();

		uchar* src = (uchar*) one->imageData;
		uchar* dst = (uchar*) two->imageData;

		float thresh = maxDistance/65535.f*255.f;

		for (int j = 0; j < one->imageSize; j++)
		{
			float diff = src[j];		
			
			if (diff > thresh) diff = 0.f;

            dst[j] = diff;		
		}
		
		cvConvertScale(two, m_image);

		cvReleaseImage(&one);
		cvReleaseImage(&two);
		
		m_counter++;
		
		if(m_oldFrame->imageData && m_counter <30)
		{
			cvMul(m_oldFrame, m_image, m_comparison);
			cvCopyImage(m_comparison, m_oldFrame);
		}
		else
		{
			imageNr = comparingImages(minProbability, probability);
			cvCopyImage(m_image, m_comparison);
			m_counter = 0;
		}

		return imageNr;
	}

	void shutdown()
	{
		cvReleaseImage(&m_image);
		cvReleaseImage(&m_comparison);
		cvReleaseImage(&m_oldFrame);
		m_isShutDown = true;
	}

	//---------------------------------------------------------------------------
	// File Management
	//---------------------------------------------------------------------------

	/*
	 *  Determines the number of files of a certain type in a certain folder.
	 */
	int getNumberOfFiles (const char *path, const char *type)
	{
		int counter = 0;
		DIR *pdir = NULL; 
		pdir = opendir (path);
		struct dirent *pent = NULL;
		if (pdir == NULL)
		{
			printf ("\nERROR! Directory could not be initialised correctly");
			return -1;
		}

		while (pent = readdir (pdir))
		{
			if (pent == NULL)
			{
				printf ("\nERROR! File could not be initialised correctly");
				return -1;
			}
			string filename = pent->d_name;
			size_t pos = filename.find(".");
			string extension = filename.substr(pos+1);
			if (extension == type) counter++;
		}
		closedir (pdir);
		return counter;
	}

	/*
	 *  Read in the scan-line parameters of an image from the apropriate txt-file.
	 */
	int* getLines(const char* name)
	{
		int *lines = NULL;
		lines = new int[36];

		string STRING;
		char* cstr;
		ifstream infile;
		infile.open(name);
		int i = 0;

		while(!infile.eof()) // to get all the lines
		{
			getline(infile, STRING); // saves the current line in STRING.
			if(STRING.size() == 0) continue;
			
			size_t pos0 = STRING.find(" ");
			string temp = STRING.substr(pos0+1);


			if(STRING[0]=='l')
			{
				cstr = new char [temp.size()+1];
				strcpy (cstr, temp.c_str());
				lines[i] = atoi(cstr);
				i++;
			}

		}
		infile.close();
		return lines;
	}

	/*
	 *  Load all the poses saved as images in a certain folder and the appropriate line-data.
	 */
	int loadImages(string path)
	{
		m_allPoses.clear();
		m_allLines->clear();

		m_numberOfFiles = getNumberOfFiles(path.c_str(), "png");

		if (m_numberOfFiles ==0) return 1;

		for (int i = 0; i < m_numberOfFiles; i++)
		{	
			string filename = path;
			filename.append("/pose");
			string name = path;
			name.append("/data");
			char buffer [33];
			itoa(i+1, buffer, 10);
			filename.append(buffer);
			name.append(buffer);
			filename.append(".png");
			name.append(".txt");
			m_allPoses.push_back(cvLoadImage(filename.c_str(),0));

			int *lines = NULL;
			lines = getLines(name.c_str());

			m_allLines->push_back(lines);
		}
		return 0;
	}

	/*
	 *  Creates a 2-colored subimage containing the biggest blob of an image. 
	 */
	bool findBiggestBlobImage(IplImage* img, int color, IplImage* &output)
	{
		CBlobResult blobs;
		CBlob *currentBlob;

		blobs = CBlobResult( img, NULL, 0 );
		blobs.Filter( blobs, B_EXCLUDE, CBlobGetArea(), B_LESS, m_minBlobSize );

		double biggestArea = m_minBlobSize;
		int biggestBlob = -1;

		for (int i = 0; i < blobs.GetNumBlobs(); i++ )
		{
			currentBlob = blobs.GetBlob(i);
			double blobArea = currentBlob->Area();
			if(blobArea > biggestArea) 
			{
				biggestBlob = i;
				biggestArea = blobArea;
			}
		}

		if(biggestBlob >= 0)
		{
		//	if (!color)	blobs.GetBlob(biggestBlob)->FillBlob(displayImage, CV_RGB(255,0,0));

			int x = (int) blobs.GetBlob(biggestBlob)->MinX();
			int y = (int) blobs.GetBlob(biggestBlob)->MinY();
			int width= (int) blobs.GetBlob(biggestBlob)->MaxX()-x;
			int height= (int) blobs.GetBlob(biggestBlob)->MaxY()-y;

			IplImage* temp = cvCreateImage(cvGetSize(img),IPL_DEPTH_8U, 1);
			IplImage* temp2 = cvCreateImage(cvSize(width, height),IPL_DEPTH_8U, 1);
			IplImage* result = cvCreateImage(cvSize(width, height),IPL_DEPTH_8U, 1);

			if(biggestBlob>=0) blobs.GetBlob(biggestBlob)->FillBlob(temp,cvScalar(255),x,y);

			cvSetImageROI(temp, cvRect(x, y, width, height));

			cvCopy(temp,temp2);

			uchar* tempData;

			uchar* resultData;

			tempData = (uchar *)(temp2->imageData);
			resultData = (uchar *) (result->imageData);

			for (int j = 0; j < width*height; j++)
			{
				if (tempData[j]==255) resultData[j] = color;
				else	resultData[j] = 0;
			}

			cvResize(result, output);

			cvReleaseImage(&temp);
			cvReleaseImage(&temp2);
			cvReleaseImage(&result);

			return true;
		}
		else
			return false;
	}

	//---------------------------------------------------------------------------
	// Line operations
	//---------------------------------------------------------------------------

	/*
	 *  Determines the number of pixel along a line from the center of an image to a certain point at the corner.
	 */
	int getPixelLine(IplImage img, CvPoint pt)
	{
		CvLineIterator iterator;
		int sum = 0;
		int li = cvInitLineIterator(&img, cvPoint(100,100), pt, &iterator, 8 );
		for( int i = 0; i < li; i++ )
		{
			if(iterator.ptr[0]>127)
			sum++;
			CV_NEXT_LINE_POINT(iterator);
		}
		return sum;
	}

	/*
	 *  Determines all lines for identifying the current image.
	 */
	int getCurrentLines(int* &line, IplImage* blub)
	{
		int sum = 0;
		for(int i = 0; i < 9; i++)
		{			
			line[i+0] = getPixelLine(*blub,cvPoint(i*25, 0));
			sum+= line[i+0];
			line[i+9] = getPixelLine(*blub,cvPoint(i*25, 200));
			sum+= line[i+9];
			line[i+18] = getPixelLine(*blub,cvPoint(0, i*25));
			sum+= line[i+18];
			line[i+27] = getPixelLine(*blub,cvPoint(200, i*25));
			sum+= line[i+27];
		}
		return sum;
	}

	/*
	 *  Compares the line data of two images and sums up the result.
	 */
	int lineComparison(int* comp, int* current)
	{
		int result = 0;

		for(int i= 0; i < 36; i++)
			result+= (comp[i]-current[i])*(comp[i]-current[i]);

		return result;
	}

	//---------------------------------------------------------------------------
	// Image operations
	//---------------------------------------------------------------------------

	/*
	 *  Pixelwise comparison of two images.
	 *  Correct an missing pixels are summed up to a result.
	 */
	double imageComparison(IplImage* comp, IplImage* current)
	{
		IplImage* curr = cvCreateImage(cvGetSize(comp),IPL_DEPTH_8U, 1);
	
		cvResize(current, curr);

		IplImage* result = cvCreateImage(cvGetSize(comp),IPL_DEPTH_8U, 1);

		cvSub(comp, curr, result);

		uchar* resultData;
		resultData = (uchar *)(result->imageData);
		double correct = 0;
		double incorrect = 0;

		for (int j = 0; j < cvGetSize(comp).height*cvGetSize(comp).width; j++)
		{
			if (resultData[j]==127)	correct++;
			else if (resultData[j]==255)	incorrect++;
		}

		cvReleaseImage(&curr);
		cvReleaseImage(&result);		

		double all = correct+incorrect;

		return ((correct-incorrect)/all)*100;
	}

	//---------------------------------------------------------------------------
	// Pose estimation
	//---------------------------------------------------------------------------

	/*
	 *  Determines which pose-image is closest to the current pose.
	 *  Dertermines which scan-line-list ist closest to the one of the current pose.
	 *  Displays the resulting pose-image on the screen.
	 */
	int comparingImages(int minProbability, double &probability)
	{
		IplImage* temp = cvCreateImage(cvSize(200,200),IPL_DEPTH_8U, 1);
		if(findBiggestBlobImage(m_comparison, 128, temp))
		{
			int * lines = NULL;
			lines = new int[36];

			int lineSum = getCurrentLines(lines, temp);

			int biggestVal = -1;

			int lineValue = 100000;
			int lineResult = 110000;
			int bestLinePose = -1;


			for(int i=0; i<m_allPoses.size(); i++)
			{
				double result = imageComparison(m_allPoses[i], temp);

				lineResult = lineComparison(m_allLines->at(i), lines);
		
				if(result > probability)
				{
					biggestVal=i;
					probability = result;
				}
	
				if(lineResult < lineValue)
				{
					bestLinePose = i;
					lineValue = lineResult;
				}
			}

			cvReleaseImage(&temp);
			if((biggestVal == bestLinePose) && (probability > minProbability && probability < 100))
				return bestLinePose;
		}
		return -1;
	}
	
private:
	bool	m_isShutDown;
	IplImage* m_image;
	IplImage* m_comparison;
	IplImage* m_oldFrame;
	vector <IplImage*> m_allPoses;
	vector <int*> m_allLines[36];
	int		m_counter;
	int		m_numberOfFiles;
	int		m_minBlobSize;
};

ShapeRecognitionBlock::ShapeRecognitionBlock() : 
	Block(),
	m_blockImpl( new ShapeRecognitionBlockImpl() )
{
	std::cout << "ShapeRecognitionblock .ctor" << std::endl;
}

ShapeRecognitionBlock::~ShapeRecognitionBlock()
{
	std::cout << "ShapeRecognitionblock .dtor" << std::endl;

	if( m_blockImpl )
	{
		delete m_blockImpl;
		m_blockImpl = NULL;
	}
}

void ShapeRecognitionBlock::setup( BlockHandle &block )
{
	try
	{
		m_Block = block;

		// inlet handles
		m_depthImageIn = m_Block.getInletHandle("depth_image");
		m_dataPathIn = m_Block.getInletHandle("data_path");
		m_maxDistanceIn = m_Block.getInletHandle("max_distance");
		m_minProbabilityIn = m_Block.getInletHandle("min_probability");

		//outlet handles
		m_fileNameOut =	m_Block.getOutletHandle("file_name");
		m_probabilityOut = m_Block.getOutletHandle("probability");

		const Image &image = m_depthImageIn.getReadableRef< Image >();
		
		const std::string &path = m_dataPathIn.getReadableRef< std::string >();

		if( m_blockImpl )
		m_blockImpl->setup(image, path);
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
		Exception exc( e.what() );
		throw exc;
	}
}

void ShapeRecognitionBlock::update()
{
	try
	{
		const Image &image = m_depthImageIn.getReadableRef< Image >();
		string filename = m_dataPathIn.getReadableRef< std::string >();

		int maxDist = m_maxDistanceIn.getReadableRef< int >();
		int minProb = m_minProbabilityIn.getReadableRef< int >();

		double probability = 0.0;

		if( m_blockImpl )
		{			
			int fileNr = m_blockImpl->update(image, filename.c_str(), maxDist, minProb, probability);

			if (fileNr >=0)
			{
				filename.append("/pose");
				char buffer [33];
				itoa(fileNr+1, buffer, 10);
				filename.append(buffer);
				filename.append(".png");

				m_fileNameOut.getWriteableRef<std::string>() = filename;
				m_probabilityOut.getWriteableRef<double>() = probability;
			}
			else
			{
				m_fileNameOut.discard();
				m_probabilityOut.discard();
			}
		}
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
	}
}

void ShapeRecognitionBlock::shutdown() {
	if( m_blockImpl )
		m_blockImpl->shutdown();
}