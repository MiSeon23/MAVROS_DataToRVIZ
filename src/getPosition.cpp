#include "ros/ros.h"
#include <geometry_msgs/PoseStamped.h>
#include <visualization_msgs/Marker.h>

#define PI 3.141592653

bool flag = false;

struct received_position{
    double x, y, z;
}r_position;

struct received_orientation{
    double x, y, z, w;
}r_orientation;

struct EulerAngles{
    double roll, pitch, yaw;
}angles;

visualization_msgs::Marker marker;

void advertise_marker();

void msgCallback(const geometry_msgs::PoseStampedConstPtr& msg){
    flag = true;
    r_position.x = msg->pose.position.x;
    r_position.y = msg->pose.position.y;
    //r_position.z = msg->pose.position.z;
    r_position.z = 0;

    r_orientation.x = msg->pose.orientation.x;
    r_orientation.y = msg->pose.orientation.y;
    r_orientation.z = msg->pose.orientation.z;
    r_orientation.w = msg->pose.orientation.w;

    ROS_INFO("\n ---------- \n");
    ROS_INFO("x = %lf", r_position.x);
    ROS_INFO("y = %lf", r_position.y);
    ROS_INFO("z = %lf", r_position.z);
    ROS_INFO("orientation x = %lf", r_orientation.x);
    ROS_INFO("orientation y = %lf", r_orientation.y);
    ROS_INFO("orientation z = %lf", r_orientation.z);
    ROS_INFO("orientation_w = %lf", r_orientation.w);
}

void advertise_marker(){
    uint32_t shape = visualization_msgs::Marker::ARROW;

    marker.header.frame_id = "/position";
    marker.header.stamp = ros::Time::now();

    marker.ns = "get_position";
    marker.id = 0;

    marker.type = shape;

    marker.action = visualization_msgs::Marker::ADD;

    marker.pose.position.x = r_position.x;
    marker.pose.position.y = r_position.y;
    marker.pose.position.z = r_position.z;
    marker.pose.orientation.x = r_orientation.x;
    marker.pose.orientation.y = r_orientation.y;
    marker.pose.orientation.z = r_orientation.z;
    marker.pose.orientation.w = r_orientation.w;

    marker.scale.x = 0.5;
    marker.scale.y = 0.05;
    marker.scale.z = 0.05;
 
    marker.color.r = 0.0f;
    marker.color.g = 1.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0;

    marker.lifetime = ros::Duration();
}

EulerAngles ToEulerAngles(received_orientation q){
    // roll (x-axis rotation)
    double sinr_cosp = 2*(q.w*q.x + q.y*q.z);
    double cosr_cosp = 1 - 2*(q.x*q.x + q.y*q.y);
    angles.roll = std::atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis ratation)
    double sinp = 2*(q.w*q.y - q.z*q.x);
    if(std::abs(sinp) >= 1)
        angles.pitch = std::copysign(M_PI/2, sinp); // use 90 degrees if out of range
    else
        angles.pitch = std::asin(sinp);

    //yaw (z-axis rotation)
    double siny_cosp = 2*(q.w*q.z + q.x*q.y);
    double cosy_cosp = 1 - 2*(q.y*q.y + q.z*q.z);
    angles.yaw = std::atan2(siny_cosp, cosy_cosp);

    ROS_INFO("yaw = %lf", angles.yaw * 180 / PI);

    return angles;
}

int main(int argc, char **argv){
    ros::init(argc, argv, "sub");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("mavros/local_position/pose", 10, msgCallback);
    ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);

    while(ros::ok()){
        while(flag){
            advertise_marker();
            ToEulerAngles(r_orientation);
            marker_pub.publish(marker);
            flag = false;
        }
        ros::spinOnce();
    }

    ros::spin();
    
    return 0;
}