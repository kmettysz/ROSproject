#include <signal.h>
#include <stdio.h>
#include <termios.h>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

#define KEYCODE_UP 'w'
#define KEYCODE_LEFT 'a'
#define KEYCODE_DOWN 's'
#define KEYCODE_RIGHT 'd' 
class Robot
{
  public:
    Robot();
    void keyLoop();

  private:
    ros::NodeHandle nh;
    double linear, angular;
    ros::Publisher twistPub;
};

Robot::Robot() : linear(0), angular(0)
{
  twistPub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
}

int kfd = 0;
struct termios before, after;

void quit(int sig)
{
  (void)sig;
  tcsetattr(kfd, TCSANOW, &before);
  ros::shutdown();
  exit(0);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "robot_teleop");
  Robot robot;
  signal(SIGINT, quit);

  robot.keyLoop();

  return 0;
}

void Robot::keyLoop()
{
  char c;
  bool dirty = false;

  // get the console in raw mode
  tcgetattr(kfd, &before);
  memcpy(&after, &before, sizeof(struct termios));
  after.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(kfd, TCSANOW, &after);

  puts("Varom a billentyuzet leuteset:");
  puts("Hasynald a WASD billenyukat a robot mozgatasahoz");

  while (ros::ok())
  {
    c = getchar();

    linear = angular = 0;
    ROS_DEBUG("value: 0x%02X\n", c);

    switch (c)
    {
      case KEYCODE_LEFT:
        ROS_DEBUG("LEFT");
        angular = 1.0;
        dirty = true;
        break;
      case KEYCODE_RIGHT:
        ROS_DEBUG("RIGHT");
        angular = -1.0;
        dirty = true;
        break;
      case KEYCODE_UP:
        ROS_DEBUG("UP");
        linear = 1.0;
        dirty = true;
        break;
      case KEYCODE_DOWN:
        ROS_DEBUG("DOWN");
        linear = -1.0;
        dirty = true;
        break;
    }

    geometry_msgs::Twist twist;
    twist.angular.z = angular;
    twist.linear.x = linear;
    if (dirty == true)
    {
      twistPub.publish(twist);
      dirty = false;
    }
  }

  return;
}
