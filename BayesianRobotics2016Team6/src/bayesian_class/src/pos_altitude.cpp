/*
 * pos_altitude.cpp
 *
 *  Created on: May 4, 2016
 *      Author: s
 */


#include <ros/ros.h>

#include <ardrone_autonomy/Navdata.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;




void callback(const ardrone_autonomy::Navdata::ConstPtr& msg)
{
	cout<<"height="<<msg->altd/1000<<endl;
	cout<<"left/right tilt(rotX)="<<msg->rotX<<", Forward/backward tilt(rotY)="<<msg->rotY<<", Orientation(rotZ)="<<msg->rotZ<<endl;
	cout<<"linear speed: vx="<<msg->vx/1000<<", vy="<<msg->vy/1000<<", vz="<<msg->vz/1000<<endl;
}
int main(int argc, char **argv)
{

	  ros::init(argc, argv, "cmd_vel_controller");
	  ros::NodeHandle n;
	  ros::Subscriber navdata_sub = n.subscribe("/ardrone/navdata", 1, callback);


	  int rate=1000;
	  ros::Rate loop_rate(rate);


	   while (ros::ok())
	   {
			ros::spinOnce();

			loop_rate.sleep();
	  }


	  return 0;
}
