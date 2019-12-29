//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2015 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#ifndef DOUBLEQUEUE_H_
#define DOUBLEQUEUE_H_

#include "QueueingDefs.h"

namespace queueing {

class Job;

/**
 * Abstract base class for single-server queues.
 */
class QUEUEING_API DoubleQueue : public cSimpleModule
{
    private:
        simsignal_t droppedSignal;
        simsignal_t queue1LengthSignal;
        simsignal_t queue2LengthSignal;
        simsignal_t zoneLengthSignal;
        simsignal_t queueingTimeSignal;
        simsignal_t busySignal;
        simsignal_t elaborationTimeSignal;
        Job *jobServiced;
        cMessage *endServiceMsg;
        //cMessage *deletedAvatar;
        cQueue queue1;
        cQueue queue2;

        int capacity;
        bool fifo;
        Job *getFromQueue();
        int activeQueue;
        int wait;
        cModule* coordinatore;
        simtime_t time;

        int numberServers;
        int rrCounter=-1;
        int outGateIndex = -1;



    public:
        DoubleQueue();
        virtual ~DoubleQueue();
        int length1();
        int length2();

    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void refreshDisplay() const override;
        virtual void finish() override;

        // hook functions to (re)define behaviour
        virtual void arrival(Job *job);
        virtual simtime_t startService(Job *job);
        virtual void endService(Job *job);
    private:
        void checkQueue(cQueue queue);
        void getAgainFromQueue(cQueue queue);
};

}; //namespace

#endif /* DOUBLEQUEUE_H_ */
