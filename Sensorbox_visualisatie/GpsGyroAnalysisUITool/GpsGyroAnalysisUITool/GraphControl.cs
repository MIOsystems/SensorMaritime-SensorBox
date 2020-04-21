using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using GpsGyroAnalysisUITool.Networking;

namespace GpsGyroAnalysisUITool
{
    public partial class GraphControl : UserControl
    {

        private SensorData data;

        public GraphControl()
        {
            InitializeComponent();
            data = new SensorData();

            SetStyle(ControlStyles.OptimizedDoubleBuffer, true);

            SetStyle(ControlStyles.AllPaintingInWmPaint, true);

            SetStyle(ControlStyles.UserPaint, true);
        }


        public void Load(DataReceiver dataReceiver) {
            dataReceiver.actions.Add((sensordata) =>
            {
                data = new SensorData(sensordata);

            });

        }


        double ToDegrees(double radians)
        {
            double r = radians * (180 / Math.PI);
            return radians * (180 / Math.PI);
        }

        private void GraphControl_Paint(object sender, PaintEventArgs e)
        {

            Graphics g = e.Graphics;

            drawRotationBar(g, data.min_x, data.max_x, data.avg_x, 50, "x");
            drawRotationBar(g, data.min_y, data.max_y, data.avg_y, 120, "y");
            drawRotationBar(g, data.min_z, data.max_z, data.avg_z, 190, "z");
        }


        void drawRotationBar(Graphics g, double min, double max, double avg, int positionx, string label)
        {

            int range = (Height - 100);

            double avgDeg = ((90.0 + ToDegrees(avg)) / 180.0) * range;
            double minDeg = ((90.0 + ToDegrees(min)) / 180.0) * range;
            double maxDeg = ((90.0 + ToDegrees(max)) / 180.0) * range;


            double gavgDeg = ((90.0 - ToDegrees(avg)) / 180.0) * range;
            double gminDeg = ((90.0 - ToDegrees(min)) / 180.0) * range;
            double gmaxDeg = ((90.0 - ToDegrees(max)) / 180.0) * range;

            int maxRange = (int)(maxDeg - minDeg);

            int gmaxRange = (int)(gminDeg - gmaxDeg );

            g.DrawString(label, SystemFonts.DefaultFont, new SolidBrush(Color.FromArgb(80, 80, 80)), new Point(positionx + 10, 6));

          

            g.FillRectangle(Brushes.Gray, new Rectangle(positionx, 20, 50, Height - 100));
            g.DrawLine(Pens.Black, positionx - 1, 20, positionx + (50 - 1), 20);
            g.DrawLine(Pens.Black, positionx - 1, Height - 80, positionx + (50 - 1), Height - 80);
           // g.DrawLine(Pens.Black, positionx + 15, 20, positionx + 15, Height - 80);
            g.FillRectangle(Brushes.Blue, new Rectangle(positionx, 20 + (int)gmaxDeg, 50, gmaxRange));
            g.DrawLine(Pens.Red, positionx - 1, 20+(int)gavgDeg, positionx + (50 - 1), 20+(int)gavgDeg);
            g.DrawString("min: " + Math.Round(ToDegrees(min),2), SystemFonts.DefaultFont, new SolidBrush(Color.FromArgb(80, 80, 80)), new Point(positionx + 10, Height - 80));
            g.DrawString("max: " + Math.Round(ToDegrees(max),2), SystemFonts.DefaultFont, new SolidBrush(Color.FromArgb(80, 80, 80)), new Point(positionx + 10, Height - 70));
            g.DrawString("avg: " + Math.Round(ToDegrees(avg),2), SystemFonts.DefaultFont, new SolidBrush(Color.FromArgb(80, 80, 80)), new Point(positionx + 10, Height - 60));
        }
    }
}
