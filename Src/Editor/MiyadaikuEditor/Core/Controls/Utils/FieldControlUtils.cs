using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Input;

namespace Miyadaiku.Editor.Core.Controls.Utils
{
    internal class FieldControlUtils
    {

        static public void FormatFloatingPointInput(object sender, KeyboardFocusChangedEventArgs e)
        {
            TextBox textbox = sender as TextBox;

            string input = textbox.Text.ToLower();

            // Remove all invalid characters
            input = Regex.Replace(input, @"[^+\-e.0-9]", "");

            // Remove exceptions
            input = Regex.Replace(input, @"(?<=e.*)e", "");

            var mantisaExponentSplit = input.Split(new[] { 'e' }, 2);
            // Split the mantissa and exponent
            string mantissa = mantisaExponentSplit[0];
            string exponent = (mantisaExponentSplit.Length > 1 && mantisaExponentSplit[1] != "")
                ? "e" + mantisaExponentSplit[1]
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


            if (mantissa == "" || mantissa == "-" || mantissa == "+")
                mantissa = "0";

            if (exponent == "e-" || exponent == "e+")
                exponent = "e0";

            textbox.Text = mantissa + exponent;
        }

    }
}
