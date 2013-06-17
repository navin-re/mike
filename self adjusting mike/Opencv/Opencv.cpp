
#include "stdafx.h"
#include <highgui.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include<cv.h>
#include <iostream>
#include "Tserial.h"
#include <string.h>
#include <vector>

using namespace std;
using namespace cv;
static float a[25],b[25];
char s,t;
int snd,sndpr=0,lo,mk,sm=0,in=0,brk,nface=0;
vector <int> v;

void detect(IplImage *img);

Tserial *arduino;
CvMemStorage *storage;
CvHaarClassifierCascade *cascade;

int _tmain(int argc, _TCHAR* argv[])
{
	CvCapture* capture;
  IplImage *frame,*frame1;
  arduino = new Tserial();
  int i,j;
  if(arduino!=NULL)
	 arduino->connect ("COM5",9600,spNONE);
  for(int i=0,j=90,k=0;i<=24&&j>=0&&k<=180;i++,j=j-4,k=k+8)
  {
	a[i]=j;
	b[i]=k;
  }  
  storage=cvCreateMemStorage(1);
  cascade= (CvHaarClassifierCascade *) cvLoad("haarcascade_frontalface_alt.xml",0,0,0);
  for(int i=0;i<24;i++)
		  cout<<a[i]<<"\t"<<b[i]<<"\t"<<180-a[i]<<"\n";
  capture = cvCaptureFromCAM(0);
		if( capture )
		{
			while( true )
			{
				frame = cvQueryFrame( capture );
				cvAddS(frame,cvScalar(50,50,50),frame);
				if(frame !=NULL)
				{
					detect( frame );
				}
				else
				{
				cout<<"No frame Captured";
				} 
				if(cvWaitKey(5)==27)
				break;
				if(nface>20)
				{
					cout<<"waiting for a face\n";
					sndpr=0;
					lo=0;
					sm=0;
				}

			}
  }	
  arduino->sendChar(0);
  cvReleaseHaarClassifierCascade(&cascade);
  cvReleaseMemStorage(&storage);
  cvReleaseCapture(&capture);
	arduino->disconnect ();
  delete arduino;
  arduino=0;

	return 0;
}

void detect(IplImage *img)
{
	CvSeq * faces;
	CvRect *r;
	float x,y;
	int k,kpr=0,t,temp;
	in++;
	storage=cvCreateMemStorage();
	cvClearMemStorage(storage);
	cvNamedWindow("Test",1);
	if(cascade)
	{
		faces=cvHaarDetectObjects(img,cascade,storage,1.4,4,CV_HAAR_DO_CANNY_PRUNING,cvSize(40,40));
	}
	t=faces->total;
	if(t>0)
	{
		nface=0;
		 r=(CvRect *)cvGetSeqElem(faces,1);
		 cvRectangle(img,cvPoint(r->x,r->y),cvPoint(r->x+r->height,r->y+r->width),CV_RGB(0,255,0),2,8,0);
		 x=(r->x+r->height)+10;
		 y=(r->y+r->width)+10;
		 cvCircle(img,Point(x-r->height/2,y-r->width/3),1,Scalar(255,0,0),2,8,0);
		 k=(int)(y-r->width/3)/20-2;
		 lo=a[k];
		 v.push_back(lo);
		 if(in>10)
		 {
			 for(temp=0;temp<v.size();temp++)
			 {
				 sm=sm+v[temp];
				 snd=sm/v.size();
			 }
			 if(sndpr==0)
			 {
				 arduino->sendChar(snd);
				 cout<<snd<<"\n";
			 }
			 sndpr=snd;
			v.clear();
		 }
	}
	else
		nface++;
	
	cvShowImage("Test",img);
	cvResetImageROI(img);
}



