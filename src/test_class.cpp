#include <ros/ros.h> 
#include <std_msgs/Int64.h> 
#include <geometry_msgs/Twist.h>
#include <bot_mapper/StartService.h>
#include <math.h>



class BotMapper {

    private: 

       
        ros::Publisher vel_pub; 
        
        //Insert ros rate identifier
        ros::ServiceServer start_service; 
        ros::ServiceServer stop_and_rotate_service;

        
        

    public: 


        BotMapper(ros::NodeHandle *nh) {

            vel_pub = nh->advertise<geometry_msgs::Twist>("/cmd_vel",  10) ; 

            //move_bot();    
            
            //start_service = nh->serviceClient<>("start_bot");

            //ROS_INFO("Trying to start the start_bot service!");
            start_service = nh->advertiseService("/start_bot", &BotMapper::handle_start_bot_service, this);

            stop_and_rotate_service = nh->advertiseService("/stop_and_rotate_bot" , &BotMapper::handle_stop_and_rotate_bot_service, this);
            

        }

        void stop_bot() {
            
            geometry_msgs::Twist vel_msg; 
            vel_msg.linear.x= 0 ;
            vel_pub.publish(vel_msg);

        }

        void rotate_bot() {

            ROS_INFO("Inside the rotate_bot function!");

            int t0 = ros::Time::now().toSec(); 

            double degree_ang = 90 ; 

            double speed_ang = 10 ; 

            double curr_ang=  0; 

            geometry_msgs::Twist vel_msg; 

            vel_msg.angular.z = speed_ang * atan(1) * 4 / 180.0; 

            while(curr_ang < degree_ang) {

                vel_pub.publish(vel_msg);

                int t1 = ros::Time::now().toSec(); 

                curr_ang = 1.0 * speed_ang * (t1 - t0) ; 
    
                std::cout << "curr_ang: " << curr_ang << std::endl;

            }

            vel_msg.angular.z = 0 ; 

            vel_pub.publish(vel_msg);
            
            
        }

        bool handle_stop_and_rotate_bot_service(bot_mapper::StartService::Request &req, bot_mapper::StartService::Response &res) {

            ROS_INFO("Inside the handle_stop_and_rotate_bot_service function");

            //stop_bot(); 

            rotate_bot();

            return true;


        }

        bool handle_start_bot_service(bot_mapper::StartService::Request &req, bot_mapper::StartService::Response &res) {
            
            ROS_INFO("Inside the handle_start_bot_service!");
            move_bot();
            return true;

        }

        void move_bot() {

            ROS_INFO("Inside the move_bot function!");
            geometry_msgs::Twist vel_msg ; 

            vel_msg.linear.x = 1; 

            vel_pub.publish(vel_msg);
      
        }
        


      

};

int main(int argc,  char **argv) {


    ros::init(argc, argv, "bot_mapper");
    ros::NodeHandle nh ; 

    BotMapper nc = BotMapper(&nh);
    ros::spin();

}