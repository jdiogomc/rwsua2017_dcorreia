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
#include "std_msgs/String.h"

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
		ros::NodeHandle n;

		ros::Subscriber sub;

		MyPlayer(string name, string team): Player(name, team){
			cout << "Inicialized MyPlayer" << endl;

			sub = n.subscribe("/make_a_play",1000, &MyPlayer::makeAPlay,this);
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
		}
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
