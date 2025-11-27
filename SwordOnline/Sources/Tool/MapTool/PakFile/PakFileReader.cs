using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Linq;

namespace MapTool.PakFile
{
    /// <summary>
    /// Reads files from .pak archive files
    /// Based on XPackFile.cpp format
    /// </summary>
    public class PakFileReader : IDisposable
    {
        private string _pakFilePath;
        private FileStream _pakStream;
        private PakHeader _header;
        private Dictionary<uint, PakIndexEntry> _fileIndex;  // ID -> Entry
        private Dictionary<string, uint> _nameToId;          // Filename -> ID

        /// <summary>
        /// Pak file header structure (32 bytes)
        /// struct XPackFileHeader
        /// </summary>
        private class PakHeader
        {
            public uint Signature;          // 'PACK' = 0x4b434150
            public uint FileCount;          // Number of files
            public uint IndexTableOffset;   // Offset to index table
            public uint DataOffset;         // Offset to data section
            public uint Crc32;              // Checksum
            // 12 bytes reserved

            public const uint SIGNATURE = 0x4b434150; // 'PACK'
        }

        /// <summary>
        /// Index entry structure (16 bytes per file)
        /// struct XPackIndexInfo
        /// </summary>
        private class PakIndexEntry
        {
            public uint Id;                 // File ID (hash)
            public uint Offset;             // File offset in pak
            public int Size;                // Original (uncompressed) size
            public int CompressSizeFlag;    // Compressed size + compression method
                                            // High byte = method, low 3 bytes = compressed size

            public int CompressedSize => CompressSizeFlag & 0x00FFFFFF;
            public CompressionMethod Method => (CompressionMethod)(CompressSizeFlag & unchecked((int)0xFF000000));
            public bool IsCompressed => Method != CompressionMethod.None;
        }

        /// <summary>
        /// Compression methods (from XPACK_METHOD enum)
        /// </summary>
        private enum CompressionMethod
        {
            None    = 0x00000000,   // No compression
            UCL     = 0x01000000,   // UCL compression (ucl_nrv2b_decompress_8)
            BZIP2   = 0x02000000,   // BZIP2 compression
            Frame   = 0x10000000,   // Frame compression (for sprites)
            UCL2    = 0x20000000,   // UCL2 compression
        }

        public PakFileReader(string pakFilePath)
        {
            _pakFilePath = pakFilePath;
            _fileIndex = new Dictionary<uint, PakIndexEntry>();
            _nameToId = new Dictionary<string, uint>(StringComparer.OrdinalIgnoreCase);

            Open();
        }

        private void Open()
        {
            if (!File.Exists(_pakFilePath))
            {
                throw new FileNotFoundException($"Pak file not found: {_pakFilePath}");
            }

            _pakStream = new FileStream(_pakFilePath, FileMode.Open, FileAccess.Read, FileShare.Read);

            // Read header
            _header = ReadHeader();

            // Read index table
            ReadIndexTable();

            // Load filename → ID mapping from .txt file
            LoadFilenameIndex();
        }

        private PakHeader ReadHeader()
        {
            if (_pakStream.Length < 32)
            {
                throw new Exception("Invalid pak file: too small");
            }

            _pakStream.Seek(0, SeekOrigin.Begin);
            BinaryReader reader = new BinaryReader(_pakStream, Encoding.ASCII, true);

            PakHeader header = new PakHeader
            {
                Signature = reader.ReadUInt32(),
                FileCount = reader.ReadUInt32(),
                IndexTableOffset = reader.ReadUInt32(),
                DataOffset = reader.ReadUInt32(),
                Crc32 = reader.ReadUInt32()
            };

            // Skip reserved 12 bytes
            reader.ReadBytes(12);

            // Validate header
            if (header.Signature != PakHeader.SIGNATURE)
            {
                throw new Exception($"Invalid pak signature: 0x{header.Signature:X8}, expected 0x{PakHeader.SIGNATURE:X8}");
            }

            if (header.FileCount == 0 || header.FileCount > 1000000)
            {
                throw new Exception($"Invalid file count: {header.FileCount}");
            }

            if (header.IndexTableOffset < 32 || header.IndexTableOffset >= _pakStream.Length)
            {
                throw new Exception($"Invalid index table offset: {header.IndexTableOffset}");
            }

            return header;
        }

        private void ReadIndexTable()
        {
            _pakStream.Seek(_header.IndexTableOffset, SeekOrigin.Begin);
            BinaryReader reader = new BinaryReader(_pakStream, Encoding.ASCII, true);

            for (uint i = 0; i < _header.FileCount; i++)
            {
                PakIndexEntry entry = new PakIndexEntry
                {
                    Id = reader.ReadUInt32(),
                    Offset = reader.ReadUInt32(),
                    Size = reader.ReadInt32(),
                    CompressSizeFlag = reader.ReadInt32()
                };

                _fileIndex[entry.Id] = entry;
            }
        }

        private void LoadFilenameIndex()
        {
            // Load from .pak.txt file if exists
            string txtPath = _pakFilePath + ".txt";
            if (!File.Exists(txtPath))
            {
                return; // No index file, ID-only lookup
            }

            try
            {
                string[] lines = File.ReadAllLines(txtPath, Encoding.GetEncoding("GB2312"));

                foreach (string line in lines.Skip(2)) // Skip header lines
                {
                    string[] parts = line.Split('\t');
                    if (parts.Length < 4) continue;

                    // Parse: Index, ID(hex), Time, FileName, ...
                    if (uint.TryParse(parts[1], System.Globalization.NumberStyles.HexNumber, null, out uint fileId))
                    {
                        string fileName = parts[3].Trim();
                        if (!string.IsNullOrEmpty(fileName))
                        {
                            // Normalize path separators
                            fileName = fileName.Replace('/', '\\');
                            _nameToId[fileName] = fileId;
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                // Index file parse error - continue without filename lookup
                Console.WriteLine($"Warning: Failed to load pak index: {ex.Message}");
            }
        }

        /// <summary>
        /// Check if file exists in pak (by name or ID)
        /// </summary>
        public bool FileExists(string fileName)
        {
            uint fileId = GetFileId(fileName);
            return fileId != 0 && _fileIndex.ContainsKey(fileId);
        }

        /// <summary>
        /// Check if file exists in pak (by ID)
        /// </summary>
        public bool FileExists(uint fileId)
        {
            return _fileIndex.ContainsKey(fileId);
        }

        /// <summary>
        /// Read file from pak
        /// </summary>
        public byte[] ReadFile(string fileName)
        {
            uint fileId = GetFileId(fileName);
            if (fileId == 0)
            {
                return null;
            }

            return ReadFile(fileId);
        }

        /// <summary>
        /// Read file from pak by ID
        /// </summary>
        public byte[] ReadFile(uint fileId)
        {
            if (!_fileIndex.ContainsKey(fileId))
            {
                return null;
            }

            PakIndexEntry entry = _fileIndex[fileId];

            // Read compressed data
            _pakStream.Seek(entry.Offset, SeekOrigin.Begin);
            byte[] compressedData = new byte[entry.CompressedSize];
            _pakStream.Read(compressedData, 0, entry.CompressedSize);

            // Decompress if needed
            if (entry.IsCompressed)
            {
                return Decompress(compressedData, entry.Size, entry.Method);
            }

            return compressedData;
        }

        /// <summary>
        /// Get file ID from filename
        /// </summary>
        private uint GetFileId(string fileName)
        {
            if (string.IsNullOrEmpty(fileName))
            {
                return 0;
            }

            // Normalize path (backslashes, like game engine)
            fileName = FileNameHasher.NormalizePath(fileName);

            // Method 1: Try direct lookup from .pak.txt index
            if (_nameToId.ContainsKey(fileName))
            {
                return _nameToId[fileName];
            }

            // Try variations with/without leading slash
            string withoutSlash = fileName.TrimStart('\\');
            if (_nameToId.ContainsKey(withoutSlash))
            {
                return _nameToId[withoutSlash];
            }

            string withSlash = "\\" + withoutSlash;
            if (_nameToId.ContainsKey(withSlash))
            {
                return _nameToId[withSlash];
            }

            // Method 2: Calculate hash using game's algorithm (fallback)
            // This ensures we can find files even if .pak.txt is missing or incomplete
            uint calculatedId = FileNameHasher.CalculateFileId(fileName);
            if (_fileIndex.ContainsKey(calculatedId))
            {
                // Found by calculated hash! Cache it for future lookups
                _nameToId[fileName] = calculatedId;
                return calculatedId;
            }

            // Try variations with calculated hash
            uint withoutSlashId = FileNameHasher.CalculateFileId(withoutSlash);
            if (_fileIndex.ContainsKey(withoutSlashId))
            {
                _nameToId[withoutSlash] = withoutSlashId;
                return withoutSlashId;
            }

            uint withSlashId = FileNameHasher.CalculateFileId(withSlash);
            if (_fileIndex.ContainsKey(withSlashId))
            {
                _nameToId[withSlash] = withSlashId;
                return withSlashId;
            }

            return 0;
        }

        /// <summary>
        /// Decompress file data
        /// </summary>
        private byte[] Decompress(byte[] compressedData, int uncompressedSize, CompressionMethod method)
        {
            switch (method)
            {
                case CompressionMethod.None:
                    return compressedData;

                case CompressionMethod.UCL:
                case CompressionMethod.UCL2:
                    return DecompressUCL(compressedData, uncompressedSize);

                case CompressionMethod.BZIP2:
                    throw new NotImplementedException("BZIP2 decompression not implemented");

                default:
                    throw new NotSupportedException($"Unsupported compression method: {method}");
            }
        }

        /// <summary>
        /// Decompress UCL data using managed C# implementation
        /// </summary>
        private byte[] DecompressUCL(byte[] compressedData, int uncompressedSize)
        {
            // Allocate output buffer
            byte[] decompressedData = new byte[uncompressedSize];

            // Decompress using managed UCL NRV2B implementation
            int result = UclDecompressor.DecompressNrv2b(compressedData, decompressedData);

            if (result != UclDecompressor.UCL_E_OK)
            {
                throw new Exception(
                    $"UCL decompression failed: {UclDecompressor.GetErrorMessage(result)}\n" +
                    $"Compressed size: {compressedData.Length}, Expected output: {uncompressedSize}");
            }

            return decompressedData;
        }

        /// <summary>
        /// Get file information
        /// </summary>
        public PakFileInfo GetFileInfo(string fileName)
        {
            uint fileId = GetFileId(fileName);
            if (fileId == 0 || !_fileIndex.ContainsKey(fileId))
            {
                return null;
            }

            PakIndexEntry entry = _fileIndex[fileId];
            return new PakFileInfo
            {
                FileName = fileName,
                FileId = fileId,
                Size = entry.Size,
                CompressedSize = entry.CompressedSize,
                IsCompressed = entry.IsCompressed,
                CompressionMethod = entry.Method.ToString()
            };
        }

        /// <summary>
        /// Get all filenames known from .pak.txt index
        /// </summary>
        public List<string> GetAllFileNames()
        {
            return _nameToId.Keys.ToList();
        }

        /// <summary>
        /// Get pak statistics
        /// </summary>
        public PakStatistics GetStatistics()
        {
            var stats = new PakStatistics
            {
                TotalFiles = _header.FileCount,
                CompressedFiles = 0,
                UncompressedFiles = 0,
                TotalSize = 0,
                CompressedSize = 0,
                FilesWithoutNames = (uint)(_header.FileCount - _nameToId.Count)
            };

            foreach (var entry in _fileIndex.Values)
            {
                stats.TotalSize += entry.Size;
                stats.CompressedSize += entry.CompressedSize;

                if (entry.IsCompressed)
                    stats.CompressedFiles++;
                else
                    stats.UncompressedFiles++;
            }

            return stats;
        }

        public void Dispose()
        {
            if (_pakStream != null)
            {
                _pakStream.Dispose();
                _pakStream = null;
            }
        }
    }

    /// <summary>
    /// File information from pak
    /// </summary>
    public class PakFileInfo
    {
        public string FileName { get; set; }
        public uint FileId { get; set; }
        public int Size { get; set; }
        public int CompressedSize { get; set; }
        public bool IsCompressed { get; set; }
        public string CompressionMethod { get; set; }

        public double CompressionRatio => Size > 0 ? (double)CompressedSize / Size * 100 : 0;

        public override string ToString()
        {
            return $"{FileName} - {Size} bytes" +
                   (IsCompressed ? $" (compressed {CompressedSize} bytes, {CompressionRatio:F1}%)" : "");
        }
    }

    /// <summary>
    /// Pak file statistics
    /// </summary>
    public class PakStatistics
    {
        public uint TotalFiles { get; set; }
        public uint CompressedFiles { get; set; }
        public uint UncompressedFiles { get; set; }
        public uint FilesWithoutNames { get; set; }
        public long TotalSize { get; set; }
        public long CompressedSize { get; set; }

        public double AverageCompressionRatio => TotalSize > 0 ? (double)CompressedSize / TotalSize * 100 : 0;

        public override string ToString()
        {
            return $"Pak Statistics:\n" +
                   $"  Total Files: {TotalFiles}\n" +
                   $"  Compressed: {CompressedFiles}\n" +
                   $"  Uncompressed: {UncompressedFiles}\n" +
                   $"  Total Size: {TotalSize:N0} bytes\n" +
                   $"  Compressed Size: {CompressedSize:N0} bytes\n" +
                   $"  Avg Compression: {AverageCompressionRatio:F1}%";
        }
    }
}
