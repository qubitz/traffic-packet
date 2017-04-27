// Program Information ////////////////////////////////////////////////////////
/**
 * @file vehicle.h
 *
 * @brief Definition file for vehicle classes
 *
 * @author Brandon Thai Nguyen
 *
 * @details Specifies all member methods of the vehicle classes
 *
 * @version 2.00
 *          Tyler Michael Goffinet (24 October 2016)
 *          Added getId(), getLocation(), getDirection(), hasPacket(),
 *          getNextLocation(), move(), setPacket(), protected functions, and
 *          member variables
 *
 *          1.00
 *          Brandon Thai Nguyen (03 October 2016)
 *          Original Code
 *
 * @note None
 */

// Precompiler directives /////////////////////////////////////////////////////

#ifndef CLASS_VEHICLE_H
#define CLASS_VEHICLE_H

// Header files ///////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
//#include "world.h"

// Class definition  //////////////////////////////////////////////////////////

enum VehicleDir
{
  NaN,
  UP,
  DOWN,
  LEFT,
  RIGHT
};


struct Packet
{
    int packetId;
    int srcId;
    int destX = 0;
    int destY = 0;
    int destId;
    int srcX;
    int srcY;
    int age;
    std::string message;
    std::vector<int> ids;
    bool thrown = false;
    bool atDest = false;
};

struct vehicleLocation{
    int vehicleID;
    int destX;
    int destY;
    int srcX;
    int srcY;
    bool thrown;
};

class Vehicle
{
 public:
  // Constructors
  Vehicle( int x, int y, int rowNum, int colNum, bool hasPkt = false );
  ~Vehicle() {}

  // Accessors
  virtual char getId() { return 'V'; }
  void getLocation( int& x, int& y )
  {
    x = xPos;
    y = yPos;
  }
  void getNextLocation( int& x, int& y )
  {
    x = xNextPos;
    y = yNextPos;
  }
  void getDestination( int& x, int& y )
  {
    x = xDest;
    y = yDest;
  }

  int getPacketSize()
  {
    return packets.size();
  }
  VehicleDir getDirection() { return vehicleDir; }
  bool       hasPacket() { return hasPkt; }
  bool       hasMoved() { return hasMvd; }

  // Modifiers
  void move();
  void redirect();
  void setPacket( bool holdsPacket ) { hasPkt = holdsPacket; }
  void setHasMoved( bool hasMoved ) { hasMvd = hasMoved; }

  std::vector<Packet *> packets;
  std::vector<Packet *> updates;

  bool throwPacket(Vehicle *targetVehicle, Packet thrownPacket, bool update = false);
  bool packetCaught(Packet thrownPacket);
  bool updatePacketCaught(Packet thrownPacket);

  bool vehicleRun();
  bool bestDestinationAlgorithm();
  void updateLocation();

 protected:
  // Member variables
  int  xPos, yPos;
  int  xDest, yDest;
  int  xNextPos, yNextPos;
  bool hasPkt;
  bool hasMvd;
  bool hasUpdate;
  int vehicleId;
  Packet * newPacket;
  unsigned int redirectCounter;
  VehicleDir   vehicleDir;
  std::vector<vehicleLocation> locations;

  int rowMax, colMax;

  // Member functions
  void calcNextLocation();
  bool calcAltDirection();

  void stop();
  bool planUp();
  bool planDown();
  bool planLeft();
  bool planRight();
  static int vehicleCount;
public:
    int getVehicleId() const;

    //vehicle adjacency list stored as follows
    /*
     *      _5_|_6_|_7_
     *      _3_|_X_|_4_
     *      _0_|_1_|_2_
     *
     *      (X-1, Y+1), (X, Y+1), (X+1, Y+1), (X-1, Y), (X+1, Y), (X-1, Y-1), (X, Y-1), (X+1, Y-1)
     *          0           1           2         3         4         5           6         7
     */
    Vehicle    * nearByVehicles[8];

protected:
    // Pure virtual fuctions
  virtual void calculateDestination() = 0;

};

class Taxi : public Vehicle
{
 public:
  // Constructors
  Taxi( int x, int y, int rowMax, int colMax, bool hasPkt = false );

  // Accessors
  char getId() { return 'T'; }
  bool inTransition();

  // Modifiers
  void calculateDestination();
 private:
  
  // Varibles
  int ticksToMove;
  int tickCounter;
};

// Terminating precompiler directives  ////////////////////////////////////////

#endif  // #ifndef CLASS_VEHICLE_H
