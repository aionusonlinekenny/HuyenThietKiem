using System;
using System.Windows.Forms;
using System.IO;
using MapTool;

namespace PakExtractTool
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            // Set up global exception handlers FIRST
            Application.ThreadException += Application_ThreadException;
            AppDomain.CurrentDomain.UnhandledException += CurrentDomain_UnhandledException;

            try
            {
                // Initialize DebugLogger early (before MainForm constructor)
                DebugLogger.Initialize();
                DebugLogger.Log("=== PAK Extract Tool Starting ===");
                DebugLogger.Log($"Executable: {System.Reflection.Assembly.GetExecutingAssembly().Location}");
                DebugLogger.Log($"Working Directory: {Environment.CurrentDirectory}");

                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);

                DebugLogger.Log("Creating MainForm...");
                Application.Run(new MainForm());
                DebugLogger.Log("Application exited normally");
            }
            catch (Exception ex)
            {
                HandleFatalError("Fatal error in Main()", ex);
            }
        }

        private static void Application_ThreadException(object sender, System.Threading.ThreadExceptionEventArgs e)
        {
            HandleFatalError("Unhandled thread exception", e.Exception);
        }

        private static void CurrentDomain_UnhandledException(object sender, UnhandledExceptionEventArgs e)
        {
            if (e.ExceptionObject is Exception ex)
            {
                HandleFatalError("Unhandled domain exception", ex);
            }
        }

        private static void HandleFatalError(string context, Exception ex)
        {
            try
            {
                // Try to log to debug logger
                DebugLogger.Log($"💥 FATAL ERROR: {context}");
                DebugLogger.Log($"   Exception: {ex.GetType().FullName}");
                DebugLogger.Log($"   Message: {ex.Message}");
                DebugLogger.Log($"   Stack trace: {ex.StackTrace}");

                if (ex.InnerException != null)
                {
                    DebugLogger.Log($"   Inner exception: {ex.InnerException.GetType().FullName}");
                    DebugLogger.Log($"   Inner message: {ex.InnerException.Message}");
                    DebugLogger.Log($"   Inner stack: {ex.InnerException.StackTrace}");
                }

                // Also write to a crash log file
                string crashLogPath = Path.Combine(
                    Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location),
                    $"PakExtractTool_Crash_{DateTime.Now:yyyyMMdd_HHmmss}.log");

                File.WriteAllText(crashLogPath,
                    $"PAK Extract Tool Crash Report\n" +
                    $"Time: {DateTime.Now}\n" +
                    $"Context: {context}\n\n" +
                    $"Exception: {ex.GetType().FullName}\n" +
                    $"Message: {ex.Message}\n\n" +
                    $"Stack Trace:\n{ex.StackTrace}\n\n" +
                    (ex.InnerException != null ?
                        $"Inner Exception: {ex.InnerException.GetType().FullName}\n" +
                        $"Inner Message: {ex.InnerException.Message}\n\n" +
                        $"Inner Stack Trace:\n{ex.InnerException.StackTrace}\n" : ""));

                MessageBox.Show(
                    $"{context}\n\n" +
                    $"Exception: {ex.GetType().FullName}\n" +
                    $"Message: {ex.Message}\n\n" +
                    $"Stack trace:\n{ex.StackTrace}\n\n" +
                    $"Crash log saved to:\n{crashLogPath}\n\n" +
                    $"Debug log: {DebugLogger.GetLogFilePath()}",
                    "Fatal Error",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Error);
            }
            catch
            {
                // Last resort - show simple message
                MessageBox.Show(
                    $"Fatal error: {ex.Message}",
                    "Error",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Error);
            }

            Environment.Exit(1);
        }
    }
}
