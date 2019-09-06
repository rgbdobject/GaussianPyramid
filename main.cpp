#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;

const char* filename;

int nOctaves=4;
int nOctaveLayers=3;
double sigma=1.6;

Mat source,image;
int DELAY_CAPTION = 5000;
int DELAY_BLUR = 1000;
char window_name[] = "Gaussian Pyramid Demo";
vector<Mat> pyramids;




void buildGaussianPyramid( const Mat& base, std::vector<Mat>& pyr, int nOctaves )
{
	vector<double> sig(nOctaveLayers+3);
	pyr.resize(nOctaves*(nOctaveLayers+3));
    // precompute Gaussian sigmas using the following formula:
    //  \sigma_{total}^2 = \sigma_{i}^2 + \sigma_{i-1}^2
	sig[0]=sigma;
	double k=std::pow(2.,1./nOctaveLayers);

	for(int i=1;i<nOctaveLayers+3;i++)
	{
		double sig_prev=std::pow(k,(double)(i-1))*sigma;
		double sig_total = sig_prev*k;
		sig[i] = std::sqrt(sig_total*sig_total - sig_prev*sig_prev);

	}

	for(int o=0;o<nOctaves;o++)
	{
		for(int i=0;i<nOctaveLayers+3;i++)
		{
			Mat& dst=pyr[o*(nOctaveLayers+3)+i];

			if(o==0 && i==0)
				dst=base;
			// base of new octave is halved image from end of previous octave
			else if(i==0)
			{
				const Mat& src = pyr[(o-1)*(nOctaveLayers + 3) + nOctaveLayers];
				resize(src,dst,Size(src.cols/2,src.rows/2),0,0,INTER_NEAREST);
			}
			else
			{
				const Mat& src=pyr[o*(nOctaveLayers+3)+i-1];
				GaussianBlur(src,dst,Size(),sig[i],sig[i]);
			}

		}
	}

}

int display_dst( int delay )
{
	namedWindow(window_name,  1 );
    imshow( window_name, image );
    int c = waitKey ( delay );
    if( c >= 0 ) { return -1; }
    return 0;
}

int display_caption( const char* caption )
{
    image = Mat::zeros( source.size(), source.type() );
    putText( image, caption,
             Point( source.cols/4, source.rows/2),
             FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255) );
    return display_dst(DELAY_CAPTION);
}

void
parseCommandLine (int argc, char *argv[])
{
	 filename = argc >1 ? argv[1] : "lena1K1K.jpg";
	 source= imread(filename,  CV_LOAD_IMAGE_GRAYSCALE);
	 if (source.empty()) //check whether the image is loaded or not
		 {
		 cout << "Error : Image cannot be loaded..!!" << endl;

		 exit (EXIT_FAILURE);
		 }
}

int main(int argc, char *argv[])
{
	parseCommandLine(argc, argv);

	cout<<source.cols<<" "<<source.rows<<endl;

	buildGaussianPyramid(source, pyramids, nOctaves );

    if( display_caption( "DEMO for Gaussian Pyramid" ) != 0 )
    {
        return 0;
    }

    image=source.clone();

    if( display_dst( DELAY_CAPTION ) != 0 )
    {
        return 0;
    }

    if( display_caption( "Gaussian Pyramid Starts..." ) != 0 )
    {
        return 0;
    }

    if( display_caption( "Octave 1" ) != 0 )
    {
        return 0;
    }

    for ( int i = 0; i < 6; i++ )
    {
        image=pyramids[i].clone();
        if( display_dst( DELAY_BLUR ) != 0 )
        {
            return 0;
        }
    }

    if( display_caption( "Octave 2" ) != 0 )
    {
        return 0;
    }

    for ( int i = 6; i < 12; i++ )
    {
        image=pyramids[i].clone();
        if( display_dst( DELAY_BLUR ) != 0 )
        {
            return 0;
        }
    }

    if( display_caption( "Octave 3" ) != 0 )
    {
        return 0;
    }

    for ( int i = 12; i < 18; i++ )
    {
        image=pyramids[i].clone();
        if( display_dst( DELAY_BLUR ) != 0 )
        {
            return 0;
        }
    }

    if( display_caption( "Octave 4" ) != 0 )
    {
        return 0;
    }

    for ( int i = 18; i < 24; i++ )
    {
        image=pyramids[i].clone();
        if( display_dst( DELAY_BLUR ) != 0 )
        {
            return 0;
        }
    }


//	namedWindow("Pyramid", CV_WINDOW_NORMAL);
//	imshow("Pyramid", pyramids[18]);
//	waitKey(0);

    return 0;

}
