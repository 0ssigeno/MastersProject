//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2015 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include "DoubleQueue.h"
#include "Job.h"

namespace queueing {

Define_Module(DoubleQueue);

DoubleQueue::DoubleQueue()
{
    jobServiced = nullptr;
    endServiceMsg = nullptr;

}
DoubleQueue::~DoubleQueue()
{
    delete jobServiced;
    cancelAndDelete(endServiceMsg);
}


void DoubleQueue::initialize()
{
    droppedSignal = registerSignal("dropped");
    queueingTimeSignal = registerSignal("queueingTime");
    queue1LengthSignal = registerSignal("queueLength1");
    queue2LengthSignal = registerSignal("queueLength2");
    zoneLengthSignal = registerSignal("zoneLength");
    elaborationTimeSignal=registerSignal("elaborationTime");
    emit(queue1LengthSignal, 0);
    emit(queue2LengthSignal, 0);
    emit(zoneLengthSignal, 0);

    busySignal = registerSignal("busy");
    emit(busySignal, false);
    endServiceMsg = new cMessage("end-service");
    fifo = par("fifo");
    capacity = par("capacity");
    queue1.setName("queue1");
    queue2.setName("queue2");

    activeQueue=1;
    wait=1;

    coordinatore = this->getParentModule()->getParentModule()->getSubmodule("Coordinatore");
    numberServers= getParentModule()->getParentModule()->par("numberServer"); //deve partire da 0
    numberServers-=1;


}
void DoubleQueue::checkQueue(cQueue queue){
    if(!queue.isEmpty()){
        jobServiced = getFromQueue();
        activeQueue==1 ? emit(queue1LengthSignal, length1()) : emit(queue2LengthSignal, length2())  ;
        simtime_t serviceTime = startService(jobServiced);
        scheduleAt(simTime()+serviceTime, endServiceMsg);
    }

    else{
        jobServiced = nullptr;
        emit(busySignal, false);
        wait=1;
        this->sendDirect(new cMessage("FE"), coordinatore, "receiveFE");
        emit(elaborationTimeSignal,(simTime()-time));
    }
}

void DoubleQueue::getAgainFromQueue(cQueue queue){
    if (queue.isEmpty()) {
       jobServiced = nullptr;
       emit(busySignal, false);
       wait=1;
       this->sendDirect(new cMessage("FE"), coordinatore, "receiveFE");
       emit(elaborationTimeSignal,(simTime()-time));
   }
   else {
       jobServiced = getFromQueue();
       emit(queue1LengthSignal, length1());
       simtime_t serviceTime = startService(jobServiced);
       scheduleAt(simTime()+serviceTime, endServiceMsg);
   }
}

void DoubleQueue::handleMessage(cMessage *msg){

    if(msg->arrivedOn("receiveIE")){
        time=simTime();
        wait=0;
        emit(zoneLengthSignal,length1()+length2());
        activeQueue==1 ? activeQueue=2 : activeQueue=1;
        cancelAndDelete(msg);
        //printf("RecIE");
        if(activeQueue==1){
            checkQueue(queue1);

        }else{
            checkQueue(queue2);

        }

    }else{
       if (msg == endServiceMsg) {
            endService(jobServiced);
            if(wait==0){
               if(activeQueue==1){
                   getAgainFromQueue(queue1);
               }else{
                   getAgainFromQueue(queue2);
               }

            }

        }
        else {
            Job *job = check_and_cast<Job *>(msg);
            arrival(job);
                if (activeQueue==1){
                   queue2.insert(job);
                   emit(queue2LengthSignal, length2());
                   job->setQueueCount(job->getQueueCount() + 1);
               }else{
                   queue1.insert(job);
                   emit(queue1LengthSignal, length1());
                   job->setQueueCount(job->getQueueCount() + 1);
               }
        }
    }

}

void DoubleQueue::refreshDisplay() const
{
    getDisplayString().setTagArg("i2", 0, jobServiced ? "status/execute" : "");
    getDisplayString().setTagArg("i", 1, queue1.isEmpty() ? "" : "cyan");
    getDisplayString().setTagArg("i", 1, queue2.isEmpty() ? "" : "cyan");
}

Job *DoubleQueue::getFromQueue()
{
    Job *job;
    if(activeQueue==1){
        if (fifo) {
                job = (Job *)queue1.pop();
            }
            else {
                job = (Job *)queue1.back();
                // FIXME this may have bad performance as remove uses linear search
                queue1.remove(job);
            }
    }else{
        if (fifo) {
                job = (Job *)queue2.pop();
            }
            else {
                job = (Job *)queue2.back();
                // FIXME this may have bad performance as remove uses linear search
                queue2.remove(job);
            }
    }

    return job;
}

int DoubleQueue::length1()
{
    return queue1.getLength();
}
int DoubleQueue::length2()
{
    return queue2.getLength();
}
void DoubleQueue::arrival(Job *job)
{
    job->setTimestamp();
}

simtime_t DoubleQueue::startService(Job *job)
{
    // gather queueing time statistics
    simtime_t d = simTime() - job->getTimestamp();
    emit(queueingTimeSignal, d);
    job->setTotalQueueingTime(job->getTotalQueueingTime() + d);
    EV << "Starting service of " << job->getName() << endl;
    job->setTimestamp();
    //new
    job->decreaseValue();

    //Get correct OutGateIndex
    if(rrCounter<=numberServers){
        outGateIndex = 0;

    }else{
        outGateIndex= rrCounter - numberServers;

    }
    if(job->getValue()<=0){
        outGateIndex= 2;
    }

    rrCounter = (rrCounter +1 ) % (numberServers +2) ; //nella delete ci va solo in casi speciali, cioè se il suo valore k è uguale a 0

    if(outGateIndex==0){
        return par("serviceTimeAnotherServer").doubleValue();
    }else if(outGateIndex==1){
        return par("serviceTimeSameServer").doubleValue();
    }else if(outGateIndex==2){
        return par("serviceTimeDelete").doubleValue();
    }else{
        printf("We have a problem here\n");
        return 0;
    }

}

void DoubleQueue::endService(Job *job)
{
    EV << "Finishing service of " << job->getName() << endl;
    simtime_t d = simTime() - job->getTimestamp();
    job->setTotalServiceTime(job->getTotalServiceTime() + d);
    //TODO
    send(job, "out", outGateIndex);
}

void DoubleQueue::finish()
{
}

}; //namespace

