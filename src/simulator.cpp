// Program Information ////////////////////////////////////////////////////////
/**
 * @file simulator.cpp
 *
 * @author Brandon Thai Nguyen
 *
 * @brief Implementation file for simulator class
 *
 * @details Implements all member methods of the simulator class
 *
 * @version 1.0
 *          Brandon Thai Nguyen (03 October 2016)
 *          Original Code
 *
 * @note Requires simulator.h
 */
// Precompiler directives /////////////////////////////////////////////////////

#ifndef CLASS_SIMULATOR_CPP
#define CLASS_SIMULATOR_CPP

// Header files ///////////////////////////////////////////////////////////////

#include "simulator.h"
#include <sys/wait.h>

// Class implementation ///////////////////////////////////////////////////////
/**
* @brief Simulator Constructor
*
* @details Constructs the Simulator Class object
*
* @pre None
*
* @post Simulator class object called
*
* @par Algorithm
*      None
*
* @exception None
*
* @param None
*
* @return None
*
* @note None
*/
Simulator::Simulator()
{
  // No data members to initialize
}

/**
* @brief Simulator Destructor
*
* @details Implicitly destructs all data structures
*
* @pre Assume initialized class object
*
* @post None
*
* @par Algorithm
*      Simulator class data member destructors called implicitely upon deletion
*      of the simulator class
*
* @exception None
*
* @param None
*
* @return None
*
* @note None
*/
Simulator::~Simulator()
{
  // Class data members destructor implictly called
}

/**
* @brief Run
*
* @details Runs the simulation
*
* @pre None
*
* @post Simulation ran
*
* @par Algorithm
*      Run the simulation in a loop executing commands from the user until the
*      user terminates the program
*
* @exception None
*
* @param None
*
* @return None
*
* @note Essentially the main driver
*/
void Simulator::run()
{
  using std::cout;
  using std::cin;
  using std::endl;

  // Varible Declaration
  char command;
  int  worldSizeX = 0, worldSizeY = 0, ticks, numObjects;
  bool terminate = false, success;
  int  runType;

  cout << "Select which Transfer Algorithm you wish to use" << endl;
  cout << " 1 - Flood" << endl;
  cout << " 2 - Dest Goal" << endl;

  cin >> runType;

  if( runType == 1 )
  {
    cout << "Using Flood Algorith" << endl;
    world.RUNALGORITH = FLOOD;
  }
  else if( runType == 2 )
  {
    cout << "Using Dest Goal Algorith" << endl;
    world.RUNALGORITH = DESTSEARCH;
  }
  else
  {
    cout << "ERROR: Invalid input Using Flood algorithm by default" << endl;
    world.RUNALGORITH = FLOOD;
    cin.clear();
    cin.ignore();
  }

  // Run simulator until termination
  while( !terminate )
  {
    // Display menu
    command = displayMenu();

    // Read command from user
    switch( command )
    {
      // Display world
      case 'd':
        world.displayWorld();
        break;

      // Initialize world
      case 'i':
        // Prompt world size from user
        cout << "Enter world size X: ";
        cin >> worldSizeX;
        cout << "Enter world size Y: ";
        cin >> worldSizeY;
        cout << endl;

        // Init world
        success = world.initWorld( worldSizeX, worldSizeY );

        // Print status
        if( success )
        {
          cout << worldSizeX << " by " << worldSizeY;
          cout << " world created . . ." << endl;
          cout << endl;
        }
        else
        {
          cout << "ERROR: INVALID NUMBER ENTERED; WORLD NOT ";
          cout << "INITIALIZED . . ." << endl << endl;
        }
        break;

      // Populate World
      case 'p':
        // Prompt for number of objects
        cout << "Enter number of vehicles to generate ";
        cout << "(Note: Must be less than ";
        cout << ( worldSizeX * worldSizeY ) / 2 << "): ";
        cin >> numObjects;
        cout << endl;

        // Populate world
        success = world.populateWorld( numObjects );

        // Print status
        if( success )
        {
          cout << numObjects << " vehicles created . . .";
          cout << endl << endl;
        }
        else
        {
          cout << "ERROR: INVALID NUMBER ENTERED; NO VEHICLES ";
          cout << "CREATED . . ." << endl << endl;
        }
        break;

      // Run world a single tick
      case 'r':
        world.runWorld( 1 );
        break;

      // Run world n ticks
      case 'R':
        // Prompt for number of ticks
        cout << "Enter number of ticks: ";
        cin >> ticks;
        cout << endl;
        world.runWorld( ticks );
        break;

      // Terminate
      case 'q':
        cout << "Simulation Terminating . . . " << endl;
        cout << endl;
        terminate = true;
        break;

      // Generate Packet
      case 'g':
        if( world.getNumObjects() <= 1 )
        {
          cout << "ERROR: TOO FEW VEHICLES IN WORLD TO GENERATE PACKET" << endl;
          cout << "NO PACKET GENERATED . . ." << endl;
        }
        else
        {
          world.generatePacket();
        }

        break;
      // Unknown command
      default:
        cout << "ERROR: INVALID COMMAND . . ." << endl;
        cout << endl;
        break;
    }
  }
}

/**
* @brief Display Menu
*
* @details Prints the list of available commands
*
* @pre None
*
* @post Displays the menu
*
* @par Algorithm
*      Standard I/O stream operations
*
* @exception None
*
* @param None
*
* @return Char
*
* @note None
*/
char Simulator::displayMenu()
{
  using std::cout;
  using std::cin;
  using std::endl;

  // Varible Declaration
  char command;

  // Display list of commands
  cout << "Simulator Commands" << endl;
  cout << "  d - Display world" << endl;
  cout << "  i - Initialize world" << endl;
  cout << "  p - Populate world" << endl;
  cout << "  g - Generate Packet" << endl;
  cout << "  r - Run world one tick" << endl;
  cout << "  R - Run world n ticks" << endl;
  cout << "  q - Terminate Simulator" << endl;
  cout << "Command: ";
  cin >> command;
  cout << endl;

  // Return command entered
  return command;
}

/**
* @brief Initialize world
*
* @details Dynamically constructs the world with given parameters
*
* @pre Assume initialized world object
*
* @post World initialized
*
* @par Algorithm
*      Call the world class initWorld function
*
* @exception None
*
* @param [in] sizeX
*             Max x-axis world size to be set
*
*        [in] sizeY
*             Max y-axos world size to be set
*
* @return None
*
* @note None
*/
void Simulator::initWorld( int sizeX, int sizeY )
{
  world.initWorld( sizeX, sizeY );
}
// Terminating precompiler directives  ////////////////////////////////////////

#endif  // #ifndef CLASS_SIMULATOR_CPP
