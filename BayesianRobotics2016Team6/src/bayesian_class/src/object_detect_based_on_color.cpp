


#include <ros/ros.h>

#include <geometry_msgs/Pose.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <opencv2/core/core.hpp>

static const std::string OPENCV_WINDOW = "Image window";
static const std::string OBJECT_WINDOW = "object window";
//static const std::string EDGE_WINDOW = "edge window";
//static const std::string LINE_WINDOW = "line window";

using namespace std;

class ObjectDetectBasedOnColor
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;

public:
  ObjectDetectBasedOnColor()
    : it_(nh_)
  {
    // Subscrive to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/uav1/ardrone/bottom/image_raw", 1,
      &ObjectDetectBasedOnColor::imageCb, this);


    cv::namedWindow(OPENCV_WINDOW);
    cv::namedWindow(OBJECT_WINDOW);
//    cv::namedWindow(EDGE_WINDOW);
//    cv::namedWindow(LINE_WINDOW);


  }

  ~ObjectDetectBasedOnColor()
  {
    cv::destroyWindow(OPENCV_WINDOW);
    cv::destroyWindow(OBJECT_WINDOW);
//    cv::destroyWindow(EDGE_WINDOW);
//    cv::destroyWindow(LINE_WINDOW);
  }

  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }




    cv::Mat object_image;
    cv_ptr->image.copyTo(object_image);



    int rows=cv_ptr->image.rows;
    int cols=cv_ptr->image.cols;




//    cout<<"rows="<<rows<<", cols="<<cols<<endl;
    int N=10;
    int segmentation_rows=9*N;
    int grid_rows=floor(rows/segmentation_rows);
    int segmentation_cols=16*N;
    int grid_cols=floor(cols/segmentation_cols);
//    cout<<"grid_rows="<<grid_rows<<", grid_cols="<<grid_cols<<endl;

    cv::Mat color_segmentation=cv::Mat::zeros(segmentation_rows,segmentation_cols,CV_8UC3);

    for (int i=0;i<segmentation_rows;i++)
    {
        for (int j=0;j<segmentation_cols;j++)
        {
        	float temp_sum_b=0;
        	float temp_sum_g=0;
        	float temp_sum_r=0;
        	float temp_count=0;
        	for (int ii=0;ii<grid_rows;ii++)
        	{
        		for (int jj=0;jj<grid_cols;jj++)
        		{
        			cv::Vec3b bgrPixel=cv_ptr->image.at<cv::Vec3b>(i*grid_rows+ii, j*grid_cols+jj);
        			float b=bgrPixel.val[0];
        			float g=bgrPixel.val[1];
        			float r=bgrPixel.val[2];
        			temp_sum_b+=b;
        			temp_sum_g+=g;
        			temp_sum_r+=r;
        			temp_count++;
        		}
        	}
//        	cout<<"i="<<i<<", j="<<j<<", b="<<temp_sum_b/temp_count<<", g="<<temp_sum_g/temp_count<<", r="<<temp_sum_r/temp_count<<endl;

        	float b=temp_sum_b/temp_count;
        	float g=temp_sum_g/temp_count;
        	float r=temp_sum_r/temp_count;

            *(color_segmentation.data+(i*color_segmentation.cols+j)*color_segmentation.channels()+0)=b;
            *(color_segmentation.data+(i*color_segmentation.cols+j)*color_segmentation.channels()+1)=g;
            *(color_segmentation.data+(i*color_segmentation.cols+j)*color_segmentation.channels()+2)=r;
            cv::rectangle(object_image, cv::Point(j*grid_rows,i*grid_rows), cv::Point((j+1)*grid_rows-1,(i+1)*grid_rows-1),CV_RGB(r,g,b),CV_FILLED);



        }
    }

    int red_count=0;
    int red_sum_i=0;
    int red_sum_j=0;
    int green_count=0;
    int green_sum_i=0;
    int green_sum_j=0;
    for (int i=0;i<segmentation_rows;i++)
    {
        for (int j=0;j<segmentation_cols;j++)
        {
        	cv::Vec3b bgrPixel=color_segmentation.at<cv::Vec3b>(i,j);
			float b=bgrPixel.val[0];
			float g=bgrPixel.val[1];
			float r=bgrPixel.val[2];

			if	((r>200)&&(g/r<0.5)&&(b/r<0.5))
			{
				cv::circle(object_image, cv::Point((j+0.5)*grid_cols, (i+0.5)*grid_rows), 20/N, CV_RGB(0,255,0),CV_FILLED);
				red_count++;
				red_sum_i+=i;
				red_sum_j+=j;
//				cout<<"red_pixel: b="<<b<<", g="<<g<<", r="<<r<<", g/r="<<g/r<<", b/r="<<b/r<<endl;

			}
			if	(((float)(r/g)<0.9)&&(g>150)&&((float)(b/g)<0.9))
			{
				cv::circle(object_image, cv::Point((j+0.5)*grid_cols, (i+0.5)*grid_rows), 20/N, CV_RGB(255,0,0),CV_FILLED);
				green_count++;
				green_sum_i+=i;
				green_sum_j+=j;
//				cout<<"green_pixel: b="<<b<<", g="<<g<<", r="<<r<<", r/g="<<r/g<<", b/g="<<b/g<<endl;
			}
		}
	}
    cout<<"red_seg_number="<<red_count<<endl;
    cout<<"green_seg_number="<<green_count<<endl;

    if(red_count>10*N)
    {
    	float red_center_i=(red_sum_i/red_count+0.5)*grid_rows;
    	float red_center_j=(red_sum_j/red_count+0.5)*grid_cols;
        cout<<"red_object at("<<red_center_i<<","<<red_center_i<<")"<<endl;
		cv::circle(object_image, cv::Point(red_center_j, red_center_i),
				sqrt(red_count*grid_cols*grid_rows), CV_RGB(0,255,0),10);
    }

    if(green_count>10*N)
    {
    	float green_center_i=(green_sum_i/green_count+0.5)*grid_rows;
    	float green_center_j=(green_sum_j/green_count+0.5)*grid_cols;
        cout<<"green_object at("<<green_center_i<<","<<green_center_i<<")"<<endl;
		cv::circle(object_image, cv::Point(green_center_j, green_center_i),
				sqrt(green_count*grid_cols*grid_rows), CV_RGB(255,0,0),10);
    }



    //draw grid
    for (int i=0;i<segmentation_rows+1;i++)
    {cv::line(object_image, cv::Point(0,i*grid_rows),cv::Point(cols,i*grid_rows),CV_RGB(128,128,128));}
    for (int j=0;j<segmentation_cols+1;j++)
    {cv::line(object_image, cv::Point(j*grid_cols,0),cv::Point(j*grid_cols,rows),CV_RGB(128,128,128));}


    // Update GUI Window
    cv::imshow(OPENCV_WINDOW, cv_ptr->image);
    cv::waitKey(3);
    cv::imshow(OBJECT_WINDOW, object_image);
      cv::waitKey(3);

  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "object_detect_based_on_color");
  ObjectDetectBasedOnColor od;
  ros::spin();
  return 0;
}
