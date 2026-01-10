using System;
using System.Runtime.InteropServices;

namespace MapTool.SPR
{
    /// <summary>
    /// SPR file header - 20 bytes
    /// Based on KSprite.h from client source
    /// </summary>
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct SprHeader
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
        public byte[] Comment;      // "SPR\0" - Magic number

        public ushort Width;        // Image width
        public ushort Height;       // Image height
        public ushort CenterX;      // Center X position
        public ushort CenterY;      // Center Y position
        public ushort Frames;       // Number of frames
        public ushort Colors;       // Number of colors in palette
        public ushort Directions;   // Number of directions (8 or 16)
        public ushort Interval;     // Time between frames (game ticks)

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
        public ushort[] Reserved;   // Reserved for future use

        public bool IsValid()
        {
            if (Comment == null || Comment.Length < 3)
                return false;
            return Comment[0] == 'S' && Comment[1] == 'P' && Comment[2] == 'R';
        }
    }

    /// <summary>
    /// RGB color palette entry - 3 bytes
    /// </summary>
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct Palette24
    {
        public byte B;  // Blue
        public byte G;  // Green
        public byte R;  // Red
    }

    /// <summary>
    /// Frame offset and length - 8 bytes
    /// </summary>
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct SpriteOffset
    {
        public uint Offset;  // Frame data offset from file start
        public uint Length;  // Frame data length in bytes
    }

    /// <summary>
    /// Frame header (before RLE data)
    /// </summary>
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct FrameHeader
    {
        public ushort Width;    // Frame width
        public ushort Height;   // Frame height
        public short OffsetX;   // X offset from top-left
        public short OffsetY;   // Y offset from top-left
    }

    /// <summary>
    /// Loaded SPR sprite data
    /// </summary>
    public class SpriteData
    {
        public SprHeader Header { get; set; }
        public Palette24[] Palette { get; set; }
        public SpriteOffset[] FrameOffsets { get; set; }
        public byte[] FrameData { get; set; }  // Raw frame data section

        // Cached decoded frames
        public byte[][] DecodedFrames { get; set; }

        public string FilePath { get; set; }

        public int Width => Header.Width;
        public int Height => Header.Height;
        public int FrameCount => Header.Frames;
        public int DirectionCount => Header.Directions;
        public int Interval => Header.Interval;
    }
}
