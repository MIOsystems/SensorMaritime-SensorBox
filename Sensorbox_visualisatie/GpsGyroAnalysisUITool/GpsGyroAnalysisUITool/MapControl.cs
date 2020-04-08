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
using BruTile.Web;
using SharpMap.Layers;
using GeoAPI.Geometries;
using DavisBridgeTool3.Entities;
using System.Collections.ObjectModel;
using SharpMap.Data.Providers;
using GeoAPI.CoordinateSystems.Transformations;

namespace GpsGyroAnalysisUITool
{
    public partial class MapControl : UserControl
    {

        private DataReceiver dataReceiver;
        private NMEAData data;
        private GoogleTileSource tileSource;
        private GeometryProvider boatLayerProvider;
        private IGeometryFactory gf;
        private Collection<IMultiPoint> boatLocationLayerCollection;
        public MapControl()
        {
            InitializeComponent();

            tileSource = new GoogleTileSource(GoogleMapType.GoogleMap);
            gf = GeoAPI.GeometryServiceProvider.Instance.CreateGeometryFactory();


            //add arrow layers


            ArrowLayer bc = new ArrowLayer("b");


            IMultiPoint boatLocationDelta = gf.CreateMultiPoint(new Coordinate[] { new Coordinate(4.0, 52.0), new Coordinate(4.1, 52.1) });
            bc.CoordinateTransformation = Transformations.Wgs84ToGoogle;
            boatLocationLayerCollection = new Collection<IMultiPoint>();
            boatLocationLayerCollection.Add(boatLocationDelta);
            boatLayerProvider = new GeometryProvider(boatLocationLayerCollection);


            bc.DataSource = boatLayerProvider;

            mapBox1.Map.Layers.Add(bc);





            TileAsyncLayer mapTileLayer = new TileAsyncLayer(tileSource, "o");
            mapBox1.Map.BackgroundLayer.Add(mapTileLayer);
            mapBox1.Map.MinimumZoom = 20;
            mapBox1.Map.MaximumZoom = 3000000;
            mapBox1.Map.Zoom = 3000000;
            mapBox1.ActiveTool = SharpMap.Forms.MapBox.Tools.Pan;

            SetStyle(ControlStyles.OptimizedDoubleBuffer, true);

            SetStyle(ControlStyles.AllPaintingInWmPaint, true);

            SetStyle(ControlStyles.UserPaint, true);
        }

        public void Load(DataReceiver  dataReceiver) {
            this.dataReceiver = dataReceiver;
            dataReceiver.gpsActions.Add((nmeaData) =>
            {
                data = new NMEAData(nmeaData);
                Coordinate currentGPSCoordinate = new Coordinate(nmeaData.latitude, nmeaData.longtitude);
                Coordinate unitCoord = new Coordinate(currentGPSCoordinate.X - 1.0 * Math.Cos(((Math.PI / 180) * (nmeaData.degreeAngle + 90))), currentGPSCoordinate.Y + 1.0 * Math.Sin((Math.PI / 180) * (nmeaData.degreeAngle + 90)));
                boatLayerProvider.Geometries[0] = gf.CreateMultiPoint(new Coordinate[] { currentGPSCoordinate, unitCoord });
                mapBox1.Map.Center = GeometryTransform.TransformCoordinate(currentGPSCoordinate, Transformations.Wgs84ToGoogle.MathTransform);
                mapBox1.Refresh();
            });
        }
    }
}
