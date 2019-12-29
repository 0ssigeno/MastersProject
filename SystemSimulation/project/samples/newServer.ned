import org.omnetpp.queueing.Delay;
import org.omnetpp.queueing.DoubleQueue;
import org.omnetpp.queueing.Fork;
import org.omnetpp.queueing.JobList;
import org.omnetpp.queueing.PassiveQueue;
import org.omnetpp.queueing.Queue;
import org.omnetpp.queueing.Router;
import org.omnetpp.queueing.Server;
import org.omnetpp.queueing.Sink;
import org.omnetpp.queueing.Source;
import org.omnetpp.queueing.SourceOnce;


network Project
{
    parameters:
        @display("bgb=716,368");
        int numberServer = default(4);
        int maxAvatar = default(40);
    submodules:
        source[numberServer]: Source {
            @display("p=148,37;b=50,40");
        }
        Coordinatore: Queue {
            @display("p=148,115;b=36,23");
        }
        routerRR: Router {
            @display("p=257,115;b=61,42");
            routingAlgorithm = "roundRobin";
        }

        routerBetwenServers: Router {
            @display("p=348,246;b=54,41");
            routingAlgorithm = "random";
        }
        realsink: Sink {
            @display("p=674,115;b=41,34");
        }
        newServer[numberServer]: newServer {
            @display("p=427,115");
        }

    //    }
    connections:
        Coordinatore.out --> routerRR.in++;
        for i=0..numberServer-1 {
            source[i].out --> Coordinatore.in++;

            routerRR.out++ --> newServer[i].fromCoordinator;
            routerBetwenServers.out++ --> newServer[i].fromIntraServer;
            //      newServer[i].toIntraServer --> delay.in++;
            newServer[i].toIntraServer --> routerBetwenServers.in++;
            //      newServer[i].toSink --> delay1.in++;
            newServer[i].toSink --> realsink.in++;
        }


   
}
module newServer
{
    parameters:
        @display("bgb=416,217");
    gates:
        input fromIntraServer;
        input fromCoordinator;

        output toSink;
        output toIntraServer;
        //output sendFE @directIn;
    submodules:
        doubleQueue: DoubleQueue {
            @display("p=244,126;b=56,26");
        }
        sourceOnce: SourceOnce {
            @display("p=84,102");
        }
    connections:
        doubleQueue.out[0] --> toIntraServer;
        doubleQueue.out[1] --> doubleQueue.in++;
        doubleQueue.out[2] --> toSink;
        fromCoordinator --> doubleQueue.in++;
        fromIntraServer --> doubleQueue.in++;
        sourceOnce.out --> doubleQueue.in++;
}
