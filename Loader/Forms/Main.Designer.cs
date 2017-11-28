namespace Loader
{
    partial class Main
    {
        /// <summary>
        /// Erforderliche Designervariable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Verwendete Ressourcen bereinigen.
        /// </summary>
        /// <param name="disposing">True, wenn verwaltete Ressourcen gelöscht werden sollen; andernfalls False.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Vom Windows Form-Designer generierter Code

        /// <summary>
        /// Erforderliche Methode für die Designerunterstützung.
        /// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
        /// </summary>
        private void InitializeComponent()
        {
            this.listBox = new System.Windows.Forms.ListBox();
            this.btnInject = new System.Windows.Forms.Button();
            this.lblStatus = new System.Windows.Forms.Label();
            this.lblStatusValue = new System.Windows.Forms.Label();
            this.lblWelcome = new System.Windows.Forms.Label();
            this.lblUsername = new System.Windows.Forms.Label();
            this.lblExpiresOn = new System.Windows.Forms.Label();
            this.lblExpiresOnValue = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // listBox
            // 
            this.listBox.FormattingEnabled = true;
            this.listBox.Items.AddRange(new object[] {
            "Counter-Strike: Global Offensive",
            "Minecraft",
            "Shower with Dad"});
            this.listBox.Location = new System.Drawing.Point(12, 12);
            this.listBox.Name = "listBox";
            this.listBox.Size = new System.Drawing.Size(205, 173);
            this.listBox.TabIndex = 0;
            this.listBox.SelectedIndexChanged += new System.EventHandler(this.listBox_SelectedIndexChanged);
            // 
            // btnInject
            // 
            this.btnInject.Enabled = false;
            this.btnInject.Location = new System.Drawing.Point(223, 162);
            this.btnInject.Name = "btnInject";
            this.btnInject.Size = new System.Drawing.Size(175, 23);
            this.btnInject.TabIndex = 1;
            this.btnInject.Text = "Inject";
            this.btnInject.UseVisualStyleBackColor = true;
            this.btnInject.Click += new System.EventHandler(this.btnInject_Click);
            // 
            // lblStatus
            // 
            this.lblStatus.AutoSize = true;
            this.lblStatus.Location = new System.Drawing.Point(224, 52);
            this.lblStatus.Name = "lblStatus";
            this.lblStatus.Size = new System.Drawing.Size(40, 13);
            this.lblStatus.TabIndex = 2;
            this.lblStatus.Text = "Status:";
            // 
            // lblStatusValue
            // 
            this.lblStatusValue.AutoSize = true;
            this.lblStatusValue.Location = new System.Drawing.Point(261, 52);
            this.lblStatusValue.Name = "lblStatusValue";
            this.lblStatusValue.Size = new System.Drawing.Size(0, 13);
            this.lblStatusValue.TabIndex = 3;
            // 
            // lblWelcome
            // 
            this.lblWelcome.AutoSize = true;
            this.lblWelcome.Location = new System.Drawing.Point(224, 13);
            this.lblWelcome.Name = "lblWelcome";
            this.lblWelcome.Size = new System.Drawing.Size(58, 13);
            this.lblWelcome.TabIndex = 4;
            this.lblWelcome.Text = "Welcome, ";
            // 
            // lblUsername
            // 
            this.lblUsername.AutoSize = true;
            this.lblUsername.Location = new System.Drawing.Point(277, 13);
            this.lblUsername.Name = "lblUsername";
            this.lblUsername.Size = new System.Drawing.Size(0, 13);
            this.lblUsername.TabIndex = 5;
            // 
            // lblExpiresOn
            // 
            this.lblExpiresOn.AutoSize = true;
            this.lblExpiresOn.Location = new System.Drawing.Point(224, 39);
            this.lblExpiresOn.Name = "lblExpiresOn";
            this.lblExpiresOn.Size = new System.Drawing.Size(59, 13);
            this.lblExpiresOn.TabIndex = 6;
            this.lblExpiresOn.Text = "Expires on:";
            // 
            // lblExpiresOnValue
            // 
            this.lblExpiresOnValue.AutoSize = true;
            this.lblExpiresOnValue.Location = new System.Drawing.Point(278, 39);
            this.lblExpiresOnValue.Name = "lblExpiresOnValue";
            this.lblExpiresOnValue.Size = new System.Drawing.Size(0, 13);
            this.lblExpiresOnValue.TabIndex = 7;
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(410, 203);
            this.Controls.Add(this.lblExpiresOnValue);
            this.Controls.Add(this.lblExpiresOn);
            this.Controls.Add(this.lblUsername);
            this.Controls.Add(this.lblWelcome);
            this.Controls.Add(this.lblStatusValue);
            this.Controls.Add(this.lblStatus);
            this.Controls.Add(this.btnInject);
            this.Controls.Add(this.listBox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Main";
            this.ShowIcon = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Mechanics";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListBox listBox;
        private System.Windows.Forms.Button btnInject;
        private System.Windows.Forms.Label lblStatus;
        private System.Windows.Forms.Label lblStatusValue;
        private System.Windows.Forms.Label lblWelcome;
        private System.Windows.Forms.Label lblUsername;
        private System.Windows.Forms.Label lblExpiresOn;
        private System.Windows.Forms.Label lblExpiresOnValue;
    }
}

