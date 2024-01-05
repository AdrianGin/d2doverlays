using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;
using System.Xml.Linq;

namespace Overlay
{



    class OverlayViewModel : Data
    {

        // props
        private Color selectedColor;
        public Color SelectedColor
        {
            get { return selectedColor; }
            set { SetField(ref selectedColor, value); }
        }


        private double size;
        public double Size {
            get { return size; }
            set { SetField(ref size, value); }
        }

        private double stroke;
        public double Stroke
        {
            get { return stroke; }
            set { SetField(ref stroke, value); }
        }


        private int xoffset;
        public int XOffset
        {
            get { return xoffset; }
            set { SetField(ref xoffset, value); }
        }

        private int yoffset;
        public int YOffset
        {
            get { return yoffset; }
            set { SetField(ref yoffset, value); }
        }

    }
}
