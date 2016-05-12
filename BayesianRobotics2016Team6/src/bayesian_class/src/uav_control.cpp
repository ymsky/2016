/*
 * uav_control.cpp
 *
 *  Created on: May 4, 2016
 *      Author: s
 */



#include <ros/ros.h>


#include <geometry_msgs/Pose.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <opencv2/core/core.hpp>
#include <geometry_msgs/Twist.h>
# define M_PI       3.14159265358979323846  /* pi */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "std_msgs/UInt16.h"
#include "std_msgs/Empty.h"

using namespace std;

int m_index=1;
int m_count=0;
bool b_uav1=false;
bool b_uav2=false;

void callback(const std_msgs::UInt16::ConstPtr& msg)
{
	m_index=msg->data;
	cout<<"command_index"<<m_index<<endl;
	m_count=0;
}


int main(int argc, char **argv)
{

	  ros::init(argc, argv, "cmd_vel_controller");
	  ros::NodeHandle n;
	  ros::Publisher cmd_vel_pub1 = n.advertise<geometry_msgs::Twist>("/uav1/cmd_vel", 1000);
	  ros::Publisher takeoff_pub1 = n.advertise<std_msgs::Empty>("/uav1/ardrone/takeoff", 1,true);
	  ros::Publisher land_pub1 = n.advertise<std_msgs::Empty>("/uav1/ardrone/land", 1,true);
	  ros::Publisher reset_pub1 = n.advertise<std_msgs::Empty>("/uav1/ardrone/reset", 1,true);


	  ros::Publisher cmd_vel_pub2 = n.advertise<geometry_msgs::Twist>("/uav2/cmd_vel", 1000);
	  ros::Publisher takeoff_pub2 = n.advertise<std_msgs::Empty>("/uav2/ardrone/takeoff", 1,true);
	  ros::Publisher land_pub2 = n.advertise<std_msgs::Empty>("/uav2/ardrone/land", 1,true);
	  ros::Publisher reset_pub2 = n.advertise<std_msgs::Empty>("/uav2/ardrone/reset", 1,true);

	  ros::Subscriber gui_cmd_sub = n.subscribe("/command_request", 1, callback);

	  int rate=1000;
	  ros::Rate loop_rate(rate);


	   while (ros::ok())
	   {




		   geometry_msgs::Twist cv;
		   geometry_msgs::Twist cv1;
		   geometry_msgs::Twist cv2;


		   cv.linear.x=0;
		   cv.linear.y=0;
		   cv.linear.z=0;
		   cv.angular.x=0;
		   cv.angular.y=0;
		   cv.angular.z=0;
		   cv1=cv;
		   cv2=cv;
		   std_msgs::Empty myMsg;

		   if(m_index==1)	{	}

		   else if(m_index==2) 	   {cv.linear.x=1;}
		   else if(m_index==3)	   {cv.linear.x=-1;}
		   else if(m_index==4)	   {cv.linear.y=1;}
		   else if(m_index==5)	   {cv.linear.y=-1;}
		   else if(m_index==6)	   {cv.linear.z=1;}
		   else if(m_index==7)	   {cv.linear.z=-1;}
		   else if(m_index==8)	   {cv.angular.z=1;}
		   else if(m_index==9)	   {cv.angular.z=-1;}
		   else if(m_index==10)
		   {
			   if (b_uav1) takeoff_pub1.publish(myMsg);
			   if (b_uav2) takeoff_pub2.publish(myMsg);
		   }
		   else if(m_index==11)
		   {
			   if (b_uav1) land_pub1.publish(myMsg);
			   if (b_uav2) land_pub2.publish(myMsg);
		   }
		   else if(m_index==12)
		   {
			   if (b_uav1) reset_pub1.publish(myMsg);
			   if (b_uav2) reset_pub2.publish(myMsg);
		   }
		   else if(m_index==100)	   {b_uav1=true;b_uav2=false;}
		   else if(m_index==101)	   {b_uav1=false;b_uav2=true;}
		   else if(m_index==102)	   {b_uav1=true;b_uav2=true;}


//		   cout<<"count="<<m_count<<endl;
		   if (b_uav1) cv1=cv;
		   if (b_uav2) cv2=cv;

			cmd_vel_pub1.publish(cv1);
			cmd_vel_pub2.publish(cv2);
			m_count++;
			if(m_count>30)
			{m_index=1;}
			if(m_count>1000)
			{m_count=50;}
			ros::spinOnce();

			loop_rate.sleep();
	  }


	  return 0;
}
