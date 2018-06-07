#include "ros/ros.h"
#include "std_msgs/UInt16.h"
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sstream>


  char getch()
{
    int flags = fcntl(0, F_GETFL, 0);
    fcntl(0, F_SETFL, flags | O_NONBLOCK);

    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0) {
        perror("tcsetattr()");
    }
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0) {
        perror("tcsetattr ICANON");
    }
    if (read(0, &buf, 1) < 0) {
        //perror ("read()");
    }
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0) {
        perror ("tcsetattr ~ICANON");
    }
    return (buf);
}

int main(int argc, char **argv)

{
  ros::init(argc, argv, "talker");
  ros::NodeHandle n;
  ros::Publisher servo1_pub = n.advertise<std_msgs::UInt16>("servo1", 1000);
  ros::Publisher servo2_pub = n.advertise<std_msgs::UInt16>("servo2", 1000);
  std_msgs::UInt16 msg;
    
  msg.data = 50;

  ros::Rate loop_rate(50);

  int count = 0;


  while (ros::ok())
  {
    if(msg.data==180)
     {
        int c = getch();
        	if (c != EOF) {
            		switch (c) {
                //case 113:    // key up
                	//msg.data  += 5;                	break;
                case 122:    // key down
                	msg.data -= 5;
                	break;
                        }}}
    else if(msg.data==0)
      {
        int c = getch();
           if (c != EOF) {
                 switch (c) {
                   case 113:    // key up
                    msg.data  += 5;                       break;
                        //case 122:    // key down
                        //msg.data -= 5;
                        //break;
                            }
                          }
      }
     else
      {
        int c = getch();
           if (c != EOF) {
                 switch (c) {
                   case 113:    // key up
                    msg.data  += 5;                       break;
                   case 122:    // key down
                    msg.data -= 5;
                        break;
                            }
                          }
           //std::cout << c << std::endl;
      }


    ROS_INFO("%d", msg.data);

    servo1_pub.publish(msg);
    servo2_pub.publish(msg);
    ros::spinOnce();
    loop_rate.sleep();
    ++count;
  }
  return 0;
}
  
  



