#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

int
main (int argc, char *argv[])
{

  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NodeContainer nodes;
  nodes.Create (6);
  NodeContainer n0n1 = NodeContainer (nodes.Get (0), nodes.Get (1));
  NodeContainer n0n2 = NodeContainer (nodes.Get (0), nodes.Get (2));
  NodeContainer n1n2 = NodeContainer (nodes.Get (1), nodes.Get (2));
  NodeContainer n1n3 = NodeContainer (nodes.Get (1), nodes.Get (3));
  NodeContainer n1n4 = NodeContainer (nodes.Get (1), nodes.Get (4));
  NodeContainer n2n4 = NodeContainer (nodes.Get (2), nodes.Get (4));
  NodeContainer n3n4 = NodeContainer (nodes.Get (3), nodes.Get (4));
  NodeContainer n4n5 = NodeContainer (nodes.Get (4), nodes.Get (5));

  InternetStackHelper internet;
  internet.Install (nodes);

  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));
  NetDeviceContainer d0d1 = p2p.Install (n0n1);
  NetDeviceContainer d0d2 = p2p.Install (n0n2);
  NetDeviceContainer d1d2 = p2p.Install (n1n2);
  NetDeviceContainer d1d3 = p2p.Install (n1n3);
  NetDeviceContainer d1d4 = p2p.Install (n1n4);
  NetDeviceContainer d2d4 = p2p.Install (n2n4);
  NetDeviceContainer d3d4 = p2p.Install (n3n4);
  NetDeviceContainer d4d5 = p2p.Install (n4n5);

  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i0i1 = ipv4.Assign (d0d1);
  ipv4.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer i0i2 = ipv4.Assign (d0d2);
  ipv4.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer i1i2 = ipv4.Assign (d1d2);
  ipv4.SetBase ("10.1.4.0", "255.255.255.0");
  Ipv4InterfaceContainer i1i3 = ipv4.Assign (d1d3);
  ipv4.SetBase ("10.1.5.0", "255.255.255.0");
  Ipv4InterfaceContainer i1i4 = ipv4.Assign (d1d4);
  ipv4.SetBase ("10.1.6.0", "255.255.255.0");
  Ipv4InterfaceContainer i2i4 = ipv4.Assign (d2d4);
  ipv4.SetBase ("10.1.7.0", "255.255.255.0");
  Ipv4InterfaceContainer i3i4 = ipv4.Assign (d3d4);
  ipv4.SetBase ("10.1.8.0", "255.255.255.0");
  Ipv4InterfaceContainer i4i5 = ipv4.Assign (d4d5);

  // Create router nodes, initialize routing database and set up the routing
  // tables in the nodes.
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  UdpEchoServerHelper echoServer (6);

  ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (20.0));

  UdpEchoClientHelper echoClient (i1i2.GetAddress (0), 6);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (nodes.Get (2));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (20.0));

  UdpEchoClientHelper echoClient1 (i1i2.GetAddress (0), 6);
  echoClient1.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient1.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient1.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps1 = echoClient1.Install (nodes.Get (0));
  clientApps1.Start (Seconds (4.0));
  clientApps1.Stop (Seconds (20.0));

  UdpEchoClientHelper echoClient2 (i1i2.GetAddress (0), 6);
  echoClient2.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient2.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient2.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps2 = echoClient.Install (nodes.Get (3));
  clientApps2.Start (Seconds (6.0));
  clientApps2.Stop (Seconds (20.0));

  UdpEchoClientHelper echoClient3 (i1i2.GetAddress (0), 6);
  echoClient3.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient3.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient3.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps3 = echoClient.Install (nodes.Get (4));
  clientApps3.Start (Seconds (8.0));
  clientApps3.Stop (Seconds (20.0));

  UdpEchoClientHelper echoClient4 (i1i2.GetAddress (0), 6);
  echoClient4.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient4.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient4.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps4 = echoClient.Install (nodes.Get (5));
  clientApps4.Start (Seconds (10.0));
  clientApps4.Stop (Seconds (20.0));

  Ipv4GlobalRoutingHelper g;
  Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> ("temp.routes", std::ios::out);
  g.PrintRoutingTableAllAt (Seconds (12), routingStream);

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
