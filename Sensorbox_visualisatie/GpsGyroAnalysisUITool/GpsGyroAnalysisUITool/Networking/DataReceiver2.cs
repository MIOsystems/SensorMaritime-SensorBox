using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace GpsGyroAnalysisUITool.Networking
{
    public class DataReceiver2
    {

        private ConcurrentQueue<SensorData> queue;
        private UdpClient client;
        private const string LocalIP = "192.168.0.10", SensorIP = "192.168.0.9";
        private const int DestPort = 52256, ConstPort = 62510;

        public List<Action<SensorData>> actions;
        public List<Action<NMEAData>> gpsActions;

        // public Action<SensorData> handler;

        public DataReceiver2()
        {
            queue = new ConcurrentQueue<SensorData>();
            actions = new List<Action<SensorData>>();
            gpsActions = new List<Action<NMEAData>>();
            client = new UdpClient(new IPEndPoint(IPAddress.Parse(SensorIP), DestPort));
            Thread thread = new Thread(RunPackageQueue);
            thread.Start();

        }


        private double MinuteToDegree(double point)
        {
            double deg;
            double mins;

            deg = (long)(point / 100);
            mins = point / 100 - deg;
            mins /= 60;
            mins *= 100;

            return deg + mins;
        }


        public void RunPackageQueue()
        {
            while (true)
            {
                IPEndPoint remoteEnd = new IPEndPoint(IPAddress.Parse(LocalIP), ConstPort);
                byte[] message = client.Receive(ref remoteEnd);

                SensorData data = new SensorData();


                //if (message[0] == 5)
                //{
                //    data.avg_x = BitConverter.ToDouble(message, 1);
                //    data.avg_y = BitConverter.ToDouble(message, 9);
                //    data.avg_z = BitConverter.ToDouble(message, 17);
                //    data.min_x = BitConverter.ToDouble(message, 25);
                //    data.min_y = BitConverter.ToDouble(message, 33);
                //    data.min_z = BitConverter.ToDouble(message, 41);
                //    data.max_x = BitConverter.ToDouble(message, 49);
                //    data.max_y = BitConverter.ToDouble(message, 57);
                //    data.max_z = BitConverter.ToDouble(message, 65);
                //    foreach (var action in actions)
                //    {
                //        action(data);
                //    }

                //}
                //else
                //{
                //    string nmeaString = Encoding.ASCII.GetString(message, 1, message.Length - 2);
                //    Console.WriteLine("strs: " + nmeaString);
                //    string[] gpsData = nmeaString.Substring(0, nmeaString.Length - 1).Split(',');

                //    NMEAData nmeaData = new NMEAData();
                //    if (gpsData[8] != string.Empty && gpsData[3] != string.Empty && gpsData[5] != string.Empty)
                //    {
                //        nmeaData.degreeAngle = double.Parse(gpsData[8].Replace('.', ','));
                //        nmeaData.latitude = MinuteToDegree(double.Parse(gpsData[5].Replace('.', ',')));
                //        nmeaData.longtitude = MinuteToDegree(double.Parse(gpsData[3].Replace('.', ',')));
                //        nmeaData.headingSet = true;
                //    }



                //    //Console.WriteLine("wl: "+ nmeaData.degreeAngle);
                //    //Console.WriteLine("lat: "+ nmeaData.latitude);
                //    //Console.WriteLine("lng: " + nmeaData.longtitude);

                //    foreach (var gpsAction in gpsActions)
                //    {
                //        gpsAction(nmeaData);
                //    }

                //}
                string s = Encoding.UTF8.GetString(message, 0, message.Length);


                if (s.StartsWith("ADMS"))
                {

                  

                   // Console.WriteLine(s);

                    s = s.Substring(5, s.IndexOf("$")-5);
                    string[] stringValues =  s.Split(',');

                    Console.WriteLine(s);
                    data.isMS2 = true;
                    data.avg_x = double.Parse(stringValues[0], NumberStyles.Any, CultureInfo.InvariantCulture);
                    data.avg_y = double.Parse(stringValues[1], NumberStyles.Any, CultureInfo.InvariantCulture);
                    data.avg_z = double.Parse(stringValues[2], NumberStyles.Any, CultureInfo.InvariantCulture);
                    data.min_x = double.Parse(stringValues[3], NumberStyles.Any, CultureInfo.InvariantCulture);
                    data.min_y = double.Parse(stringValues[4], NumberStyles.Any, CultureInfo.InvariantCulture);
                    data.min_z = double.Parse(stringValues[5], NumberStyles.Any, CultureInfo.InvariantCulture);
                    data.max_x = double.Parse(stringValues[6], NumberStyles.Any, CultureInfo.InvariantCulture);
                    data.max_y = double.Parse(stringValues[7], NumberStyles.Any, CultureInfo.InvariantCulture);
                    data.max_z = double.Parse(stringValues[8], NumberStyles.Any, CultureInfo.InvariantCulture);
                    foreach (var action in actions)
                    {
                        action(data);
                    }
                }

                else if (s.StartsWith("ADR"))
                {
                   // string s = Encoding.UTF8.GetString(message, 1, message.Length - 1);

                    Console.WriteLine(s);
                    s = s.Substring(4, s.IndexOf("$")-4);
                    string[] stringValues = s.Split(',');
                    data.avg_x = double.Parse(stringValues[0], NumberStyles.Any, CultureInfo.InvariantCulture);
                    data.avg_y = double.Parse(stringValues[1], NumberStyles.Any, CultureInfo.InvariantCulture);
                    data.avg_z = double.Parse(stringValues[2], NumberStyles.Any, CultureInfo.InvariantCulture);
                    data.min_x = double.Parse(stringValues[3], NumberStyles.Any, CultureInfo.InvariantCulture);
                    data.min_y = double.Parse(stringValues[4], NumberStyles.Any, CultureInfo.InvariantCulture);
                    data.min_z = double.Parse(stringValues[5], NumberStyles.Any, CultureInfo.InvariantCulture);
                    data.max_x = double.Parse(stringValues[6], NumberStyles.Any, CultureInfo.InvariantCulture);
                    data.max_y = double.Parse(stringValues[7], NumberStyles.Any, CultureInfo.InvariantCulture);
                    data.max_z = double.Parse(stringValues[8], NumberStyles.Any, CultureInfo.InvariantCulture);
                    foreach (var action in actions)
                    {
                        action(data);
                    }

                }
                else {
                   // string nmeaString = Encoding.ASCII.GetString(message, 1, message.Length - 2);
                   // Console.WriteLine("strs: " + nmeaString);
                    string[] gpsData = s.Substring(0, s.Length - 1).Split(',');

                    NMEAData nmeaData = new NMEAData();
                    if (gpsData[8] != string.Empty && gpsData[3] != string.Empty && gpsData[5] != string.Empty)
                    {
                        nmeaData.degreeAngle = double.Parse(gpsData[8].Replace('.', ','));
                        nmeaData.latitude = MinuteToDegree(double.Parse(gpsData[5].Replace('.', ',')));
                        nmeaData.longtitude = MinuteToDegree(double.Parse(gpsData[3].Replace('.', ',')));
                        nmeaData.headingSet = true;
                    }



                    //Console.WriteLine("wl: "+ nmeaData.degreeAngle);
                    //Console.WriteLine("lat: "+ nmeaData.latitude);
                    //Console.WriteLine("lng: " + nmeaData.longtitude);

                    foreach (var gpsAction in gpsActions)
                    {
                        gpsAction(nmeaData);
                    }
                }



                //data.avg_x = BitConverter.ToDouble(message, 0);
                //data.avg_y = BitConverter.ToDouble(message, 8);
                //data.avg_z = BitConverter.ToDouble(message, 16);
                //data.min_x = BitConverter.ToDouble(message, 24);
                //data.min_y = BitConverter.ToDouble(message, 32);
                //data.min_z = BitConverter.ToDouble(message, 40);
                //data.max_x = BitConverter.ToDouble(message, 48);
                //data.max_y = BitConverter.ToDouble(message, 56);
                //data.max_z = BitConverter.ToDouble(message, 64);
                //handler?.Invoke(data);

                //foreach (var action in actions) {
                //    action(data);
                //}

                //Console.WriteLine("data X:  avg:{0} min:{1} max:{2}", data.avg_x, data.min_x, data.max_x);
                //Console.WriteLine("data Y:  avg:{0} min:{1} max:{2}", data.avg_y, data.min_y, data.max_y);
                //Console.WriteLine("data Z:  avg:{0} min:{1} max:{2}", data.avg_z, data.min_z, data.max_z);

                //Console.WriteLine("f");
                //string mStr = Encoding.Default.GetString(message);



                //if (mStr.StartsWith("gyro"))
                //{

                //    Console.WriteLine(mStr);

                //    float x = BitConverter.ToSingle(message, 4);
                //    float y = BitConverter.ToSingle(message, 8);
                //    float z = BitConverter.ToSingle(message, 12);
                //    Console.WriteLine("strvals x:{0} y:{1}  z:{2}", x, y, z);

                //}

            }

        }
    }
}
