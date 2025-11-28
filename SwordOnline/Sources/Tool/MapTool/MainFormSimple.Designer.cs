namespace MapTool
{
    partial class MainFormSimple
    {
        private System.ComponentModel.IContainer components = null;

        // Controls
        private System.Windows.Forms.Panel mapPanel;
        private System.Windows.Forms.Button btnBrowseFolder;
        private System.Windows.Forms.Button btnLoadMap;
        private System.Windows.Forms.Button btnExport;
        private System.Windows.Forms.Button btnClear;
        private System.Windows.Forms.Button btnRemoveLast;
        private System.Windows.Forms.Button btnZoomIn;
        private System.Windows.Forms.Button btnZoomOut;
        private System.Windows.Forms.TextBox txtGameFolder;
        private System.Windows.Forms.TextBox txtMapId;
        private System.Windows.Forms.TextBox txtWorldX;
        private System.Windows.Forms.TextBox txtWorldY;
        private System.Windows.Forms.TextBox txtRegionX;
        private System.Windows.Forms.TextBox txtRegionY;
        private System.Windows.Forms.TextBox txtRegionID;
        private System.Windows.Forms.TextBox txtCellX;
        private System.Windows.Forms.TextBox txtCellY;
        private System.Windows.Forms.TextBox txtScriptFile;
        private System.Windows.Forms.Label lblMapInfo;
        private System.Windows.Forms.ToolStripStatusLabel lblStatus;
        private System.Windows.Forms.ListBox lstEntries;
        private System.Windows.Forms.RadioButton rdoServer;
        private System.Windows.Forms.RadioButton rdoClient;
        private System.Windows.Forms.GroupBox grpGameFolder;
        private System.Windows.Forms.GroupBox grpMapLoad;
        private System.Windows.Forms.GroupBox grpMapInfo;
        private System.Windows.Forms.GroupBox grpCoordinates;
        private System.Windows.Forms.GroupBox grpEntries;
        private System.Windows.Forms.StatusStrip statusStrip;

        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        private void InitializeComponent()
        {
            this.mapPanel = new System.Windows.Forms.Panel();
            this.btnBrowseFolder = new System.Windows.Forms.Button();
            this.btnLoadMap = new System.Windows.Forms.Button();
            this.btnExport = new System.Windows.Forms.Button();
            this.btnClear = new System.Windows.Forms.Button();
            this.btnRemoveLast = new System.Windows.Forms.Button();
            this.btnZoomIn = new System.Windows.Forms.Button();
            this.btnZoomOut = new System.Windows.Forms.Button();
            this.txtGameFolder = new System.Windows.Forms.TextBox();
            this.txtMapId = new System.Windows.Forms.TextBox();
            this.txtWorldX = new System.Windows.Forms.TextBox();
            this.txtWorldY = new System.Windows.Forms.TextBox();
            this.txtRegionX = new System.Windows.Forms.TextBox();
            this.txtRegionY = new System.Windows.Forms.TextBox();
            this.txtRegionID = new System.Windows.Forms.TextBox();
            this.txtCellX = new System.Windows.Forms.TextBox();
            this.txtCellY = new System.Windows.Forms.TextBox();
            this.txtScriptFile = new System.Windows.Forms.TextBox();
            this.lblMapInfo = new System.Windows.Forms.Label();
            this.lstEntries = new System.Windows.Forms.ListBox();
            this.rdoServer = new System.Windows.Forms.RadioButton();
            this.rdoClient = new System.Windows.Forms.RadioButton();
            this.grpGameFolder = new System.Windows.Forms.GroupBox();
            this.grpMapLoad = new System.Windows.Forms.GroupBox();
            this.grpMapInfo = new System.Windows.Forms.GroupBox();
            this.grpCoordinates = new System.Windows.Forms.GroupBox();
            this.grpEntries = new System.Windows.Forms.GroupBox();
            this.statusStrip = new System.Windows.Forms.StatusStrip();
            this.lblStatus = new System.Windows.Forms.ToolStripStatusLabel();

            this.grpGameFolder.SuspendLayout();
            this.grpMapLoad.SuspendLayout();
            this.grpMapInfo.SuspendLayout();
            this.grpCoordinates.SuspendLayout();
            this.grpEntries.SuspendLayout();
            this.statusStrip.SuspendLayout();
            this.SuspendLayout();

            // mapPanel
            this.mapPanel.BackColor = System.Drawing.Color.FromArgb(32, 32, 32);
            this.mapPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.mapPanel.Location = new System.Drawing.Point(12, 12);
            this.mapPanel.Name = "mapPanel";
            this.mapPanel.Size = new System.Drawing.Size(900, 700);
            this.mapPanel.TabIndex = 0;
            this.mapPanel.AutoScroll = true;
            this.mapPanel.Paint += new System.Windows.Forms.PaintEventHandler(this.mapPanel_Paint);
            this.mapPanel.MouseDown += new System.Windows.Forms.MouseEventHandler(this.mapPanel_MouseDown);
            this.mapPanel.MouseMove += new System.Windows.Forms.MouseEventHandler(this.mapPanel_MouseMove);
            this.mapPanel.MouseUp += new System.Windows.Forms.MouseEventHandler(this.mapPanel_MouseUp);
            this.mapPanel.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.mapPanel_MouseDoubleClick);

            // grpGameFolder
            this.grpGameFolder.Controls.Add(new System.Windows.Forms.Label { Text = "Game Folder:", Location = new System.Drawing.Point(10, 25), AutoSize = true });
            this.grpGameFolder.Controls.Add(this.txtGameFolder);
            this.grpGameFolder.Controls.Add(this.btnBrowseFolder);
            this.grpGameFolder.Controls.Add(this.rdoServer);
            this.grpGameFolder.Controls.Add(this.rdoClient);
            this.grpGameFolder.Location = new System.Drawing.Point(920, 12);
            this.grpGameFolder.Name = "grpGameFolder";
            this.grpGameFolder.Size = new System.Drawing.Size(340, 100);
            this.grpGameFolder.TabIndex = 1;
            this.grpGameFolder.TabStop = false;
            this.grpGameFolder.Text = "1. Select Game Folder";

            this.txtGameFolder.Location = new System.Drawing.Point(10, 45);
            this.txtGameFolder.Name = "txtGameFolder";
            this.txtGameFolder.Size = new System.Drawing.Size(240, 23);
            this.txtGameFolder.TabIndex = 0;

            this.btnBrowseFolder.Location = new System.Drawing.Point(255, 44);
            this.btnBrowseFolder.Name = "btnBrowseFolder";
            this.btnBrowseFolder.Size = new System.Drawing.Size(75, 25);
            this.btnBrowseFolder.TabIndex = 1;
            this.btnBrowseFolder.Text = "Browse...";
            this.btnBrowseFolder.UseVisualStyleBackColor = true;
            this.btnBrowseFolder.Click += new System.EventHandler(this.btnBrowseFolder_Click);

            this.rdoServer.AutoSize = true;
            this.rdoServer.Checked = true;
            this.rdoServer.Location = new System.Drawing.Point(10, 73);
            this.rdoServer.Name = "rdoServer";
            this.rdoServer.Size = new System.Drawing.Size(60, 19);
            this.rdoServer.TabIndex = 2;
            this.rdoServer.TabStop = true;
            this.rdoServer.Text = "Server";
            this.rdoServer.UseVisualStyleBackColor = true;
            this.rdoServer.CheckedChanged += new System.EventHandler(this.rdoServer_CheckedChanged);

            this.rdoClient.AutoSize = true;
            this.rdoClient.Location = new System.Drawing.Point(80, 73);
            this.rdoClient.Name = "rdoClient";
            this.rdoClient.Size = new System.Drawing.Size(57, 19);
            this.rdoClient.TabIndex = 3;
            this.rdoClient.Text = "Client";
            this.rdoClient.UseVisualStyleBackColor = true;

            // grpMapLoad
            this.grpMapLoad.Controls.Add(new System.Windows.Forms.Label { Text = "Map ID:", Location = new System.Drawing.Point(10, 30), AutoSize = true });
            this.grpMapLoad.Controls.Add(this.txtMapId);
            this.grpMapLoad.Controls.Add(this.btnLoadMap);
            this.grpMapLoad.Location = new System.Drawing.Point(920, 118);
            this.grpMapLoad.Name = "grpMapLoad";
            this.grpMapLoad.Size = new System.Drawing.Size(340, 70);
            this.grpMapLoad.TabIndex = 2;
            this.grpMapLoad.TabStop = false;
            this.grpMapLoad.Text = "2. Load Map";

            this.txtMapId.Location = new System.Drawing.Point(65, 27);
            this.txtMapId.Name = "txtMapId";
            this.txtMapId.Size = new System.Drawing.Size(100, 23);
            this.txtMapId.TabIndex = 0;
            this.txtMapId.Text = "11";

            this.btnLoadMap.Location = new System.Drawing.Point(175, 25);
            this.btnLoadMap.Name = "btnLoadMap";
            this.btnLoadMap.Size = new System.Drawing.Size(150, 28);
            this.btnLoadMap.TabIndex = 1;
            this.btnLoadMap.Text = "Load Map";
            this.btnLoadMap.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold);
            this.btnLoadMap.UseVisualStyleBackColor = true;
            this.btnLoadMap.Click += new System.EventHandler(this.btnLoadMap_Click);

            // grpMapInfo
            this.grpMapInfo.Controls.Add(this.lblMapInfo);
            this.grpMapInfo.Location = new System.Drawing.Point(920, 194);
            this.grpMapInfo.Name = "grpMapInfo";
            this.grpMapInfo.Size = new System.Drawing.Size(340, 140);
            this.grpMapInfo.TabIndex = 3;
            this.grpMapInfo.TabStop = false;
            this.grpMapInfo.Text = "Map Information";

            this.lblMapInfo.Location = new System.Drawing.Point(10, 20);
            this.lblMapInfo.Name = "lblMapInfo";
            this.lblMapInfo.Size = new System.Drawing.Size(320, 110);
            this.lblMapInfo.TabIndex = 0;
            this.lblMapInfo.Text = "No map loaded";

            // grpCoordinates
            this.grpCoordinates.Location = new System.Drawing.Point(920, 340);
            this.grpCoordinates.Name = "grpCoordinates";
            this.grpCoordinates.Size = new System.Drawing.Size(340, 180);
            this.grpCoordinates.TabIndex = 4;
            this.grpCoordinates.TabStop = false;
            this.grpCoordinates.Text = "Selected Coordinates";

            this.grpCoordinates.Controls.Add(new System.Windows.Forms.Label { Text = "World X:", Location = new System.Drawing.Point(10, 25), AutoSize = true });
            this.grpCoordinates.Controls.Add(this.txtWorldX);
            this.grpCoordinates.Controls.Add(new System.Windows.Forms.Label { Text = "World Y:", Location = new System.Drawing.Point(175, 25), AutoSize = true });
            this.grpCoordinates.Controls.Add(this.txtWorldY);

            this.grpCoordinates.Controls.Add(new System.Windows.Forms.Label { Text = "Region X:", Location = new System.Drawing.Point(10, 55), AutoSize = true });
            this.grpCoordinates.Controls.Add(this.txtRegionX);
            this.grpCoordinates.Controls.Add(new System.Windows.Forms.Label { Text = "Region Y:", Location = new System.Drawing.Point(175, 55), AutoSize = true });
            this.grpCoordinates.Controls.Add(this.txtRegionY);

            this.grpCoordinates.Controls.Add(new System.Windows.Forms.Label { Text = "RegionID:", Location = new System.Drawing.Point(10, 85), AutoSize = true });
            this.grpCoordinates.Controls.Add(this.txtRegionID);

            this.grpCoordinates.Controls.Add(new System.Windows.Forms.Label { Text = "Cell X:", Location = new System.Drawing.Point(10, 115), AutoSize = true });
            this.grpCoordinates.Controls.Add(this.txtCellX);
            this.grpCoordinates.Controls.Add(new System.Windows.Forms.Label { Text = "Cell Y:", Location = new System.Drawing.Point(175, 115), AutoSize = true });
            this.grpCoordinates.Controls.Add(this.txtCellY);

            this.grpCoordinates.Controls.Add(new System.Windows.Forms.Label { Text = "Script:", Location = new System.Drawing.Point(10, 145), AutoSize = true });
            this.grpCoordinates.Controls.Add(this.txtScriptFile);

            this.txtWorldX.Location = new System.Drawing.Point(75, 22);
            this.txtWorldX.Name = "txtWorldX";
            this.txtWorldX.ReadOnly = true;
            this.txtWorldX.Size = new System.Drawing.Size(80, 23);
            this.txtWorldX.TabIndex = 0;

            this.txtWorldY.Location = new System.Drawing.Point(240, 22);
            this.txtWorldY.Name = "txtWorldY";
            this.txtWorldY.ReadOnly = true;
            this.txtWorldY.Size = new System.Drawing.Size(80, 23);
            this.txtWorldY.TabIndex = 1;

            this.txtRegionX.Location = new System.Drawing.Point(75, 52);
            this.txtRegionX.Name = "txtRegionX";
            this.txtRegionX.ReadOnly = true;
            this.txtRegionX.Size = new System.Drawing.Size(80, 23);
            this.txtRegionX.TabIndex = 2;

            this.txtRegionY.Location = new System.Drawing.Point(240, 52);
            this.txtRegionY.Name = "txtRegionY";
            this.txtRegionY.ReadOnly = true;
            this.txtRegionY.Size = new System.Drawing.Size(80, 23);
            this.txtRegionY.TabIndex = 3;

            this.txtRegionID.Location = new System.Drawing.Point(75, 82);
            this.txtRegionID.Name = "txtRegionID";
            this.txtRegionID.ReadOnly = true;
            this.txtRegionID.Size = new System.Drawing.Size(245, 23);
            this.txtRegionID.TabIndex = 4;

            this.txtCellX.Location = new System.Drawing.Point(75, 112);
            this.txtCellX.Name = "txtCellX";
            this.txtCellX.ReadOnly = true;
            this.txtCellX.Size = new System.Drawing.Size(80, 23);
            this.txtCellX.TabIndex = 5;

            this.txtCellY.Location = new System.Drawing.Point(240, 112);
            this.txtCellY.Name = "txtCellY";
            this.txtCellY.ReadOnly = true;
            this.txtCellY.Size = new System.Drawing.Size(80, 23);
            this.txtCellY.TabIndex = 6;

            this.txtScriptFile.Location = new System.Drawing.Point(75, 142);
            this.txtScriptFile.Name = "txtScriptFile";
            this.txtScriptFile.Size = new System.Drawing.Size(245, 23);
            this.txtScriptFile.TabIndex = 7;
            this.txtScriptFile.Text = @"\script\maps\trap\1\1.lua";

            // grpEntries
            this.grpEntries.Controls.Add(this.lstEntries);
            this.grpEntries.Controls.Add(this.btnExport);
            this.grpEntries.Controls.Add(this.btnClear);
            this.grpEntries.Controls.Add(this.btnRemoveLast);
            this.grpEntries.Location = new System.Drawing.Point(920, 526);
            this.grpEntries.Name = "grpEntries";
            this.grpEntries.Size = new System.Drawing.Size(340, 160);
            this.grpEntries.TabIndex = 5;
            this.grpEntries.TabStop = false;
            this.grpEntries.Text = "Trap Entries (Double-click map to add)";

            this.lstEntries.FormattingEnabled = true;
            this.lstEntries.HorizontalScrollbar = true;
            this.lstEntries.Location = new System.Drawing.Point(10, 22);
            this.lstEntries.Name = "lstEntries";
            this.lstEntries.Size = new System.Drawing.Size(320, 95);
            this.lstEntries.TabIndex = 0;

            this.btnExport.Location = new System.Drawing.Point(10, 125);
            this.btnExport.Name = "btnExport";
            this.btnExport.Size = new System.Drawing.Size(100, 25);
            this.btnExport.TabIndex = 1;
            this.btnExport.Text = "Export to File";
            this.btnExport.UseVisualStyleBackColor = true;
            this.btnExport.Click += new System.EventHandler(this.btnExport_Click);

            this.btnRemoveLast.Location = new System.Drawing.Point(120, 125);
            this.btnRemoveLast.Name = "btnRemoveLast";
            this.btnRemoveLast.Size = new System.Drawing.Size(100, 25);
            this.btnRemoveLast.TabIndex = 2;
            this.btnRemoveLast.Text = "Remove Last";
            this.btnRemoveLast.UseVisualStyleBackColor = true;
            this.btnRemoveLast.Click += new System.EventHandler(this.btnRemoveLast_Click);

            this.btnClear.Location = new System.Drawing.Point(230, 125);
            this.btnClear.Name = "btnClear";
            this.btnClear.Size = new System.Drawing.Size(100, 25);
            this.btnClear.TabIndex = 3;
            this.btnClear.Text = "Clear All";
            this.btnClear.UseVisualStyleBackColor = true;
            this.btnClear.Click += new System.EventHandler(this.btnClear_Click);

            // Zoom buttons
            this.btnZoomIn.Location = new System.Drawing.Point(12, 718);
            this.btnZoomIn.Name = "btnZoomIn";
            this.btnZoomIn.Size = new System.Drawing.Size(75, 28);
            this.btnZoomIn.TabIndex = 6;
            this.btnZoomIn.Text = "Zoom +";
            this.btnZoomIn.UseVisualStyleBackColor = true;
            this.btnZoomIn.Click += new System.EventHandler(this.btnZoomIn_Click);

            this.btnZoomOut.Location = new System.Drawing.Point(93, 718);
            this.btnZoomOut.Name = "btnZoomOut";
            this.btnZoomOut.Size = new System.Drawing.Size(75, 28);
            this.btnZoomOut.TabIndex = 7;
            this.btnZoomOut.Text = "Zoom -";
            this.btnZoomOut.UseVisualStyleBackColor = true;
            this.btnZoomOut.Click += new System.EventHandler(this.btnZoomOut_Click);

            // Status strip
            this.statusStrip.Location = new System.Drawing.Point(0, 755);
            this.statusStrip.Name = "statusStrip";
            this.statusStrip.Size = new System.Drawing.Size(1272, 22);
            this.statusStrip.TabIndex = 8;
            this.statusStrip.Text = "statusStrip1";

            this.lblStatus = new System.Windows.Forms.ToolStripStatusLabel();
            this.lblStatus.Name = "lblStatus";
            this.lblStatus.Size = new System.Drawing.Size(39, 17);
            this.lblStatus.Text = "Ready";
            this.statusStrip.Items.Add(this.lblStatus);

            // MainFormSimple
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1272, 777);
            this.Controls.Add(this.statusStrip);
            this.Controls.Add(this.btnZoomOut);
            this.Controls.Add(this.btnZoomIn);
            this.Controls.Add(this.grpEntries);
            this.Controls.Add(this.grpCoordinates);
            this.Controls.Add(this.grpMapInfo);
            this.Controls.Add(this.grpMapLoad);
            this.Controls.Add(this.grpGameFolder);
            this.Controls.Add(this.mapPanel);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "MainFormSimple";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Simple Map Tool - Auto Load";

            this.grpGameFolder.ResumeLayout(false);
            this.grpGameFolder.PerformLayout();
            this.grpMapLoad.ResumeLayout(false);
            this.grpMapLoad.PerformLayout();
            this.grpMapInfo.ResumeLayout(false);
            this.grpCoordinates.ResumeLayout(false);
            this.grpCoordinates.PerformLayout();
            this.grpEntries.ResumeLayout(false);
            this.statusStrip.ResumeLayout(false);
            this.statusStrip.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();
        }
    }
}
