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
		MyPlayer(string name, string team): Player(name, team){
			cout << "Inicialized MyPlayer" << endl;
		}
		
		vector<string> teamMates;

		void printTeamMates(){
		    for(size_t i = 0; i<teamMates.size(); i++){
			cout << teamMates[i] << endl;
		    }
		}
	};
}

using namespace rwsua2017;

int main()
{
//Creating an instance of class Player
    MyPlayer player("dcorreia","blue");
    //player.setTeamName("red");

    cout << "player.name is " << player.name << endl;
    cout << "team is " << player.get_team_name() << endl;

    player.teamMates.push_back("fsilva");
    player.teamMates.push_back("vsilva");

    player.printTeamMates();

    vector< boost::shared_ptr<Player> > teamMates;

    return 1;
}
