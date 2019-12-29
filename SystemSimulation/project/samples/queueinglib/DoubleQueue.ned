//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2015 OpenSim Ltd.
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

package org.omnetpp.queueing;

//
// A queue with a built-in server
//
simple DoubleQueue
{
    parameters:
        
        @group(Queueing);
        @display("i=block/queue;q=queue");
        @signal[dropped](type="long");
        @signal[queueLength1](type="long");
        @signal[queueLength2](type="long");
        @signal[queueingTime](type="simtime_t");
        @signal[busy](type="bool");
        @signal[elaborationTime](type="simtime_t");
        @signal[zoneLength](type="long");
        @statistic[dropped](title="drop event";record=vector?,count;interpolationmode=none);
        @statistic[queueingTime](title="queueing time at dequeue";record=vector?,mean,max;unit=s;interpolationmode=none);
        @statistic[busy](title="server busy state";record=vector?,timeavg;interpolationmode=sample-hold);
		@statistic[elaborationTime](title="elaboration time at queue empty";record=vector,min,mean,max;unit=s;interpolationmode=none);
        @statistic[zoneLength](title="zone length";record=vector,timeavg,max;interpolationmode=sample-hold);
        
        int capacity = default(-1);    // negative capacity means unlimited queue
        bool fifo = default(true);     // whether the module works as a queue (fifo=true) or a stack (fifo=false)
        
        
        volatile double serviceTimeSameServer @unit(s);
        volatile double serviceTimeAnotherServer @unit(s);
        volatile double serviceTimeDelete @unit(s);
    gates:
        input receiveIE @directIn;
        //output sendIE @directIn;
        input in[];
        output out[3];
}
