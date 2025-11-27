using System;

namespace MapTool.PakFile
{
    /// <summary>
    /// UCL NRV2B decompression implementation
    /// Ported from UCL library (GPL licensed)
    /// Copyright (C) 1996-2002 Markus Franz Xaver Johannes Oberhumer
    ///
    /// This implementation is based on ucl_nrv2b_decompress_8 from the UCL library
    /// UCL library: http://www.oberhumer.com/opensource/ucl/
    /// </summary>
    public static class UclDecompressor
    {
        // Error codes from UCL library
        public const int UCL_E_OK = 0;
        public const int UCL_E_ERROR = -1;
        public const int UCL_E_INPUT_OVERRUN = -201;
        public const int UCL_E_OUTPUT_OVERRUN = -202;
        public const int UCL_E_LOOKBEHIND_OVERRUN = -203;
        public const int UCL_E_EOF_NOT_FOUND = -204;
        public const int UCL_E_INPUT_NOT_CONSUMED = -205;

        /// <summary>
        /// Decompress UCL NRV2B compressed data
        /// Based on ucl_nrv2b_decompress_8 algorithm
        /// </summary>
        /// <param name="src">Compressed data</param>
        /// <param name="dst">Output buffer (must be pre-allocated to uncompressed size)</param>
        /// <returns>Error code (UCL_E_OK on success)</returns>
        public static int DecompressNrv2b(byte[] src, byte[] dst)
        {
            if (src == null || dst == null)
                return UCL_E_ERROR;

            uint srcLen = (uint)src.Length;
            uint dstLen = (uint)dst.Length;

            uint bb = 0;        // Bit buffer
            uint ilen = 0;      // Input position
            uint olen = 0;      // Output position
            uint lastMOff = 1;  // Last match offset

            while (true)
            {
                uint mOff, mLen;

                // Copy literal bytes (while getbit returns 1)
                while (GetBit8(ref bb, src, ref ilen) != 0)
                {
                    if (ilen >= srcLen)
                        return UCL_E_INPUT_OVERRUN;
                    if (olen >= dstLen)
                        return UCL_E_OUTPUT_OVERRUN;

                    dst[olen++] = src[ilen++];
                }

                // Read match offset
                mOff = 1;
                do
                {
                    mOff = mOff * 2 + GetBit8(ref bb, src, ref ilen);
                    if (ilen >= srcLen)
                        return UCL_E_INPUT_OVERRUN;
                    if (mOff > 0xffffff + 3)
                        return UCL_E_LOOKBEHIND_OVERRUN;
                } while (GetBit8(ref bb, src, ref ilen) == 0);

                // Check for end marker
                if (mOff == 2)
                {
                    mOff = lastMOff;
                }
                else
                {
                    if (ilen >= srcLen)
                        return UCL_E_INPUT_OVERRUN;

                    mOff = (mOff - 3) * 256 + src[ilen++];
                    if (mOff == 0xffffffff)
                        break; // End of compressed data

                    lastMOff = ++mOff;
                }

                // Read match length
                mLen = GetBit8(ref bb, src, ref ilen);
                mLen = mLen * 2 + GetBit8(ref bb, src, ref ilen);

                if (mLen == 0)
                {
                    mLen++;
                    do
                    {
                        mLen = mLen * 2 + GetBit8(ref bb, src, ref ilen);
                        if (ilen >= srcLen)
                            return UCL_E_INPUT_OVERRUN;
                        if (mLen >= dstLen)
                            return UCL_E_OUTPUT_OVERRUN;
                    } while (GetBit8(ref bb, src, ref ilen) == 0);

                    mLen += 2;
                }

                mLen += (mOff > 0xd00) ? 1u : 0u;

                // Validate bounds
                if (olen + mLen > dstLen)
                    return UCL_E_OUTPUT_OVERRUN;
                if (mOff > olen)
                    return UCL_E_LOOKBEHIND_OVERRUN;

                // Copy match from lookbehind buffer
                uint mPos = olen - mOff;
                dst[olen++] = dst[mPos++];
                while (mLen > 0)
                {
                    dst[olen++] = dst[mPos++];
                    mLen--;
                }
            }

            // Check if all input was consumed
            return (ilen == srcLen) ? UCL_E_OK :
                   (ilen < srcLen) ? UCL_E_INPUT_NOT_CONSUMED : UCL_E_INPUT_OVERRUN;
        }

        /// <summary>
        /// Get one bit from compressed stream (8-bit version)
        /// Implements getbit_8 macro from UCL library
        /// </summary>
        /// <param name="bb">Bit buffer (modified)</param>
        /// <param name="src">Source data</param>
        /// <param name="ilen">Input position (modified)</param>
        /// <returns>0 or 1</returns>
        private static uint GetBit8(ref uint bb, byte[] src, ref uint ilen)
        {
            // C macro: #define getbit_8(bb, src, ilen)
            //   (((bb = bb & 0x7f ? bb*2 : ((unsigned)src[ilen++]*2+1)) >> 8) & 1)

            if ((bb & 0x7f) != 0)
            {
                bb = bb * 2;
            }
            else
            {
                bb = (uint)src[ilen++] * 2 + 1;
            }

            return (bb >> 8) & 1;
        }

        /// <summary>
        /// Get error message for UCL error code
        /// </summary>
        public static string GetErrorMessage(int errorCode)
        {
            switch (errorCode)
            {
                case UCL_E_OK:
                    return "Success";
                case UCL_E_ERROR:
                    return "General error";
                case UCL_E_INPUT_OVERRUN:
                    return "Input overrun (compressed data truncated)";
                case UCL_E_OUTPUT_OVERRUN:
                    return "Output overrun (decompressed size mismatch)";
                case UCL_E_LOOKBEHIND_OVERRUN:
                    return "Lookbehind overrun (corrupted compressed data)";
                case UCL_E_EOF_NOT_FOUND:
                    return "EOF marker not found";
                case UCL_E_INPUT_NOT_CONSUMED:
                    return "Input not fully consumed (extra data after end)";
                default:
                    return $"Unknown error code: {errorCode}";
            }
        }
    }
}
