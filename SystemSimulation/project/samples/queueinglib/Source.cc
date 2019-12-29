//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2015 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include "Source.h"
#include "Job.h"

namespace queueing {

void SourceBase::initialize()
{
    createdSignal = registerSignal("created");
    jobCounter = 0;
    WATCH(jobCounter);
    jobName = par("jobName").stringValue();
    if (jobName == "")
        jobName = getName();
}

Job *SourceBase::createJob(int time)
{
    char buf[80];
    sprintf(buf, "%.60s-%d", jobName.c_str(), ++jobCounter);
    Job *job = new Job(buf,time,true);
    job->setKind(par("jobType"));
    job->setPriority(par("jobPriority"));
    return job;
}

void SourceBase::finish()
{
    emit(createdSignal, jobCounter);
}

//----

Define_Module(Source);

void Source::initialize()
{
    SourceBase::initialize();
    startTime = par("startTime");
    stopTime = par("stopTime");
    numJobs = par("numJobs");
    timeAlive= par("timeAliveJob");
    // schedule the first message timer for start time
    scheduleAt(startTime, new cMessage("newJobTimer"));

    //printf("---TIMEALIVE: %d \n",timeAlive);
}

void Source::handleMessage(cMessage *msg)
{
    ASSERT(msg->isSelfMessage());
    if ((numJobs < 0 || numJobs > jobCounter) && (stopTime < 0 || stopTime > simTime())) {
        // reschedule the timer for the next message
        scheduleAt(simTime() + par("interArrivalTime").doubleValue(), msg);

        Job *job = createJob(timeAlive);
        send(job, "out");
    }
    else {
        // finished
        delete msg;
    }
}

//----

Define_Module(SourceOnce);

void SourceOnce::initialize()
{
    SourceBase::initialize();
    simtime_t time = par("time");
    scheduleAt(time, new cMessage("newJobTimer"));
    //strangeValue= (maxAvatar/(2*numberServers))*numberServers;
    timeAlive= par("timeAliveJob");

    //printf("---TIMEALIVE: %d \n",timeAlive);
}

void SourceOnce::handleMessage(cMessage *msg)
{

    ASSERT(msg->isSelfMessage());
    delete msg;
    int maxAvatar=this->getParentModule()->getParentModule()->par("maxAvatar");
    int numberServer=this->getParentModule()->getParentModule()->par("numberServer");
    int n= (maxAvatar/(2*numberServer));
    //int n = par("numJobs");
    for (int i = 0; i < n; i++) {
        Job *job = createJob(timeAlive);

        send(job, "out");
    }
}

}; //namespace

