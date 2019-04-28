using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace WPFClient
{
    /// <summary>
    /// Interaction logic for HTTPControl.xaml
    /// </summary>
    public partial class HTTPControl : UserControl
    {
        public HTTPControl()
        {
            InitializeComponent();
        }

        private void Attribute_Click(object sender, RoutedEventArgs e)
        {
            RowDefinition r1 = new RowDefinition();
            r1.Height = new GridLength(1, GridUnitType.Star);
            attrbGrid.RowDefinitions.Add(r1);
            var count = attrbGrid.RowDefinitions.Count();
            TextBox key = new TextBox
            {
                Text = "",
                Name = "K" + (count).ToString()
            };
            key.KeyDown += TKey_KeyDown;
            TextBox value = new TextBox
            {
                Text = "",
                Name = "V" + (count).ToString()
            };
            value.KeyDown += TValue_KeyDown;
            Grid.SetColumn(key, 0);
            Grid.SetRow(key, count-1);
            Grid.SetColumn(value, 1);
            Grid.SetRow(value, count-1);
            attrbGrid.Children.Add(key);
            attrbGrid.Children.Add(value);
        }

        private void TKey_KeyDown(object sender, KeyEventArgs e)
        {
            TextBox name = sender as TextBox;

            if (e.Key == Key.Enter)
            {
                MainWindow win = (MainWindow)Window.GetWindow(this);

                var key = name.Text;

                if (!win.attributes_.ContainsKey(key))
                    win.attributes_.Add(key, "");
            }
        }

        private void TValue_KeyDown(object sender, KeyEventArgs e)
        {
            TextBox name = sender as TextBox;

            if (e.Key == Key.Enter)
            {
                MainWindow win = (MainWindow)Window.GetWindow(this);

                var value = name.Text;

                var temp = name.Name;
                var k = temp[1];
                var keyName = "K" + k;

                foreach (UIElement uie in attrbGrid.Children)
                {
                    if(((System.Windows.FrameworkElement)uie).Name == keyName)
                    {
                        var keyV = ((System.Windows.Controls.TextBox)uie).Text;


                        if (!win.attributes_.ContainsKey(keyV))
                            win.attributes_.Add(keyV, value);
                        else
                            win.attributes_[keyV] = value;
                    }
                }
            }
        }

        private void Send_Click(object sender, RoutedEventArgs e)
        {
            MainWindow win = (MainWindow)Window.GetWindow(this);
            if (win.staturBar.Text != "Connected")
                return;

            List<String> msg = new List<String>();
            var methodType_ = (methodType.SelectedValue as ComboBoxItem).Content as string;
            var url = servLink.Text;
            var find = url.IndexOf('/');
            var fileSpec = url.Substring(find + 1);
            if (fileSpec == "")
                msg.Add(methodType_ + " ");
            else
                msg.Add(methodType_ + " " + fileSpec);
            foreach(KeyValuePair<string, string> entry in win.attributes_)
            {
                string temp = entry.Key + ":" + entry.Value;
                msg.Add(temp);
            }

            List<String> response = new List<string>();
            Stopwatch stopWatch = new Stopwatch();
            stopWatch.Start();
            response = win.translator.postMessage(msg);
            stopWatch.Stop();
            TimeSpan ts = stopWatch.Elapsed;
            int time = ts.Milliseconds;
            win.processReply(response, time);
            win.tabC.SelectedIndex = 2;
        }

        private void MsgBody_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                MainWindow win = (MainWindow)Window.GetWindow(this);
                if (msgBody.Text == "")
                {
                    win.attributes_.Remove("body");
                    win.attributes_.Remove("content-length");
                }
                else
                {
                    win.attributes_["body"] = msgBody.Text;
                    win.attributes_["content-length"] = msgBody.Text.Length.ToString();
                }
            }
        }
    }
}
