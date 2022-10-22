using System.Windows.Controls;
using System.Text.RegularExpressions;
using System.Windows;
using System.Windows.Input;
namespace Miyadaiku.EditorCore.Controls.Views
{
    /// <summary>
    /// Interaction logic for Vector3
    /// </summary>
    public partial class Vector3 : UserControl
    {
        public static void FloatingPointInputValidator(object sender, KeyboardFocusChangedEventArgs e)
        {
            TextBox textbox = sender as TextBox;

            string input = textbox.Text.ToLower();
            // Only valid characters in floating point numbers are
            // +/-
            // .
            // 0-9
            // e
            // Remove all invalid characters
            input = Regex.Replace(input, @"[^+\-e.0-9]", "");

            // There can only be a single 'e' in the expression. Remove all but the first one
            input = Regex.Replace(input, @"(?<=e.*)e", "");

            var mantisaExpSplit = input.Split(new[] { 'e' }, 2);
            // Split the mantissa and exponent part of the string
            string mantissa = mantisaExpSplit[0];
            string exponent = (mantisaExpSplit.Length > 1 && mantisaExpSplit[1] != "")
                ? "e" + mantisaExpSplit[1]
                : "";

            // Remove '.'s from exponent
            exponent = Regex.Replace(exponent, @"\.", "");

            // +/- can only immediately follow e (e+10, e-10). Remove all other occurances of +/-
            exponent = Regex.Replace(exponent, @"(?<!e)[+-]", "");


            // Remove 'e's from mantissa
            mantissa = Regex.Replace(mantissa, @"e", "");

            // There can only be a single '.' in the expression. Remove all but the first one
            mantissa = Regex.Replace(mantissa, @"(?<=\..*)\.", "");

            // +/- can only exist at the start of the string. Remove all other occurances of +/-
            mantissa = Regex.Replace(mantissa, @"(?!^-)-", "");


            // Known bugs: "-", "+", "12e+" "12e-"
            // Needs to handle no-digit checks better
            // The following fixes above bugs, but (TODO) need to come back and refactor this better
            if (mantissa == "" || mantissa == "-" || mantissa == "+")
                mantissa = "0";

            if (exponent == "e-" || exponent == "e+")
                exponent = "e0";

            textbox.Text = mantissa + exponent;
        }

        public Vector3()
        {
            InitializeComponent();

            // register float value validator
            TextBoxX.LostKeyboardFocus += FloatingPointInputValidator;
            TextBoxY.LostKeyboardFocus += FloatingPointInputValidator;
            TextBoxZ.LostKeyboardFocus += FloatingPointInputValidator;

            // initial values
            TextBoxX.Text = "0";
            TextBoxY.Text = "0";
            TextBoxZ.Text = "0";

            // mouse events
            TextBoxX.PreviewMouseDown += TextBox_MouseDown;
            TextBoxY.PreviewMouseDown += TextBox_MouseDown;
            TextBoxZ.PreviewMouseDown += TextBox_MouseDown;

            TextBoxX.PreviewMouseUp += TextBox_MouseUp;
            TextBoxY.PreviewMouseUp += TextBox_MouseUp;
            TextBoxZ.PreviewMouseUp += TextBox_MouseUp;

            TextBoxX.PreviewMouseMove += TextBoxX_MouseMove;
            TextBoxY.PreviewMouseMove += TextBoxX_MouseMove;
            TextBoxZ.PreviewMouseMove += TextBoxX_MouseMove;
        }
        private void NumberValidationTextBox(object sender, TextCompositionEventArgs e)
        {
            bool parseSucceed = false;
            parseSucceed = float.TryParse(e.Text, out _) && (e.Text != string.Empty);

            e.Handled = parseSucceed;
        }

        

        public string TextX
        {
            get; set;
        }
        public string TextY
        {
            get; set;
        }
        public string TextZ
        {
            get; set;
        }

        bool isDragging = false;
        Point lastPoint;
        Point dragStartPoint;
        object editingSender = null;

        private void TextBoxX_MouseMove(object sender, MouseEventArgs e)
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
