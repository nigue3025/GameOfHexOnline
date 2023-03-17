
namespace HexGameUiForm
{
    partial class Form1
    {
        /// <summary>
        /// 設計工具所需的變數。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清除任何使用中的資源。
        /// </summary>
        /// <param name="disposing">如果應該處置受控資源則為 true，否則為 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 設計工具產生的程式碼

        /// <summary>
        /// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改
        /// 這個方法的內容。
        /// </summary>
        private void InitializeComponent()
        {
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.btn_init = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.txtbx_name = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.txtbx_port = new System.Windows.Forms.TextBox();
            this.btn_connect = new System.Windows.Forms.Button();
            this.txtbx_ip = new System.Windows.Forms.TextBox();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.panel_board = new System.Windows.Forms.Panel();
            this.rtxbx_msg = new System.Windows.Forms.RichTextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label_hint = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).BeginInit();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.label_hint);
            this.splitContainer1.Panel1.Controls.Add(this.label4);
            this.splitContainer1.Panel1.Controls.Add(this.btn_init);
            this.splitContainer1.Panel1.Controls.Add(this.label3);
            this.splitContainer1.Panel1.Controls.Add(this.txtbx_name);
            this.splitContainer1.Panel1.Controls.Add(this.label2);
            this.splitContainer1.Panel1.Controls.Add(this.label1);
            this.splitContainer1.Panel1.Controls.Add(this.txtbx_port);
            this.splitContainer1.Panel1.Controls.Add(this.btn_connect);
            this.splitContainer1.Panel1.Controls.Add(this.txtbx_ip);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.splitContainer2);
            this.splitContainer1.Size = new System.Drawing.Size(1011, 449);
            this.splitContainer1.SplitterDistance = 343;
            this.splitContainer1.TabIndex = 0;
            // 
            // btn_init
            // 
            this.btn_init.Location = new System.Drawing.Point(15, 141);
            this.btn_init.Name = "btn_init";
            this.btn_init.Size = new System.Drawing.Size(289, 43);
            this.btn_init.TabIndex = 8;
            this.btn_init.Text = "initialize";
            this.btn_init.UseVisualStyleBackColor = true;
            this.btn_init.Click += new System.EventHandler(this.btn_init_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 85);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(98, 18);
            this.label3.TabIndex = 7;
            this.label3.Text = "Player Name";
            // 
            // txtbx_name
            // 
            this.txtbx_name.Location = new System.Drawing.Point(114, 82);
            this.txtbx_name.Name = "txtbx_name";
            this.txtbx_name.Size = new System.Drawing.Size(193, 29);
            this.txtbx_name.TabIndex = 6;
            this.txtbx_name.TextChanged += new System.EventHandler(this.txtbx_name_TextChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 50);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(86, 18);
            this.label2.TabIndex = 5;
            this.label2.Text = "Server Port";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(73, 18);
            this.label1.TabIndex = 4;
            this.label1.Text = "Server IP";
            // 
            // txtbx_port
            // 
            this.txtbx_port.Location = new System.Drawing.Point(114, 47);
            this.txtbx_port.Name = "txtbx_port";
            this.txtbx_port.Size = new System.Drawing.Size(193, 29);
            this.txtbx_port.TabIndex = 3;
            this.txtbx_port.Text = "8888";
            this.txtbx_port.TextChanged += new System.EventHandler(this.txtbx_port_TextChanged);
            // 
            // btn_connect
            // 
            this.btn_connect.Location = new System.Drawing.Point(12, 190);
            this.btn_connect.Name = "btn_connect";
            this.btn_connect.Size = new System.Drawing.Size(292, 43);
            this.btn_connect.TabIndex = 2;
            this.btn_connect.Text = "connect";
            this.btn_connect.UseVisualStyleBackColor = true;
            this.btn_connect.Click += new System.EventHandler(this.btn_connect_Click);
            // 
            // txtbx_ip
            // 
            this.txtbx_ip.Location = new System.Drawing.Point(114, 12);
            this.txtbx_ip.Name = "txtbx_ip";
            this.txtbx_ip.Size = new System.Drawing.Size(193, 29);
            this.txtbx_ip.TabIndex = 1;
            this.txtbx_ip.Text = "10.35.82.170";
            // 
            // splitContainer2
            // 
            this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer2.Location = new System.Drawing.Point(0, 0);
            this.splitContainer2.Name = "splitContainer2";
            this.splitContainer2.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.Controls.Add(this.panel_board);
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.Controls.Add(this.rtxbx_msg);
            this.splitContainer2.Size = new System.Drawing.Size(664, 449);
            this.splitContainer2.SplitterDistance = 206;
            this.splitContainer2.TabIndex = 0;
            // 
            // panel_board
            // 
            this.panel_board.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel_board.Location = new System.Drawing.Point(0, 0);
            this.panel_board.Name = "panel_board";
            this.panel_board.Size = new System.Drawing.Size(664, 206);
            this.panel_board.TabIndex = 0;
            this.panel_board.Click += new System.EventHandler(this.panel_board_Click);
            this.panel_board.Paint += new System.Windows.Forms.PaintEventHandler(this.panel_board_Paint);
            this.panel_board.MouseUp += new System.Windows.Forms.MouseEventHandler(this.panel_board_MouseUp);
            // 
            // rtxbx_msg
            // 
            this.rtxbx_msg.Dock = System.Windows.Forms.DockStyle.Fill;
            this.rtxbx_msg.Location = new System.Drawing.Point(0, 0);
            this.rtxbx_msg.Name = "rtxbx_msg";
            this.rtxbx_msg.Size = new System.Drawing.Size(664, 239);
            this.rtxbx_msg.TabIndex = 0;
            this.rtxbx_msg.Text = "";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(12, 254);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(34, 18);
            this.label4.TabIndex = 9;
            this.label4.Text = "hint";
            // 
            // label_hint
            // 
            this.label_hint.AutoSize = true;
            this.label_hint.Location = new System.Drawing.Point(12, 286);
            this.label_hint.Name = "label_hint";
            this.label_hint.Size = new System.Drawing.Size(35, 18);
            this.label_hint.TabIndex = 10;
            this.label_hint.Text = "   ...";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 18F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1011, 449);
            this.Controls.Add(this.splitContainer1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel1.PerformLayout();
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).EndInit();
            this.splitContainer2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox txtbx_name;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txtbx_port;
        private System.Windows.Forms.Button btn_connect;
        private System.Windows.Forms.TextBox txtbx_ip;
        private System.Windows.Forms.SplitContainer splitContainer2;
        private System.Windows.Forms.Panel panel_board;
        private System.Windows.Forms.RichTextBox rtxbx_msg;
        private System.Windows.Forms.Button btn_init;
        private System.Windows.Forms.Label label_hint;
        private System.Windows.Forms.Label label4;
    }
}

