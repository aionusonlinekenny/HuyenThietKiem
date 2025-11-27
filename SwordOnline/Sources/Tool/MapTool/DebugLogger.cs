using System;
using System.IO;
using System.Text;

namespace MapTool
{
    /// <summary>
    /// Debug logger that writes to file for diagnosing issues
    /// </summary>
    public static class DebugLogger
    {
        private static string _logFilePath;
        private static readonly object _lockObj = new object();
        private static bool _initialized = false;

        /// <summary>
        /// Initialize logger - creates log file in same directory as exe
        /// </summary>
        public static void Initialize()
        {
            if (_initialized)
                return;

            try
            {
                // Create log file in exe directory
                string exeDir = AppDomain.CurrentDomain.BaseDirectory;
                string timestamp = DateTime.Now.ToString("yyyyMMdd_HHmmss");
                _logFilePath = Path.Combine(exeDir, $"MapTool_Debug_{timestamp}.log");

                // Write header
                lock (_lockObj)
                {
                    File.WriteAllText(_logFilePath,
                        "=".PadRight(80, '=') + "\n" +
                        $"MapTool Debug Log - {DateTime.Now:yyyy-MM-dd HH:mm:ss}\n" +
                        "=".PadRight(80, '=') + "\n\n",
                        Encoding.UTF8);
                }

                _initialized = true;
                Log($"✓ Debug log initialized: {_logFilePath}");
            }
            catch (Exception ex)
            {
                // Fallback: write to temp
                _logFilePath = Path.Combine(Path.GetTempPath(), $"MapTool_Debug_{DateTime.Now:yyyyMMdd_HHmmss}.log");
                _initialized = true;
                Log($"⚠ Log file created in temp: {_logFilePath}");
                Log($"Error creating log in exe dir: {ex.Message}");
            }
        }

        /// <summary>
        /// Write log message
        /// </summary>
        public static void Log(string message)
        {
            if (!_initialized)
                Initialize();

            try
            {
                lock (_lockObj)
                {
                    string timestamp = DateTime.Now.ToString("HH:mm:ss.fff");
                    string logLine = $"[{timestamp}] {message}\n";

                    File.AppendAllText(_logFilePath, logLine, Encoding.UTF8);

                    // Also write to console for debugging
                    Console.WriteLine(message);
                }
            }
            catch
            {
                // Ignore errors writing to log
            }
        }

        /// <summary>
        /// Write separator line
        /// </summary>
        public static void LogSeparator()
        {
            Log("-".PadRight(80, '-'));
        }

        /// <summary>
        /// Get log file path
        /// </summary>
        public static string GetLogFilePath()
        {
            return _logFilePath;
        }
    }
}
