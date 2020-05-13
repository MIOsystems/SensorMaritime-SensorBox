using GpsGyroAnalysisUITool.Networking;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using tmr = System.Windows.Forms;

namespace GpsGyroAnalysisUITool
{
    public partial class Form1 : Form
    {

        private DataReceiver receiver;
        private DataReceiver2 receiver2;
        private tmr.Timer timer = new tmr.Timer();

 


        private void timer_Tick(object sender, EventArgs e)
        {
            graphController.Invalidate();
           
        }



        public Form1()
        {
            InitializeComponent();

            receiver2 = new DataReceiver2();
            //receiver = new DataReceiver();

            graphController.Load(receiver2);
            mapController.Load(receiver2);
           

            SetStyle(ControlStyles.OptimizedDoubleBuffer, true);

            SetStyle(ControlStyles.AllPaintingInWmPaint, true);

            SetStyle(ControlStyles.UserPaint, true);

            //run redraw timer
            timer.Interval = 1;
            timer.Tick += new EventHandler(timer_Tick);
            timer.Start();

        }



        //double ToDegrees(double radians) {
        //    double r = radians * (180 / Math.PI);
        //    return radians * (180 / Math.PI);
        //}



        //void drawRotationBar(Graphics g, double min, double max, double avg, int positionx, string label) {

        //    int range = (Height - 80);

        //    double avgDeg = ((90.0 + ToDegrees(avg))/180.0)   * range;
        //    double minDeg = ((90.0 + ToDegrees(min)) / 180.0) * range;
        //    double maxDeg = ((90.0 + ToDegrees(max)) / 180.0) * range;

        //    int maxRange = (int)(maxDeg - minDeg);

        //    g.DrawString(label, SystemFonts.DefaultFont, new SolidBrush(Color.FromArgb(80, 80, 80)), new Point(positionx+10, 6));
        //    g.FillRectangle(Brushes.Gray, new Rectangle(positionx, 20, 30, Height - 100));
        //    g.DrawLine(Pens.Black, positionx-1, 20, positionx+(30-1), 20);
        //    g.DrawLine(Pens.Black, positionx - 1, Height - 80, positionx + (30 - 1), Height - 80);
        //    g.DrawLine(Pens.Black, positionx+15, 20, positionx+15, Height - 80);
        //    g.FillRectangle(Brushes.Blue, new Rectangle(positionx, 20+(int)minDeg, 30, maxRange));
        //    g.DrawLine(Pens.Red, positionx - 1, 20+(int)avgDeg , positionx + (30 - 1), 20+(int)avgDeg);
        //    g.DrawString("min: "+ (int)ToDegrees(avg), SystemFonts.DefaultFont, new SolidBrush(Color.FromArgb(80, 80, 80)), new Point(positionx + 10, Height-80));
        //    g.DrawString("max: "+(int)ToDegrees(min), SystemFonts.DefaultFont, new SolidBrush(Color.FromArgb(80, 80, 80)), new Point(positionx + 10, Height - 70));
        //    g.DrawString("avg: "+ (int)ToDegrees(max), SystemFonts.DefaultFont, new SolidBrush(Color.FromArgb(80, 80, 80)), new Point(positionx + 10, Height - 60));
        //}

    }
}
