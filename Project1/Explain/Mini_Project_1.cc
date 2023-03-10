//      *********************************** Topology ***********************************
//      +-----+      +-----+      +-----+         +-----+      +-----+      +-----+ 
//      | STA |      | STA |      | STA |         | STA |      | STA |      | STA |
//      +-----+      +-----+      +-----+         +-----+      +-----+      +-----+
//     IPaddress    IPaddress     IPaddress      IPaddress     IPaddress   IPaddress
//      --------     --------     --------        --------     --------     --------
//      WIFI STA     WIFI STA     WIFI STA        WIFI STA     WIFI STA     WIFI STA
//      --------     --------     --------        --------     --------     --------
//        ((*))       ((*))         ((*))    |      ((*))        ((*))        ((*))
//                                           |
//                    ((*))                  |                   ((*))
//                   -------                                    -------
//                   WIFI AP         CSMA ========= CSMA        WIFI AP
//                   -------         ----           ----        -------
//                ##############                             ##############
//                    BRIDGE                                     BRIDGE
//                ##############                             ##############
//                   IPaddress                                  IPaddress
//                  +---------+                                +---------+
//                  | AP Node |                                | AP Node |
//                  +---------+                                +---------+
//      *********************************** Topology ***********************************

#include "ns3/command-line.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/rectangle.h"
#include "ns3/string.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/ssid.h"
#include "ns3/mobility-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/on-off-helper.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/csma-helper.h"
#include "ns3/bridge-helper.h"
#include "ns3/packet-socket-address.h"

#include "ns3/netanim-module.h"
#include "ns3/packet-sink.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/ipv6-address-helper.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

int main (int argc, char *argv[])
{
  uint32_t nWifis = 2;
  
  /**************************************************************/
  // change the number of sta to 3
  uint32_t nStas = 2;
  /**************************************************************/

  NodeContainer backboneNodes;
  NetDeviceContainer backboneDevices;
  
  /**************************************************************/
  // change Ipv4InterfaceContainer to Ipv6InterfaceContainer
  Ipv4InterfaceContainer backboneInterfaces;
  /**************************************************************/
  
  std::vector<NodeContainer> staNodes;
  std::vector<NetDeviceContainer> staDevices;
  std::vector<NetDeviceContainer> apDevices;

  /**************************************************************/
  // change Ipv4InterfaceContainer to Ipv6InterfaceContainer
  std::vector<Ipv4InterfaceContainer> staInterfaces;
  std::vector<Ipv4InterfaceContainer> apInterfaces;
  /**************************************************************/

  InternetStackHelper stack;
  CsmaHelper csma;

  /**************************************************************/
  // change ipv4AddressHelper to ipv6AddressHelper
  // ??????????????? ns-3.33/examples/ipv6/ ?????????????????????????????? ipv6AddressHelper SetBase ???????????????
  Ipv4AddressHelper ip;
  ip.SetBase ("192.168.0.0", "255.255.255.0");
  /**************************************************************/

  backboneNodes.Create (nWifis);
  stack.Install (backboneNodes);

  backboneDevices = csma.Install (backboneNodes);

  double wifiX = 0.0;

  YansWifiPhyHelper wifiPhy;
  wifiPhy.SetPcapDataLinkType (WifiPhyHelper::DLT_IEEE802_11_RADIO);

  for (uint32_t i = 0; i < nWifis; ++i)
    {
      // calculate ssid for wifi subnetwork
      std::ostringstream oss;
      oss << "wifi-default-" << i;
      Ssid ssid = Ssid (oss.str ());

      NodeContainer sta;
      NetDeviceContainer staDev;
      NetDeviceContainer apDev;
      
      /**************************************************************/
      // change Ipv4InterfaceContainer to Ipv6InterfaceContainer
      Ipv4InterfaceContainer staInterface;
      Ipv4InterfaceContainer apInterface;
      /**************************************************************/
      
      MobilityHelper mobility;
      BridgeHelper bridge;
      WifiHelper wifi;
      WifiMacHelper wifiMac;
      YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
      wifiPhy.SetChannel (wifiChannel.Create ());

      sta.Create (nStas);
      mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                     "MinX", DoubleValue (wifiX),
                                     "MinY", DoubleValue (0.0),
                                     "DeltaX", DoubleValue (5.0),
                                     "DeltaY", DoubleValue (5.0),
                                     "GridWidth", UintegerValue (1),
                                     "LayoutType", StringValue ("RowFirst"));

      // setup the AP.
      mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
      mobility.Install (backboneNodes.Get (i));
      wifiMac.SetType ("ns3::ApWifiMac",
                       "Ssid", SsidValue (ssid));
      apDev = wifi.Install (wifiPhy, wifiMac, backboneNodes.Get (i));

      NetDeviceContainer bridgeDev;
      bridgeDev = bridge.Install (backboneNodes.Get (i), NetDeviceContainer (apDev, backboneDevices.Get (i)));

      // assign AP IP address to bridge, not wifi
      apInterface = ip.Assign (bridgeDev);

      // setup the STAs
      stack.Install (sta);
      mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                                 "Mode", StringValue ("Time"),
                                 "Time", StringValue ("2s"),
                                 "Speed", StringValue ("ns3::ConstantRandomVariable[Constant=1.0]"),
                                 "Bounds", RectangleValue (Rectangle (wifiX, wifiX + 5.0,0.0, (nStas + 1) * 5.0)));
      mobility.Install (sta);
      wifiMac.SetType ("ns3::StaWifiMac",
                       "Ssid", SsidValue (ssid));
      staDev = wifi.Install (wifiPhy, wifiMac, sta);
      staInterface = ip.Assign (staDev);

      // save everything in containers.
      staNodes.push_back (sta);
      apDevices.push_back (apDev);
      apInterfaces.push_back (apInterface);
      staDevices.push_back (staDev);
      staInterfaces.push_back (staInterface);

      wifiX += 20.0;
    }
  
  // ??????????????? Server UDP Application ??? Client UDP Application
  // ???????????????????????? Server UDP Application ????????? Client UDP Application 
  // Server ??????????????????Client ??????????????????Server ???????????????????????????????????? Client ???
  // ??????????????? code ??? Server ????????????????????? Wifi ?????? STA Node 0???Client ????????????????????? Wifi ?????? STA Node 0
  // StaNodes[0] ????????????????????? Wifi ???????????? STA Node???StaNodes[1] ????????????????????? Wifi ???????????? STA Node
  // ????????????????????????????????? for ??????????????????????????? 3 ??????????????? Server ??? Client UDP Application????????? Server ??? Client Application ??????????????????????????? port
  //***********************************************//
  uint16_t port = 1000;
  uint32_t packetSize = 1024;
  uint32_t maxPacketCount = 10;
  // ?????????????????? Application ???????????????????????????
  Time interPacketInterval = Seconds (1.);
  
  uint32_t i = 0;
  UdpEchoServerHelper server(port);
  ApplicationContainer serverApp = server.Install(staNodes[0].Get(i));
  
  UdpEchoClientHelper client(staInterfaces[0].GetAddress(i, 0), port);
  client.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  client.SetAttribute ("Interval", TimeValue (interPacketInterval));
  client.SetAttribute ("PacketSize", UintegerValue (packetSize));

  ApplicationContainer clientApp = client.Install(staNodes[1].Get(i));
  
  // set start and stop time of the server application
  serverApp.Start(Seconds(1.0));
  serverApp.Stop(Seconds(10.0));
  
  // set start and stop time of the client application
  clientApp.Start(Seconds(2.0));
  clientApp.Stop(Seconds(10.0));
  //***********************************************//

  // ??????????????? csma ??????
  // ????????????????????????????????? pcap ??? (p1-0-1.pcap, p1-1-1.pcap)
  // ??????????????? pcap ???????????????????????????????????????????????????
  // ?????????ipv6????????????????????????????????????????????????????????????...???
  csma.EnablePcap("p1", backboneDevices.Get(0));
  csma.EnablePcap("p1", backboneDevices.Get(1));

  Simulator::Stop (Seconds (10.0));

  // ??????????????? p1.xml ???????????????????????? NetAnim ??????????????????????????????
  AnimationInterface  anim("p1.xml");
  anim.SetStartTime(Seconds(0));
  anim.SetStopTime(Seconds(10));
  Simulator::Run ();
  Simulator::Destroy ();
}