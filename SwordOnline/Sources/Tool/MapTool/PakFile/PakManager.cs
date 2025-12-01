using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace MapTool.PakFile
{
    /// <summary>
    /// Manages multiple PAK files and provides transparent file access
    /// Mimics KPakList functionality from client
    /// </summary>
    public class PakManager : IDisposable
    {
        private List<PakFileReader> _pakFiles;
        private string _basePath;

        public PakManager(string clientBasePath)
        {
            _basePath = clientBasePath;
            _pakFiles = new List<PakFileReader>();
        }

        /// <summary>
        /// Load all PAK files from client data folders
        /// </summary>
        public void LoadPakFiles()
        {
            DebugLogger.Log($"         Loading PAK files from: {_basePath}");

            // Load from data/ folder
            string dataPath = Path.Combine(_basePath, "data");
            if (Directory.Exists(dataPath))
            {
                LoadPakFilesFromDirectory(dataPath);
            }

            // Load from data2/ folder
            string data2Path = Path.Combine(_basePath, "data2");
            if (Directory.Exists(data2Path))
            {
                LoadPakFilesFromDirectory(data2Path);
            }

            DebugLogger.Log($"         ✓ Loaded {_pakFiles.Count} PAK files");
        }

        private void LoadPakFilesFromDirectory(string directory)
        {
            string[] pakFiles = Directory.GetFiles(directory, "*.pak", SearchOption.TopDirectoryOnly);

            foreach (string pakFile in pakFiles)
            {
                try
                {
                    var reader = new PakFileReader(pakFile);
                    _pakFiles.Add(reader);
                    DebugLogger.Log($"            ✓ Loaded PAK: {Path.GetFileName(pakFile)}");
                }
                catch (Exception ex)
                {
                    DebugLogger.Log($"            ✗ Failed to load PAK {Path.GetFileName(pakFile)}: {ex.Message}");
                }
            }
        }

        /// <summary>
        /// Try to read a file from PAK archives
        /// Returns null if not found
        /// </summary>
        public byte[] ReadFile(string fileName)
        {
            if (string.IsNullOrEmpty(fileName))
                return null;

            // Normalize path
            fileName = FileNameHasher.NormalizePath(fileName);

            // Search all loaded PAK files
            foreach (var pakFile in _pakFiles)
            {
                try
                {
                    byte[] data = pakFile.ReadFile(fileName);
                    if (data != null)
                    {
                        return data;
                    }
                }
                catch
                {
                    // Continue to next PAK file
                }
            }

            return null;
        }

        /// <summary>
        /// Check if file exists in any PAK
        /// </summary>
        public bool FileExists(string fileName)
        {
            if (string.IsNullOrEmpty(fileName))
                return false;

            fileName = FileNameHasher.NormalizePath(fileName);

            foreach (var pakFile in _pakFiles)
            {
                if (pakFile.FileExists(fileName))
                {
                    return true;
                }
            }

            return false;
        }

        /// <summary>
        /// Try to read file from PAK, fall back to disk if not found
        /// </summary>
        public byte[] ReadFileWithFallback(string fileName, string diskBasePath)
        {
            // Try PAK first
            byte[] pakData = ReadFile(fileName);
            if (pakData != null)
            {
                return pakData;
            }

            // Fall back to disk
            string diskPath = Path.Combine(diskBasePath, fileName.TrimStart('\\'));
            if (File.Exists(diskPath))
            {
                return File.ReadAllBytes(diskPath);
            }

            return null;
        }

        public void Dispose()
        {
            foreach (var pakFile in _pakFiles)
            {
                pakFile?.Dispose();
            }
            _pakFiles.Clear();
        }
    }
}
