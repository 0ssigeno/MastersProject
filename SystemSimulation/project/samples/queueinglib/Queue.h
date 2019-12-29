//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2015 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#ifndef __QUEUEING_QUEUE_H
#define __QUEUEING_QUEUE_H

#include "QueueingDefs.h"

namespace queueing {

class Job;

/**
 * Abstract base class for single-server queues.
 */
class QUEUEING_API Queue : public cSimpleModule
{
    private:
		simsignal_t droppedSignal;
		simsignal_t queueLengthSignal;
		simsignal_t queueingTimeSignal;
		simsignal_t avatarRefusedSignal;
		simsignal_t busySignal;
		Job *jobServiced;
        cMessage *endServiceMsg;
        //cMessage *deletedAvatar;
        cQueue queue;
        int capacity;
        bool fifo;
        //mine
        int avatarInSystem;
        int maxAvatar;
        Job *getFromQueue();
        int answers;
        int numberServers;
        bool first;
        int startAvatar;
        int avatarRefused;

    public:
        Queue();
        virtual ~Queue();
        int length();
        int getAvatars();
        void setAvatars(int value);


    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void refreshDisplay() const override;
        virtual void finish() override;
        virtual void sendIE();

        // hook functions to (re)define behaviour
        virtual void arrival(Job *job);
        virtual simtime_t startService(Job *job);
        virtual void endService(Job *job);

};

}; //namespace

#endif
