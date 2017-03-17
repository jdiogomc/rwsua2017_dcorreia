#if 0
#include <iostream>

int main()
{
    std::cout << "Hello world" << std::endl;           
    return 1;                                                      
}
#endif

#include <iostream>
#include <vector>
#include <boost/shared_ptr.hpp> 
#include <rwsua2017_libs/player.h>
#include <rwsua2017_msgs/MakeAPlay.h>
#include "ros/ros.h"
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include "std_msgs/String.h"
#include <boost/make_shared.hpp>
#include <visualization_msgs/Marker.h>


#define MAX_ANGLE M_PI/30

double randNumber(){
		struct timeval t1;
		gettimeofday(&t1,NULL);
		srand(t1.tv_usec);
		double x =((((double)rand()/(double)RAND_MAX)*2 -1)*5);

		return x;
}


using namespace std;
                                               
namespace rwsua2017{
#if 0
	class Player
	{
	    public:

	    Player(std::string name, std::string team = "blue")
	    {
		this->name = name;
		this->setTeamName(team);
	    }


	    //Set team name, if given a correct team name (accessor)
	    int setTeamName(std::string team)
	    {
		if (team=="red" || team=="green" || team=="blue")
		{
		    this->team = team;
		    return 1;
		}
		else
		{
		   cout << "cannot set team name to " << team << endl;
		    return 0;
		}
	    }

	    void setTeamName(void){
		setTeamName("red");
	    }

	    string getTeam(void) {return team;}

	    string name;

	    private:
		string team;
	};
#endif
	class MyPlayer: public Player{
	
		public:
		//ros::NodeHandle n;
		ros::Subscriber sub;
		tf::TransformBroadcaster br;
		ros::Publisher vis_pub;

		tf::TransformListener listener;
/*
		vector<string> red_team;
		vector<string> green_team;
		vector<string> blue_team;
		boost::shared_ptr<vector<string> > my_team;
		boost::shared_ptr<vector<string> > preys_team;
		boost::shared_ptr<vector<string> > hunters_team;
*/
		MyPlayer(string name, string team): Player(name, team){
/*			//ler parametros com lista de jogadores
			n.getParam("red",red_team);
			n.getParam("green",green_team);
			n.getParam("blue",blue_team);
			
			cout << "Red Team:" << endl;
			for(int i = 0; i<red_team.size();i++){
					cout << red_team[i] << endl;
			}

			cout << endl << "Green Team:" << endl;
			for(int i = 0; i<red_team.size();i++){
					cout << green_team[i] << endl;
			}

			cout << endl << "Blue Team:" << endl;
			for(int i = 0; i<red_team.size();i++){
					cout << blue_team[i] << endl;
			}
			cout << endl;		
	
			if (isMyTeam(red_team, "red_team")){

			}else if(isMyTeam(blue_team, "blue_team")){

			}else if(isMyTeam(green_team, "green_team")){

			}else{
					cout << "I'm not in any team :(" << endl;
			}
*/
		tf::Transform t1;

			t1.setOrigin(tf::Vector3(randNumber(),randNumber(),0));
			tf::Quaternion q;
			q.setRPY(0,0,randNumber());
			t1.setRotation(q);
			br.sendTransform(tf::StampedTransform(t1, ros::Time::now(),"map",name));

		
			cout << endl;

			sub = n.subscribe("/make_a_play/dog",1000, &MyPlayer::makeAPlay,this);

			cout << "Inicialized MyPlayer" << endl;

			vis_pub = n.advertise<visualization_msgs::Marker>( "/bocas", 0 );
		}

		vector<string> teamMates;

		void printTeamMates(){
		    for(size_t i = 0; i<teamMates.size(); i++){
						cout << teamMates[i] << endl;
		    }
		}

		void makeAPlay(const rwsua2017_msgs::MakeAPlay::ConstPtr& msg)
		{
		  cout << "received a makeAPlay msg" << endl;
			cout << "max_dispalcemente: " << msg->max_displacement << endl; 

			//float turn_angle = M_PI/10;
			float displacement = msg->max_displacement;

			

			double dist[3];
			double mindistH = 100000;
			int idxH = 0;
			for(int i = 0; i< msg->green_alive.size();i++){
				dist[i] = getDistFromTo(name, msg->green_alive[i]);
				if(dist[i] < mindistH){
							mindistH = dist[i];
							idxH = i;
					}
			}

			int safedist = 2;
			double angleC;
			if(mindistH < safedist){
					if(msg->green_alive.size() > 0){
						angleC = -getAngleFromTo(name,msg->green_alive[idxH]);
					}else{
						angleC = MAX_ANGLE;
					}
			}else{
				double mindist = 1000000;
				int idx = 0;
				for(int i = 0; i< msg->red_alive.size();i++){
					double dis = getDistFromTo(name, msg->red_alive[i]);
					if(dis < mindist){
							mindist = dis;
							idx = i;
					}
				}
			
				if(msg->red_alive.size() > 0){
					angleC = getAngleFromTo(name,msg->red_alive[idx]);
					}else{
						angleC = MAX_ANGLE;
					}
			}

			move(displacement, angleC, displacement, MAX_ANGLE);

			visualization_msgs::Marker marker;
			marker.header.frame_id = name;
			marker.header.stamp = ros::Time();
			marker.ns = "dcorreia_namespace";
			marker.id = 0;
			marker.type = visualization_msgs::Marker::TEXT_VIEW_FACING;
			marker.action = visualization_msgs::Marker::ADD;
			marker.pose.position.x = 0;
			marker.pose.position.y = 0.4;
			marker.pose.position.z = 0;
			marker.pose.orientation.x = 0.0;
			marker.pose.orientation.y = 0.0;
			marker.pose.orientation.z = 0.0;
			marker.pose.orientation.w = 1.0;
			marker.scale.z = 0.4;
			marker.color.a = 1.0; // Don't forget to set the alpha!
			marker.color.r = 0.3;
			marker.color.g = 0.3;
			marker.color.b = 0.3;
			marker.frame_locked = 1;
			marker.lifetime = ros::Duration(1);

			marker.text = "dnfd";
			vis_pub.publish( marker );

	

		}


tf::StampedTransform getPose(float time_to_wait = 0.1){

	tf::StampedTransform transform;
	ros::Time now = ros::Time(0);
	 try{
			listener.waitForTransform("/map",name,now, ros::Duration(time_to_wait));
     listener.lookupTransform("/map", name,
															now,transform);
    }
    catch (tf::TransformException ex){
      ROS_ERROR("%s",ex.what());
      //ros::Duration(1.0).sleep();
    }

	return transform;
}

double getDistFromTo(string from, string to){

	tf::StampedTransform transform;
	ros::Time now = ros::Time(0);
	float time_to_wait = 0.1;
	double dist = 0;
	 try{
			listener.waitForTransform(from,to,now, ros::Duration(time_to_wait));
     listener.lookupTransform(from, to,
															now,transform);
    }
    catch (tf::TransformException ex){
      ROS_ERROR("%s",ex.what());
      //ros::Duration(1.0).sleep();
    }


			double x = transform.getOrigin().x();
        double y = transform.getOrigin().y();
        dist = sqrt(x*x + y*y);

	return dist;
}

double getAngleFromTo(string myPlayer, string player){

	tf::StampedTransform transform;
	ros::Time now = ros::Time(0);
	float time_to_wait = 0.1;
	 try{
			listener.waitForTransform("/map",name,now, ros::Duration(time_to_wait));
     listener.lookupTransform(myPlayer, player,
															now,transform);
    }
    catch (tf::TransformException ex){
      ROS_ERROR("%s",ex.what());
      //ros::Duration(1.0).sleep();
    }

		double anglle = atan2(transform.getOrigin().y(),
                          transform.getOrigin().x());

		return anglle;
}


void move(float displacement, float turn_angle, int max_displacement, float max_turnAngle){
			tf::Transform tmov;
			tf::Quaternion q;
			
			if(turn_angle > MAX_ANGLE){ turn_angle = max_turnAngle;}
			if(turn_angle < -MAX_ANGLE){ turn_angle = -max_turnAngle;}
			q.setRPY(0,0,turn_angle);

			tmov.setRotation(q);
			tmov.setOrigin(tf::Vector3(displacement*1.1,0,0));

			tf::Transform t = getPose() * tmov;

			br.sendTransform(tf::StampedTransform(t, ros::Time::now(),"map",name));

}
/*
		bool isMyTeam(vector<string> team, string teamName){
				for(int i = 0; i<team.size();i++){
						if(team[i] == name){
							cout << "My Team is " << teamName << endl;
							return true;
						}
				}
				return false;
		}
*/
	};
}


using namespace rwsua2017;

int main(int argc, char **argv)
{

    ros::init(argc,argv,"player_dcorreia");
//Creating an instance of class Player
    MyPlayer player("dcorreia","blue");
    //player.setTeamName("red");

    cout << "player.name is " << player.name << endl;
    cout << "team is " << player.get_team_name() << endl;

    player.teamMates.push_back("fsilva");
    player.teamMates.push_back("vsilva");

    player.printTeamMates();

    vector< boost::shared_ptr<Player> > teamMates;


    ros::spin();
    return 1;
}
