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

#include "User.h"

Define_Module(User);

/***********************************/

void User::initialize()
{
    pos.x= atoi(getDisplayString().getTagArg("p", 0));
    pos.y= atoi(getDisplayString().getTagArg("p", 1));

    T=par("T");
    t=par("t");
    m=par("m");

    //controllo dei parametri
    if(T==0){
        error("invalid parameter T=0");
    }

    //numGate=gateSize("out");
    nodeState.isActive=true;
    nodeState.firstSuccessfulReception=true;
    nodeState.firstSlot=-1;

    recvMsg.msgList=new cMessage*[T];
    recvMsg.collisions=new int[T];
    recvMsg.receivingTime=new double[T];

    //statistiche
    newNode=registerSignal("newNode");
    activationTime=registerSignal("activationTime");
    newCollision=registerSignal("newCollision");
    numConnections=registerSignal("numConnections");
    deactivatedUser=registerSignal("deactivatedUser");
    setTau=registerSignal("setTau");

    //inizializzazione della struttura
    for(int i=0;i<T;i++){
       recvMsg.msgList[i]=nullptr;
       recvMsg.collisions[i]=0;
       recvMsg.receivingTime[i]=-1;
    }

    //generazione del valore (tau) nececessario al disallineamento degli slot
    tau=intuniform(1,T-1,0);

    //codice eseguito dalla sorgente del messaggio
    if (par("sendInitialMessage").boolValue())
    {
       tau=0;
       nodeState.firstMsg=false;
       nodeState.firstSuccessfulReception=false;
       cMessage* msg=new cMessage("text message");

       //se richiesto viene generata dinamicamente la rete
       if(par("dynamicGeneration").boolValue()){
           startGeneration();
           numGate=gateSize("out");
       }
       else if(par("dynamicRadius").boolValue()){
           radiusGeneration();
           numGate=gateSize("out");
       }

       //invio il messaggio a tutti i vicini (entro il raggio)
       for(int i=0; i<numGate; i++){
           cMessage *msgN = new cMessage(*msg);
           send(msgN, "out",i);
       }

       EV<<"I am "<<getName()<<" and I am the source"<<endl;
       EV<<getName()<<": trigger a "<<(simTime()+(T*t))<<endl;
       scheduleAt(simTime()+(T*t),msg);
    }
    else
       nodeState.firstMsg=true;

    EV<<getName()<<" x: "<<pos.x<<" y: "<<pos.y<<endl;
}

/**************************************/

void User::handleMessage(cMessage *msg)
{
    //i nodi disattivati ignorano i messaggi ricevuti
    if(!nodeState.isActive){
        EV<<getName()<<": sono inattivo"<<endl;
        delete msg;
        return;
    }

    //mi ricavo l'indice dello slot relativo al messaggio appena ricevuto
    int indexSlot=((int)(simTime().dbl()/t))%T;

    if(msg->isSelfMessage())
        selfMessageHandler(msg,indexSlot);
    else
        receivedMessageHandler(msg,indexSlot);
}

/**********************************************/

void User::selfMessageHandler(cMessage* msg, int indexSlot){

    EV<<getName()<<": mi sono svegliato"<<endl;

    //questo blocco viene eseguito allo slot successivo rispetto alla ricezione del primo messaggio
    //se sono state rilevate collisioni
    if(nodeState.firstMsg){

        EV<<getName()<<" eliminazione collisioni al firstSlot "<<endl;

        //vengono eliminate le informazioni riguardanti il messaggio che ha subito collisioni
        indexSlot=(indexSlot-1)<0 ? (T-1) : (indexSlot-1);
        recvMsg.collisions[indexSlot]=0;
        recvMsg.receivingTime[indexSlot]=-1;
        delete recvMsg.msgList[indexSlot]; //è equivalente a fare "delete msg"
        recvMsg.msgList[indexSlot]=nullptr;

        return;
    }

    //se la prima ricezione avviene con successo registriamo i signal per le statistiche
    cMessage* newMsg=new cMessage(*msg); //necessario!!!

    if(nodeState.firstSuccessfulReception){
        nodeState.firstSlot=-1;
        nodeState.firstSuccessfulReception=false;
        numGate=gateSize("out");
        emit(newNode,1);
        emit(activationTime,simTime());
        emit(setTau,tau);
    }
    //dopo la firstSuccessfulReception "msg" non viene deallocato nel "for" finale
    else
        delete msg;

    //applicazione del trickle relaying, se sono state ricevute
    //almeno m copie correttamente si disattiva il nodo,
    //altrimenti si ritrasmette ai vicini
    int count=0;
    for(int i=0;i<T;i++){
        if(recvMsg.msgList[i]!=nullptr && recvMsg.collisions[i]==0)
            count++;
    }
    EV<<getName()<<": numero di copie: "<<count<<endl;

    if(count<m)
        for(int i=0; i<numGate; i++){
            cMessage* msg_n=new cMessage(*newMsg);
            send(msg_n, "out",i);
        }
    else{
        EV<<getName()<<": numero massimo di copie raggiunte"<<endl;
        nodeState.isActive=false;
        emit(deactivatedUser,1);
    }

    //ogni T slot si resetta la memoria relativa ai messaggi ricevuti
    EV<<getName()<<": reset slot"<<endl;

    for(int i=0;i<T;i++){
        if(recvMsg.msgList[i]!=nullptr){
            delete recvMsg.msgList[i]; //alla firstSuccessfulReception elimina anche "msg"
            recvMsg.msgList[i]=nullptr;
            recvMsg.collisions[i]=0;
            recvMsg.receivingTime[i]=-1;
        }
    }

    //si rischedula la funzione ogni T slot
    EV<<getName()<<": trigger a "<<(simTime()+(T*t))<<endl;
    scheduleAt(simTime()+(T*t),newMsg);

}

/**********************************************/

void User::receivedMessageHandler(cMessage* msg, int indexSlot){

    int gateIndex=msg->getArrivalGate()->getIndex();
    EV<<getName()<<": ricevuto pacchetto da "<<gateIndex<<endl;
    EV<<getName()<<": slot indexSlot: "<<indexSlot<<endl;

    //se lo slot attuale e' vuoto vengono salvati il messaggio ricevuto e le sue informazioni
    if(recvMsg.msgList[indexSlot]==nullptr){

        recvMsg.msgList[indexSlot]=new cMessage(*msg);
        recvMsg.receivingTime[indexSlot]=simTime().dbl();
        delete msg;
    }
    //altrimenti si e' verificata una collisione
    else{

        //se il messaggio e' stato ricevuto nello stesso istante di quello memorizzato si ha una collisione
        if(recvMsg.receivingTime[indexSlot]==simTime().dbl()){
            recvMsg.collisions[indexSlot]++;
            emit(newCollision,1);
        }
        //altrimenti si ha una falsa collisione dovuta al disallineamento degli slot,
        //che nei primi T+tau slot causa la ricezione di una copia del primo messaggio
        //esattamente T slot dopo, quindi ottenendo lo stesso indexSlot
        else{
            EV<<getName()<<": rilevato caso falsa collisione nei primi T+tau slot"<<endl;

            //vengono aggiornate le informazioni del messaggio,
            //il vecchio messaggio non puo' essere cancellato perche'
            //non sarebbe raggiungibile dalla "cancelEvent"
            recvMsg.collisions[indexSlot]=0;
            recvMsg.receivingTime[indexSlot]=simTime().dbl();
            delete msg;

            return;
        }

        //gestisce un eventuale prima collisione al primo messaggio ricevuto
        if(!nodeState.firstMsg && indexSlot==nodeState.firstSlot){

            EV<<getName()<<" [t "<<recvMsg.receivingTime[indexSlot]<<"] rilevata collisione al primo messaggio ricevuto"<<endl;

            //la prima ricezione non e' considerata valida, percio' si resetta lo stato del nodo
            nodeState.firstSlot=-1;
            nodeState.firstMsg=true;

            //si cancella la schedulazione del primo selfMessageHandler e al termine dell'attuale slot
            //si resetteranno il messaggio e le sue informazioni
            cancelEvent(recvMsg.msgList[indexSlot]);
            scheduleAt(simTime()+t,recvMsg.msgList[indexSlot]);
            delete msg;

            return;
        }
        //per tutte le collisioni successive il messaggio ricevuto viene scartato
        else if(recvMsg.collisions[indexSlot]>1){
            delete msg;
            return;
        }

        EV<<getName()<<": collisione n "<<recvMsg.collisions[indexSlot]<<" rilevata all'indice "<<indexSlot<<endl;

        delete msg;
    }

    //gestione del primo messaggio ricevuto
    if(nodeState.firstMsg){

        //schedulazione del primo selfMessageHandler.
        scheduleAt(simTime()+((tau+T)*t),recvMsg.msgList[indexSlot]);

        EV<<getName()<<": tau->"<<tau<<endl;
        EV<<getName()<<": trigger a "<<simTime()+((tau+T)*t)<<endl;

        //aggiornamento dello stato del nodo
        nodeState.firstMsg=false;
        nodeState.firstSlot=indexSlot;
    }

    EV<<getName()<<": ricezione conclusa"<<endl;
}

/********************************************/

void User::finish(){

    for(int i=0;i<T;i++){
        if(recvMsg.msgList[i]!=nullptr){
            cancelEvent(recvMsg.msgList[i]);
            delete recvMsg.msgList[i];
            recvMsg.msgList[i]=nullptr;
        }
    }

    delete [] recvMsg.msgList;
}


