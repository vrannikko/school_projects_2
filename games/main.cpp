/*  COMP.CS.100 Project 2: GAME STATISTICS
 *
 * Description:
 *   The program works as a simple game statistics with a clean UI.
 *  In the beginning the program asks for a file as user input, from
 *  which - via successful opening - it saves three bits of data:
 *  game, player, and points from said game. The data file's lines should be
 *  in format game;player;score. Otherwise the program execution terminates
 *  instantly (but still gracefully).
 *   The program then stores this information to a later defined
 *  STATISTICS container, from which the information can be accessed via
 *  a group of simple commands. The user writes in the commands accordingly,
 *  anything else resulting in an invalid input, requiring a retry. Lowercase
 *  and uppercase letters are both allowed when giving commands.
 *
 *  Commands:
 * ALL_GAMES - Prints all known game names
 * GAME <game name> - Prints all players playing the given game
 * ALL_PLAYERS - Prints all known player names
 * PLAYER <player name> - Prints all games the given player plays
 * ADD_GAME <game name> - Adds a new game
 * ADD_PLAYER <game name> <player name> <score> - Adds a new player with the
 * given score for the given game, or updates the player's score if the player
 * already playes the game
 * REMOVE <player name> - Removes the player from all games
 *
 * QUIT - The program terminates
 *
 * =================================
 * PROGRAM'S WRITER
 * =================================
 * Name: Vilhelmiina Rannikko
 * Student ID: H290652
 * Username: vqvira
 * E-Mail: vilhelmiina.rannikko@tuni.fi
 * */

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <set>
#include <utility>
#include <algorithm>

using STATISTICS = std::map<std::string, std::map<std::string, int>>;

//Converts given string to uppercase.
std::string uppercase(std::string &s)
{
    std::string upper_s = "";
    for ( unsigned long int i = 0; i < s.length(); i++ )
    {
        upper_s.push_back(toupper(s[i]));
    }

    return upper_s;
}

// Casual split func, if delim char is between "'s, ignores it.
std::vector<std::string> split( const std::string& str, char delim = ';' )
{
    std::vector<std::string> result = {""};
    bool inside_quatation = false;

    for ( auto current_char : str )
    {
        if ( current_char == '"' )
        {
            inside_quatation = not inside_quatation;
        }
        else if ( current_char == delim and not inside_quatation )
        {
            result.push_back("");
        }
        else
        {
            result.back().push_back(current_char);
        }
    }
    if ( result.back() == "" )
    {
        result.pop_back();
    }

    return result;
}

//Creates a game-themed STATISTICS container from given file.
//Also checks for erroneous lines in the file, and upon an
//encounter, returns an empty container.
STATISTICS create_statistics(std::ifstream& file)
{
    std::string line = "";
    STATISTICS s = {};

    while ( getline(file, line) )
    {
        if ( split(line).size() != 3 )
        {
            return s;
        }

        if ( split(line).at(0) == " " or split(line).at(1) == " ")
        {
            return s;
        }

        std::vector<std::string> data = split(line);
        std::string game = data.at(0);
        std::string person = data.at(1);
        int points = stoi(data.at(2));

        if ( s.find(game) == s.end() )
        {
            std::map<std::string, int> player_data = {};
            player_data.insert( std::make_pair(person, points));
            s.insert( std::make_pair(game, player_data) );
        }
        else
        {
            s[game].insert( std::make_pair(person, points) );
        }
    }

    return s;
}

//Prints every game.
void all_games(const STATISTICS &s)
{
    std::cout << "All games in alphabetical order:" << std::endl;

    for ( const auto &game : s )
    {
        std::cout << game.first << std::endl;
    }
}

//Prints the points and players of given game.
void game(const STATISTICS s, std::string& game_name)
{
    std::map<std::string, int> player_data = s.at(game_name);

    //Storing the values in ascending order to map m.
    std::map<int, std::set<std::string> > m;

    for ( auto &pair : player_data )
    {
        if ( m.find(pair.second) == m.end() )
        {
            m.insert( {pair.second, {pair.first} } );
        }
        else
        {
            m.at(pair.second).insert(pair.first);
        }
    }

    std::cout << "Game " << game_name << " has these scores and players, "
                 "listed in ascending order:" << std::endl;

    //Defining an iterator to commence loop for going through players.
    std::set<std::string>::iterator i;

    for ( auto &pair : m )
    {
        std::cout << pair.first << " : ";

        for ( i = pair.second.begin(); i != pair.second.end(); i++ )
        {
            if ( i != pair.second.begin() ) std::cout << ", ";
            std::cout << *i;
        }

        std::cout << std::endl;
    }

}

//Prints every player.
void all_players(const STATISTICS &s)
{
    std::cout << "All players in alphabetical order:" << std::endl;
    std::set<std::string> players;

    for ( auto &pair : s )
    {
        for ( auto &pair2 : pair.second )
        {
            players.insert(pair2.first);
        }
    }

    for ( const std::string &name : players )
    {
        std::cout << name << std::endl;
    }
}

//Finds whether the player exists under its respective game.
bool does_player_exist(const STATISTICS s, std::string& player_name)
{
    for ( const auto &pair : s )
    {
        if ( pair.second.find(player_name) != pair.second.end() )
        {
            return true;
        }
    }

    return false;
}

//Prints all the games played by given player.
void player(STATISTICS s, std::string player_name)
{
    std::cout << "Player " << player_name << " playes the following"
                 " games:" << std::endl;

    for ( const auto &pair : s )
    {
        if ( pair.second.find(player_name) != pair.second.end() )
        {
            std::cout << pair.first << std::endl;
        }
    }
}

//Adds a new game to a STATISTICS container.
void add_game(STATISTICS &s, std::string &game_name)
{
    std::map<std::string, int> player_data = {};
    s.insert( {game_name, player_data});

    std::cout << "Game was added." << std::endl;
}

//Adds a new player to given game. If player exists already,
//the function updates the player's points.
void add_player(STATISTICS &s, std::string game_name,
                std::string &player_name, int &points)
{
    std::map<std::string, int> &player_data = s.at(game_name);
    std::map<std::string, int>::iterator i = player_data.find(player_name);

    if ( i != player_data.end() )
    {
        i->second = points;
    }

    else
    {
        s[game_name].insert( std::make_pair(player_name, points) );
    }

    std::cout << "Player was added." << std::endl;
}

//Removes a player from all games stated in given STATISTICS container.
void remove(STATISTICS &s, std::string &player_name)
{
    for ( auto &pair : s)
    {
        if ( pair.second.find(player_name) != pair.second.end() )
        {
            pair.second.erase(player_name);
        }
    }

    std::cout << "Player was removed from all games." << std::endl;
}

int main()
{
    //Asking for user input file.
    std::string input_file = "";
    std::cout << "Give a name for input file: ";
    getline(std::cin, input_file);

    std::ifstream open_file(input_file);


    if ( !open_file )
    {
        std::cout << "Error: File could not be read." << std::endl;
        return EXIT_FAILURE;
    }

    //Saving the file's contents to a STATISTICS container.
    STATISTICS games = create_statistics(open_file);

    open_file.close();

    //If create_statistics has returned an empty container, an
    //erroneous line was found from the file and the program terminates.
    if ( games.empty() )
    {
        std::cout << "Error: Invalid format in file."  << std::endl;
        return EXIT_FAILURE;
    }

    //Commencing while loop to manage user input commands.
    while ( true )
    {
        std::string user_input;
        std::cout << "games> ";
        getline(std::cin, user_input);
        std::vector<std::string> full_command = split(user_input, ' ');
        std::string unrefined_first_command = full_command.at(0);

        //Converting to uppercase, and saving to variable first_command.
        std::string first_command = uppercase(unrefined_first_command);

        if ( first_command == "ALL_GAMES" and full_command.size() == 1 )
        {
            all_games(games);
        }

        else if ( first_command == "GAME" and full_command.size() >= 2 )
        {
            std::string input_game = full_command.at(1);
            if ( games.find(input_game) == games.end()
                 or full_command.size() > 2 )
            {
                std::cout << "Error: Game could not be found." << std::endl;
            }
            else
            {
                game(games, input_game);
            }
        }

        else if ( first_command == "ALL_PLAYERS" and full_command.size() == 1 )
        {
            all_players(games);
        }

        else if ( first_command == "PLAYER" and full_command.size() == 2 )
        {
            std::string input_player = full_command.at(1);
            if ( !does_player_exist(games, input_player) )
            {
                std::cout << "Error: Player could not be found." << std::endl;
            }
            else
            {
                player(games, input_player);
            }
        }

        else if ( first_command == "ADD_GAME" and full_command.size() == 2 )
        {
            std::string input_game = full_command.at(1);
            if ( games.find(input_game) != games.end() )
            {
                std::cout << "Error: Already exists." << std::endl;
            }
            else
            {
                add_game(games, input_game);
            }
        }

        else if ( first_command == "ADD_PLAYER" and full_command.size() == 4 )
        {
            std::string input_game = full_command.at(1);
            if ( games.find(input_game) == games.end() )
            {
                std::cout << "Error: Game could not be found." << std::endl;
            }
            else
            {
                std::string input_player = full_command.at(2);
                int points = stoi(full_command.at(3));
                add_player(games, input_game, input_player, points);
            }
        }

        else if ( first_command == "REMOVE" and full_command.size() == 2)
        {
            std::string input_player = full_command.at(1);

            if ( !does_player_exist(games, input_player) )
            {
                std::cout << "Error: Player could not be found." << std::endl;
            }
            else
            {
                remove(games, input_player);
            }
        }

        else if ( first_command == "QUIT" and full_command.size() == 1)
        {
            return EXIT_SUCCESS;
        }

        else
        {
            std::cout << "Error: Invalid input." << std::endl;
        }
    }
}
