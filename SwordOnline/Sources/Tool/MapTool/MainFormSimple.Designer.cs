namespace MapTool
{
    partial class MainFormSimple
    {
        private System.ComponentModel.IContainer components = null;

        // Controls
        private System.Windows.Forms.TabControl tabMain;
        private System.Windows.Forms.TabPage tabTrap;
        private System.Windows.Forms.TabPage tabNPC;
        private System.Windows.Forms.Panel mapPanel;
        private System.Windows.Forms.Button btnBrowseFolder;
        private System.Windows.Forms.Button btnLoadMap;
        private System.Windows.Forms.Button btnExport;
        private System.Windows.Forms.Button btnExtractAllRegions;
        private System.Windows.Forms.Button btnLoadTrapFile;
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

        // NPC Controls
        private System.Windows.Forms.GroupBox grpNpcInput;
        private System.Windows.Forms.TextBox txtNpcId;
        private System.Windows.Forms.TextBox txtNpcLevel;
        private System.Windows.Forms.Button btnLoadNpcPreview;
        private System.Windows.Forms.GroupBox grpNpcPreview;
        private System.Windows.Forms.PictureBox picNpcPreview;
        private System.Windows.Forms.Label lblNpcName;
        private System.Windows.Forms.Button btnPlayAnimation;
        private System.Windows.Forms.Label lblFrameInfo;
        private System.Windows.Forms.Timer animationTimer;
        private System.Windows.Forms.Button btnPreviewZoomIn;
        private System.Windows.Forms.Button btnPreviewZoomOut;
        private System.Windows.Forms.GroupBox grpNpcEntries;
        private System.Windows.Forms.ListBox lstNpcEntries;
        private System.Windows.Forms.Button btnLoadNpcsFromServer;
        private System.Windows.Forms.Button btnExtractNpcList;
        private System.Windows.Forms.Button btnRemoveLastNpc;
        private System.Windows.Forms.Button btnClearNpcs;

        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                if (animationTimer != null)
                {
                    animationTimer.Stop();
                    animationTimer.Dispose();
                }
                if (components != null)
                {
                    components.Dispose();
                }
            }
            base.Dispose(disposing);
        }

        private void InitializeComponent()
        {
            this.tabMain = new System.Windows.Forms.TabControl();
            this.tabTrap = new System.Windows.Forms.TabPage();
            this.tabNPC = new System.Windows.Forms.TabPage();
            this.mapPanel = new System.Windows.Forms.Panel();
            this.btnBrowseFolder = new System.Windows.Forms.Button();
            this.btnLoadMap = new System.Windows.Forms.Button();
            this.btnExport = new System.Windows.Forms.Button();
            this.btnExtractAllRegions = new System.Windows.Forms.Button();
            this.btnLoadTrapFile = new System.Windows.Forms.Button();
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

            // NPC Controls
            this.grpNpcInput = new System.Windows.Forms.GroupBox();
            this.txtNpcId = new System.Windows.Forms.TextBox();
            this.txtNpcLevel = new System.Windows.Forms.TextBox();
            this.btnLoadNpcPreview = new System.Windows.Forms.Button();
            this.grpNpcPreview = new System.Windows.Forms.GroupBox();
            this.picNpcPreview = new System.Windows.Forms.PictureBox();
            this.lblNpcName = new System.Windows.Forms.Label();
            this.btnPlayAnimation = new System.Windows.Forms.Button();
            this.lblFrameInfo = new System.Windows.Forms.Label();
            this.animationTimer = new System.Windows.Forms.Timer();
            this.btnPreviewZoomIn = new System.Windows.Forms.Button();
            this.btnPreviewZoomOut = new System.Windows.Forms.Button();
            this.grpNpcEntries = new System.Windows.Forms.GroupBox();
            this.lstNpcEntries = new System.Windows.Forms.ListBox();
            this.btnLoadNpcsFromServer = new System.Windows.Forms.Button();
            this.btnExtractNpcList = new System.Windows.Forms.Button();
            this.btnRemoveLastNpc = new System.Windows.Forms.Button();
            this.btnClearNpcs = new System.Windows.Forms.Button();

            this.tabMain.SuspendLayout();
            this.tabTrap.SuspendLayout();
            this.tabNPC.SuspendLayout();
            this.grpGameFolder.SuspendLayout();
            this.grpMapLoad.SuspendLayout();
            this.grpMapInfo.SuspendLayout();
            this.grpCoordinates.SuspendLayout();
            this.grpEntries.SuspendLayout();
            this.grpNpcInput.SuspendLayout();
            this.grpNpcPreview.SuspendLayout();
            this.grpNpcEntries.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picNpcPreview)).BeginInit();
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
            this.grpGameFolder.Location = new System.Drawing.Point(6, 6);
            this.grpGameFolder.Name = "grpGameFolder";
            this.grpGameFolder.Size = new System.Drawing.Size(330, 100);
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
            this.grpMapLoad.Location = new System.Drawing.Point(6, 112);
            this.grpMapLoad.Name = "grpMapLoad";
            this.grpMapLoad.Size = new System.Drawing.Size(330, 70);
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
            this.btnLoadMap.Font = new System.Drawing.Font(".VnArial", 9F, System.Drawing.FontStyle.Bold);
            this.btnLoadMap.UseVisualStyleBackColor = true;
            this.btnLoadMap.Click += new System.EventHandler(this.btnLoadMap_Click);

            // grpMapInfo
            this.grpMapInfo.Controls.Add(this.lblMapInfo);
            this.grpMapInfo.Location = new System.Drawing.Point(6, 188);
            this.grpMapInfo.Name = "grpMapInfo";
            this.grpMapInfo.Size = new System.Drawing.Size(330, 140);
            this.grpMapInfo.TabIndex = 3;
            this.grpMapInfo.TabStop = false;
            this.grpMapInfo.Text = "Map Information";

            this.lblMapInfo.Location = new System.Drawing.Point(10, 20);
            this.lblMapInfo.Name = "lblMapInfo";
            this.lblMapInfo.Size = new System.Drawing.Size(320, 110);
            this.lblMapInfo.TabIndex = 0;
            this.lblMapInfo.Text = "No map loaded";

            // grpCoordinates
            this.grpCoordinates.Location = new System.Drawing.Point(6, 334);
            this.grpCoordinates.Name = "grpCoordinates";
            this.grpCoordinates.Size = new System.Drawing.Size(330, 180);
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
            this.grpEntries.Controls.Add(this.btnLoadTrapFile);
            this.grpEntries.Controls.Add(this.btnExport);
            this.grpEntries.Controls.Add(this.btnExtractAllRegions);
            this.grpEntries.Controls.Add(this.btnClear);
            this.grpEntries.Controls.Add(this.btnRemoveLast);
            this.grpEntries.Location = new System.Drawing.Point(6, 520);
            this.grpEntries.Name = "grpEntries";
            this.grpEntries.Size = new System.Drawing.Size(330, 225);
            this.grpEntries.TabIndex = 5;
            this.grpEntries.TabStop = false;
            this.grpEntries.Text = "Trap Entries (Double-click map to add)";

            this.lstEntries.FormattingEnabled = true;
            this.lstEntries.HorizontalScrollbar = true;
            this.lstEntries.Location = new System.Drawing.Point(10, 22);
            this.lstEntries.Name = "lstEntries";
            this.lstEntries.Size = new System.Drawing.Size(310, 120);
            this.lstEntries.TabIndex = 0;

            this.btnLoadTrapFile.Location = new System.Drawing.Point(10, 150);
            this.btnLoadTrapFile.Name = "btnLoadTrapFile";
            this.btnLoadTrapFile.Size = new System.Drawing.Size(150, 25);
            this.btnLoadTrapFile.TabIndex = 1;
            this.btnLoadTrapFile.Text = "Load Trap File";
            this.btnLoadTrapFile.UseVisualStyleBackColor = true;
            this.btnLoadTrapFile.Click += new System.EventHandler(this.btnLoadTrapFile_Click);

            this.btnExtractAllRegions.Location = new System.Drawing.Point(165, 150);
            this.btnExtractAllRegions.Name = "btnExtractAllRegions";
            this.btnExtractAllRegions.Size = new System.Drawing.Size(155, 25);
            this.btnExtractAllRegions.TabIndex = 2;
            this.btnExtractAllRegions.Text = "Extract All Regions";
            this.btnExtractAllRegions.UseVisualStyleBackColor = true;
            this.btnExtractAllRegions.Click += new System.EventHandler(this.btnExtractAllRegions_Click);

            this.btnExport.Location = new System.Drawing.Point(10, 183);
            this.btnExport.Name = "btnExport";
            this.btnExport.Size = new System.Drawing.Size(95, 25);
            this.btnExport.TabIndex = 3;
            this.btnExport.Text = "Export to File";
            this.btnExport.UseVisualStyleBackColor = true;
            this.btnExport.Click += new System.EventHandler(this.btnExport_Click);

            this.btnRemoveLast.Location = new System.Drawing.Point(110, 183);
            this.btnRemoveLast.Name = "btnRemoveLast";
            this.btnRemoveLast.Size = new System.Drawing.Size(100, 25);
            this.btnRemoveLast.TabIndex = 4;
            this.btnRemoveLast.Text = "Remove Last";
            this.btnRemoveLast.UseVisualStyleBackColor = true;
            this.btnRemoveLast.Click += new System.EventHandler(this.btnRemoveLast_Click);

            this.btnClear.Location = new System.Drawing.Point(215, 183);
            this.btnClear.Name = "btnClear";
            this.btnClear.Size = new System.Drawing.Size(105, 25);
            this.btnClear.TabIndex = 5;
            this.btnClear.Text = "Clear All";
            this.btnClear.UseVisualStyleBackColor = true;
            this.btnClear.Click += new System.EventHandler(this.btnClear_Click);

            // ==== NPC CONTROLS ====

            // grpNpcInput
            this.grpNpcInput.Controls.Add(new System.Windows.Forms.Label { Text = "NPC ID:", Location = new System.Drawing.Point(10, 25), AutoSize = true });
            this.grpNpcInput.Controls.Add(this.txtNpcId);
            this.grpNpcInput.Controls.Add(new System.Windows.Forms.Label { Text = "Level:", Location = new System.Drawing.Point(10, 55), AutoSize = true });
            this.grpNpcInput.Controls.Add(this.txtNpcLevel);
            this.grpNpcInput.Controls.Add(this.btnLoadNpcPreview);
            this.grpNpcInput.Location = new System.Drawing.Point(6, 6);
            this.grpNpcInput.Name = "grpNpcInput";
            this.grpNpcInput.Size = new System.Drawing.Size(330, 120);
            this.grpNpcInput.TabIndex = 0;
            this.grpNpcInput.TabStop = false;
            this.grpNpcInput.Text = "NPC Selection";

            this.txtNpcId.Location = new System.Drawing.Point(70, 22);
            this.txtNpcId.Name = "txtNpcId";
            this.txtNpcId.Size = new System.Drawing.Size(250, 23);
            this.txtNpcId.TabIndex = 0;
            this.txtNpcId.Text = "1";

            this.txtNpcLevel.Location = new System.Drawing.Point(70, 52);
            this.txtNpcLevel.Name = "txtNpcLevel";
            this.txtNpcLevel.Size = new System.Drawing.Size(250, 23);
            this.txtNpcLevel.TabIndex = 1;
            this.txtNpcLevel.Text = "1";

            this.btnLoadNpcPreview.Location = new System.Drawing.Point(70, 82);
            this.btnLoadNpcPreview.Name = "btnLoadNpcPreview";
            this.btnLoadNpcPreview.Size = new System.Drawing.Size(250, 28);
            this.btnLoadNpcPreview.TabIndex = 2;
            this.btnLoadNpcPreview.Text = "Load NPC Preview";
            this.btnLoadNpcPreview.Font = new System.Drawing.Font(".VnArial", 9F, System.Drawing.FontStyle.Bold);
            this.btnLoadNpcPreview.UseVisualStyleBackColor = true;
            this.btnLoadNpcPreview.Click += new System.EventHandler(this.btnLoadNpcPreview_Click);

            // grpNpcPreview
            this.grpNpcPreview.Controls.Add(this.picNpcPreview);
            this.grpNpcPreview.Controls.Add(this.lblNpcName);
            this.grpNpcPreview.Controls.Add(this.btnPlayAnimation);
            this.grpNpcPreview.Controls.Add(this.lblFrameInfo);
            this.grpNpcPreview.Controls.Add(this.btnPreviewZoomIn);
            this.grpNpcPreview.Controls.Add(this.btnPreviewZoomOut);
            this.grpNpcPreview.Location = new System.Drawing.Point(6, 132);
            this.grpNpcPreview.Name = "grpNpcPreview";
            this.grpNpcPreview.Size = new System.Drawing.Size(330, 280);
            this.grpNpcPreview.TabIndex = 1;
            this.grpNpcPreview.TabStop = false;
            this.grpNpcPreview.Text = "NPC Preview";

            this.picNpcPreview.BackColor = System.Drawing.SystemColors.Control;
            this.picNpcPreview.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.picNpcPreview.Location = new System.Drawing.Point(10, 70);
            this.picNpcPreview.Name = "picNpcPreview";
            this.picNpcPreview.Size = new System.Drawing.Size(310, 165);
            this.picNpcPreview.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.picNpcPreview.TabIndex = 0;
            this.picNpcPreview.TabStop = false;

            this.lblNpcName.Location = new System.Drawing.Point(10, 20);
            this.lblNpcName.Name = "lblNpcName";
            this.lblNpcName.Size = new System.Drawing.Size(310, 20);
            this.lblNpcName.TabIndex = 1;
            this.lblNpcName.Text = "No NPC loaded";
            this.lblNpcName.Font = new System.Drawing.Font(".VnArial", 9F, System.Drawing.FontStyle.Bold);

            this.lblFrameInfo.Location = new System.Drawing.Point(10, 45);
            this.lblFrameInfo.Name = "lblFrameInfo";
            this.lblFrameInfo.Size = new System.Drawing.Size(200, 20);
            this.lblFrameInfo.TabIndex = 2;
            this.lblFrameInfo.Text = "Frame: 0 / 0";

            this.btnPlayAnimation.Location = new System.Drawing.Point(220, 42);
            this.btnPlayAnimation.Name = "btnPlayAnimation";
            this.btnPlayAnimation.Size = new System.Drawing.Size(100, 25);
            this.btnPlayAnimation.TabIndex = 3;
            this.btnPlayAnimation.Text = "▶ Play";
            this.btnPlayAnimation.UseVisualStyleBackColor = true;
            this.btnPlayAnimation.Enabled = false;
            this.btnPlayAnimation.Click += new System.EventHandler(this.btnPlayAnimation_Click);

            // animationTimer
            this.animationTimer.Interval = 100;  // 100ms = 10 FPS
            this.animationTimer.Tick += new System.EventHandler(this.animationTimer_Tick);

            // btnPreviewZoomIn
            this.btnPreviewZoomIn.Location = new System.Drawing.Point(10, 245);
            this.btnPreviewZoomIn.Name = "btnPreviewZoomIn";
            this.btnPreviewZoomIn.Size = new System.Drawing.Size(70, 25);
            this.btnPreviewZoomIn.TabIndex = 4;
            this.btnPreviewZoomIn.Text = "Zoom +";
            this.btnPreviewZoomIn.UseVisualStyleBackColor = true;
            this.btnPreviewZoomIn.Click += new System.EventHandler(this.btnPreviewZoomIn_Click);

            // btnPreviewZoomOut
            this.btnPreviewZoomOut.Location = new System.Drawing.Point(85, 245);
            this.btnPreviewZoomOut.Name = "btnPreviewZoomOut";
            this.btnPreviewZoomOut.Size = new System.Drawing.Size(70, 25);
            this.btnPreviewZoomOut.TabIndex = 5;
            this.btnPreviewZoomOut.Text = "Zoom -";
            this.btnPreviewZoomOut.UseVisualStyleBackColor = true;
            this.btnPreviewZoomOut.Click += new System.EventHandler(this.btnPreviewZoomOut_Click);

            // grpNpcEntries
            this.grpNpcEntries.Controls.Add(this.lstNpcEntries);
            this.grpNpcEntries.Controls.Add(this.btnLoadNpcsFromServer);
            this.grpNpcEntries.Controls.Add(this.btnExtractNpcList);
            this.grpNpcEntries.Controls.Add(this.btnRemoveLastNpc);
            this.grpNpcEntries.Controls.Add(this.btnClearNpcs);
            this.grpNpcEntries.Location = new System.Drawing.Point(6, 418);
            this.grpNpcEntries.Name = "grpNpcEntries";
            this.grpNpcEntries.Size = new System.Drawing.Size(330, 320);
            this.grpNpcEntries.TabIndex = 2;
            this.grpNpcEntries.TabStop = false;
            this.grpNpcEntries.Text = "NPC Entries (Double-click map to add)";

            this.lstNpcEntries.FormattingEnabled = true;
            this.lstNpcEntries.HorizontalScrollbar = true;
            this.lstNpcEntries.Location = new System.Drawing.Point(10, 50);
            this.lstNpcEntries.Name = "lstNpcEntries";
            this.lstNpcEntries.Size = new System.Drawing.Size(310, 165);
            this.lstNpcEntries.TabIndex = 0;

            this.btnLoadNpcsFromServer.Location = new System.Drawing.Point(10, 20);
            this.btnLoadNpcsFromServer.Name = "btnLoadNpcsFromServer";
            this.btnLoadNpcsFromServer.Size = new System.Drawing.Size(310, 25);
            this.btnLoadNpcsFromServer.TabIndex = 1;
            this.btnLoadNpcsFromServer.Text = "Load NPCs from Npc_Load.txt";
            this.btnLoadNpcsFromServer.UseVisualStyleBackColor = true;
            this.btnLoadNpcsFromServer.Click += new System.EventHandler(this.btnLoadNpcsFromServer_Click);

            this.btnExtractNpcList.Location = new System.Drawing.Point(10, 223);
            this.btnExtractNpcList.Name = "btnExtractNpcList";
            this.btnExtractNpcList.Size = new System.Drawing.Size(310, 28);
            this.btnExtractNpcList.TabIndex = 2;
            this.btnExtractNpcList.Text = "Save NPCs to File";
            this.btnExtractNpcList.Font = new System.Drawing.Font(".VnArial", 9F, System.Drawing.FontStyle.Bold);
            this.btnExtractNpcList.UseVisualStyleBackColor = true;
            this.btnExtractNpcList.Click += new System.EventHandler(this.btnExtractNpcList_Click);

            this.btnRemoveLastNpc.Location = new System.Drawing.Point(10, 258);
            this.btnRemoveLastNpc.Name = "btnRemoveLastNpc";
            this.btnRemoveLastNpc.Size = new System.Drawing.Size(150, 25);
            this.btnRemoveLastNpc.TabIndex = 3;
            this.btnRemoveLastNpc.Text = "Remove Selected";
            this.btnRemoveLastNpc.UseVisualStyleBackColor = true;
            this.btnRemoveLastNpc.Click += new System.EventHandler(this.btnRemoveLastNpc_Click);

            this.btnClearNpcs.Location = new System.Drawing.Point(165, 258);
            this.btnClearNpcs.Name = "btnClearNpcs";
            this.btnClearNpcs.Size = new System.Drawing.Size(155, 25);
            this.btnClearNpcs.TabIndex = 4;
            this.btnClearNpcs.Text = "Clear All";
            this.btnClearNpcs.UseVisualStyleBackColor = true;
            this.btnClearNpcs.Click += new System.EventHandler(this.btnClearNpcs_Click);

            // Zoom buttons (shared, outside tabs)
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

            // tabMain - positioned on the right side
            this.tabMain.Controls.Add(this.tabTrap);
            this.tabMain.Controls.Add(this.tabNPC);
            this.tabMain.Location = new System.Drawing.Point(920, 0);
            this.tabMain.Name = "tabMain";
            this.tabMain.SelectedIndex = 0;
            this.tabMain.Size = new System.Drawing.Size(352, 755);
            this.tabMain.TabIndex = 0;

            // tabTrap - trap controls only (map panel is shared outside)
            this.tabTrap.BackColor = System.Drawing.SystemColors.Control;
            this.tabTrap.Controls.Add(this.grpGameFolder);
            this.tabTrap.Controls.Add(this.grpMapLoad);
            this.tabTrap.Controls.Add(this.grpMapInfo);
            this.tabTrap.Controls.Add(this.grpCoordinates);
            this.tabTrap.Controls.Add(this.grpEntries);
            this.tabTrap.Location = new System.Drawing.Point(4, 24);
            this.tabTrap.Name = "tabTrap";
            this.tabTrap.Padding = new System.Windows.Forms.Padding(3);
            this.tabTrap.Size = new System.Drawing.Size(344, 727);
            this.tabTrap.TabIndex = 0;
            this.tabTrap.Text = "Add Trap";

            // tabNPC - NPC controls
            this.tabNPC.BackColor = System.Drawing.SystemColors.Control;
            this.tabNPC.Controls.Add(this.grpNpcInput);
            this.tabNPC.Controls.Add(this.grpNpcPreview);
            this.tabNPC.Controls.Add(this.grpNpcEntries);
            this.tabNPC.Location = new System.Drawing.Point(4, 24);
            this.tabNPC.Name = "tabNPC";
            this.tabNPC.Padding = new System.Windows.Forms.Padding(3);
            this.tabNPC.Size = new System.Drawing.Size(344, 727);
            this.tabNPC.TabIndex = 1;
            this.tabNPC.Text = "Add NPC";

            // MainFormSimple
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1272, 777);
            this.Controls.Add(this.mapPanel);
            this.Controls.Add(this.btnZoomIn);
            this.Controls.Add(this.btnZoomOut);
            this.Controls.Add(this.tabMain);
            this.Controls.Add(this.statusStrip);
            this.Font = new System.Drawing.Font(".VnArial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "MainFormSimple";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Simple Map Tool - Auto Load";

            this.tabMain.ResumeLayout(false);
            this.tabTrap.ResumeLayout(false);
            this.tabNPC.ResumeLayout(false);
            this.grpGameFolder.ResumeLayout(false);
            this.grpGameFolder.PerformLayout();
            this.grpMapLoad.ResumeLayout(false);
            this.grpMapLoad.PerformLayout();
            this.grpMapInfo.ResumeLayout(false);
            this.grpCoordinates.ResumeLayout(false);
            this.grpCoordinates.PerformLayout();
            this.grpEntries.ResumeLayout(false);
            this.grpNpcInput.ResumeLayout(false);
            this.grpNpcInput.PerformLayout();
            this.grpNpcPreview.ResumeLayout(false);
            this.grpNpcEntries.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.picNpcPreview)).EndInit();
            this.statusStrip.ResumeLayout(false);
            this.statusStrip.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();
        }
    }
}
