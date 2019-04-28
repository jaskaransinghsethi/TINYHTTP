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
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        internal string serverAddress;
        internal int serverPort;
        internal string serverLink;

        internal Translator translator;

        internal Dictionary<string, string> attributes_;

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            translator = new Translator();
            ConnCont.servAddress.Text = "localhost";
            ConnCont.servPort.Text = "8080";
            serverAddress = ConnCont.servAddress.Text;
            serverPort = Convert.ToInt32(ConnCont.servPort.Text);
            serverLink = serverAddress + ":" + serverPort + "/";
            HTTPCont.servLink.Text = serverLink;
            attributes_ = new Dictionary<string, string>();
        }

        internal void hostChanged()
        {
            serverLink = serverAddress + ":" + serverPort + "/";
            HTTPCont.servLink.Text = serverLink;
        }

        private void ExitBtn_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown(99);
        }

        internal void processReply(List<String> response, int time)
        {
            DispView.msgBody.Clear();
            DispView.replyTime.Clear();

            int find = response[0].IndexOf(' ');
            int find1 = response[0].IndexOf("content");
            string statusCode;
            if (find1 != -1)
                statusCode = response[0].Substring(find, find1 - find);
            else
                statusCode = response[0].Substring(find);
            DispView.statusCode.Text = statusCode;
            DispView.replyTime.Text = "";

            if (response.Count > 1)
                DispView.msgBody.Text = response[1];

            DispView.replyTime.Text = time.ToString() + "ms";

            staturBar.Text = "Disconnected";
            attributes_.Clear();
        }

        private void Window_Unloaded(object sender, RoutedEventArgs e)
        {
            Environment.Exit(99);
        }
    }
}
