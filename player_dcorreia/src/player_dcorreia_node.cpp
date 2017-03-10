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
#include "std_msgs/String.h"
#include <boost/make_shared.hpp>

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

		tf::Transform t1;
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

			t1.setOrigin(tf::Vector3(1,1,0));
			tf::Quaternion q;
			q.setRPY(0,0,0);
			t1.setRotation(q);
			br.sendTransform(tf::StampedTransform(t1, ros::Time::now(),"map",name));

		
			cout << endl;

			sub = n.subscribe("/make_a_play",1000, &MyPlayer::makeAPlay,this);

			cout << "Inicialized MyPlayer" << endl;
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

			float turn_angle = M_PI/10;
			float displacement = 0.5;

			tf::Transform tmov;
			tf::Quaternion q;
			q.setRPY(0,0,turn_angle);

			tmov.setRotation(q);
			tmov.setOrigin(tf::Vector3(displacement,0,0));

			tf::Transform t = t1 * tmov;

			br.sendTransform(tf::StampedTransform(t1, ros::Time::now(),"map",name));
			t1 = t;
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
