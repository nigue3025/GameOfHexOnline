using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Collections.Concurrent;

namespace HexGameUiForm
{
    public partial class Form1 : Form
    {

        HexChessBoard hexChessBoard; //棋盤資料儲存與UI管控
        public bool MyTurn = false;  //確認是否輪到使用者
        int MyPlayerSeqNo; //1:Blue,2:Red
        int[] IsOccupiedPlace; //管理棋盤是否已被佔據
        string playerName;

        ConcurrentQueue<string> RcvMsgQueue = new ConcurrentQueue<string>(); //管控server傳來的訊息
        ConcurrentQueue<string> SendMsgQueue = new ConcurrentQueue<string>(); //管控送往server的訊息



        int port = 0;
        public int Port
        {
            get
            {
                return port;
            }
        }

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            GameCtrlCenter(); //接收與處理server端訊息
        }
        private void panel_board_Paint(object sender, PaintEventArgs e)
        {
            if (hexChessBoard != null)
                hexChessBoard.paintPlacement(e.Graphics, e.ClipRectangle);  //重繪棋盤    
        }

        void initHexboard() //初始化棋盤
        {
            var graph = panel_board.CreateGraphics();
            Rectangle bound = new Rectangle(panel_board.Left, panel_board.Top, panel_board.Width, panel_board.Height);
            hexChessBoard = new HexChessBoard(graph, bound);
            IsOccupiedPlace = new int[hexChessBoard.size * hexChessBoard.size];
            for (int i = 0; i < IsOccupiedPlace.Length; i++)
                IsOccupiedPlace[i] = 0;
        }


        void processMsg(string str)
        {
            var msgs = str.Split('|'); //每個訊息以 "|" 作分隔
            foreach (var msg in msgs)
            {
                var msgDetail = msg.Split(','); //每個訊息內容欄位以 "," 作分隔
                switch (msgDetail[0])
                {
                    case "0": //登入訊息
                        break;
                    case "1": //遊戲開始,server通知順序
                        if (msgDetail[2] == "player1")
                        {
                            MyPlayerSeqNo = 1;
                            label_hint.BeginInvoke(new Action(() =>
                            {
                                label_hint.Text = "系統指定為藍方\r\n由左連到右獲勝";
                                label_hint.BackColor = Color.Blue;
                            }));
                        }
                        else
                        {
                            MyPlayerSeqNo = 2;

                            label_hint.BeginInvoke(new Action(() =>
                            {
                                label_hint.Text = "系統指定為紅方\r\n由上連到下獲勝";
                                label_hint.BackColor = Color.Red;
                            }));
                        }
                        MessageBox.Show($"{playerName},{msgDetail[1]}");
                        break;
                    case "2": //Server通知輪到本玩家,並更新棋盤
                        MyTurn = true;
                        for (int i = 0; i < IsOccupiedPlace.Length; ++i)
                        {
                            var moveInfoFromServer = int.Parse(msgDetail[2][i].ToString());
                            if (IsOccupiedPlace[i]!=moveInfoFromServer)
                            {
                                if (moveInfoFromServer == 1)
                                    hexChessBoard.Placements[i] = new BlueChess(hexChessBoard.Placements[i].bound);
                                else if(moveInfoFromServer ==2)
                                    hexChessBoard.Placements[i] = new RedChess(hexChessBoard.Placements[i].bound);
                             
                                hexChessBoard.paintPlacement(panel_board.CreateGraphics(), panel_board.ClientRectangle);
                            }
                        } 
                        MessageBox.Show($"{playerName},{msgDetail[1]}");
                        break;
                    case "3": //通知玩家輸贏
                        MyTurn = false;
                        MessageBox.Show($"{playerName}, {msgDetail[1]}");
                        break;
                    case "":
                        break;
                    default:
                        MessageBox.Show(msgDetail[1]);
                        break;
                }
            }
        }

        void GameCtrlCenter()
        {
            Task.Run(new Action(() =>
            {
                while (true)
                {
                    string msg;
                    if (RcvMsgQueue.TryDequeue(out msg))
                    {
                        processMsg(msg);
                        rtxbx_msg.BeginInvoke(new Action(() =>
                        {
                            rtxbx_msg.Text=$"{rtxbx_msg.Text}\r\n{msg}";
                        }));
                    }
                    System.Threading.SpinWait.SpinUntil(() => false, 1);

                }
            }));

        }
        void SendMsgLoop(NetworkStream stream)
        {
            Task.Run(() =>
            {
                while (true)
                {
                    string msg;
                    while (SendMsgQueue.TryDequeue(out msg))
                    {
                        byte[] bte = Encoding.ASCII.GetBytes(msg);
                        stream.Write(bte, 0, bte.Length);
                    }
                    System.Threading.SpinWait.SpinUntil(() => false, 1);
                }
            });
        }

        bool IsMsgLoopActivated = false;
        void RcvMsgLoop( NetworkStream stream)
        {
            if (!IsMsgLoopActivated) //只需啟動一次
            {
                IsMsgLoopActivated = true;
                Task.Run(new Action(() =>
               {
                   byte[] inData = new byte[1024];
                   while (true)
                   {
                       string responseData = String.Empty;
                       int sz = stream.Read(inData, 0, inData.Length);
                       responseData = System.Text.Encoding.ASCII.GetString(inData, 0, sz);

                       if (sz > 0)
                           RcvMsgQueue.Enqueue(responseData);
                       else
                       {
                           RcvMsgQueue.Enqueue("-1,Connection Loss");
                           break;
                       }
                   }
               }
               ));
            }
        }

        private async void btn_connect_Click(object sender, EventArgs e)
        {
            if (txtbx_name.Text == "")
            {
                rtxbx_msg.Text = $"Error!input your name\n{rtxbx_msg.Text}";
                return;
            }

            playerName = txtbx_name.Text;
            //string name = txtbx_name.Text;
            string message = $"0,{playerName}|";
            try
            {
                NetworkStream stream=null;
                byte[] inData = new byte[1024];
                //開始對局
                await Task.Run(new Action(() =>
                {
                    int port = int.Parse(txtbx_port.Text);
                    TcpClient client = new TcpClient(txtbx_ip.Text, port);
                    Byte[] data = System.Text.Encoding.ASCII.GetBytes(message);
                    stream = client.GetStream();
                    stream.Write(data, 0, data.Length);
                }
                    ));
                RcvMsgLoop(stream);
                SendMsgLoop(stream);
            }
            catch(Exception ex)
            {
                Console.WriteLine(ex.ToString());
            }
       
        }

        private void button1_Click(object sender, EventArgs e)
        {
            initHexboard();
        }

        private void panel_board_Click(object sender, EventArgs e)
        {

        }
        string NumToCordinate(int num)
        {
            return $"({num / hexChessBoard.size},{num % hexChessBoard.size})";
        }
        private void panel_board_MouseUp(object sender, MouseEventArgs e)
        {
            if (hexChessBoard != null)
            {
                if (MyTurn)
                {
                    for (int i = 0; i < hexChessBoard.Placements.Count; ++i)
                    {
                        if (hexChessBoard.Placements[i].contain(e.Location))
                        {
                            if(IsOccupiedPlace[i]!=0)
                            {
                                rtxbx_msg.Text = $"Invalid Move\r\n{rtxbx_msg.Text}";
                                break;
                            }
                            if(MyPlayerSeqNo==1)
                                hexChessBoard.Placements[i] = new BlueChess(hexChessBoard.Placements[i].bound);
                            else
                                hexChessBoard.Placements[i] = new RedChess(hexChessBoard.Placements[i].bound);

                            SendMsgQueue.Enqueue(NumToCordinate(i));
                            MyTurn = false;
                        }
                    }
                    hexChessBoard.paintPlacement(panel_board.CreateGraphics(), panel_board.ClientRectangle);
                }
                else
                    rtxbx_msg.Text = $"Not yet+\r\n{rtxbx_msg.Text}";
            }
        }

        private void btn_init_Click(object sender, EventArgs e)
        {
            initHexboard();
        }

        private void txtbx_name_TextChanged(object sender, EventArgs e)
        {
      
        }

        private void txtbx_port_TextChanged(object sender, EventArgs e)
        {
            if(!int.TryParse(txtbx_port.Text,out port))
            {
                txtbx_port.Text ="0";
            }


        }
    }
}
