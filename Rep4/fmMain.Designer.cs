namespace Rep4
{
    partial class fmUser
    {
        /// <summary>
        /// Обязательная переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            this.reportViewerUser = new Microsoft.Reporting.WinForms.ReportViewer();
            this.menuStrip = new System.Windows.Forms.MenuStrip();
            this.miFileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.miExit = new System.Windows.Forms.ToolStripMenuItem();
            this.miReports = new System.Windows.Forms.ToolStripMenuItem();
            this.miOperators = new System.Windows.Forms.ToolStripMenuItem();
            this.miBase = new System.Windows.Forms.ToolStripMenuItem();
            this.miSaveClear = new System.Windows.Forms.ToolStripMenuItem();
            this.miOpenBase = new System.Windows.Forms.ToolStripMenuItem();
            this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.statusStrip = new System.Windows.Forms.StatusStrip();
            this.statusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.statusLabel2 = new System.Windows.Forms.ToolStripStatusLabel();
            this.menuStrip.SuspendLayout();
            this.statusStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // reportViewerUser
            // 
            this.reportViewerUser.Dock = System.Windows.Forms.DockStyle.Fill;
            this.reportViewerUser.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.reportViewerUser.Location = new System.Drawing.Point(0, 24);
            this.reportViewerUser.Name = "reportViewerUser";
            this.reportViewerUser.ServerReport.BearerToken = null;
            this.reportViewerUser.Size = new System.Drawing.Size(800, 426);
            this.reportViewerUser.TabIndex = 0;
            this.reportViewerUser.Load += new System.EventHandler(this.reportViewerUser_Load);
            // 
            // menuStrip
            // 
            this.menuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.miFileToolStripMenuItem,
            this.miReports,
            this.miBase});
            this.menuStrip.Location = new System.Drawing.Point(0, 0);
            this.menuStrip.Name = "menuStrip";
            this.menuStrip.Size = new System.Drawing.Size(800, 24);
            this.menuStrip.TabIndex = 1;
            // 
            // miFileToolStripMenuItem
            // 
            this.miFileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.miExit});
            this.miFileToolStripMenuItem.Name = "miFileToolStripMenuItem";
            this.miFileToolStripMenuItem.Size = new System.Drawing.Size(48, 20);
            this.miFileToolStripMenuItem.Text = "Файл";
            // 
            // miExit
            // 
            this.miExit.Name = "miExit";
            this.miExit.Size = new System.Drawing.Size(109, 22);
            this.miExit.Text = "Выход";
            this.miExit.Click += new System.EventHandler(this.miExit_Click);
            // 
            // miReports
            // 
            this.miReports.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.miOperators});
            this.miReports.Name = "miReports";
            this.miReports.Size = new System.Drawing.Size(60, 20);
            this.miReports.Text = "Отчёты";
            // 
            // miOperators
            // 
            this.miOperators.Name = "miOperators";
            this.miOperators.Size = new System.Drawing.Size(137, 22);
            this.miOperators.Text = "Операторы";
            this.miOperators.Click += new System.EventHandler(this.miOperators_Click);
            // 
            // miBase
            // 
            this.miBase.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.miSaveClear,
            this.miOpenBase});
            this.miBase.Name = "miBase";
            this.miBase.Size = new System.Drawing.Size(43, 20);
            this.miBase.Text = "База";
            // 
            // miSaveClear
            // 
            this.miSaveClear.Name = "miSaveClear";
            this.miSaveClear.Size = new System.Drawing.Size(196, 22);
            this.miSaveClear.Text = "Сохранить и очистить";
            this.miSaveClear.Click += new System.EventHandler(this.miSaveClear_Click);
            // 
            // miOpenBase
            // 
            this.miOpenBase.Name = "miOpenBase";
            this.miOpenBase.Size = new System.Drawing.Size(196, 22);
            this.miOpenBase.Text = "Открыть базу";
            this.miOpenBase.Click += new System.EventHandler(this.miOpenBase_Click);
            // 
            // statusStrip
            // 
            this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.statusLabel1,
            this.statusLabel2});
            this.statusStrip.Location = new System.Drawing.Point(0, 428);
            this.statusStrip.Name = "statusStrip";
            this.statusStrip.Size = new System.Drawing.Size(800, 22);
            this.statusStrip.TabIndex = 2;
            this.statusStrip.Text = "statusStrip";
            // 
            // statusLabel1
            // 
            this.statusLabel1.Name = "statusLabel1";
            this.statusLabel1.Size = new System.Drawing.Size(0, 17);
            // 
            // statusLabel2
            // 
            this.statusLabel2.Name = "statusLabel2";
            this.statusLabel2.Size = new System.Drawing.Size(0, 17);
            // 
            // fmUser
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.statusStrip);
            this.Controls.Add(this.reportViewerUser);
            this.Controls.Add(this.menuStrip);
            this.MainMenuStrip = this.menuStrip;
            this.Name = "fmUser";
            this.Text = "Form1";
            this.menuStrip.ResumeLayout(false);
            this.menuStrip.PerformLayout();
            this.statusStrip.ResumeLayout(false);
            this.statusStrip.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private Microsoft.Reporting.WinForms.ReportViewer reportViewerUser;
        private System.Windows.Forms.MenuStrip menuStrip;
        private System.Windows.Forms.ToolStripMenuItem miFileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem miExit;
        private System.Windows.Forms.ToolStripMenuItem miReports;
        private System.Windows.Forms.ToolStripMenuItem miOperators;
        private System.Windows.Forms.ToolStripMenuItem miBase;
        private System.Windows.Forms.ToolStripMenuItem miSaveClear;
        private System.Windows.Forms.ToolStripMenuItem miOpenBase;
        private System.Windows.Forms.OpenFileDialog openFileDialog;
        private System.Windows.Forms.StatusStrip statusStrip;
        private System.Windows.Forms.ToolStripStatusLabel statusLabel1;
        private System.Windows.Forms.ToolStripStatusLabel statusLabel2;
    }
}

