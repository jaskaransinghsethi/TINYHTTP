using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace WPFClient
{
    /// <summary>
    /// Interaction logic for ConnControl.xaml
    /// </summary>
    public partial class ConnControl : UserControl
    {
        public ConnControl()
        {
            InitializeComponent();
        }

        private void Connect_Click(object sender, RoutedEventArgs e)
        {
            MainWindow win = (MainWindow)Window.GetWindow(this);
            bool connect = win.translator.connectServer(win.serverAddress, win.serverPort);
            if (connect)
            {
                win.staturBar.Text = "Connected";
                win.tabC.SelectedIndex = 1;
            }
            else
                win.staturBar.Text = "Failed to Connect";
        }

        private void ServAddress_KeyDown(object sender, KeyEventArgs e)
        {
            MainWindow win = (MainWindow)Window.GetWindow(this);

            if (e.Key == Key.Enter)
            {
                if (servAddress.Text == "")
                {
                    win.staturBar.Text = "Invalid Address";
                    string message = "Please input valid Address";
                    string title = "Address Null Exception";
                    System.Windows.Forms.MessageBoxButtons buttons = System.Windows.Forms.MessageBoxButtons.OK;
                    System.Windows.Forms.DialogResult result = System.Windows.Forms.MessageBox.Show(message, title, buttons);
                    if (result == System.Windows.Forms.DialogResult.OK)
                    {
                        win.tabC.SelectedIndex = 0;
                    }
                    return;
                }
                win.serverAddress = servAddress.Text;
                win.staturBar.Text = "Ready";
                win.hostChanged();
            }
        }

        private void ServPort_KeyDown(object sender, KeyEventArgs e)
        {
            MainWindow win = (MainWindow)Window.GetWindow(this);

            if (e.Key == Key.Enter)
            {
                if (servPort.Text == "")
                {
                    win.staturBar.Text = "Invalid Port";
                    string message = "Please input valid Port";
                    string title = "Address Null Exception";
                    System.Windows.Forms.MessageBoxButtons buttons = System.Windows.Forms.MessageBoxButtons.OK;
                    System.Windows.Forms.DialogResult result = System.Windows.Forms.MessageBox.Show(message, title, buttons);
                    if (result == System.Windows.Forms.DialogResult.OK)
                    {
                        win.tabC.SelectedIndex = 0;
                    }
                    return;
                }
                win.serverPort = Convert.ToInt32(servPort.Text);
                win.staturBar.Text = "Ready";
                win.hostChanged();
            }
        }
    }
}
