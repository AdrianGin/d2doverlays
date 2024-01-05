using Overlay.Properties;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Overlay
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {

        [DllImport("d2doverlay.dll")]
        public static extern void InitOverlay();
        [DllImport("d2doverlay.dll")]
        public static extern void KillOverlay();

        [DllImport("d2doverlay.dll")]
        public static extern void SetColour(int color);

        [DllImport("d2doverlay.dll")]
        public static extern void SetSize(float size, float stroke);
        [DllImport("d2doverlay.dll")]
        public static extern void SetOffset(int x, int y);

        [DllImport("d2doverlay.dll")]
        public static extern void Update();


        OverlayViewModel viewModel;

        public MainWindow()
        {
            viewModel = new OverlayViewModel();
            DataContext = viewModel;

            InitializeComponent();

            InitOverlay();

            Properties.Settings.Default.Reload();

            viewModel.Size = (float)Properties.Settings.Default.size;
            viewModel.Stroke = (float)Properties.Settings.Default.stroke;
            viewModel.XOffset = Properties.Settings.Default.XOffset;
            viewModel.YOffset = Properties.Settings.Default.YOffset;

            int color = Properties.Settings.Default.colour;
            viewModel.SelectedColor = System.Windows.Media.Color.FromArgb((byte)((color & 0xFF000000) >> 24), (byte)((color & 0x00FF0000) >> 16), (byte)((color & 0x0000FF00) >> 8), (byte)(color & 0xFF));

            viewModel.PropertyChanged += ViewModel_PropertyChanged;
            Closing += MainWindow_Closing;

            UpdateDisplay();

        }

        private void MainWindow_Closing(object? sender, System.ComponentModel.CancelEventArgs e)
        {
            Properties.Settings.Default.Save();
            Properties.Settings.Default.Reload();
            KillOverlay();
        }

        private void ViewModel_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            switch(e.PropertyName)
            {
                case "SelectedColor":
                    int color = viewModel.SelectedColor.A << 24 | viewModel.SelectedColor.R << 16 | viewModel.SelectedColor.G << 8 | viewModel.SelectedColor.B;
                    Properties.Settings.Default.colour = color;
                    break;

                case "Stroke":
                    float stroke = (float)viewModel.Stroke;
                    Properties.Settings.Default.stroke = stroke;
                    break;

                case "Size":
                    float size = (float)viewModel.Size;
                    Properties.Settings.Default.size = size;
                    break;

                case "XOffset":
                    int x = viewModel.XOffset;
                    Properties.Settings.Default.XOffset = x;
                    break;

                case "YOffset":
                    int y = viewModel.YOffset;
                    Properties.Settings.Default.YOffset = y;
                    break;
            }

            UpdateDisplay();
        }

        private void UpdateDisplay()
        {
            SetSize( (float)viewModel.Size, (float)viewModel.Stroke);
            SetOffset(viewModel.XOffset, viewModel.YOffset);
            int color = viewModel.SelectedColor.A << 24 | viewModel.SelectedColor.R << 16 | viewModel.SelectedColor.G << 8 | viewModel.SelectedColor.B;
            SetColour(color);

        }

    }
}
