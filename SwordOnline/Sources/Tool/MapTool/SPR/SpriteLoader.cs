using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Runtime.InteropServices;

namespace MapTool.SPR
{
    /// <summary>
    /// SPR file loader and decoder
    /// Supports RLE decompression and bitmap conversion
    /// </summary>
    public class SpriteLoader
    {
        /// <summary>
        /// Load SPR file from disk
        /// </summary>
        public static SpriteData Load(string filePath)
        {
            if (!File.Exists(filePath))
            {
                throw new FileNotFoundException($"SPR file not found: {filePath}");
            }

            using (FileStream fs = new FileStream(filePath, FileMode.Open, FileAccess.Read))
            using (BinaryReader reader = new BinaryReader(fs))
            {
                return LoadFromStream(reader, filePath);
            }
        }

        /// <summary>
        /// Load SPR from byte array (from PAK file)
        /// </summary>
        public static SpriteData LoadFromBytes(byte[] data, string fileName = "")
        {
            if (data == null || data.Length == 0)
            {
                throw new ArgumentException("SPR data is null or empty");
            }

            using (MemoryStream ms = new MemoryStream(data))
            using (BinaryReader reader = new BinaryReader(ms))
            {
                return LoadFromStream(reader, fileName);
            }
        }

        /// <summary>
        /// Load SPR from stream
        /// </summary>
        public static SpriteData LoadFromStream(BinaryReader reader, string filePath = "")
        {
            SpriteData sprite = new SpriteData();
            sprite.FilePath = filePath;

            // Read header
            sprite.Header = ReadStructure<SprHeader>(reader);

            if (!sprite.Header.IsValid())
            {
                throw new InvalidDataException($"Invalid SPR file: {filePath}");
            }

            // Read palette
            sprite.Palette = new Palette24[sprite.Header.Colors];
            for (int i = 0; i < sprite.Header.Colors; i++)
            {
                sprite.Palette[i] = ReadStructure<Palette24>(reader);
            }

            // Read frame offsets
            sprite.FrameOffsets = new SpriteOffset[sprite.Header.Frames];
            for (int i = 0; i < sprite.Header.Frames; i++)
            {
                sprite.FrameOffsets[i] = ReadStructure<SpriteOffset>(reader);
            }

            // Read frame data (rest of file)
            long dataStart = reader.BaseStream.Position;
            long dataLength = reader.BaseStream.Length - dataStart;
            sprite.FrameData = reader.ReadBytes((int)dataLength);

            return sprite;
        }

        /// <summary>
        /// Decode specific frame to indexed pixel array
        /// </summary>
        public static byte[] DecodeFrame(SpriteData sprite, int frameIndex)
        {
            if (frameIndex < 0 || frameIndex >= sprite.FrameCount)
            {
                throw new ArgumentOutOfRangeException(nameof(frameIndex));
            }

            SpriteOffset offset = sprite.FrameOffsets[frameIndex];

            // All offsets in SPR files are RELATIVE to the start of frame data section
            // No conversion needed - use offset directly
            long posInFrameData = offset.Offset;

            if (posInFrameData < 0 || posInFrameData >= sprite.FrameData.Length)
            {
                string error = $"Invalid frame offset: offset={offset.Offset}, FrameData.Length={sprite.FrameData.Length}";
                throw new ArgumentOutOfRangeException(nameof(frameIndex), error);
            }

            using (MemoryStream ms = new MemoryStream(sprite.FrameData))
            using (BinaryReader reader = new BinaryReader(ms))
            {
                ms.Seek(posInFrameData, SeekOrigin.Begin);

                // Read frame header
                FrameHeader frameHeader = ReadStructure<FrameHeader>(reader);

                DebugLogger.Log($"                           Frame size: {frameHeader.Width}x{frameHeader.Height}");

                // Decode RLE data
                return DecodeRLE(reader, frameHeader.Width, frameHeader.Height);
            }
        }

        /// <summary>
        /// Decode RLE compressed sprite data
        /// RLE format (from client DrawSpriteMP.inc): [Count][Alpha][Optional Pixel Data]
        /// - Alpha == 0: Transparent run (skip 'count' pixels, no data follows)
        /// - Alpha > 0: Opaque run (read 'count' pixel indices)
        /// </summary>
        private static byte[] DecodeRLE(BinaryReader reader, int width, int height)
        {
            byte[] pixels = new byte[width * height];

            // Initialize all pixels to 0 (transparent)
            for (int i = 0; i < pixels.Length; i++)
            {
                pixels[i] = 0;
            }

            // Line-based RLE decoding
            for (int y = 0; y < height; y++)
            {
                int lineStart = y * width;
                int x = 0;

                while (x < width && reader.BaseStream.Position < reader.BaseStream.Length)
                {
                    byte count = reader.ReadByte();

                    // Check for line terminator (count == 0 means end of line)
                    if (count == 0)
                    {
                        break;
                    }

                    byte alpha = reader.ReadByte();

                    if (alpha == 0)
                    {
                        // Transparent run - skip 'count' pixels (leave as 0)
                        x += count;
                    }
                    else
                    {
                        // Opaque run - read 'count' pixel indices
                        for (int i = 0; i < count && x < width; i++)
                        {
                            pixels[lineStart + x] = reader.ReadByte();
                            x++;
                        }
                    }
                }
            }

            return pixels;
        }

        /// <summary>
        /// Convert frame to Bitmap using palette
        /// </summary>
        public static Bitmap FrameToBitmap(SpriteData sprite, int frameIndex)
        {
            byte[] pixels = DecodeFrame(sprite, frameIndex);

            // Get frame dimensions from frame header
            SpriteOffset offset = sprite.FrameOffsets[frameIndex];

            // All offsets are relative to frame data start
            long posInFrameData = offset.Offset;

            FrameHeader frameHeader;
            using (MemoryStream ms = new MemoryStream(sprite.FrameData))
            using (BinaryReader reader = new BinaryReader(ms))
            {
                ms.Seek(posInFrameData, SeekOrigin.Begin);
                frameHeader = ReadStructure<FrameHeader>(reader);
            }

            int width = frameHeader.Width;
            int height = frameHeader.Height;

            // Create bitmap
            Bitmap bmp = new Bitmap(width, height, PixelFormat.Format24bppRgb);

            BitmapData bmpData = bmp.LockBits(
                new Rectangle(0, 0, width, height),
                ImageLockMode.WriteOnly,
                PixelFormat.Format24bppRgb);

            unsafe
            {
                byte* ptr = (byte*)bmpData.Scan0;
                int stride = bmpData.Stride;

                for (int y = 0; y < height; y++)
                {
                    for (int x = 0; x < width; x++)
                    {
                        int pixelIndex = y * width + x;
                        byte colorIndex = pixels[pixelIndex];

                        // Handle transparent color (index 0)
                        if (colorIndex == 0)
                        {
                            // Magenta for transparency
                            ptr[y * stride + x * 3 + 0] = 255; // B
                            ptr[y * stride + x * 3 + 1] = 0;   // G
                            ptr[y * stride + x * 3 + 2] = 255; // R
                        }
                        else if (colorIndex < sprite.Palette.Length)
                        {
                            Palette24 color = sprite.Palette[colorIndex];
                            ptr[y * stride + x * 3 + 0] = color.B;
                            ptr[y * stride + x * 3 + 1] = color.G;
                            ptr[y * stride + x * 3 + 2] = color.R;
                        }
                    }
                }
            }

            bmp.UnlockBits(bmpData);

            // Make magenta pixels transparent
            bmp.MakeTransparent(Color.Magenta);

            return bmp;
        }

        /// <summary>
        /// Helper to read structure from binary stream
        /// </summary>
        private static T ReadStructure<T>(BinaryReader reader) where T : struct
        {
            int size = Marshal.SizeOf<T>();
            byte[] bytes = reader.ReadBytes(size);
            GCHandle handle = GCHandle.Alloc(bytes, GCHandleType.Pinned);
            try
            {
                return Marshal.PtrToStructure<T>(handle.AddrOfPinnedObject());
            }
            finally
            {
                handle.Free();
            }
        }
    }
}
