// Program Information ////////////////////////////////////////////////////////
/**
 * @file world.cpp
 *
 * @author Brandon Thai Nguyen
 *
 * @brief Implementation file for world class
 *
 * @details Implements all member methods of the world class
 *
 * @version
 *          1.3
 *          Daniel Smith
 *          Added generate packet and packet transfering capabilities
 *
 *          1.2
 *          Tyler Goffinet
 *          Implemented display and updated runWorld()
 *
 *          1.1
 *          Tyler Goffinet
 *          Updated for use with vehicle classes
 *
 *          1.0
 *          Brandon Thai Nguyen (03 October 2016)
 *          Original Code
 *
 * @note Requires world.h
 */

// Precompiler directives /////////////////////////////////////////////////////

#ifndef CLASS_WORLD_CPP
#define CLASS_WORLD_CPP

// Header files ///////////////////////////////////////////////////////////////

#include "world.h"

// Class implementation ///////////////////////////////////////////////////////

/**
* @brief Default/Initialization constructor
*
* @details Constructs World Class
*
* @pre None
*
* @post Initializes all data quantities
*
* @par Algorithm
*      Standard initialization operation
*
* @exception None
*
* @param None
*
* @return None
*
* @note None
*/
template <class DataType>
World<DataType>::World() : worldSizeX( 0 ), worldSizeY( 0 ), numObjects( 0 )
{
  // Init list used
}

/**
* @brief World Destructor
*
* @details Deletes the world and all its objects
*
* @pre Assumes at least one node constructed
*
* @post All linked list nodes are removed
*
* @par Algorithm
*      Dynamically allocate an array of pointers (x-axis) then dynamically
*
* @exception None
*
* @param None
*
* @return None
*
* @note None
*/
template <class DataType>
World<DataType>::~World()
{
  // Varible Declaration
  DataType **tempWorldYPtr;
  DataType * tempObjectPtr;

  // Delete world
  for( int x = 0; x < worldSizeX; x++ )
  {
    // Delete objects
    for( int y = 0; y < worldSizeY; y++ )
    {
      if( world[x][y] != NULL )
      {
        tempObjectPtr = world[x][y];
        delete tempObjectPtr;
      }
    }

    // Delete world Y
    tempWorldYPtr = world[x];
    delete tempWorldYPtr;
  }

  delete world;

  worldSizeX = 0;
  worldSizeY = 0;
}

/**
* @brief Initialize world
*
* @details Dynamically constructs the world with given parameters
*
* @pre None
*
* @post World initialized
*
* @par Algorithm
*      Dynamically allocates the xCoor pointer and the yCoor pointers based on
*      the parameters and sets all object pointers to NULL
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
* @note Can be ran multiple times
*
* @note World is empty after initialization
*/
template <class DataType>
bool World<DataType>::initWorld( int sizeX, int sizeY )
{
  // Check sizes
  if( sizeX <= 0 || sizeY <= 0 )
  {
    return false;
  }

  // Clear world
  if( numObjects > 0 )
  {
    clearWorld();
  }

  // Set city size limits
  worldSizeX = sizeX;
  worldSizeY = sizeY;

  // Dynamically size city
  world = new DataType **[worldSizeX];
  for( int x = 0; x < worldSizeX; x++ )
  {
    world[x] = new DataType *[worldSizeY];

    // Point object pointers to NULL
    for( int y = 0; y < worldSizeY; y++ )
    {
      world[x][y] = NULL;
    }
  }
  return true;
}

/**
* @brief Displays world in ASCII format
*
* @details Outputs to terminal all intersections and interminate roadways
*
* @pre Assumes initialized world object and world data member
*
* @post World filled with objects
*
* @par Algorithm
*      Loop through intersections within world displaying their symbols
*
* @exception Cannot display before world initialization
*
* @param None
*
* @return None
*
* @note None
*/
template <class DataType>
void World<DataType>::displayWorld()
{
  using std::cout;
  using std::endl;

  DataType *data;

  std::vector<Intersect1>               tempVect;
  std::vector<std::vector<Intersect1> > intersects;

  cout << endl;

  // Construct intersects
  for( int x = 0; x < worldSizeX; x++ )
  {
    tempVect = std::vector<Intersect1>();

    for( int y = 0; y < worldSizeY; y++ )
    {
      data = world[x][y];

      // Enter object data into new intersection
      if( data == NULL )
      {
        tempVect.push_back( Intersect1() );
      }
      else
      {
        // std::cout << (char)(data->getVehicleId() + '0') << std::endl;
        tempVect.push_back(
            Intersect1( 'T', data->hasPacket(), data->getDirection() ) );
      }
    }

    intersects.push_back( tempVect );
  }

  // Stitch intersections together
  for( int y = 0; y < worldSizeY; y++ )
  {
    for( int i = 0; i < Intersect1::NUM_ROWS; i++ )
    {
      // Ouput each row of intersections
      for( int x = 0; x < worldSizeX; x++ )
      {
        cout << intersects[x][y][i];

        if( x < worldSizeX - 1 )
        {
          cout << Intersect1::VERT_ROADWAY[i];
        }
      }

      cout << Intersect1::HORI_ROADWAY[0];
      cout << endl;
    }
  }

  cout << endl;
}

/**
 * @brief Populate World
 *
 * @details Fills the world with objects
 *
 * @pre Assumes initialized world object and world data member
 *
 * @post World filled with objects
 *
 * @par Algorithm
 *      Create n amound of new objects at random coordinates without overlapping
 *      previously created objects
 *
 * @exception Cannot populate before world initialization
 *
 * @param [in] numObjects
*              Number of objects to fill the world with
 *
 * @return Boolean signifying populating success
 *
 * @note Number of objects can not exceed world capacity
 *
 * @note INCOMPLETE FUNCTION: Need something to assign the coordinates to the
 *                            vehicle
 */
template <class DataType>
bool World<DataType>::populateWorld( int numObjectsToInsert )
{
  // Varible Declaration
  bool objectPresent;
  int  xCoor, yCoor;

  // Seed number generator
  srand( time( NULL ) );

  // Check number if objects being added to avoid overflow
  if( ( numObjects + numObjectsToInsert ) >= ( worldSizeX * worldSizeY / 2 ) )
  {
    return false;
  }

  // Check if number of objects exceed capacity
  if( numObjectsToInsert >= ( worldSizeX * worldSizeY / 2 ) &&
      numObjectsToInsert > 0 )
  {
    return false;
  }

  // Populate world
  for( int i = 0; i < numObjectsToInsert; i++ )
  {
    // Loop until empty coordinates found
    do
    {
      // Get random coordinates
      xCoor = rand() % ( worldSizeX - 1 );
      yCoor = rand() % ( worldSizeY - 1 );

      // Check if object is present at that coordinate
      objectPresent = isObjectPresent( xCoor, yCoor );

      // If no object present, create new object
      if( !objectPresent )
      {
        // Create new object
        world[xCoor][yCoor] =
            new DataType( xCoor, yCoor, worldSizeX, worldSizeY );

        // Push new object onto list
        objectList.push_back( world[xCoor][yCoor] );
        objectActionCounter.push_back( 0 );

        // Increment object counter
        numObjects++;
      }
    } while( objectPresent );
  }

  // Return true indicating population success
  return true;
}

/**
* @brief Clear World
*
* @details Deletes all objects present in the world
*
* @pre Assume initialized class object
*
* @post All elements in world deleted
*
* @par Algorithm
*      Go through each world array element and delete object. Set the object
*      pointer to NULL
*
* @exception None
*
* @param None
*
* @return None
*
* @note None
*/
template <class DataType>
void World<DataType>::clearWorld()
{
  // Varible Declaration
  DataType *tempObjectPtr;

  // Go through the world and delete any elements present
  for( int x = 0; x < worldSizeX; x++ )
  {
    for( int y = 0; y < worldSizeY; y++ )
    {
      // Delete object if found
      if( world[x][y] != NULL )
      {
        tempObjectPtr = world[x][y];
        delete tempObjectPtr;
        world[x][y] = NULL;
      }
    }
  }

  // Clear list
  objectList.clear();
  numObjects = 0;
}

/**
 * @brief Run World
 *
 * @details Runs the world an arbituary amount of ticks
 *
 * @pre Assume initialized class object
 *
 * @post World ran
 *
 * @par Algorithm
 *      Go through each array element checking each
 *
 * @exception None
 *
 * @param [in] ticks
 *             Number of times to run each element's function
 *
 * @return None
 *
 * @note None
 */
template <class DataType>
void World<DataType>::runWorld( int ticks )
{

  // Run world for n amount of ticks
  for( int i = 0; i < ticks; i++ )
  {
    // look for packets and throw to next destination
    if( RUNALGORITH == FLOOD )
    {
      runFlood();
    }

    else if( RUNALGORITH == DESTSEARCH )
    {
      runDest();
    }
  }

  displayWorld();
}

/**
 * @brief Is object present
 *
 * @details Checks if an object is present at given location
 *
 * @pre Assumes initialized class object
 *
 * @post Returns if the object is present or not
 *
 * @par Algorithm
 *      Check if the coordinates are in range, then check if a NULL ptr is not
 *      present at given coordinates
 *
 * @exception None
 *
 * @param [in] xCoor
 *             X-axis coordinate
 *
 *        [in] yCoor
 *             Y-axis coordinate
 *
 * @return Boolean signifying if an object is present
 *
 * @note None
 */
template <class DataType>
bool World<DataType>::isObjectPresent( int xCoor, int yCoor )
{
  // Check range
  if( xCoor < 0 || worldSizeX <= xCoor || yCoor < 0 || worldSizeY <= yCoor )
  {
    return false;
  }

  // Check if object is present
  if( world[xCoor][yCoor] != NULL )
  {
    return true;
  }

  return false;
}

/**
* @brief Get Number of Objects
*
* @details Returns the number of objects present in the world
*
* @pre Assumes initialized class object
*
* @post Number of objects returned
*
* @par Algorithm
*      Return numObject varible
*
* @exception None
*
* @param None
*
* @return Integer specifying of objects present in the world
*
* @note None
*/
template <class DataType>
int World<DataType>::getNumObjects()
{
  return numObjects;
}

/**
* @brief Get Object List
*
* @details Returns the list of objects in the world
*
* @pre Assume initialized object class
*
* @post List of objects returned
*
* @par Algorithm
*      Return the list of objects's address
*
* @exception None
*
* @param None
*
* @return Address of list of object pointers
*
* @note None
*/
template <class DataType>
std::vector<DataType *> &World<DataType>::getObjectList()
{
  return objectList;
}

/**
* @brief Insert Object
*
* @details Inserts object at specified coordinates
*
* @pre Assume initialized class object
*
* @post Object inserted into world
*
* @par Algorithm
*      Check if there is not already an object present at given coordinates and
*      insert the object into the world and push that object into the list
*
* @exception None
*
* @param [in] xCoor
*             X-axis coordinate
*
*        [in] yCoor
*             Y-axis coordinate
*
*        [out] object
*              Object to insert into the world
*
* @return Bool stating insertion success
*
* @note None
*/
template <class DataType>
bool World<DataType>::insertObject( int xCoor, int yCoor, DataType *object )
{
  // Check range
  if( xCoor > worldSizeX || yCoor > worldSizeY )
  {
    return false;
  }

  // Insert object if nothing is present
  if( !isObjectPresent( xCoor, yCoor ) )
  {
    // Create new object
    world[xCoor][yCoor] = object;

    // Insert at end of object list
    objectList.push_back( world[xCoor][yCoor] );

    // Return
    return true;
  }

  // Returns false if object is already present
  return false;
}

/**
* @brief Get object
*
* @details Returns the object from the world at the specified coordinates
*
* @pre Assume initialized world object
*
* @post Object from world returned
*
* @par Algorithm
*      Check to see if the coordinates given are in range and see if there is
*      an object present then return the address of the object
*
* @exception None
*
* @param [in] xCoor
*             X-axis coordinate
*
*        [in] yCoor
*             Y-axis coordinate
*
*        [out] object
*              Object returned from the world
*
* @return boolean specifying if object returned sucessfully
*
* @note Returns false if the coordinates given are out of range and if there is
*       no object at specified coordinates
*/
template <class DataType>
bool World<DataType>::getObject( int xCoor, int yCoor, DataType *&object )
{
  // Check range
  if( xCoor > worldSizeX || yCoor > worldSizeY )
  {
    return false;
  }

  // Return object if present
  if( isObjectPresent( xCoor, yCoor ) )
  {
    // Return object
    object = world[xCoor][yCoor];
    // Return
    return true;
  }

  // Returns false if object not present
  return false;
}

/**
 * @brief Remove Object
 *
 * @details Returns object and removes it from the world
 *
 * @pre Assume initialized class object
 *
 * @post Object returned and removed from world and list of objects present
 *
 * @par Algorithm
 *      Check to see if the coordinates given are in range and see if there is
 *      an object present then return the address of the object and remove it
 *      from the world and list of objects present
 *
 * @exception None
 *
 * @param [in] xCoor
 *             X-axis coordinate
 *
 *        [in] yCoor
 *             Y-axis coordinate
 *
 *        [out] object
 *              Object returned from the world
 *
 * @return Bool indicating success
 *
 * @note Returns false if the coordinates given are out of range and if there is
 *       no object at specified coordinates
 *
 * @note Function does not delete the object, only removes and returns it
 */
template <class DataType>
bool World<DataType>::removeObject( int xCoor, int yCoor, DataType *object )
{
  // Varible Declaration
  int index;

  // Check range
  if( xCoor > worldSizeX || yCoor > worldSizeY )
  {
    return false;
  }

  // Return object if present
  if( isObjectPresent( xCoor, yCoor ) )
  {
    // Return object
    object = world[xCoor][yCoor];

    // Remove object from list
    index = findFromList( object ); /* Index returns -1 if not found */

    if( index <= -1 ) /* This should theoretically never happen but . . . */
    {
      std::cout << "FATAL ERROR: OBJECT REMOVED FROM WORLD BUT ";
      std::cout << "UNSUCCESSFULLY REMOVED FROM LIST . . ." << std::endl;
      return false;
    }
    removeFromList( index );

    // Return
    return true;
  }

  // Returns false if object not present
  return false;
}

/**
 * @brief Find From List
 *
 * @details Finds the matching object in the object address list
 *
 * @pre None
 *
 * @post Vector index at marching object returned
 *
 * @par Algorithm
 *      Go through the vector searching for a matching object address
 *
 * @exception None
 *
 * @param [in] object
 *             Object address to search for
 *
 * @return Vector index at marching object
 *
 * @note Returns -1 if no object found
 */
template <class DataType>
int World<DataType>::findFromList( DataType *object )
{
  // Go through the list until matching object address found
  for( int index = 0; index < objectList.size(); index++ )
  {
    // Compare addresses
    if( objectList[index] == object )
    {
      // Return the vector index at the matching object address
      return index;
    }
  }
  return -1;  // -1 stating that search failed
}

/**
 * @brief Remove from list
 *
 * @details Removes item from the vector
 *
 * @pre None
 *
 * @post Item removed from list
 *
 * @par Algorithm
 *      Shift vector elements forward overwriting the element at index specified
 *
 * @exception None
 *
 * @param [in] index
 *             Vector index to delete
 *
 * @return Boolean stated if deletion is sucessful
 *
 * @note None
 */
template <class DataType>
bool World<DataType>::removeFromList( int index )
{
  // Check if index is in range
  if( index >= objectList.size() )
  {
    return false;
  }

  // Shift elements back
  for( int i = index; index < objectList.size() - 1; index++ )
  {
    objectList[i] = objectList[i + 1];
  }

  // Delete last element
  objectList.pop_back();

  // Return
  return true;
}
/**
 * @brief Remove from list
 *
 * @details Removes item from the vector
 *
 * @pre None
 *
 * @post Item removed from list
 *
 * @par Algorithm
 *      Shift vector elements forward overwriting the element at index specified
 *
 * @exception None
 *
 * @param [in] index
 *             Vector index to delete
 *
 * @return Boolean stated if deletion is sucessful
 *
 * @note None
 */
template <class DataType>
bool World<DataType>::findObject( int id, DataType *object )
{
  int vId;

  for( DataType *cycleobject : objectList )
  {
    vId = cycleobject->getVehicleId();

    if( vId == id )
    {
      object = cycleobject;
      return true;
    }
  }
  return false;
}

/**
 * @brief generates packet
 *
 * @details takes use input to deterimne the message and destination and src for
 * a packet
 *
 * @pre World must be initilized and populated
 *
 * @post src vehicle has new packet
 *
 * @par Algorithm None
 *
 * @exception None
 *
 * @param [in] None
 *
 * @return Boolean stated if packet is added succesfully
 *
 * @note None
 */
template <class DataType>
void World<DataType>::generatePacket()
{
  Packet *newPacket;
  newPacket = new Packet;
  std::string message;
  int         srcId;
  int         destinationId;
  bool        found = false;
  DataType *  destPacketHolder;
  DataType *  srcPacketHolder;

  std::cin.clear();
  std::cin.ignore();
  std::cout << "Enter Packet Message: ";
  std::getline( std::cin, message );
  newPacket->message = message;
  std::cout << "This is the message: " << message << std::endl;

  while( !found )
  {
    std::cout << "Enter Destination Vehicle Id Number ";
    std::cin >> destinationId;

    found = findObject( destinationId, destPacketHolder );
    if( !found )
    {
      std::cout << "\nError! Can not find passed vehicle" << std::endl;
      std::cin.clear();
      std::cin.ignore();
    }
  }
  newPacket->destId = destinationId;
  objectList[destinationId]->getLocation( newPacket->destX, newPacket->destY );

  found = false;

  while( !found )
  {
    std::cout << "Enter Starting Vehicle Id Number ";
    std::cin >> srcId;
    found = findObject( srcId, srcPacketHolder );
    if( !found || destinationId == srcId )
    {
      std::cout << "\nError! Invalid src destination specified" << std::endl;
      std::cin.clear();
      std::cin.ignore();
      found = false;
    }
  }

  newPacket->srcId = srcId;
  objectList[srcId]->getLocation( newPacket->srcX, newPacket->srcY );

  newPacket->packetId = packetids++;

  if( objectList[srcId]->packetCaught( *newPacket ) )
    objectList[srcId]->packets.back()->age = 1;
  else
    std::cout << "World<DataType>::generatePacket()-ERROR: Packet not Generated"
              << std::endl;
}

// Terminating precompiler directives  ////////////////////////////////////////

/**
 * @brief transfers packet in a flood style algorithm
 *
 * @details each vehicle broadcasts its packets to every vehicle in its range
 *
 * @pre None
 *
 * @post Item removed from list
 *
 * @par Algorithm
 *      iterate through each vehicle, iterates through the surrounding
 * intersections
 *      if a vehicle is dicovered teh packet is passed to that vehicle
 *      If the throwing vehicle does not throw the packet then it will try again
 * next tick
 *      A packet may live with a vehicle for 5 ticks before ageing out
 *
 * @exception None
 *
 * @param [in] None
 *
 * @return None
 *
 * @note None
 */
template <class DataType>
void World<DataType>::runFlood()
{

  // Variable Declaration
  DataType *object;

  for( int objIndex = 0; objIndex < objectList.size(); objIndex++ )
  {
    DataType *target;
    int       x;
    int       y;
    // use this to find empty vehicles and reset haspacket
    bool emptyPackets = false;
    bool foundVehicle = true;
    object            = objectList[objIndex];
    if( object->hasPacket() )
    {
      // std::cout << "Alright lets see what we got: " << object->getVehicleId()
      // << std::endl;
      object->getLocation( x, y );
      // search for vehicles in radius
      for( int packetIndex = 0; packetIndex < object->getPacketSize();
           packetIndex++ )
      {
        object->packets[packetIndex]->age++;

        if( object->packets[packetIndex]->age > 1 )
        {
          if( !object->packets[packetIndex]->thrown )
          {
            for( int xOffset = -1; xOffset <= 1; xOffset++ )
            {
              for( int yOffset = -1; yOffset <= 1; yOffset++ )
              {
                if( yOffset != 0 || xOffset != 0 )
                {
                  if( isObjectPresent( x + xOffset, y + yOffset ) )
                  {
                    getObject( x + xOffset, y + yOffset, target );
                    // throw each packet
                    object->throwPacket( objectList[target->getVehicleId()],
                                         *object->packets[packetIndex], false );
                    emptyPackets = true;
                  }
                  else
                    foundVehicle = false;
                }
              }
            }
          }
          else
          {
          }
          // age out packets after 5 cycles
          if( object->packets[packetIndex]->age >= 5 )
            object->packets[packetIndex]->thrown = true;
        }
        else
          emptyPackets = true;
      }
      if( !emptyPackets && foundVehicle ) object->setPacket( false );
    }
  }
  // Move each object in list
  moveVehicles();
}

/**
 * @brief Uses the destination search algorithm to transfer packets
 *
 * @details each vehicle will start its life by broadcasting its starting point
 * and
 *  its current destination, this will be spread by the flood algorithm.
 *  Each vehicle then uses the dest search algotim inside the vehicle class
 *
 * @pre None
 *
 * @post packets are moved
 *
 * @par Algorithm None
 *
 * @exception None
 *
 * @param [in] None
 *
 * @return None
 *
 * @note None
 */

template <class DataType>
void World<DataType>::runDest()
{
  // Variable Declaration
  DataType *object;

  // move each vehicle
  // Move each object in list
  moveVehicles();

  // Update Adjacency Lists
  updateAdjacency();
  // if first time running through have each vehicle send out an update packet
  if( !initializedLocations )
  {
    for( int vectIndex = 0; vectIndex < objectList.size(); vectIndex++ )
    {
      object = objectList[vectIndex];
      object->updateLocation();
    }
    initializedLocations = true;
  }

  // run vehicle functions for each vehicle
  for( int vectIndex = 0; vectIndex < objectList.size(); vectIndex++ )
  {
    object = objectList[vectIndex];
    object->vehicleRun();
  }
}

/**
 * @brief Moves vehicles if possible
 *
 * @details iterates through each vehicle and moves it towards its destination
 *
 * @pre None
 *
 * @post Vehicles move
 *
 * @par Algorithm
 *
 * @exception None
 *
 * @param [in] None
 *
 * @return Boolean stated if deletion is sucessful
 *
 * @note None
 */
template <class DataType>
void World<DataType>::moveVehicles()
{
  // Variable Declaration
  int       xCoor, yCoor;
  int       xNextCoor, yNextCoor;
  DataType *vehicle;
  
  // Move each object in list
  for( int vectIndex = 0; vectIndex < objectList.size(); vectIndex++ )
  {
    // Get object from list
    vehicle = objectList[vectIndex];
    
    // Check if in transit between intersections
    if( !vehicle->inTransition() ) /* Function also moves object closer to next
                                     intersection */
    {
      // Calc next location
      vehicle->getNextLocation( xNextCoor, yNextCoor );
      vehicle->getLocation( xCoor, yCoor );

      // Move object to new location if empty
      if( !isObjectPresent( xNextCoor, yNextCoor ) && !vehicle->hasMoved() )
      {
        world[xCoor][yCoor] = NULL;
        vehicle->move();
        world[xNextCoor][yNextCoor] = vehicle;

        objectActionCounter[vectIndex] = 0;
        vehicle->setHasMoved( true );
      }
      else if( isObjectPresent( xNextCoor, yNextCoor ) )
      {
        int xOtherCoor, yOtherCoor;
        int xOtherNextCoor, yOtherNextCoor;
        
        xOtherCoor = xNextCoor;
        yOtherCoor = yNextCoor;
        
        DataType *otherVehicle = world[xOtherCoor][yOtherCoor];
        otherVehicle->getNextLocation( xOtherNextCoor, yOtherNextCoor );
        
        // Check if they want to swap places
        if( xOtherNextCoor == xCoor && yOtherNextCoor == yCoor)
        {
          if( !otherVehicle->hasMoved() )
          {
            // Swap
            vehicle->move();
            otherVehicle->move();
            world[xNextCoor][yNextCoor]           = vehicle;
            world[xOtherNextCoor][yOtherNextCoor] = otherVehicle;
            
            // Prevent future movement
            otherVehicle->setHasMoved( true );
          }
        }
        else
        {
          ++objectActionCounter[vectIndex];
          
          if( objectActionCounter[vectIndex] == 2 )
          {
            objectList[vectIndex]->redirect();
          }
        }
      }
    }
  }
  
  for( DataType *vehicleTemp : objectList )
  {
    vehicleTemp->setHasMoved( false );
  }
}

/**
 * @brief updates eaqch vehicles adjacancy list
 *
 * @details finds all vehicles nearby and adds them to the list of near by
 * vehicles
 *
 * @pre None
 *
 * @post Each vehicle knows what vehicles are in the immediate area
 *
 * @par Algorithm None
 *
 * @exception None
 *
 * @param [in] None
 *
 * @return None
 *
 * @note None
 */
template <class DataType>
void World<DataType>::updateAdjacency()
{
  DataType *object;
  // update each vehicles adjacency list
  for( int objIndex = 0; objIndex < objectList.size(); objIndex++ )
  {
    DataType *target;
    int       x;
    int       y;
    int       adjacencyIndex = 0;
    object                   = objectList[objIndex];
    object->getLocation( x, y );
    // search for vehicles in radius
    for( int yOffset = 1; yOffset >= -1; yOffset-- )
    {
      for( int xOffset = -1; xOffset <= 1; xOffset++ )
      {
        if( yOffset != 0 || xOffset != 0 )
        {
          if( isObjectPresent( x + xOffset, y + yOffset ) )
          {
            getObject( x + xOffset, y + yOffset, target );
            object->nearByVehicles[adjacencyIndex] = target;
            adjacencyIndex++;
          }

          else
          {
            object->nearByVehicles[adjacencyIndex] = NULL;
            adjacencyIndex++;
          }
        }
      }
    }
  }
}

#endif  // #ifndef CLASS_WORLD_CPP
