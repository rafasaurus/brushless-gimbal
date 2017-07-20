using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.Threading;

namespace COM
{
   
    public partial class Form1 : Form
    {

       static void RECEIVE_DATA()
        {
            Form1 ob = new Form1();
            try
            {
               
                ob.textBox1.Text = ob.serialPort1.ReadLine();
            }
            catch (TimeoutException)
            {
                ob.textBox1.Text = "Time Out Exception";
            }
        }

        Thread T = new Thread(RECEIVE_DATA);
        
        
        public Form1()
        {
            InitializeComponent();
            getAvailablePortNames();
        }
        void getAvailablePortNames()
        {
            String[] ports=SerialPort.GetPortNames();
            comboBox1.Items.AddRange(ports);

        }
        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {//receive button execution 
            try
            {
                textBox1.Text = serialPort1.ReadLine();
            }
            catch(TimeoutException)
            {
                textBox1.Text = "Time Out Exception";
            }
        }

    

        private void button2_Click(object sender, EventArgs e)
        {
            try
            {
                if (comboBox1.Text == "")
                {
                    textBox1.Text = "select port";
                }
                else
                {
                    serialPort1.PortName = comboBox1.Text;
                    serialPort1.BaudRate = Convert.ToInt32(comboBox2.Text);
                    serialPort1.Open();
                    progressBar1.Value = 100;
                    button1.Enabled = true; //receive
                    button2.Enabled = false;//open port
                    textBox1.Enabled = true;
                    button3.Enabled = true; //close
                  //  groupBox1.Enabled = true;

                }
            }
            catch(UnauthorizedAccessException)
            {
                textBox1.Text = "unathorized access";
            }
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            
        }

        private void button3_Click(object sender, EventArgs e)
        {
            serialPort1.Close();
            progressBar1.Value = 0;
            button1.Enabled = false; //receive
            button2.Enabled = true;//open port
            textBox1.Enabled = false;
            button3.Enabled = false;

        }

        private void textBox1_TextChanged_1(object sender, EventArgs e)
        {

        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            T.Start();
        }
    }
}
