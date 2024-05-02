#include "User.h"

void User::startGeneration(){

    int N = getParentModule()->par("N");
    cModule* sons[N-1];

    //si generano N - 1 user e si inseriscono in un vettore temporaneo
    for(int i=0; i<N-1; i++){
        char name[10];
        sprintf(name, "user%d", i+1);
        sons[i] = createUser(name, i+1);
    }

    //per ogni user vengono calcolati i collegamenti
    for(int i=0; i<N-1; i++)
        findNeighbours(sons, i);
}

/******************************************/

void User::radiusGeneration(){

    int N = getParentModule()->par("N");
    cModule* users[N-1];

    //si ricavo gli altri user e si inseriscono in un vettore temporaneo
    for(int i=0; i<N-1; i++){
        char name[10];
        sprintf(name,"user%d",i);
        users[i]=getParentModule()->getSubmodule(name);
    }

    //per ogni user vengono calcolati i collegamenti
    for(int i=0; i<N-1; i++)
        findNeighbours(users, i);

}

/**********************************/

cModule* User::createUser(const char* name, int idUser){

    cModule* parentmod = getParentModule();
    if(!parentmod)
        EV<<"[ERR] Parent Module non trovato"<<endl;

    //si estrae il tipo del modulo da generare
    cModuleType* nodeType = cModuleType::get("projectgroup5.User");
    if(!nodeType)
        EV<<"Module Type \"projectgroup5.User\" not found"<<endl;

    //creazione del nuovo modulo
    cModule* newNode = nodeType->create(name, parentmod, getParentModule()->par("N"), idUser);

    //inizializzazione dei parametri
    newNode->getParentModule()->par("N").setIntValue(getParentModule()->par("N"));
    newNode->par("radius").setDoubleValue(par("radius"));
    newNode->finalizeParameters();

    //generazione casuale della posizione
    int x=intuniform(1,getParentModule()->par("maxWidth"),1);
    int y=intuniform(1,getParentModule()->par("maxHeight"),1);
    char display[10];
    sprintf(display, "p=%d,%d", x, y);
    newNode->getDisplayString().parse(display);
    dynamic_cast<User*>(newNode)->setX(x);
    dynamic_cast<User*>(newNode)->setY(y);

    //inserimento dell'user nella timeline degli eventi
    newNode->buildInside();
    newNode->scheduleStart(simTime());

    return newNode;
}

/***********************************************/

void User::findNeighbours(cModule** sons, int index){

    int connections=0; //conta il numero di connessioni create fra gli user
    int N = getParentModule()->par("N");
    User* son = dynamic_cast<User*>(sons[index]);

    //generazione collegamenti tra gli user
    for(int j=0; j<N-1; j++){

        //sons[index].out --> sons[index].in++
        User* other = dynamic_cast<User*>(sons[j]);
        if(index!=j && insideRadius(son, other)){
            sons[index]->getOrCreateFirstUnconnectedGate("out", 0, false, true)->connectTo(sons[j]->getOrCreateFirstUnconnectedGate("in", 0, false, true));
            connections++;
        }
    }

    //generazione collegamenti con il padre
    if(insideRadius(son, this)){

        sons[index]->getOrCreateFirstUnconnectedGate("out", 0, false, true)->connectTo(getOrCreateFirstUnconnectedGate("in", 0, false, true));
        getOrCreateFirstUnconnectedGate("out", 0, false, true)->connectTo(sons[index]->getOrCreateFirstUnconnectedGate("in", 0, false, true));
        connections+=2;
    }

    //essendo le connessioni bidirezionali, ne vengono prese solo la metà
    emit(numConnections,(connections/2));

}

/*******************************************/

bool User::insideRadius(User* user1, User* user2){

    double x = user1->getX() - user2->getX();
    double y = user1->getY() - user2->getY();

    // distanza euclidea
    double distance;
    distance = pow(x, 2) + pow(y, 2);
    distance = sqrt(distance);

    double radius = par("radius");
    EV<< user1->getName() << ": X = " << user1->getX() << " | Y = " << user1->getY() << endl;
    EV<< user2->getName() << ": X = " << user2->getX() << " | Y = " << user2->getY() << endl;
    EV<< user1->getName() << ": Radius = " << radius << " | Distance = " << distance << endl;
    return distance <= radius;
}

