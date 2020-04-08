using GeoAPI.Geometries;
using SharpMap;
using SharpMap.Utilities;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace DavisBridgeTool3.Entities
{
	class ImageMapRenderer
	{
		private const string ArrowLocation = "../../Images/Arrow.png";
		private static Bitmap ArrowIcon;


		//icon for a boat actually
		public static Bitmap GetStandardArrowIcon()
		{
			if (ArrowIcon == null)
			{

				ArrowIcon = new Bitmap(new Bitmap(ArrowLocation), new Size(16, 16));
			}
			return ArrowIcon;
		}


		/**
		 * Draws a ship icon with the given angle as course
		 * */
		[MethodImpl(MethodImplOptions.Synchronized)]
		public static void DrawArrow(
		 Graphics g,
		 IMultiPoint point,
		 Brush b,
		 float size,
		 PointF offset,
		 Map map)
		{


			Bitmap bmap = GetStandardArrowIcon();

			if (point == null)
				return;
			if (point.Length < 2) {
				PointF pointFC = Transform.WorldtoMap(point[0].Coordinate, map);
				g.FillEllipse(Brushes.Red, new Rectangle( (int)pointFC.X - 3, (int)pointFC.Y - 3, 6, 6));
				return;
			}
			PointF pointF = Transform.WorldtoMap(point[0].Coordinate, map);
			double diffCoordX = (((IPoint)point[1]).X - ((IPoint)point[0]).X);
			double diffCoordY = (((IPoint)point[1]).Y - ((IPoint)point[0]).Y);

			double length = Math.Sqrt(diffCoordX * diffCoordX + diffCoordY * diffCoordY);

			diffCoordX /= length;
			diffCoordY /= length;

			float width = size;
			float height = size;

			double angle = Math.Atan2(diffCoordY, diffCoordX);
			double adeg = angle * (180.0f / Math.PI);


			g.TranslateTransform((float)pointF.X, (float)pointF.Y);
			g.RotateTransform(-(int)((angle - (Math.PI / 2.0f)) * (180.0f / Math.PI)));
			g.TranslateTransform(-((float)pointF.X), -((float)pointF.Y));
			g.DrawImage(bmap, new RectangleF((float)pointF.X - size / 2.0f, (float)pointF.Y - size / 2.0f, size, size));
			g.ResetTransform();
		}

	}
}
