// Program Information ////////////////////////////////////////////////////////
/**
 * @file vehicle.cpp
 *
 * @author Brandon Thai Nguyen
 *
 * @brief Implementation file for vehicle classes
 *
 * @details Implements all member methods of the vehicle classes
 *
 * @version
 *          2.0
 *          Tyler Goffinet (24 October 2016)
 *          Implemented functions
 *
 *          1.0
 *          Brandon Thai Nguyen (03 October 2016)
 *          Original Code
 *
 * @note Requires vehicle.h
 */

// Precompiler directives /////////////////////////////////////////////////////

#ifndef CLASS_VEHICLE_CPP
#define CLASS_VEHICLE_CPP

// Header files ///////////////////////////////////////////////////////////////

#include "vehicle.h"

// Class implementation ///////////////////////////////////////////////////////

int Vehicle::vehicleCount = 0;
/**
 * @brief Constructor for abstract Vehicle class
 *
 * @details Sets up class member variables. Destination set to given position.
 *          recommend calling setDestination() from derived class constructors.
 *
 * @pre None
 *
 * @post None
 *
 * @par Algorithm
 *      Sets destination to given position
 *
 * @exception
 *
 * @param [in] x
 *             X position of vehicle
 *
 *        [in] y
 *             Y position of vehicle
 *
 *        [in] rowMax
 *             Number of rows allowed
 *
 *        [in] colNum
 *             Number of columns allowed
 *
 *        [in] hasPkt
 *             Contains packet
 *
 * @note None
 */
Vehicle::Vehicle( int x, int y, int rowMax, int columnMax, bool hasPkt )
    : xPos( x )
    , yPos( y )
    , hasPkt( hasPkt )
    , hasMvd( false )
    , rowMax( rowMax )
    , colMax( columnMax )
{
  xDest = xPos;
  yDest = yPos;

  xNextPos = xPos;
  yNextPos = yPos;

  vehicleDir = NaN;

  redirectCounter = 0;

  vehicleId = vehicleCount;

  vehicleCount++;
  //  std::cout << packets.size() << " packet size 1" << std::endl;
  //  newPacket = new Packet;
  //  packets.push_back(newPacket);
  //  std::cout << packets.size() << " packet size 2" << std::endl;
}

/**
 * @brief Moves vehicle
 *
 * @details Sets current location to next location and recalcuates
 *          next location
 *
 * @pre None
 *
 * @post Current location, next location, and direction are updated
 *
 * @par Algorithm
 *      Set current location to next location
 *      If current location is destination, then change destination
 *      Calculate new next location and direction
 *
 * @exception None
 *
 * @param None
 *
 * @return None
 *
 * @note Warning: Does NOT know about other vehicles
 */
void Vehicle::move()
{
  xPos = xNextPos;
  yPos = yNextPos;

  redirectCounter = 0;

  // If destination reached, calculate new destination and send update to others
  if( xPos == xDest && yPos == yDest )
  {
    calculateDestination();
    updateLocation();
  }

  // DEBUG:
  int        xTemp = xNextPos, yTemp = yNextPos;
  VehicleDir dirTemp = vehicleDir;
  calcNextLocation();
  if( xNextPos < 0 || colMax <= xNextPos || yNextPos < 0 || rowMax <= yNextPos )
  {
    xNextPos = 0;
  }
  xNextPos   = xTemp;
  yNextPos   = yTemp;
  vehicleDir = dirTemp;

  // Recalculate next location
  calcNextLocation();
}

/**
 * @brief Redirects the vehicle
 *
 * @details Attempts to have the car to take an alternative route
 *
 * @pre None
 *
 * @post Vehicle direction and next position is modified
 *
 * @par Algorithm
 *      Stage 1: Find route alternative
 *      Stage 2: Attempt to wiggle your way out
 *      Stage 3: Give up all hope
 *
 * @exception None
 *
 * @param None
 *
 * @return None
 *
 * @note Calling multiple times while not calling move will invoke
 *       different behaviors
 */
void Vehicle::redirect()
{
  ++redirectCounter;

  if( redirectCounter == 1 )
  {
    calcAltDirection();
  }
  // Attempt to 'wiggle' your way out; if more than 3 trys, give up
  else if( redirectCounter == 2 )
  {
    // Invert direction
    switch( vehicleDir )
    {
      case NaN:
        vehicleDir = NaN;
        break;
      case UP:
        planDown();
        break;
      case DOWN:
        planUp();
        break;
      case LEFT:
        planRight();
        break;
      case RIGHT:
        planLeft();
        break;
    }
  }
  else if( redirectCounter == 3 )
  {
    // Try a different direction
    switch( vehicleDir )
    {
      case NaN:
        calcAltDirection();
        break;
      case UP:
        planLeft();
        break;
      case DOWN:
        planRight();
        break;
      case LEFT:
        planUp();
        break;
      case RIGHT:
        planDown();
        break;
    }
  }
  else if( redirectCounter == 4 )
  {
    // Try an even different direction
    switch( vehicleDir )
    {
      case NaN:
        calcAltDirection();
        break;
      case UP:
        planRight();
        break;
      case DOWN:
        planLeft();
        break;
      case LEFT:
        planDown();
        break;
      case RIGHT:
        planUp();
        break;
    }
  }

  if( redirectCounter == 1 )
  {
    // Do nothing
  }
  else if( 1 < redirectCounter && redirectCounter < 5 )
  {
    // Recalculate with new direction
    calcNextLocation();
  }
  else
  {
    // Give up :(
    vehicleDir = NaN;
  }
}

/**
 * @brief Calculates vehicles's next location
 *
 * @details Using current location, next location, and direction,
 *          recalculates the location the car should be next
 *
 * @pre None
 *
 * @post None
 *
 * @par Algorithm
 *      Based on the direction, move towards destination along x axis first
 *      If wanted direction is behind, then make alternate turn
 *      If direction set to NaN, then any turn is possible
 *
 * @exception None
 *
 * @param None
 *
 * @return None
 *
 * @note Does not allow immediate U-turns. If no other options are
 *       available two move() calls are required to complete a U-turn.
 *       If direction set to NaN, then any turn is possible
 */
void Vehicle::calcNextLocation()
{
  xNextPos = xPos;
  yNextPos = yPos;

  // Get front facing locations
  switch( vehicleDir )
  {
    case NaN:
      if( xPos != xDest )
      {
        ( xDest < xPos ) ? planLeft() : planRight();
      }
      else if( yPos != yDest )
      {
        ( yDest < yPos ) ? planUp() : planDown();
      }
      else /* xPos == xDest && yPos == yDest */
      {
        vehicleDir = NaN;
      }
      break;

    case UP:
      if( xPos != xDest )
      {
        ( xDest < xPos ) ? planLeft() : planRight();
      }
      else if( yPos != yDest )
      {
        ( yDest < yPos ) ? planUp() : calcAltDirection();
      }
      else /* xPos == xDest && yPos == yDest */
      {
        vehicleDir = NaN;
      }
      break;

    case DOWN:
      if( xPos != xDest )
      {
        ( xDest < xPos ) ? planLeft() : planRight();
      }
      else if( yPos != yDest )
      {
        ( yDest < yPos ) ? calcAltDirection() : planDown();
      }
      else /* xPos == xDest && yPos == yDest */
      {
        vehicleDir = NaN;
      }
      break;

    case LEFT:
      if( xPos != xDest )
      {
        ( xDest < xPos ) ? planLeft() : calcAltDirection();
      }
      else if( yPos != yDest )
      {
        ( yDest < yPos ) ? planUp() : planDown();
      }
      else /* xPos == xDest && yPos == yDest */
      {
        vehicleDir = NaN;
      }
      break;

    case RIGHT:
      if( xPos != xDest )
      {
        ( xDest < xPos ) ? calcAltDirection() : planRight();
      }
      else if( yPos != yDest )
      {
        ( yDest < yPos ) ? planUp() : planDown();
      }
      else /* xPos == xDest && yPos == yDest */
      {
        vehicleDir = NaN;
      }
      break;
  }
}

/**
 * @brief Calculates an alterative direction
 *
 * @details Determines an optimal alterative way to the destination ignoring the
 *          possiblity of going fowards or backward
 *
 * @pre None
 *
 * @post Vehicle direction not forward or backward; next position is modified
 *
 * @par Algorithm
 *      Find optimal 90 degree alternate direction
 *      If no other direction possible, then direction set to NaN
 *
 * @exception None
 *
 * @param None
 *
 * @return Whether vehicle is boxed in on its sides
 *
 * @note If vehicle is boxed in on both sides left/right or up/down, then
 *       stops vehicle
 */
bool Vehicle::calcAltDirection()
{
  bool canMove = false;

  // Reset next position
  xNextPos = xPos;
  yNextPos = yPos;

  // Want to go alternate direction, but can't make U-turn
  switch( vehicleDir )
  {
    case NaN:
      vehicleDir = NaN;
      break;

    case UP:
    case DOWN:
      canMove = ( xDest < xPos ) ? planLeft() : planRight();

      if( !canMove ) /* can't move; Go in other direction */
      {
        canMove = ( xDest < xPos ) ? planRight() : planLeft();

        if( !canMove ) stop(); /* boxed in */
      }
      break;

    case LEFT:
    case RIGHT:
      canMove = ( yDest < yPos ) ? planUp() : planDown();

      if( !canMove ) /* can't move; Go in other direction */
      {
        canMove = ( yDest < yPos ) ? planDown() : planUp();

        if( !canMove ) stop(); /* boxed in */
      }
      break;
  }

  return canMove;
}

/**
 * @brief Stop vehicle
 *
 * @details Sets next position equal to current position and sets direction
 *          to NaN
 *
 * @pre None
 *
 * @post Next location = current position; direction = NaN;
 *
 * @par Algorithm
 *      Set vehicle direction = NaN
 *      Set next position equal = current position
 *
 * @exception None
 *
 * @param None
 *
 * @return None
 *
 * @note None
 */
void Vehicle::stop()
{
  vehicleDir = NaN;
  xNextPos   = xPos;
  yNextPos   = yPos;
}

/**
 * @brief Sets next location and direction to go up
 *
 * @details Increments X next position and sets direction up if
 *          within boundaries
 *
 * @pre None
 *
 * @post Next location and direction are modified
 *
 * @par Algorithm
 *      Checks if next position would be out of bounds
 *      Alters vehicle direction and next position
 *
 * @exception None
 *
 * @param None
 *
 * @return Returns true if turn is not out of bounds; false otherwise
 *
 * @note None
 */ bool
Vehicle::planUp()
{
  if( yNextPos - 1 >= 0 )
  {
    vehicleDir = UP;
    --yNextPos;

    return true;
  }

  return false;
}

/**
 * @brief Sets next location and direction to go down
 *
 * @details Increments X next position and sets direction down if
 *          within boundaries
 *
 * @pre None
 *
 * @post Next location and direction are modified
 *
 * @par Algorithm
 *      Checks if next position would be out of bounds
 *      Alters vehicle direction and next position
 *
 * @exception None
 *
 * @param None
 *
 * @return Returns true if turn is not out of bounds; false otherwise
 *
 * @note None
 */
bool Vehicle::planDown()
{
  if( yNextPos + 1 < rowMax )
  {
    vehicleDir = DOWN;
    ++yNextPos;

    return true;
  }

  return false;
}

/**
 * @brief Sets next location and direction to go left
 *
 * @details Decrements X next position and sets direction left if
 *          within boundaries
 *
 * @pre None
 *
 * @post Next location and direction are modified
 *
 * @par Algorithm
 *      Checks if next position would be out of bounds
 *      Alters vehicle direction and next position
 *
 * @exception None
 *
 * @param None
 *
 * @return Returns true if turn is not out of bounds; false otherwise
 *
 * @note None
 */
bool Vehicle::planLeft()
{
  if( xNextPos - 1 >= 0 )
  {
    vehicleDir = LEFT;
    --xNextPos;

    return true;
  }

  return false;
}

/**
 * @brief Sets next location and direction to go right
 *
 * @details Increments X next position and sets direction right if
 *          within boundaries
 *
 * @pre None
 *
 * @post Next location and direction are modified
 *
 * @par Algorithm
 *      Checks if next position would be out of bounds
 *      Alters vehicle direction and next position
 *
 * @exception None
 *
 * @param None
 *
 * @return Returns true if turn is not out of bounds; false otherwise
 *
 * @note None
 */
bool Vehicle::planRight()
{
  if( xNextPos + 1 < colMax )
  {
    vehicleDir = RIGHT;
    ++xNextPos;

    return true;
  }

  return false;
}

/**
 * @brief Throws packet to the passed vehicle and ouptputs a successful transfer
 * message
 *
 * @details Helper function
 *
 * @pre None
 *
 * @post Packet is added to target vehicle
 *
 * @par None
 *
 * @exception None
 *
 * @param None
 *
 * @return Returns true if turn is not out of bounds; false otherwise
 *
 * @note None
 */
bool Vehicle::throwPacket( Vehicle *targetVehicle, Packet thrownPacket,
                           bool update )
{
  if( !update && targetVehicle->packetCaught( thrownPacket ) )
  {
    return true;
  }
  else if( targetVehicle->updatePacketCaught( thrownPacket ) )
  {
    return true;
  }

  return false;
}

/**
 * @brief checks to see if passed paket has reacched its destination or if it
 * can be taken in
 *
 *
 * @details Looks at packet destination , if not current packet checks to see if
 * vehicle id is in packets id list
 * If Id list is empty, knows to create initial packet information
 * If packet is to be added copies function members and adds to vehicles packet
 * list
 *
 * @pre throwPacket()
 *
 * @post Packet is added to packet list
 *
 * @par None
 *
 * @exception None
 *
 * @param None
 *
 * @return Returns true if packet was added and false otherwise
 *
 * @note None
 */
bool Vehicle::packetCaught( Packet thrownPacket )
{
  bool inList         = false;
  bool insertingFirst = false;
  bool hitDest        = false;

  if( thrownPacket.srcId == vehicleId )
  {
    if( thrownPacket.ids.empty() )
    {

      insertingFirst = true;
      setPacket( true );
      thrownPacket.ids.reserve( 1 );
    }
    else
      return false;
  }

  else
  {

    for( int index = 0; index < packets.size(); index++ )
    {
      if( packets[index]->packetId == thrownPacket.packetId )
      {
        return false;
      }
    }

    // search for packet in id list to see if vehicle has thrown packet before
    for( int i = 0; i < thrownPacket.ids.size(); i++ )
    {
      if( thrownPacket.ids[i] == vehicleId )
      {
        inList = true;

        break;
      }
    }
  }

  // check to see if packet is at destination
  if( thrownPacket.destId == vehicleId )
  {
    std::cout << "PACKET HAS REACHED DESTINATION ... OUTPUT MESSSAGE: "
              << thrownPacket.message << std::endl;
    hitDest = true;
  }

  // add packet if not already in list
  if( !inList || insertingFirst )
  {
    newPacket          = new Packet;
    newPacket->destId  = thrownPacket.destId;
    newPacket->destX   = thrownPacket.destX;
    newPacket->destY   = thrownPacket.destY;
    newPacket->srcId   = thrownPacket.srcId;
    newPacket->srcX    = thrownPacket.srcX;
    newPacket->srcY    = thrownPacket.srcY;
    newPacket->message = thrownPacket.message;
    newPacket->ids     = thrownPacket.ids;
    newPacket->ids.push_back( vehicleId );
    newPacket->packetId = thrownPacket.packetId;
    newPacket->thrown   = false;
    newPacket->age      = 0;
    newPacket->atDest   = hitDest;
    packets.push_back( newPacket );
    setPacket( true );
    return true;
  }

  return false;
}

/**
 * @brief throws packets if they exist
 *
 * @details throws regular packets using bestDest and throws updates to all
 * nearby vehicles
 *
 * @pre called from World Run
 *
 * @post Packets are thrown
 *
 * @par None
 *
 * @exception None
 *
 * @param None
 *
 * @return Returns false only right now
 *
 * @note None
 */
bool Vehicle::vehicleRun()
{

  bool emptyPackets = false;
  bool foundVehicle = true;

  // check for packets
  if( hasPkt )
  {
    bestDestinationAlgorithm();
  }

  if( hasUpdate )
  {
    for( int j = 0; j < updates.size(); j++ )
    {
      for( int i = 0; i < 8; i++ )
        if( nearByVehicles[i] != NULL )
        {
          throwPacket( nearByVehicles[i], *updates[j], true );
          emptyPackets = true;
        }
        else
        {
          foundVehicle = false;
        }
    }
  }

  if( !emptyPackets && foundVehicle ) hasUpdate = false;
  return false;
}

/**
 * @brief searches for best vehicle to throw to
 *
 *
 * @details gives each vehicle in proximity of current vehicle a score based on
 * how
 * well the vehicle will get the packet to the appropriate location. This score
 * is based
 * on the destination of the target vehicle, the direction the vehicles are
 * moving and
 * where the vehicle is going(destination)
 *
 * @pre called from vehicle Run function
 *
 * @post Packet is thrown to ideal vehicle and packet is removed from vehicle
 *
 * @par None
 *
 * @exception None
 *
 * @param None
 *
 * @return Returns true if packet is thrown and false if packet is kept
 *
 * @note None
 */
bool Vehicle::bestDestinationAlgorithm()
{
  // set best vehicle to self
  Vehicle *bestChoice = this;
  int      idealScore = 1;
  int      vehicleScore;
  bool     allPacketsThrown = true;
  bool     triedToThrow     = false;

  for( int packetIndex = 0; packetIndex < packets.size(); packetIndex++ )
  {
    packets[packetIndex]->age++;
    if( packets[packetIndex]->age > 1 )
    {
      triedToThrow = true;
      // for each packet determine best place to throw next
      int idealX;
      int idealY;
      int destX = 0;
      int destY = 0;
      // search through list of locations to see if destination vehicle is known
      // if dest vehicle is known use its current destination to determine
      // ideal throwing location
      // if location is not known aim for center of screen

      for( int i = 0; i < locations.size(); i++ )
      {
        if( locations[i].vehicleID == packets[packetIndex]->destId )
        {
          destX = packets[packetIndex]->destX;
          destY = packets[packetIndex]->destY;
          break;
        }
      }

      if( destX > xPos )
      {
        idealX = 1;
      }
      else if( destX < xPos )
      {
        idealX = -1;
      }
      else
      {
        idealX = 0;
      }

      if( destY > xPos )
      {
        idealY = 1;
      }
      else if( destY < xPos )
      {
        idealY = -1;
      }
      else
      {
        idealY = 0;
      }
      if( idealX == 1 && vehicleDir == RIGHT )
        idealScore++;
      else if( idealX == -1 && vehicleDir == LEFT )
        idealScore++;
      else if( idealY == 1 && vehicleDir == UP )
        idealScore++;
      else if( idealY == -1 && vehicleDir == DOWN )
        idealScore++;

      for( int adjacencyIndex = 0; adjacencyIndex < 8; adjacencyIndex++ )
      {
        vehicleScore = 0;
        if( nearByVehicles[adjacencyIndex] != NULL )
        {

          // throw to packet if destination vehicle is in the right spot
          if( nearByVehicles[adjacencyIndex]->vehicleId ==
              packets[packetIndex]->destId )
          {
            if( throwPacket( nearByVehicles[adjacencyIndex],
                             *packets[packetIndex] ) )
              packets[packetIndex]->thrown = true;
          }
          // check to see if vehicle is at ideal location
          if( nearByVehicles[adjacencyIndex]->xPos == xPos + idealX &&
              nearByVehicles[adjacencyIndex]->yPos == yPos + idealY )
          {
            vehicleScore = vehicleScore + 3;
          }

          // increase score if vehicle is going in the right direction
          if( idealX == 1 &&
              nearByVehicles[adjacencyIndex]->vehicleDir == RIGHT )
            vehicleScore++;
          else if( idealX == -1 &&
                   nearByVehicles[adjacencyIndex]->vehicleDir == LEFT )
            vehicleScore++;
          else if( idealY == 1 &&
                   nearByVehicles[adjacencyIndex]->vehicleDir == UP )
            vehicleScore++;
          else if( idealY == -1 &&
                   nearByVehicles[adjacencyIndex]->vehicleDir == DOWN )
            vehicleScore++;

          // increase score if vehicle destination is in the right direction
          if( nearByVehicles[adjacencyIndex]->xDest > xPos && idealX == 1 )
          {
            vehicleScore++;
          }
          else if( nearByVehicles[adjacencyIndex]->xDest < xPos &&
                   idealX == -1 )
          {
            vehicleScore++;
          }

          if( nearByVehicles[adjacencyIndex]->yDest < yPos && idealY == -1 )
          {
            vehicleScore++;
          }
          else if( nearByVehicles[adjacencyIndex]->yDest > yPos && idealY == 1 )
          {
            vehicleScore++;
          }
        }
        // if found vehicle idealness is > currentbest make new vehicle best
        if( vehicleScore > idealScore )
        {
          bestChoice = nearByVehicles[adjacencyIndex];
          idealScore = vehicleScore;
        }
      }

      // throw packet to best vehicle
      if( bestChoice != this )
      {
        if( throwPacket( bestChoice, *packets[packetIndex] ) )
        {
          packets[packetIndex]->thrown = true;
        }
        else
          allPacketsThrown = false;
      }
      else
      {
        allPacketsThrown = false;
      }
    }
    if( packets[packetIndex]->age >= 5 )
    {
      packets[packetIndex]->thrown = true;
    }
  }
  if( allPacketsThrown && triedToThrow )
  {
    hasPkt = false;
  }
  return false;
}

/**
 * @brief checks to see if passed paket has reacched its destination or if it
 * can be taken in
 *
 *
 * @details Looks at packet destination , if not current packet checks to see if
 * vehicle id is in packets id list
 * If Id list is empty, knows to create initial packet information
 * If packet is to be added copies function members and adds to vehicles packet
 * list
 *
 * @pre throwPacket()
 *
 * @post Packet is added to packet list
 *
 * @par None
 *
 * @exception None
 *
 * @param None
 *
 * @return Returns true if packet was added and false otherwise
 *
 * @note None
 */
bool Vehicle::updatePacketCaught( Packet thrownPacket )
{
  bool            inList         = false;
  bool            insertingFirst = false;
  bool            hitDest        = false;
  vehicleLocation updatedLocation;

  if( thrownPacket.srcId == vehicleId )
  {
    if( thrownPacket.ids.empty() )
    {
      insertingFirst = true;
      setPacket( true );
      thrownPacket.ids.reserve( 1 );
    }
    else
      return false;
  }
  else
  {

    for( int index = 0; index < updates.size(); index++ )
    {
      if( updates[index]->packetId == thrownPacket.packetId )
      {
        return false;
      }
    }

    // search for packet in id list to see if vehicle has thrown packet before
    for( int i = 0; i < thrownPacket.ids.size(); i++ )
    {
      if( thrownPacket.ids[i] == vehicleId )
      {
        inList = true;
        return false;
      }
    }
  }

  // add packet if not already in list
  if( !inList || insertingFirst )
  {
    //    int debug = 0;
    newPacket          = new Packet;
    newPacket->destId  = thrownPacket.destId;
    newPacket->destX   = thrownPacket.destX;
    newPacket->destY   = thrownPacket.destY;
    newPacket->srcId   = thrownPacket.srcId;
    newPacket->srcX    = thrownPacket.srcX;
    newPacket->srcY    = thrownPacket.srcY;
    newPacket->message = thrownPacket.message;
    newPacket->ids     = thrownPacket.ids;
    newPacket->ids.push_back( vehicleId );
    newPacket->packetId = thrownPacket.packetId;
    newPacket->thrown   = false;
    newPacket->age      = 0;
    newPacket->atDest   = hitDest;
    updates.push_back( newPacket );
    hasUpdate = true;

    inList = false;
    // check to see if vehicle is already in list, if not add new vehicle
    for( int i = 0; i < locations.size(); i++ )
    {
      if( locations[i].vehicleID == thrownPacket.srcId )
      {
        locations[i].destX  = thrownPacket.destX;
        locations[i].destY  = thrownPacket.destY;
        locations[i].srcX   = thrownPacket.srcX;
        locations[i].srcY   = thrownPacket.srcY;
        locations[i].thrown = false;

        inList = true;
      }
    }

    if( !inList )
    {  // if not in list already add the new vehicle
      updatedLocation.vehicleID = thrownPacket.srcId;
      updatedLocation.destX     = thrownPacket.destX;
      updatedLocation.destY     = thrownPacket.destY;
      updatedLocation.srcX      = thrownPacket.srcX;
      updatedLocation.srcY      = thrownPacket.srcY;
      updatedLocation.thrown    = false;

      locations.push_back( updatedLocation );
    }

    return true;
  }

  return false;
}
/**
 * @brief gets vehicle id
 *
 * @details Returns id
 *
 * @pre None
 *
 * @post None
 *
 * @par None
 *
 * @exception None
 *
 * @param None
 *
 * @return vehicle id (int)
 *
 * @note None
 */
int Vehicle::getVehicleId() const
{
  return vehicleId;
}

void Vehicle::updateLocation()
{
  Packet *newDestination;
  newDestination        = new Packet;
  newDestination->destX = xDest;
  newDestination->destY = yDest;
  newDestination->age   = 0;
  newDestination->srcX  = xPos;
  newDestination->srcY  = yPos;
  newDestination->srcId = vehicleId;
  newDestination->ids.push_back( vehicleId );
  newDestination->thrown = false;

  hasUpdate = true;

  updates.push_back( newDestination );
}

/**
 * @brief Constructor for Taxi class derived from abstrat Vehicle class
 *
 * @details Calculates destination and next location
 *
 * @pre None
 *
 * @post None
 *
 * @par Algorithm
 *      Calculates destination
 *      Calculates next location
 *
 * @exception None
 *
 * @param [in] x
 *             X position of vehicle
 *
 *        [in] y
 *             Y position of vehicle
 *
 *        [in] rowMax
 *             Number of rows allowed
 *
 *        [in] colNum
 *             Number of columns allowed
 *
 *        [in] hasPkt
 *             Contains packet
 *
 * @note None
 */
Taxi::Taxi( int x, int y, int rowMax, int colNum, bool hasPkt )
    : Vehicle( x, y, rowMax, colNum, hasPkt )
{
  ticksToMove = 0;
  tickCounter = 0;
  calculateDestination();
  calcNextLocation();
}

/**
 * @brief In Transisition
 *
 * @details Checks if the vehicle is in transition between interesections and
 *          pseudo-moves it a tick closer to the next intersection
 *
 * @pre None
 *
 * @post Returns a bool stating if the vehicle is in transition and pseudo-moves
 *       it a tick closer to the next intersection
 *
 * @par Algorithm
 *      Check to see if tick counter is less than ticks to move, and if so
 *      increments the tick counter and returns true, otherwise, reset the tick
 *      counter and return false
 *
 * @exception None
 *
 * @param None
 *
 * @note None
 */
bool Taxi::inTransition()
{
  // Check tick counter, increment if less than ticksToMove
  if( tickCounter < ticksToMove )
  {
    tickCounter++;
    return true;
  }
  // Reset counter
  tickCounter = 0;
  return false;
}

/**
 * @brief Calculates Taxi's destination
 *
 * @details Randomly assigns destination within given row and column
 *          boundaries using rand() generator
 *
 * @pre number of columns > 0
 *      number of rows > 0
 *
 * @post New destination != old location
 *
 * @par Algorithm
 *      Generate random xy coordinates until coordinates != old destination
 *
 * @exception None
 *
 * @param None
 *
 * @return None
 *
 * @note Recommend setting current location coordinates = old destination
 *       before call
 */
void Taxi::calculateDestination()
{
  int xNewDest, yNewDest;

  do
  {
    xNewDest = rand() % ( colMax - 1 );
    yNewDest = rand() % ( rowMax - 1 );
  } while( xNewDest == xDest && yNewDest == yDest );

  xDest = xNewDest;
  yDest = yNewDest;
}

// Terminating precompiler directives  ////////////////////////////////////////

#endif  // #ifndef CLASS_VEHICLE_CPP
