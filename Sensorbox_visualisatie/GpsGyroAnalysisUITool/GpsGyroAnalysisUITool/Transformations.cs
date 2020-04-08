﻿using GeoAPI.CoordinateSystems;
using GeoAPI.CoordinateSystems.Transformations;
using ProjNet.CoordinateSystems;
using ProjNet.CoordinateSystems.Transformations;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DavisBridgeTool3.Entities
{

	/**
	 * <summary> helper class that contains multiple usefull map projections</summary>
	 * 
	 * */
	public class Transformations
	{

		// Wgs84 to Google Mercator Coordinate Transformation
		private static ICoordinateTransformation _wgs84ToGoogle;
		public static ICoordinateTransformation Wgs84ToGoogle
		{
			get
			{
				if (_wgs84ToGoogle == null)
				{
					var csFac = new CoordinateSystemFactory();
					var ctFac = new CoordinateTransformationFactory();

					var wgs84 = csFac.CreateGeographicCoordinateSystem(
					  "WGS 84", AngularUnit.Degrees, HorizontalDatum.WGS84, PrimeMeridian.Greenwich,
					  new AxisInfo("north", AxisOrientationEnum.North), new AxisInfo("east", AxisOrientationEnum.East));

					var parameters = new List<ProjectionParameter>();
					parameters.Add(new ProjectionParameter("semi_major", 6378137.0));
					parameters.Add(new ProjectionParameter("semi_minor", 6378137.0));
					parameters.Add(new ProjectionParameter("latitude_of_origin", 0.0));
					parameters.Add(new ProjectionParameter("central_meridian", 0.0));
					parameters.Add(new ProjectionParameter("scale_factor", 1.0));
					parameters.Add(new ProjectionParameter("false_easting", 0.0));
					parameters.Add(new ProjectionParameter("false_northing", 0.0));

					var projection = csFac.CreateProjection("Google Mercator", "mercator_1sp", parameters);
					var epsg900913 = csFac.CreateProjectedCoordinateSystem(
					  "Google Mercator", wgs84, projection, LinearUnit.Metre, new AxisInfo("East", AxisOrientationEnum.East),
					  new AxisInfo("North", AxisOrientationEnum.North));

					((CoordinateSystem)epsg900913).DefaultEnvelope = new[] { -20037508.342789, -20037508.342789, 20037508.342789, 20037508.342789 };

					_wgs84ToGoogle = ctFac.CreateFromCoordinateSystems(wgs84, epsg900913);
				}
				return _wgs84ToGoogle;
			}
		}
	}
}
