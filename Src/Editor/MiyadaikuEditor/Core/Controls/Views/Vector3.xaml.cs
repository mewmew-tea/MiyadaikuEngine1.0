using System.Windows.Controls;
using System.Text.RegularExpressions;
using System.Windows;
using System.Windows.Input;

using Miyadaiku.Editor.Core.Controls.Utils;

namespace Miyadaiku.Editor.Core.Controls.Views
{
    /// <summary>
    /// Interaction logic for Vector3
    /// </summary>
    public partial class Vector3 : UserControl
    {
        public Vector3()
        {
            InitializeComponent();

            // register float value validator
            TextBoxX.LostKeyboardFocus += FieldControlUtils.FormatFloatingPointInput;
            TextBoxY.LostKeyboardFocus += FieldControlUtils.FormatFloatingPointInput;
            TextBoxZ.LostKeyboardFocus += FieldControlUtils.FormatFloatingPointInput;

            // initial values
            //TextBoxX.Text = "0";
            //TextBoxY.Text = "0";
            //TextBoxZ.Text = "0";

            // mouse events
            TextBoxX.PreviewMouseDown += TextBox_MouseDown;
            TextBoxY.PreviewMouseDown += TextBox_MouseDown;
            TextBoxZ.PreviewMouseDown += TextBox_MouseDown;

            TextBoxX.PreviewMouseUp += TextBox_MouseUp;
            TextBoxY.PreviewMouseUp += TextBox_MouseUp;
            TextBoxZ.PreviewMouseUp += TextBox_MouseUp;

            TextBoxX.PreviewMouseMove += TextBox_MouseMove;
            TextBoxY.PreviewMouseMove += TextBox_MouseMove;
            TextBoxZ.PreviewMouseMove += TextBox_MouseMove;
        }
        //private void NumberValidationTextBox(object sender, TextCompositionEventArgs e)
        //{
        //    bool parseSucceed = false;
        //    parseSucceed = float.TryParse(e.Text, out _) && (e.Text != string.Empty);

        //    e.Handled = parseSucceed;
        //}

        bool isDragging = false;
        Point lastPoint;
        Point dragStartPoint;
        object editingSender = null;

        /// <summary>
        /// Event function for mouse move
        /// </summary>
        private void TextBox_MouseMove(object sender, MouseEventArgs e)
        {
            if (isDragging && editingSender == sender)
            {
                var txtBox = sender as TextBox;

                Point p = e.GetPosition(this);
                
                var dist = lastPoint.Y - p.Y;
                bool isPositive = dist > 0;

                lastPoint = p;

                var distFromStart = p.Y - dragStartPoint.Y;
                if (System.Math.Abs(distFromStart) < 10)
                {
                    return;
                }

                if (txtBox.Cursor != Cursors.ScrollNS)
                {
                    txtBox.Cursor = Cursors.SizeNS;
                }

                if (System.Math.Abs(dist) > 1)
                {
                    float step = 0.25f;

                    float f;
                    bool isFloat = float.TryParse(txtBox.Text, out f);

                    float value = (isFloat ? f : 0.0f) + (isPositive ? step : -step);
                    txtBox.Text = value.ToString();
                }

            }
        }

        /// <summary>
        /// Event function for mouse up
        /// </summary>
        private void TextBox_MouseUp(object sender, MouseButtonEventArgs e)
        {
            if (isDragging)
            {
                isDragging = false;
                var txtBox = sender as TextBox;
                txtBox.Cursor = Cursors.Arrow;

                editingSender = null;
            }
            
        }

        /// <summary>
        /// Event function for mouse down
        /// </summary>
        private void TextBox_MouseDown(object sender, MouseButtonEventArgs e)
        {

            if (!isDragging)
            {
                isDragging = true;
                editingSender = sender;
                dragStartPoint = e.GetPosition(this);
            }
        }
    }
}
