using System;
using System.Drawing;
using System.Windows.Forms;
using System.IO;

namespace MapTool.UI
{
    /// <summary>
    /// Theme types
    /// </summary>
    public enum ThemeType
    {
        Light,
        Dark
    }

    /// <summary>
    /// Modern theme manager for MapTool
    /// Provides professional Dark and Light color schemes
    /// </summary>
    public class ThemeManager
    {
        private static ThemeManager _instance;
        private ThemeType _currentTheme = ThemeType.Light;
        private string _configPath;

        public static ThemeManager Instance
        {
            get
            {
                if (_instance == null)
                    _instance = new ThemeManager();
                return _instance;
            }
        }

        private ThemeManager()
        {
            _configPath = Path.Combine(
                Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData),
                "MapTool",
                "theme.config"
            );
            LoadThemePreference();
        }

        public ThemeType CurrentTheme => _currentTheme;

        // ========== LIGHT MODE COLORS ==========
        public class LightTheme
        {
            // Background colors
            public static readonly Color FormBackground = Color.FromArgb(245, 245, 245);
            public static readonly Color PanelBackground = Color.White;
            public static readonly Color GroupBoxBackground = Color.FromArgb(250, 250, 250);

            // Text colors
            public static readonly Color PrimaryText = Color.FromArgb(33, 33, 33);
            public static readonly Color SecondaryText = Color.FromArgb(97, 97, 97);
            public static readonly Color DisabledText = Color.FromArgb(189, 189, 189);

            // Control colors
            public static readonly Color ControlBackground = Color.White;
            public static readonly Color ControlBorder = Color.FromArgb(218, 220, 224);
            public static readonly Color ControlHover = Color.FromArgb(232, 240, 254);
            public static readonly Color ControlFocus = Color.FromArgb(26, 115, 232);

            // Button colors
            public static readonly Color ButtonBackground = Color.FromArgb(26, 115, 232);
            public static readonly Color ButtonText = Color.White;
            public static readonly Color ButtonHover = Color.FromArgb(23, 103, 208);
            public static readonly Color ButtonSecondary = Color.FromArgb(95, 99, 104);

            // Special colors
            public static readonly Color AccentPrimary = Color.FromArgb(26, 115, 232); // Blue
            public static readonly Color AccentSuccess = Color.FromArgb(52, 168, 83); // Green
            public static readonly Color AccentWarning = Color.FromArgb(251, 188, 4); // Yellow
            public static readonly Color AccentError = Color.FromArgb(234, 67, 53); // Red

            // Map specific
            public static readonly Color MapBackground = Color.FromArgb(245, 245, 245);
            public static readonly Color GridLine = Color.FromArgb(224, 224, 224);
        }

        // ========== DARK MODE COLORS ==========
        public class DarkTheme
        {
            // Background colors
            public static readonly Color FormBackground = Color.FromArgb(32, 33, 36);
            public static readonly Color PanelBackground = Color.FromArgb(41, 42, 45);
            public static readonly Color GroupBoxBackground = Color.FromArgb(48, 49, 52);

            // Text colors
            public static readonly Color PrimaryText = Color.FromArgb(232, 234, 237);
            public static readonly Color SecondaryText = Color.FromArgb(154, 160, 166);
            public static readonly Color DisabledText = Color.FromArgb(95, 99, 104);

            // Control colors
            public static readonly Color ControlBackground = Color.FromArgb(48, 49, 52);
            public static readonly Color ControlBorder = Color.FromArgb(60, 64, 67);
            public static readonly Color ControlHover = Color.FromArgb(55, 57, 61);
            public static readonly Color ControlFocus = Color.FromArgb(138, 180, 248);

            // Button colors
            public static readonly Color ButtonBackground = Color.FromArgb(138, 180, 248);
            public static readonly Color ButtonText = Color.FromArgb(32, 33, 36);
            public static readonly Color ButtonHover = Color.FromArgb(165, 197, 251);
            public static readonly Color ButtonSecondary = Color.FromArgb(138, 143, 152);

            // Special colors
            public static readonly Color AccentPrimary = Color.FromArgb(138, 180, 248); // Blue
            public static readonly Color AccentSuccess = Color.FromArgb(129, 201, 149); // Green
            public static readonly Color AccentWarning = Color.FromArgb(252, 214, 103); // Yellow
            public static readonly Color AccentError = Color.FromArgb(242, 139, 130); // Red

            // Map specific
            public static readonly Color MapBackground = Color.FromArgb(32, 33, 36);
            public static readonly Color GridLine = Color.FromArgb(60, 64, 67);
        }

        /// <summary>
        /// Get current theme colors
        /// </summary>
        public Color GetFormBackground() => _currentTheme == ThemeType.Light ? LightTheme.FormBackground : DarkTheme.FormBackground;
        public Color GetPanelBackground() => _currentTheme == ThemeType.Light ? LightTheme.PanelBackground : DarkTheme.PanelBackground;
        public Color GetGroupBoxBackground() => _currentTheme == ThemeType.Light ? LightTheme.GroupBoxBackground : DarkTheme.GroupBoxBackground;
        public Color GetPrimaryText() => _currentTheme == ThemeType.Light ? LightTheme.PrimaryText : DarkTheme.PrimaryText;
        public Color GetSecondaryText() => _currentTheme == ThemeType.Light ? LightTheme.SecondaryText : DarkTheme.SecondaryText;
        public Color GetDisabledText() => _currentTheme == ThemeType.Light ? LightTheme.DisabledText : DarkTheme.DisabledText;
        public Color GetControlBackground() => _currentTheme == ThemeType.Light ? LightTheme.ControlBackground : DarkTheme.ControlBackground;
        public Color GetControlBorder() => _currentTheme == ThemeType.Light ? LightTheme.ControlBorder : DarkTheme.ControlBorder;
        public Color GetButtonBackground() => _currentTheme == ThemeType.Light ? LightTheme.ButtonBackground : DarkTheme.ButtonBackground;
        public Color GetButtonText() => _currentTheme == ThemeType.Light ? LightTheme.ButtonText : DarkTheme.ButtonText;
        public Color GetAccentPrimary() => _currentTheme == ThemeType.Light ? LightTheme.AccentPrimary : DarkTheme.AccentPrimary;
        public Color GetMapBackground() => _currentTheme == ThemeType.Light ? LightTheme.MapBackground : DarkTheme.MapBackground;

        /// <summary>
        /// Toggle between Dark and Light mode
        /// </summary>
        public void ToggleTheme()
        {
            _currentTheme = _currentTheme == ThemeType.Light ? ThemeType.Dark : ThemeType.Light;
            SaveThemePreference();
        }

        /// <summary>
        /// Set specific theme
        /// </summary>
        public void SetTheme(ThemeType theme)
        {
            _currentTheme = theme;
            SaveThemePreference();
        }

        /// <summary>
        /// Apply theme to a form and all its controls recursively
        /// </summary>
        public void ApplyTheme(Form form)
        {
            // Apply to form
            form.BackColor = GetFormBackground();
            form.ForeColor = GetPrimaryText();

            // Apply to all controls
            ApplyThemeToControl(form);
        }

        /// <summary>
        /// Apply theme to a control and its children recursively
        /// </summary>
        private void ApplyThemeToControl(Control control)
        {
            foreach (Control child in control.Controls)
            {
                // Skip map panel - it has its own rendering
                if (child.Name == "mapPanel")
                {
                    child.BackColor = GetMapBackground();
                    continue;
                }

                if (child is Button button)
                {
                    ApplyButtonTheme(button);
                }
                else if (child is TextBox textBox)
                {
                    ApplyTextBoxTheme(textBox);
                }
                else if (child is ListBox listBox)
                {
                    ApplyListBoxTheme(listBox);
                }
                else if (child is Label label)
                {
                    ApplyLabelTheme(label);
                }
                else if (child is GroupBox groupBox)
                {
                    ApplyGroupBoxTheme(groupBox);
                }
                else if (child is Panel panel)
                {
                    ApplyPanelTheme(panel);
                }
                else if (child is TabControl tabControl)
                {
                    ApplyTabControlTheme(tabControl);
                }
                else if (child is PictureBox pictureBox)
                {
                    pictureBox.BackColor = GetControlBackground();
                }
                else
                {
                    // Default for other controls
                    child.BackColor = GetControlBackground();
                    child.ForeColor = GetPrimaryText();
                }

                // Recursively apply to children
                if (child.HasChildren)
                {
                    ApplyThemeToControl(child);
                }
            }
        }

        private void ApplyButtonTheme(Button button)
        {
            button.BackColor = GetButtonBackground();
            button.ForeColor = GetButtonText();
            button.FlatStyle = FlatStyle.Flat;
            button.FlatAppearance.BorderSize = 0;
            button.FlatAppearance.BorderColor = GetButtonBackground();
            button.Cursor = Cursors.Hand;
        }

        private void ApplyTextBoxTheme(TextBox textBox)
        {
            textBox.BackColor = GetControlBackground();
            textBox.ForeColor = GetPrimaryText();
            textBox.BorderStyle = BorderStyle.FixedSingle;
        }

        private void ApplyListBoxTheme(ListBox listBox)
        {
            listBox.BackColor = GetControlBackground();
            listBox.ForeColor = GetPrimaryText();
            listBox.BorderStyle = BorderStyle.FixedSingle;
        }

        private void ApplyLabelTheme(Label label)
        {
            // Labels often have transparent backgrounds in the designer
            // Only set if it's not transparent
            if (label.BackColor != Color.Transparent)
            {
                label.BackColor = Color.Transparent;
            }
            label.ForeColor = GetPrimaryText();
        }

        private void ApplyGroupBoxTheme(GroupBox groupBox)
        {
            groupBox.BackColor = GetGroupBoxBackground();
            groupBox.ForeColor = GetPrimaryText();
        }

        private void ApplyPanelTheme(Panel panel)
        {
            panel.BackColor = GetPanelBackground();
            panel.ForeColor = GetPrimaryText();
        }

        private void ApplyTabControlTheme(TabControl tabControl)
        {
            tabControl.BackColor = GetPanelBackground();
            tabControl.ForeColor = GetPrimaryText();

            foreach (TabPage page in tabControl.TabPages)
            {
                page.BackColor = GetPanelBackground();
                page.ForeColor = GetPrimaryText();
            }
        }

        /// <summary>
        /// Save theme preference to disk
        /// </summary>
        private void SaveThemePreference()
        {
            try
            {
                string dir = Path.GetDirectoryName(_configPath);
                if (!Directory.Exists(dir))
                    Directory.CreateDirectory(dir);

                File.WriteAllText(_configPath, _currentTheme.ToString());
            }
            catch (Exception ex)
            {
                DebugLogger.Log($"Failed to save theme preference: {ex.Message}");
            }
        }

        /// <summary>
        /// Load theme preference from disk
        /// </summary>
        private void LoadThemePreference()
        {
            try
            {
                if (File.Exists(_configPath))
                {
                    string themeStr = File.ReadAllText(_configPath);
                    if (Enum.TryParse<ThemeType>(themeStr, out ThemeType theme))
                    {
                        _currentTheme = theme;
                    }
                }
            }
            catch (Exception ex)
            {
                DebugLogger.Log($"Failed to load theme preference: {ex.Message}");
            }
        }
    }
}
