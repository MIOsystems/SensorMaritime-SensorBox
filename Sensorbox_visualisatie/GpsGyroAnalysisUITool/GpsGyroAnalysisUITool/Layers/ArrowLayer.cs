
//Custom image drawing code

using Common.Logging;
using DavisBridgeTool3.Entities;
using GeoAPI;
using GeoAPI.CoordinateSystems.Transformations;
using GeoAPI.Geometries;
using SharpMap.Data;
using SharpMap.Data.Providers;
using SharpMap.Rendering;
using SharpMap.Rendering.Thematics;
using SharpMap.Styles;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Drawing;
using System.Drawing.Drawing2D;

namespace SharpMap.Layers
{
	/// <summary>Class for ship layer properties</summary>
	[Serializable]
	public class ArrowLayer : Layer, ICanQueryLayer, ILayer
	{
		private static ILog logger = LogManager.GetLogger(typeof(ArrowLayer));
		private bool _isQueryEnabled = true;
		private bool _clippingEnabled;
		private IProvider _dataSource;
		private SmoothingMode _smoothingMode;
		private ITheme _theme;

		/// <summary>Initializes a new layer</summary>
		/// <param name="layername">Name of layer</param>
		public ArrowLayer(string layername)
		  : base((SharpMap.Styles.Style)new VectorStyle())
		{
			this.LayerName = layername;
			this.SmoothingMode = SmoothingMode.AntiAlias;
		}

		/// <summary>Initializes a new layer with a specified datasource</summary>
		/// <param name="layername">Name of layer</param>
		/// <param name="dataSource">Data source</param>
		public ArrowLayer(string layername, IProvider dataSource)
		  : this(layername)
		{
			this._dataSource = dataSource;
		}

		/// <summary>
		/// Gets or sets a Dictionary with themes suitable for this layer. A theme in the dictionary can be used for rendering be setting the Theme Property using a delegate function
		/// </summary>
		public Dictionary<string, ITheme> Themes { get; set; }

		/// <summary>
		/// Gets or sets thematic settings for the layer. Set to null to ignore thematics
		/// </summary>
		public ITheme Theme
		{
			get
			{
				return this._theme;
			}
			set
			{
				this._theme = value;
			}
		}

		/// <summary>
		/// Specifies whether polygons should be clipped prior to rendering
		/// </summary>
		/// <remarks>
		/// <para>Clipping will clip <see cref="T:GeoAPI.Geometries.IPolygon" /> and
		/// <see cref="T:GeoAPI.Geometries.IMultiPolygon" /> to the current view prior
		/// to rendering the object.</para>
		/// <para>Enabling clipping might improve rendering speed if you are rendering
		/// only small portions of very large objects.</para>
		/// </remarks>
		public bool ClippingEnabled
		{
			get
			{
				return this._clippingEnabled;
			}
			set
			{
				this._clippingEnabled = value;
			}
		}

		/// <summary>
		/// Render whether smoothing (antialiasing) is applied to lines and curves and the edges of filled areas
		/// </summary>
		public SmoothingMode SmoothingMode
		{
			get
			{
				return this._smoothingMode;
			}
			set
			{
				this._smoothingMode = value;
			}
		}

		/// <summary>Gets or sets the datasource</summary>
		public IProvider DataSource
		{
			get
			{
				return this._dataSource;
			}
			set
			{
				this._dataSource = value;
			}
		}

		/// <summary>Gets or sets the rendering style of the vector layer.</summary>
		public VectorStyle Style
		{
			get
			{
				return base.Style as VectorStyle;
			}
			set
			{
				this.Style = value;
			}
		}

		/// <summary>Returns the extent of the layer</summary>
		/// <returns>Bounding box corresponding to the extent of the features in the layer</returns>
		public override Envelope Envelope
		{
			get
			{
				if (this.DataSource == null)
					throw new ApplicationException("DataSource property not set on layer '" + this.LayerName + "'");
				Envelope extents;
				lock (this._dataSource)
				{
					bool isOpen = this.DataSource.IsOpen;
					if (!isOpen)
						this.DataSource.Open();
					extents = this.DataSource.GetExtents();
					if (!isOpen)
						this.DataSource.Close();
				}
				if (this.CoordinateTransformation != null)
					return GeometryTransform.TransformBox(extents, this.CoordinateTransformation.MathTransform);
				return extents;
			}
		}

		/// <summary>
		/// Gets or sets the SRID of this VectorLayer's data source
		/// </summary>
		public override int SRID
		{
			get
			{
				if (this.DataSource == null)
					throw new ApplicationException("DataSource property not set on layer '" + this.LayerName + "'");
				return this.DataSource.SRID;
			}
			set
			{
				this.DataSource.SRID = value;
			}
		}

		/// <summary>Disposes the object</summary>
		protected override void ReleaseManagedResources()
		{
			if (this.DataSource != null)
				this.DataSource.Dispose();
			base.ReleaseManagedResources();
		}

		/// <summary>Renders the layer to a graphics object</summary>
		/// <param name="g">Graphics object reference</param>
		/// <param name="map">Map which is rendered</param>
		public override void Render(Graphics g, Map map)
		{
			if (map.Center == null)
				throw new ApplicationException("Cannot render map. View center not specified");
			g.SmoothingMode = this.SmoothingMode;
			Envelope envelope = map.Envelope;
			if (this.CoordinateTransformation != null)
			{
				if (this.ReverseCoordinateTransformation != null)
				{
					envelope = GeometryTransform.TransformBox(envelope, this.ReverseCoordinateTransformation.MathTransform);
				}
				else
				{
					this.CoordinateTransformation.MathTransform.Invert();
					envelope = GeometryTransform.TransformBox(envelope, this.CoordinateTransformation.MathTransform);
					this.CoordinateTransformation.MathTransform.Invert();
				}
			}
			if (this.DataSource == null)
				throw new ApplicationException("DataSource property not set on layer '" + this.LayerName + "'");
			if (this.Theme != null)
				this.RenderInternal(g, map, envelope, this.Theme);
			else
				this.RenderInternal(g, map, envelope);
			base.Render(g, map);
		}

		/// <summary>
		/// Method to render this layer to the map, applying <paramref name="theme" />.
		/// </summary>
		/// <param name="g">The graphics object</param>
		/// <param name="map">The map object</param>
		/// <param name="envelope">The envelope to render</param>
		/// <param name="theme">The theme to apply</param>
		protected void RenderInternal(Graphics g, Map map, Envelope envelope, ITheme theme)
		{
			FeatureDataSet ds = new FeatureDataSet();
			lock (this._dataSource)
			{
				this.DataSource.Open();
				this.DataSource.ExecuteIntersectionQuery(envelope, ds);
				this.DataSource.Close();
			}
			foreach (FeatureDataTable table in (List<FeatureDataTable>)ds.Tables)
			{
				if (this.CoordinateTransformation != null)
				{
					for (int index = 0; index < table.Count; ++index)
						table[index].Geometry = GeometryTransform.TransformGeometry(table[index].Geometry, this.CoordinateTransformation.MathTransform, GeometryServiceProvider.Instance.CreateGeometryFactory((int)this.CoordinateTransformation.TargetCS.AuthorityCode));
				}
				if (this.Style.EnableOutline)
				{
					for (int index = 0; index < table.Count; ++index)
					{
						FeatureDataRow attribute = table[index];
						VectorStyle style = theme.GetStyle(attribute) as VectorStyle;
						if (style != null && style.Enabled && (style.EnableOutline && style.MinVisible <= map.Zoom) && map.Zoom <= style.MaxVisible)
						{
							VectorStyle vectorStyle;
							using (vectorStyle = style.Clone())
							{
								if (vectorStyle != null)
								{
									if (attribute.Geometry is ILineString)
										VectorRenderer.DrawLineString(g, attribute.Geometry as ILineString, vectorStyle.Outline, map, vectorStyle.LineOffset);
									else if (attribute.Geometry is IMultiLineString)
										VectorRenderer.DrawMultiLineString(g, attribute.Geometry as IMultiLineString, vectorStyle.Outline, map, vectorStyle.LineOffset);
								}
							}
						}
					}
				}
				for (int index = 0; index < table.Count; ++index)
				{
					FeatureDataRow attribute = table[index];
					IStyle style1 = theme.GetStyle(attribute);
					if (style1 != null && style1.Enabled && (style1.MinVisible <= map.Zoom && map.Zoom <= style1.MaxVisible))
					{
						IStyle[] stylesToRender = this.GetStylesToRender(style1);
						if (stylesToRender == null)
							return;
						foreach (IStyle style2 in stylesToRender)
						{
							if (style2 is VectorStyle && style2.Enabled)
							{
								using (VectorStyle style3 = (style2 as VectorStyle).Clone())
								{
									if (style3 != null)
										this.RenderGeometry(g, map, attribute.Geometry, style3);
								}
							}
						}
					}
				}
			}
		}

		/// <summary>
		/// Method to render this layer to the map, applying <see cref="P:SharpMap.Layers.VectorLayer.Style" />.
		/// </summary>
		/// <param name="g">The graphics object</param>
		/// <param name="map">The map object</param>
		/// <param name="envelope">The envelope to render</param>
		protected void RenderInternal(Graphics g, Map map, Envelope envelope)
		{
			if (!this.Style.Enabled)
				return;
			IStyle[] stylesToRender = this.GetStylesToRender((IStyle)this.Style);
			if (stylesToRender == null)
				return;
			foreach (IStyle style1 in stylesToRender)
			{
				if (style1 is VectorStyle && style1.Enabled)
				{
					using (VectorStyle style2 = (style1 as VectorStyle).Clone())
					{
						if (style2 != null)
						{
							Collection<IGeometry> geometriesInView;
							lock (this._dataSource)
							{
								bool isOpen = this.DataSource.IsOpen;
								if (!isOpen)
									this.DataSource.Open();
								geometriesInView = this.DataSource.GetGeometriesInView(envelope);
								if (ArrowLayer.logger.IsDebugEnabled)
									ArrowLayer.logger.DebugFormat("Layer {0}, NumGeometries {1}", (object)this.LayerName, (object)geometriesInView.Count);
								if (!isOpen)
									this.DataSource.Close();
							}
							if (this.CoordinateTransformation != null)
							{
								for (int index = 0; index < geometriesInView.Count; ++index)
									geometriesInView[index] = GeometryTransform.TransformGeometry(geometriesInView[index], this.CoordinateTransformation.MathTransform, GeometryServiceProvider.Instance.CreateGeometryFactory((int)this.CoordinateTransformation.TargetCS.AuthorityCode));
							}
							if (style2.LineSymbolizer != null)
								style2.LineSymbolizer.Begin(g, map, geometriesInView.Count);
							else if (style2.EnableOutline)
							{
								foreach (IGeometry geometry in geometriesInView)
								{
									if (geometry != null)
									{
										if (geometry is ILineString)
											VectorRenderer.DrawLineString(g, geometry as ILineString, style2.Outline, map, style2.LineOffset);
										else if (geometry is IMultiLineString)
											VectorRenderer.DrawMultiLineString(g, geometry as IMultiLineString, style2.Outline, map, style2.LineOffset);
									}
								}
							}
							for (int index = 0; index < geometriesInView.Count; ++index)
							{
								if (geometriesInView[index] != null)
									this.RenderGeometry(g, map, geometriesInView[index], style2);
							}
							if (style2.LineSymbolizer != null)
							{
								style2.LineSymbolizer.Symbolize(g, map);
								style2.LineSymbolizer.End(g, map);
							}
						}
					}
				}
			}
		}

		/// <summary>Unpacks styles to render (can be nested group-styles)</summary>
		/// <param name="style"></param>
		/// <returns></returns>
		private IStyle[] GetStylesToRender(IStyle style)
		{
			IStyle[] styleArray = (IStyle[])null;
			if (style is GroupStyle)
			{
				GroupStyle groupStyle = style as GroupStyle;
				List<IStyle> styleList = new List<IStyle>();
				for (int index = 0; index < groupStyle.Count; ++index)
					styleList.AddRange((IEnumerable<IStyle>)this.GetStylesToRender((IStyle)groupStyle[index]));
				styleArray = styleList.ToArray();
			}
			else if (style is VectorStyle)
				styleArray = new IStyle[1] { style };
			return styleArray;
		}

		/// <summary>
		/// Method to render <paramref name="feature" /> using <paramref name="style" />
		/// </summary>
		/// <param name="g">The graphics object</param>
		/// <param name="map">The map</param>
		/// <param name="feature">The feature's geometry</param>
		/// <param name="style">The style to apply</param>
		protected void RenderGeometry(Graphics g, Map map, IGeometry feature, VectorStyle style)
		{

			if (feature == null)
				return;
			ImageMapRenderer.DrawArrow(g, (IMultiPoint)feature, style.PointColor, 16, style.SymbolOffset, map);
		}

		/// <summary>
		/// Returns the data associated with all the geometries that are intersected by 'geom'
		/// </summary>
		/// <param name="box">Geometry to intersect with</param>
		/// <param name="ds">FeatureDataSet to fill data into</param>
		public void ExecuteIntersectionQuery(Envelope box, FeatureDataSet ds)
		{
			if (this.CoordinateTransformation != null)
			{
				if (this.ReverseCoordinateTransformation != null)
				{
					box = GeometryTransform.TransformBox(box, this.ReverseCoordinateTransformation.MathTransform);
				}
				else
				{
					this.CoordinateTransformation.MathTransform.Invert();
					box = GeometryTransform.TransformBox(box, this.CoordinateTransformation.MathTransform);
					this.CoordinateTransformation.MathTransform.Invert();
				}
			}
			lock (this._dataSource)
			{
				this._dataSource.Open();
				int count = ds.Tables.Count;
				this._dataSource.ExecuteIntersectionQuery(box, ds);
				if (ds.Tables.Count > count)
					ds.Tables[ds.Tables.Count - 1].TableName = this.LayerName;
				this._dataSource.Close();
			}
		}

		/// <summary>
		/// Returns the data associated with all the geometries that are intersected by 'geom'
		/// </summary>
		/// <param name="geometry">Geometry to intersect with</param>
		/// <param name="ds">FeatureDataSet to fill data into</param>
		public void ExecuteIntersectionQuery(IGeometry geometry, FeatureDataSet ds)
		{
			if (this.CoordinateTransformation != null)
			{
				if (this.ReverseCoordinateTransformation != null)
				{
					geometry = GeometryTransform.TransformGeometry(geometry, this.ReverseCoordinateTransformation.MathTransform, GeometryServiceProvider.Instance.CreateGeometryFactory((int)this.CoordinateTransformation.TargetCS.AuthorityCode));
				}
				else
				{
					this.CoordinateTransformation.MathTransform.Invert();
					geometry = GeometryTransform.TransformGeometry(geometry, this.CoordinateTransformation.MathTransform, GeometryServiceProvider.Instance.CreateGeometryFactory((int)this.CoordinateTransformation.SourceCS.AuthorityCode));
					this.CoordinateTransformation.MathTransform.Invert();
				}
			}
			lock (this._dataSource)
			{
				this._dataSource.Open();
				int count = ds.Tables.Count;
				this._dataSource.ExecuteIntersectionQuery(geometry, ds);
				if (ds.Tables.Count > count)
					ds.Tables[ds.Tables.Count - 1].TableName = this.LayerName;
				this._dataSource.Close();
			}
		}

		/// <summary>
		/// Whether the layer is queryable when used in a SharpMap.Web.Wms.WmsServer, ExecuteIntersectionQuery() will be possible in all other situations when set to FALSE
		/// </summary>
		public bool IsQueryEnabled
		{
			get
			{
				return this._isQueryEnabled;
			}
			set
			{
				this._isQueryEnabled = value;
			}
		}
	}
}
