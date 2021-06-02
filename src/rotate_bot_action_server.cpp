#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <bot_mapper/RotateBotAction.h>
#include <geometry_msgs/Twist.h>


class RotateBotAction {


    protected: 

        ros::NodeHandle nh_ ; 
        actionlib::SimpleActionServer<bot_mapper::RotateBotAction> as_; 

        std::string action_name_ ; 

        bot_mapper::RotateBotFeedback feedback_; 
        bot_mapper::RotateBotResult result_; 

        ros::Publisher vel_pub;

        //ros::geometry_msgs::Pose initial_pose;

    public: 

        RotateBotAction(std::string name) : as_(nh_ , name, boost::bind(&RotateBotAction::executeCB, this, _1) , false), action_name_(name) {

                //initialiseSubscribers();

                vel_pub = nh_.advertise<geometry_msgs::Twist>("/cmd_vel", 1) ;

                //Pose subscriber to be implemented
                as_.start();

        }   


        ~RotateBotAction(void){ 

        }

       


        void executeCB(const bot_mapper::RotateBotGoalConstPtr &goal) {

            ROS_INFO ("Inside the executeCB function!");
            ros::Rate r(0.5);

            bool success = true ;

            //double degree_ang = goal->final_angle ; 

            double degree_ang = goal->final_angle ; 

            //ROS_INFO("degree_ang: %f"), 

            std::cout << "degree_ang: " << degree_ang << std::endl;

            double speed_ang = 10 ; 

            double curr_ang=  0; 
            
            geometry_msgs::Twist vel_msg; 

            int t0 = ros::Time::now().toSec(); 

            while(curr_ang < degree_ang) {


                std::cout << "curr_ang: " << curr_ang << " degree_ang: " << degree_ang << std::endl;
                if(as_.isPreemptRequested()  || !ros::ok()) {
                        
                        as_.setPreempted();
                        ROS_INFO("%s: Preempted" , action_name_.c_str());      
                        success = false;  
                        break;
                }

                ROS_INFO("Inside the while loop inside the executeCB function!");

               
                

                vel_msg.angular.z = speed_ang * atan(1) * 4 / 180.0; 

                vel_pub.publish(vel_msg);

                int t1 = ros::Time::now().toSec(); 

                curr_ang = 1.0 * speed_ang * (t1 - t0) ; 
    
                ROS_INFO("Inside the curr_ang < degree_ang while loop");

                //ROS_INFO("curr_ang: %d" , curr_ang);

                feedback_.curr_angle = curr_ang;

                as_.publishFeedback(feedback_);

                r.sleep();
            }
            
            vel_msg.angular.z =  0; 
            vel_pub.publish(vel_msg);

            if(success) {

                result_.result = true; 
                ROS_INFO("Bot turned successfully");
                as_.setSucceeded(result_ , "Printing from inside the as_.setSucceeded method!");

            }

        }

};

int main(int argc , char** argv) {

    ros::init(argc, argv, "rotate_bot_action_server_node");

    RotateBotAction rb("rotate_bot_action_server_node");

    ros::spin();

    return 0;

}
