// Program Information ////////////////////////////////////////////////////////
/**
 * @file world.h
 *
 * @brief Definition file for world class
 *
 * @author Brandon Thai Nguyen
 *
 * @details Specifies all member methods of the world class
 *
 * @version 1.00
 *          Brandon Thai Nguyen (03 October 2016)
 *          Original Code
 *
 * @note Requires world.h, vehicle.h
 */

// Precompiler directives /////////////////////////////////////////////////////

#ifndef CLASS_WORLD_H
#define CLASS_WORLD_H

// Header files ///////////////////////////////////////////////////////////////

#include <time.h>
#include <cstdlib>
#include <sstream>
#include <vector>

#include "intersect.h"
#include "vehicle.h"

enum TransferType
{
    FLOOD,
    DESTSEARCH
};
// Class definition  //////////////////////////////////////////////////////////

template <class DataType>
class World
{
 public:
  // Constructors
  World();
  ~World();

  // World control
  bool initWorld( int sizeX, int sizeY );
  void displayWorld();
  bool populateWorld( int numObjects );
  void clearWorld();
  void runWorld( int ticks );
  TransferType RUNALGORITH;

  // Accessors
  bool isObjectPresent( int xCoor, int yCoor );
  bool getObject(int xCoor, int yCoor, DataType *&object);
  bool findObject(int id, DataType *object);
  int                      getNumObjects();
  std::vector<DataType *> &getObjectList();

  // Modifiers
  bool insertObject( int xCoor, int yCoor, DataType *object );
  bool removeObject( int xCoor, int yCoor, DataType *object );
  bool deleteObject( int xCoor, int yCoor );

  void generatePacket();

private:
  // Helper Functions
  int findFromList( DataType *object );
  bool removeFromList( int index );
  void runFlood();
  void runDest();
  void moveVehicles();
  void updateAdjacency();

  // Class Variables
  int worldSizeX;
  int worldSizeY;
  int numObjects; /* number of objects present in world */
  int packetids = 0;
  bool initializedLocations = false;

  std::vector<DataType *>   objectList; /* list of objects present in world */
  std::vector<unsigned int> objectActionCounter;
  DataType ***world; /* xCoors ptrs of yCoors ptrs of object ptrs. */
};

// Terminating precompiler directives  ////////////////////////////////////////

#endif  // #ifndef CLASS_WORLD_H
