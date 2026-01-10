namespace MapTool
{
    partial class MainForm
    {
        private System.ComponentModel.IContainer components = null;
        private System.Windows.Forms.Panel mapPanel;
        private System.Windows.Forms.Button btnLoadMap;
        private System.Windows.Forms.Button btnLoadWor;
        private System.Windows.Forms.Button btnLoadRegion;
        private System.Windows.Forms.Button btnExport;
        private System.Windows.Forms.Button btnClear;
        private System.Windows.Forms.Button btnRemoveLast;
        private System.Windows.Forms.Button btnZoomIn;
        private System.Windows.Forms.Button btnZoomOut;
        private System.Windows.Forms.TextBox txtMapId;
        private System.Windows.Forms.TextBox txtRegionX;
        private System.Windows.Forms.TextBox txtRegionY;
        private System.Windows.Forms.TextBox txtWorldX;
        private System.Windows.Forms.TextBox txtWorldY;
        private System.Windows.Forms.TextBox txtRegionID;
        private System.Windows.Forms.TextBox txtCellX;
        private System.Windows.Forms.TextBox txtCellY;
        private System.Windows.Forms.TextBox txtScriptFile;
        private System.Windows.Forms.Label lblMapInfo;
        private System.Windows.Forms.Label lblStats;
        private System.Windows.Forms.ToolStripStatusLabel lblStatus;
        private System.Windows.Forms.ListBox lstRegions;
        private System.Windows.Forms.ListBox lstEntries;
        private System.Windows.Forms.CheckBox chkUseServer;
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
            this.btnLoadMap = new System.Windows.Forms.Button();
            this.btnLoadWor = new System.Windows.Forms.Button();
            this.btnLoadRegion = new System.Windows.Forms.Button();
            this.btnExport = new System.Windows.Forms.Button();
            this.btnClear = new System.Windows.Forms.Button();
            this.btnRemoveLast = new System.Windows.Forms.Button();
            this.btnZoomIn = new System.Windows.Forms.Button();
            this.btnZoomOut = new System.Windows.Forms.Button();
            this.txtMapId = new System.Windows.Forms.TextBox();
            this.txtRegionX = new System.Windows.Forms.TextBox();
            this.txtRegionY = new System.Windows.Forms.TextBox();
            this.txtWorldX = new System.Windows.Forms.TextBox();
            this.txtWorldY = new System.Windows.Forms.TextBox();
            this.txtRegionID = new System.Windows.Forms.TextBox();
            this.txtCellX = new System.Windows.Forms.TextBox();
            this.txtCellY = new System.Windows.Forms.TextBox();
            this.txtScriptFile = new System.Windows.Forms.TextBox();
            this.lblMapInfo = new System.Windows.Forms.Label();
            this.lblStats = new System.Windows.Forms.Label();
            this.lstRegions = new System.Windows.Forms.ListBox();
            this.lstEntries = new System.Windows.Forms.ListBox();
            this.chkUseServer = new System.Windows.Forms.CheckBox();
            this.grpMapInfo = new System.Windows.Forms.GroupBox();
            this.grpCoordinates = new System.Windows.Forms.GroupBox();
            this.grpEntries = new System.Windows.Forms.GroupBox();
            this.statusStrip = new System.Windows.Forms.StatusStrip();

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
            this.mapPanel.Size = new System.Drawing.Size(800, 600);
            this.mapPanel.TabIndex = 0;
            this.mapPanel.Paint += new System.Windows.Forms.PaintEventHandler(this.mapPanel_Paint);
            this.mapPanel.MouseDown += new System.Windows.Forms.MouseEventHandler(this.mapPanel_MouseDown);
            this.mapPanel.MouseUp += new System.Windows.Forms.MouseEventHandler(this.mapPanel_MouseUp);
            this.mapPanel.MouseMove += new System.Windows.Forms.MouseEventHandler(this.mapPanel_MouseMove);
            this.mapPanel.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.mapPanel_MouseDoubleClick);

            // grpMapInfo
            this.grpMapInfo.Controls.Add(new System.Windows.Forms.Label() { Text = "Map ID:", Location = new System.Drawing.Point(10, 25), AutoSize = true });
            this.grpMapInfo.Controls.Add(this.txtMapId);
            this.grpMapInfo.Controls.Add(this.btnLoadMap);
            this.grpMapInfo.Controls.Add(this.btnLoadWor);
            this.grpMapInfo.Controls.Add(this.chkUseServer);
            this.grpMapInfo.Controls.Add(this.lblMapInfo);
            this.grpMapInfo.Controls.Add(new System.Windows.Forms.Label() { Text = "Regions:", Location = new System.Drawing.Point(10, 180), AutoSize = true });
            this.grpMapInfo.Controls.Add(this.lstRegions);
            this.grpMapInfo.Location = new System.Drawing.Point(820, 12);
            this.grpMapInfo.Name = "grpMapInfo";
            this.grpMapInfo.Size = new System.Drawing.Size(350, 300);
            this.grpMapInfo.TabIndex = 1;
            this.grpMapInfo.TabStop = false;
            this.grpMapInfo.Text = "Map Information";

            // txtMapId
            this.txtMapId.Location = new System.Drawing.Point(70, 22);
            this.txtMapId.Name = "txtMapId";
            this.txtMapId.Size = new System.Drawing.Size(80, 23);
            this.txtMapId.TabIndex = 0;
            this.txtMapId.Text = "1";
            this.txtMapId.TextChanged += new System.EventHandler(this.txtMapId_TextChanged);

            // btnLoadMap
            this.btnLoadMap.Location = new System.Drawing.Point(160, 20);
            this.btnLoadMap.Name = "btnLoadMap";
            this.btnLoadMap.Size = new System.Drawing.Size(90, 28);
            this.btnLoadMap.TabIndex = 1;
            this.btnLoadMap.Text = "Load Map Dir";
            this.btnLoadMap.UseVisualStyleBackColor = true;
            this.btnLoadMap.Click += new System.EventHandler(this.btnLoadMap_Click);

            // btnLoadWor
            this.btnLoadWor.Location = new System.Drawing.Point(255, 20);
            this.btnLoadWor.Name = "btnLoadWor";
            this.btnLoadWor.Size = new System.Drawing.Size(85, 28);
            this.btnLoadWor.TabIndex = 2;
            this.btnLoadWor.Text = "Load .wor";
            this.btnLoadWor.UseVisualStyleBackColor = true;
            this.btnLoadWor.Click += new System.EventHandler(this.btnLoadWor_Click);

            // chkUseServer
            this.chkUseServer.AutoSize = true;
            this.chkUseServer.Location = new System.Drawing.Point(10, 55);
            this.chkUseServer.Name = "chkUseServer";
            this.chkUseServer.Size = new System.Drawing.Size(140, 19);
            this.chkUseServer.TabIndex = 3;
            this.chkUseServer.Text = "Use Server Files (_S.dat)";
            this.chkUseServer.UseVisualStyleBackColor = true;

            // lblMapInfo
            this.lblMapInfo.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lblMapInfo.Location = new System.Drawing.Point(10, 80);
            this.lblMapInfo.Name = "lblMapInfo";
            this.lblMapInfo.Size = new System.Drawing.Size(330, 90);
            this.lblMapInfo.TabIndex = 4;
            this.lblMapInfo.Text = "No map loaded";

            // lstRegions
            this.lstRegions.FormattingEnabled = true;
            this.lstRegions.Location = new System.Drawing.Point(10, 200);
            this.lstRegions.Name = "lstRegions";
            this.lstRegions.Size = new System.Drawing.Size(330, 84);
            this.lstRegions.TabIndex = 5;
            this.lstRegions.DoubleClick += new System.EventHandler(this.lstRegions_DoubleClick);

            // grpCoordinates
            this.grpCoordinates.Controls.Add(new System.Windows.Forms.Label() { Text = "Region X:", Location = new System.Drawing.Point(10, 25), AutoSize = true });
            this.grpCoordinates.Controls.Add(this.txtRegionX);
            this.grpCoordinates.Controls.Add(new System.Windows.Forms.Label() { Text = "Region Y:", Location = new System.Drawing.Point(10, 55), AutoSize = true });
            this.grpCoordinates.Controls.Add(this.txtRegionY);
            this.grpCoordinates.Controls.Add(this.btnLoadRegion);
            this.grpCoordinates.Controls.Add(new System.Windows.Forms.Label() { Text = "World X:", Location = new System.Drawing.Point(10, 95), AutoSize = true });
            this.grpCoordinates.Controls.Add(this.txtWorldX);
            this.grpCoordinates.Controls.Add(new System.Windows.Forms.Label() { Text = "World Y:", Location = new System.Drawing.Point(10, 125), AutoSize = true });
            this.grpCoordinates.Controls.Add(this.txtWorldY);
            this.grpCoordinates.Controls.Add(new System.Windows.Forms.Label() { Text = "Region ID:", Location = new System.Drawing.Point(10, 155), AutoSize = true });
            this.grpCoordinates.Controls.Add(this.txtRegionID);
            this.grpCoordinates.Controls.Add(new System.Windows.Forms.Label() { Text = "Cell X:", Location = new System.Drawing.Point(10, 185), AutoSize = true });
            this.grpCoordinates.Controls.Add(this.txtCellX);
            this.grpCoordinates.Controls.Add(new System.Windows.Forms.Label() { Text = "Cell Y:", Location = new System.Drawing.Point(10, 215), AutoSize = true });
            this.grpCoordinates.Controls.Add(this.txtCellY);
            this.grpCoordinates.Location = new System.Drawing.Point(820, 320);
            this.grpCoordinates.Name = "grpCoordinates";
            this.grpCoordinates.Size = new System.Drawing.Size(350, 250);
            this.grpCoordinates.TabIndex = 2;
            this.grpCoordinates.TabStop = false;
            this.grpCoordinates.Text = "Coordinates (Click on map)";

            // txtRegionX
            this.txtRegionX.Location = new System.Drawing.Point(90, 22);
            this.txtRegionX.Name = "txtRegionX";
            this.txtRegionX.Size = new System.Drawing.Size(100, 23);
            this.txtRegionX.ReadOnly = false;
            this.txtRegionX.TabIndex = 0;
            this.txtRegionX.Text = "0";

            // txtRegionY
            this.txtRegionY.Location = new System.Drawing.Point(90, 52);
            this.txtRegionY.Name = "txtRegionY";
            this.txtRegionY.Size = new System.Drawing.Size(100, 23);
            this.txtRegionY.ReadOnly = false;
            this.txtRegionY.TabIndex = 1;
            this.txtRegionY.Text = "0";

            // btnLoadRegion
            this.btnLoadRegion.Location = new System.Drawing.Point(200, 35);
            this.btnLoadRegion.Name = "btnLoadRegion";
            this.btnLoadRegion.Size = new System.Drawing.Size(140, 30);
            this.btnLoadRegion.TabIndex = 2;
            this.btnLoadRegion.Text = "Load Region";
            this.btnLoadRegion.UseVisualStyleBackColor = true;
            this.btnLoadRegion.Click += new System.EventHandler(this.btnLoadRegion_Click);

            // txtWorldX through txtCellY
            this.txtWorldX.Location = new System.Drawing.Point(90, 92);
            this.txtWorldX.Name = "txtWorldX";
            this.txtWorldX.ReadOnly = true;
            this.txtWorldX.Size = new System.Drawing.Size(100, 23);
            this.txtWorldX.TabIndex = 3;

            this.txtWorldY.Location = new System.Drawing.Point(90, 122);
            this.txtWorldY.Name = "txtWorldY";
            this.txtWorldY.ReadOnly = true;
            this.txtWorldY.Size = new System.Drawing.Size(100, 23);
            this.txtWorldY.TabIndex = 4;

            this.txtRegionID.Location = new System.Drawing.Point(90, 152);
            this.txtRegionID.Name = "txtRegionID";
            this.txtRegionID.ReadOnly = true;
            this.txtRegionID.Size = new System.Drawing.Size(100, 23);
            this.txtRegionID.TabIndex = 5;

            this.txtCellX.Location = new System.Drawing.Point(90, 182);
            this.txtCellX.Name = "txtCellX";
            this.txtCellX.ReadOnly = true;
            this.txtCellX.Size = new System.Drawing.Size(100, 23);
            this.txtCellX.TabIndex = 6;

            this.txtCellY.Location = new System.Drawing.Point(90, 212);
            this.txtCellY.Name = "txtCellY";
            this.txtCellY.ReadOnly = true;
            this.txtCellY.Size = new System.Drawing.Size(100, 23);
            this.txtCellY.TabIndex = 7;

            // grpEntries
            this.grpEntries.Controls.Add(new System.Windows.Forms.Label() { Text = "Script File:", Location = new System.Drawing.Point(10, 25), AutoSize = true });
            this.grpEntries.Controls.Add(this.txtScriptFile);
            this.grpEntries.Controls.Add(new System.Windows.Forms.Label() { Text = "Entries (Double-click map to add):", Location = new System.Drawing.Point(10, 55), AutoSize = true });
            this.grpEntries.Controls.Add(this.lstEntries);
            this.grpEntries.Controls.Add(this.lblStats);
            this.grpEntries.Controls.Add(this.btnRemoveLast);
            this.grpEntries.Controls.Add(this.btnClear);
            this.grpEntries.Controls.Add(this.btnExport);
            this.grpEntries.Location = new System.Drawing.Point(820, 580);
            this.grpEntries.Name = "grpEntries";
            this.grpEntries.Size = new System.Drawing.Size(350, 250);
            this.grpEntries.TabIndex = 3;
            this.grpEntries.TabStop = false;
            this.grpEntries.Text = "Trap Entries";

            // txtScriptFile
            this.txtScriptFile.Location = new System.Drawing.Point(90, 22);
            this.txtScriptFile.Name = "txtScriptFile";
            this.txtScriptFile.Size = new System.Drawing.Size(250, 23);
            this.txtScriptFile.TabIndex = 0;
            this.txtScriptFile.Text = @"\script\maps\trap\1\1.lua";

            // lstEntries
            this.lstEntries.FormattingEnabled = true;
            this.lstEntries.Location = new System.Drawing.Point(10, 80);
            this.lstEntries.Name = "lstEntries";
            this.lstEntries.Size = new System.Drawing.Size(330, 84);
            this.lstEntries.TabIndex = 1;

            // lblStats
            this.lblStats.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lblStats.Location = new System.Drawing.Point(10, 170);
            this.lblStats.Name = "lblStats";
            this.lblStats.Size = new System.Drawing.Size(330, 40);
            this.lblStats.TabIndex = 2;
            this.lblStats.Text = "No entries";

            // btnRemoveLast
            this.btnRemoveLast.Location = new System.Drawing.Point(10, 215);
            this.btnRemoveLast.Name = "btnRemoveLast";
            this.btnRemoveLast.Size = new System.Drawing.Size(100, 28);
            this.btnRemoveLast.TabIndex = 3;
            this.btnRemoveLast.Text = "Remove Last";
            this.btnRemoveLast.UseVisualStyleBackColor = true;
            this.btnRemoveLast.Click += new System.EventHandler(this.btnRemoveLast_Click);

            // btnClear
            this.btnClear.Location = new System.Drawing.Point(120, 215);
            this.btnClear.Name = "btnClear";
            this.btnClear.Size = new System.Drawing.Size(100, 28);
            this.btnClear.TabIndex = 4;
            this.btnClear.Text = "Clear All";
            this.btnClear.UseVisualStyleBackColor = true;
            this.btnClear.Click += new System.EventHandler(this.btnClear_Click);

            // btnExport
            this.btnExport.Location = new System.Drawing.Point(230, 215);
            this.btnExport.Name = "btnExport";
            this.btnExport.Size = new System.Drawing.Size(110, 28);
            this.btnExport.TabIndex = 5;
            this.btnExport.Text = "Export to File";
            this.btnExport.UseVisualStyleBackColor = true;
            this.btnExport.Click += new System.EventHandler(this.btnExport_Click);

            // Zoom buttons
            this.btnZoomIn.Location = new System.Drawing.Point(12, 620);
            this.btnZoomIn.Name = "btnZoomIn";
            this.btnZoomIn.Size = new System.Drawing.Size(75, 28);
            this.btnZoomIn.TabIndex = 4;
            this.btnZoomIn.Text = "Zoom +";
            this.btnZoomIn.UseVisualStyleBackColor = true;
            this.btnZoomIn.Click += new System.EventHandler(this.btnZoomIn_Click);

            this.btnZoomOut.Location = new System.Drawing.Point(93, 620);
            this.btnZoomOut.Name = "btnZoomOut";
            this.btnZoomOut.Size = new System.Drawing.Size(75, 28);
            this.btnZoomOut.TabIndex = 5;
            this.btnZoomOut.Text = "Zoom -";
            this.btnZoomOut.UseVisualStyleBackColor = true;
            this.btnZoomOut.Click += new System.EventHandler(this.btnZoomOut_Click);

            // Status strip
            this.statusStrip.Location = new System.Drawing.Point(0, 840);
            this.statusStrip.Name = "statusStrip";
            this.statusStrip.Size = new System.Drawing.Size(1184, 22);
            this.statusStrip.TabIndex = 6;
            this.statusStrip.Text = "statusStrip1";

            this.lblStatus = new System.Windows.Forms.ToolStripStatusLabel();
            this.lblStatus.Name = "lblStatus";
            this.lblStatus.Size = new System.Drawing.Size(39, 17);
            this.lblStatus.Text = "Ready";
            this.statusStrip.Items.Add(this.lblStatus);

            // MainForm
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1184, 862);
            this.Controls.Add(this.statusStrip);
            this.Controls.Add(this.btnZoomOut);
            this.Controls.Add(this.btnZoomIn);
            this.Controls.Add(this.grpEntries);
            this.Controls.Add(this.grpCoordinates);
            this.Controls.Add(this.grpMapInfo);
            this.Controls.Add(this.mapPanel);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Sword Online Map Tool - Region/Cell Coordinate Editor";

            this.grpMapInfo.ResumeLayout(false);
            this.grpMapInfo.PerformLayout();
            this.grpCoordinates.ResumeLayout(false);
            this.grpCoordinates.PerformLayout();
            this.grpEntries.ResumeLayout(false);
            this.grpEntries.PerformLayout();
            this.statusStrip.ResumeLayout(false);
            this.statusStrip.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();
        }
    }
}
