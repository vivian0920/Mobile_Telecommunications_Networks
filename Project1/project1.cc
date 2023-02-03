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

//include all module
#include "ns3/command-line.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/rectangle.h"
#include "ns3/string.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/ssid.h"
#include "ns3/mobility-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv6-address-helper.h"
#include "ns3/on-off-helper.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/csma-helper.h"
#include "ns3/bridge-helper.h"
#include "ns3/packet-socket-address.h"

#include "ns3/netanim-module.h" // use NetAnim to animate our project
#include "ns3/packet-sink.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/ipv6-address-helper.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("NS-3_Project1");//declare log object

int main (int argc, char *argv[])
{
  uint32_t nWifis = 2;
  //use log
  LogComponentEnable ("NS-3_Project1", LOG_LEVEL_ALL);
  NS_LOG_INFO ("Ns-3 Project 6");
    
  /**************************************************************/
  // change the number of sta to 3
  uint32_t nStas = 3;
  /**************************************************************/

  //Topology
  NodeContainer backboneNodes;
  NetDeviceContainer backboneDevices; //NetDevice provides various ways to manage and wire other node and channel objects.
  
  /**************************************************************/
  // Use Ipv6InterfaceContainer
  Ipv6InterfaceContainer backboneInterfaces;
  /**************************************************************/
  
  std::vector<NodeContainer> staNodes;
  std::vector<NetDeviceContainer> staDevices;
  std::vector<NetDeviceContainer> apDevices;

  /**************************************************************/
  // Use Ipv6InterfaceContainer
  std::vector<Ipv6InterfaceContainer> staInterfaces;
  std::vector<Ipv6InterfaceContainer> apInterfaces;
  /**************************************************************/

  InternetStackHelper stack; // A class to help node to install protocal
  CsmaHelper csma; // A class for network device and channel. 
  /**************************************************************/
  // Use Ipv6InterfaceContainer and the way to use setBase can find sample file in ns-3.33/examples/ipv6/ 
  // A class to manage the assignment of IP addresses
  Ipv6AddressHelper ip;
  //ip.SetBase (Ipv6Address("192.168.0.0"), "255.255.255.0");
  // SetBase is a function for IPV6 to set the base network number, network prefix, and base interface ID.
  ip.SetBase (Ipv6Address ("2001:1::"), Ipv6Prefix (64));
  /**************************************************************/

  backboneNodes.Create (n); //create two nodes
  stack.Install (backboneNodes); //install protocal（ex.TCP, UDP, IP etc...）on nodes

  backboneDevices = csma.Install (backboneNodes); // built csma device

  double wifiX = 0.0;

  YansWifiPhyHelper wifiPhy; //create and manage PHY objects for the YANS model
  wifiPhy.SetPcapDataLinkType (WifiPhyHelper::DLT_IEEE802_11_RADIO); //let pcap can trace 


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
      // Use Ipv6InterfaceContainer
      Ipv6InterfaceContainer staInterface;
      Ipv6InterfaceContainer apInterface;
      /**************************************************************/
      
      MobilityHelper mobility; // A class that set the moving modle to nodes
      BridgeHelper bridge; // A calss that add capability to bridge multiple LAN segments
      WifiHelper wifi; // A calss that helps to create WifiNetDevice objects
      WifiMacHelper wifiMac; // A class that to set MAC model
      YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default (); // A class that can build wifiChannel with propagationLoss and propagationDelay
      wifiPhy.SetChannel (wifiChannel.Create ()); // let wifiPhy（Physical layer device）connect to wifiChannel

      sta.Create (nStas); // create STA nodes

      //Set initial position for AP nodes
      mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                     "MinX", DoubleValue (wifiX),
                                     "MinY", DoubleValue (0.0),
                                     "DeltaX", DoubleValue (5.0),
                                     "DeltaY", DoubleValue (5.0),
                                     "GridWidth", UintegerValue (1),
                                     "LayoutType", StringValue ("RowFirst"));

      // setup the AP.
      // set the moving way to nodes 
      mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
      mobility.Install (backboneNodes.Get (i));
      // Set the type of MAC for AP
      wifiMac.SetType ("ns3::ApWifiMac",
                       "Ssid", SsidValue (ssid));
      apDev = wifi.Install (wifiPhy, wifiMac, backboneNodes.Get (i));

      NetDeviceContainer bridgeDev;
      bridgeDev = bridge.Install (backboneNodes.Get (i), NetDeviceContainer (apDev, backboneDevices.Get (i)));

      // assign AP IP address to bridge, not wifi
      apInterface = ip.Assign (bridgeDev);

      // setup the STAs
      stack.Install (sta);
      //Set initial position for STA nodes
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
  
  // 此處為架設 Server UDP Application 與 Client UDP Application
  // 此處只架設了一個 Server UDP Application 與一個 Client UDP Application 
  // Server 端為接收端，Client 端為發送端，Server 端接收封包後會回傳封包給 Client 端
  // 在下列範例 code 中 Server 端架設在第一個 Wifi 下的 STA Node 0，Client 端架設在第二個 Wifi 下的 STA Node 0
  // StaNodes[0] 中儲存了第一個 Wifi 下的所有 STA Node，StaNodes[1] 中儲存了第二個 Wifi 下的所有 STA Node
  // 同學可以自行更改，利用 for 迴圈進行增加，架設 3 對一對一的 Server 與 Client UDP Application，每對 Server 與 Client Application 都要有自己所使用的 port
  //***********************************************//
  uint16_t port = 1000;
  uint32_t packetSize = 1024;
  uint32_t maxPacketCount = 10;
  uint32_t pairSC=3;//the amount of Server and Client
  // 此處可以設定 Application 傳送封包的間隔時間
  Time interPacketInterval = Seconds (1.);
  
  // set UDP echo server app on nodes （server and client have to use same port.）
  for(uint32_t i=0;i<pairSC;i++){
	  port+=i;
  	UdpEchoServerHelper server(port); 
  	ApplicationContainer serverApp = server.Install(staNodes[0].Get(i));
  
  	UdpEchoClientHelper client(ca[0].GetAddress(i, 0), port);
  	client.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  	client.SetAttribute ("Interval", TimeValue (interPacketInterval));
  	client.SetAttribute ("PacketSize", UintegerValue (packetSize));

  	ApplicationContainer clientApp = client.Install(staNodes[1].Get(i));
  
  	// set start and stop time of the server application
  	serverApp.Start(Seconds(1.0+i));
  	serverApp.Stop(Seconds(10.0+i));
  
  	// set start and stop time of the client application
  	clientApp.Start(Seconds(2.0+i));
  	clientApp.Stop(Seconds(10.0+i));
}
  //***********************************************//

  // 此處為擷取 csma 封包
  // 執行完程式後會產生兩個 pcap 檔 (p1-0-1.pcap, p1-1-1.pcap)
  // 同學再依據 pcap 檔內擷取的封包進行觀察撰寫報告即可
  // 例如：ipv6協定傳送了什麼封包，這些封包的目的是什麼...等
  csma.EnablePcap("p1", backboneDevices.Get(0));
  csma.EnablePcap("p1", backboneDevices.Get(1));

  Simulator::Stop (Seconds (14.0));

  // 此處為產生 p1.xml 檔，同學可以利用 NetAnim 開啟檔案查看模擬動畫
  AnimationInterface  anim("p1.xml");
  anim.SetStartTime(Seconds(0));
  anim.SetStopTime(Seconds(14));
  Simulator::Run ();
  Simulator::Destroy ();
}
