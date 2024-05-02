//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef __PROJECTGROUP5_USER_H_
#define __PROJECTGROUP5_USER_H_

#include <omnetpp.h>

using namespace omnetpp;

/**
 * TODO - Generated class
 */
class User : public cSimpleModule
{
  private:
    //Memoria contenente le informazioni sui messaggi ricevuti negli ultimi T slot,
    //viene resettata ogni volta che il nodo è pronto a ritrasmettere
    struct receivedMsg{
      cMessage** msgList;
      int* collisions;
      double* receivingTime;
    } recvMsg;

    struct position{
      int x;
      int y;
    } pos;
    int getX(){ return pos.x; }
    int getY(){ return pos.y; }
    void setX(int newX){ pos.x=newX; }
    void setY(int newY){ pos.y=newY; }

    struct state{
      bool firstMsg;
      int firstSlot;
      bool firstSuccessfulReception;
      bool isActive;
    } nodeState;

    //parameters
    int tau;
    int T;
    double t;
    int m;
    int numGate;
    int* pregeneratedTau;

    //statistiche
    simsignal_t newNode;
    simsignal_t activationTime;
    simsignal_t newCollision;
    simsignal_t numConnections;
    simsignal_t deactivatedUser;
    simsignal_t setTau;

    void selfMessageHandler(cMessage* msg, int indexSlot);
    void receivedMessageHandler(cMessage* msg, int indexSlot);

    //generazione users
    void startGeneration();
    cModule* createUser(const char* name, int idUser);

    //generazione raggi
    void radiusGeneration();

    //utility di generazione dinamica
    void findNeighbours(cModule** sons, int index);
    bool insideRadius(User* user1, User* user2);

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();

};

#endif
