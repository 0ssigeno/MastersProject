//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2015 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include "Queue.h"
#include "Job.h"

namespace queueing {

Define_Module(Queue);

Queue::Queue()
{
    jobServiced = nullptr;
    endServiceMsg = nullptr;

}
void Queue::setAvatars(int value){
    avatarInSystem=value;
}
int Queue::getAvatars(){
    return avatarInSystem;
}
Queue::~Queue()
{
    delete jobServiced;
    cancelAndDelete(endServiceMsg);
}

void Queue::sendIE(){
    for(int i=0;i <numberServers;i++){
           cModule* cMod = this->getParentModule()->getSubmodule("newServer",i)->getSubmodule("doubleQueue");
           this->sendDirect(new cMessage("IE"), cMod, "receiveIE");
       }
       //TODO printf("SENDING IE\n");
       answers=0;
}
void Queue::initialize()
{
    droppedSignal = registerSignal("dropped");
    queueingTimeSignal = registerSignal("queueingTime");
    queueLengthSignal = registerSignal("queueLength");
    avatarRefusedSignal= registerSignal("avatarRefused");
    emit(queueLengthSignal, 0);
    busySignal = registerSignal("busy");
    emit(busySignal, false);
    maxAvatar=this->getParentModule()->par("maxAvatar");
    //deletedAvatar = new cMessage("deletedAvatar");
    endServiceMsg = new cMessage("end-service");
    fifo = par("fifo");
    capacity = par("capacity");
    queue.setName("queue");
    //Mine
    numberServers=this->getParentModule()->par("numberServer");
    startAvatar= (maxAvatar/(2*numberServers))*numberServers;
    avatarInSystem=startAvatar;

    first= true;
    avatarRefused=0;
    sendIE();

}
void Queue::handleMessage(cMessage *msg){

    if(msg->arrivedOn("receiveFE")){
        answers++;
        //TODO printf("RECEIVED FE %d\n",answers);
        if(numberServers==answers){
            answers=0;
            sendIE();
        }
        cancelAndDelete(msg);
    }else{

       if (msg == endServiceMsg) {
            endService(jobServiced);
            if (queue.isEmpty()) {
                jobServiced = nullptr;
                emit(busySignal, false);
            }
            else {
                if(avatarInSystem< maxAvatar){
                    avatarInSystem++;
                    printf("ENTER %d\n",avatarInSystem);
                    jobServiced = getFromQueue();
                    emit(queueLengthSignal, length());
                    simtime_t serviceTime = startService(jobServiced);
                    scheduleAt(simTime()+serviceTime, endServiceMsg);

                }else{
                    jobServiced = nullptr;
                    emit(busySignal, false);
                    avatarRefused++;
                    emit(avatarRefusedSignal,avatarRefused);
                    printf("REFUSED %d\n",avatarRefused);

                }

            }
        }
        else {

            Job *job = check_and_cast<Job *>(msg);
            arrival(job);
            if (!jobServiced && avatarInSystem < maxAvatar ) {
                // processor was idle
                jobServiced = job;
                emit(busySignal, true);
                avatarInSystem++;
                printf("ENTER %d\n",avatarInSystem);
                simtime_t serviceTime = startService(jobServiced);
                scheduleAt(simTime()+serviceTime, endServiceMsg);

            }
            else {
                                if(avatarInSystem >= maxAvatar){

                    avatarRefused++;
                    emit(avatarRefusedSignal,avatarRefused);
                    printf("REFUSED %d\n",avatarRefused);

                    EV << "Capacity full! Job dropped.\n";
                    if (hasGUI())
                        bubble("Dropped!");
                    emit(droppedSignal, 1);
                    delete job;
                    return;
                }
               // printf("---insert in coda-----");
                queue.insert(job);
                emit(queueLengthSignal, length());
                job->setQueueCount(job->getQueueCount() + 1);
            }
        }
    }

}

void Queue::refreshDisplay() const
{
    getDisplayString().setTagArg("i2", 0, jobServiced ? "status/execute" : "");
    getDisplayString().setTagArg("i", 1, queue.isEmpty() ? "" : "cyan");
}

Job *Queue::getFromQueue()
{
    Job *job;
    if (fifo) {
        job = (Job *)queue.pop();
    }
    else {
        job = (Job *)queue.back();
        // FIXME this may have bad performance as remove uses linear search
        queue.remove(job);
    }
    return job;
}

int Queue::length()
{
    return queue.getLength();
}

void Queue::arrival(Job *job)
{
    job->setTimestamp();
}

simtime_t Queue::startService(Job *job)
{
    // gather queueing time statistics
    simtime_t d = simTime() - job->getTimestamp();
    emit(queueingTimeSignal, d);
    job->setTotalQueueingTime(job->getTotalQueueingTime() + d);
    EV << "Starting service of " << job->getName() << endl;
    job->setTimestamp();
    return par("serviceTime").doubleValue();
}

void Queue::endService(Job *job)
{
    EV << "Finishing service of " << job->getName() << endl;
    simtime_t d = simTime() - job->getTimestamp();
    job->setTotalServiceTime(job->getTotalServiceTime() + d);
    send(job, "out");
}

void Queue::finish()
{
}

}; //namespace

