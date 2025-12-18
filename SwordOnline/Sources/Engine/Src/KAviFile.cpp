
#include "KWin32.h"
#include "KDebug.h"
#include "KMemBase.h"
#include "KAviFile.h"
static unsigned long str2ulong(char *str)
{
	unsigned long result;
	
	result = (((unsigned long)str[0] & 0xFF) |
		(((unsigned long)str[1] & 0xFF) << 8) |
		(((unsigned long)str[2] & 0xFF) << 16) | 
		(((unsigned long)str[3] & 0xFF) << 24));

	return result;
}
static unsigned long str2ushort(char *str)
{
	return (str[0] | (str[1] << 8));
}

KAviFile::KAviFile()
{
	m_video_pos   = 0;
	m_audio_posc  = 0;
	m_audio_posb  = 0;
	m_idx		  = NULL;
	m_video_index = NULL;
	m_audio_index = NULL;
}
KAviFile::~KAviFile()
{
	Close();
}
BOOL KAviFile::Open(char* Filename)
{
	char head[16];

	Close();

	if (!m_AviFile.Open(Filename))
		return FALSE;

	m_AviFile.Read(head, 12);

	if (memcmp(head, "RIFF", 4) !=0 || 
		memcmp(head + 8, "AVI ", 4) !=0 )
		return FALSE;

	m_video_pos   = 0;
	m_audio_posc  = 0;
	m_audio_posb  = 0;
	m_idx		  = NULL;
	m_video_index = NULL;
	m_audio_index = NULL;
	
	return TRUE;
}
BOOL KAviFile::FillHeader()
{
	DWORD		n;
	int			i;
	long		rate;
	long		scale;
	char*		hdrl_data;
	char*		hdrl_ptr;
	char*		hdrl_end;
	long		hdrl_len;
	int 		lasttag = 0;
	int 		vids_strh_seen = 0;
	int 		vids_strf_seen = 0;
	int 		auds_strh_seen = 0;
	int 		auds_strf_seen = 0;
	int 		num_stream = 0;
	char		data[16];
	
	// go through the AVI file and extract the header list,
	// the start position of the 'movi' list 
	// and an optionally present idx1 tag 
	
	hdrl_data = 0;
	hdrl_len = 0;
	m_movi_start = 0;

	// seek to first LIST chunk
	m_AviFile.Seek(12, FILE_BEGIN);

	// read two LIST chunks and idx1 chunks
	while (1)
	{
		if (m_AviFile.Read(data, 8) != 8)
			break; 

		// we assume it's EOF 
		n = str2ulong(data + 4);
		n = PAD_EVEN(n);
		
		if (memcmp(data, "LIST", 4) == 0)
		{
			if (m_AviFile.Read(data, 4) != 4 )
				return FALSE;
			
			n -= 4;
			
			if (memcmp(data, "hdrl", 4) == 0)
			{
				hdrl_len = n;
				
				hdrl_data = (char *)g_MemAlloc(n);
				
				if (hdrl_data == NULL)
					return FALSE;
				
				if (m_AviFile.Read(hdrl_data, n) != n)
					return FALSE;
			}
			else if (memcmp(data, "movi", 4) == 0)
			{
				m_movi_start = m_AviFile.Tell();
				
				m_AviFile.Seek(n, FILE_CURRENT);
			}
			else // skip n bytes
			{
				m_AviFile.Seek(n, FILE_CURRENT);
			}
		}
		else if (memcmp(data, "idx1", 4) == 0)
		{
			// n must be a multiple of 16, 
			// but the reading does not
			// break if this is not the case 
			m_idx_num = n / 16;

			m_idx = (char(*)[16])g_MemAlloc(n);
			
			if (m_idx == NULL)
				return FALSE;
			
			if (m_AviFile.Read((char *)m_idx, n) != n)
				break;
		}
		else // skip n bytes
		{
			m_AviFile.Seek(n, FILE_CURRENT);
		}
	}
	
	if (hdrl_data == NULL)
		return FALSE;
	
	if (m_movi_start == 0)
		return FALSE;
	
	// interpret the header list 
	hdrl_ptr = hdrl_data;
	hdrl_end = hdrl_data + hdrl_len;

	while (hdrl_ptr < hdrl_end)
	{
		// list tags are completly ignored 
		if (memcmp(hdrl_ptr, "LIST", 4) == 0)
		{ 
			hdrl_ptr += 12;
			continue;
		}
		
		n = str2ulong(hdrl_ptr + 4);
		n = PAD_EVEN(n);
		
		// interpret the tag and its args 
		if (memcmp(hdrl_ptr, "strh", 4) == 0)
		{
			hdrl_ptr += 8;
			if (memcmp(hdrl_ptr, "vids", 4) == 0 && !vids_strh_seen)
			{
				memcpy(m_compressor, hdrl_ptr + 4, 4);
				
				m_compressor[4] = 0;
				
				scale = str2ulong(hdrl_ptr + 20);
				rate  = str2ulong(hdrl_ptr + 24);
				
				if (scale != 0) 
					m_fps = rate / scale;
				
				m_video_frames = str2ulong(hdrl_ptr + 32);
				m_video_strn = num_stream;
				
				vids_strh_seen = 1;
				lasttag = 1;
			}
			else if (memcmp(hdrl_ptr, "auds", 4) == 0 && !auds_strh_seen)
			{
				m_audio_strn = num_stream;
				
				auds_strh_seen = 1;
				lasttag = 2;
			}
			else
			{
				lasttag = 0;
			}
			num_stream++;
		}
		else if (memcmp(hdrl_ptr, "strf", 4) == 0)
		{
			hdrl_ptr += 8;
			if (lasttag == 1)
			{
				// keep a copy of the bitmapinfoheader
				memcpy(&m_BitmapInfoHeader, 
					hdrl_ptr,
					sizeof(BITMAPINFOHEADER));	
				
				vids_strf_seen = 1;
			}
			else if (lasttag == 2)
			{
				// keep a copy of the WAVEFORMATEX
				memcpy(&m_WaveFormatEx,
					hdrl_ptr,
					sizeof(WAVEFORMATEX));
				
				auds_strf_seen = 1;
			}
			lasttag = 0;
		}
		else
		{
			hdrl_ptr += 8;
			lasttag = 0;
		}
		
		hdrl_ptr += n;
	}
	
	g_MemFree(hdrl_data);
	
	if (!vids_strh_seen || !vids_strf_seen || m_video_frames == 0)
	{ 
		return FALSE;
	}
	
	m_video_tag[0] = (char)(m_video_strn / 10 + '0');
	m_video_tag[1] = m_video_strn % 10 + '0';
	m_video_tag[2] = 'd';
	m_video_tag[3] = 'b';
	
	// audio tag is set to "99wb" if no audio present 
	if (m_WaveFormatEx.nChannels == 0)
		m_audio_strn = 99;
	
	m_audio_tag[0] = (char)(m_audio_strn / 10 + '0');
	m_audio_tag[1] = m_audio_strn % 10 + '0';
	m_audio_tag[2] = 'w';
	m_audio_tag[3] = 'b';
	
	// check m_idx
	if (m_idx == NULL)
		return FALSE;

	// if the file has an idx1, check if this is relative
	// to the start of the file or to the start of the movi list 
	m_idx_type = 0;

	// search the first videoframe in the idx1 
	// and look where it is in the file
	for (i = 0; i < m_idx_num; i++)
	{
		if (memcmp(m_idx[i], m_video_tag, 3) == 0)
			break;
	}

	if (i >= m_idx_num)
		return FALSE;
	
	unsigned long pos = str2ulong(m_idx[i] + 8);
	unsigned long len = str2ulong(m_idx[i] + 12);
	
	m_AviFile.Seek(pos, FILE_BEGIN);
	
	if (m_AviFile.Read(data, 8) != 8) 
		return FALSE;
	
	if (memcmp(data, m_idx[i], 4) == 0 && str2ulong(data + 4) == len)
	{
		// index from start of file
		m_idx_type = 1; 
	}
	else
	{
		m_AviFile.Seek(pos + m_movi_start - 4, FILE_BEGIN);
		
		if (m_AviFile.Read(data, 8) != 8)
			return FALSE;
		
		if (memcmp(data, m_idx[i], 4) == 0 && str2ulong(data + 4) == len)
		{
			// index from start of movi list 
			m_idx_type = 2; 
		}
	}

	// m_idx_type remains 0 if neither of the two tests above succeeds 
	if (m_idx_type == 0)
		return FALSE;

	return TRUE;
}	

BOOL KAviFile::GetAudioIndex()
{
	int i, nai, ioff, tot;

	if (!FillHeader())
		return FALSE;

	nai = 0;

	for (i = 0; i < m_idx_num; i++)
	{
		if (memcmp(m_idx[i], m_audio_tag, 4) == 0) 
			nai++;
	}
	
	m_audio_chunks = nai;
	
	if (m_audio_chunks == 0)
		return FALSE;

	m_audio_index = (audio_index_entry *)g_MemAlloc(nai * sizeof(audio_index_entry));

	if (m_audio_index == 0)
		return FALSE;
	
	nai = 0;
	tot = 0;
	
	ioff = (m_idx_type == 1)? 8 : m_movi_start + 4;
	
	for (i = 0; i < m_idx_num; i++)
	{
		if (memcmp(m_idx[i], m_audio_tag, 4) == 0)
		{
			m_audio_index[nai].pos = str2ulong(m_idx[i] + 8) + ioff;
			m_audio_index[nai].len = str2ulong(m_idx[i] + 12);
			m_audio_index[nai].tot = tot;
			
			tot += m_audio_index[nai].len;
			nai++;
		}
	}
	
	m_audio_bytes = tot;

	g_MemFree(m_idx);
	m_idx = NULL;

	return TRUE;
}

BOOL KAviFile::GetVideoIndex()
{
	int i, nvi, ioff;

	if (!FillHeader())
		return FALSE;

	nvi = 0;
	
	for (i = 0; i < m_idx_num; i++)
	{
		if (memcmp(m_idx[i], m_video_tag, 3) == 0) 
			nvi++;
	}
	
	m_video_frames = nvi;
	
	if (m_video_frames == 0) 
		return FALSE;
	
	m_video_index = (video_index_entry *)g_MemAlloc(nvi * sizeof(video_index_entry));
	
	if (m_video_index == 0)
		return FALSE;
	
	nvi = 0;
	
	ioff = (m_idx_type == 1)? 8 : m_movi_start + 4;
	
	for (i = 0; i < m_idx_num; i++)
	{
		if (memcmp(m_idx[i], m_video_tag, 3) == 0)
		{
			m_video_index[nvi].flags = str2ulong(m_idx[i] + 4);
			m_video_index[nvi].pos	 = str2ulong(m_idx[i] + 8) + ioff;
			m_video_index[nvi].len	 = str2ulong(m_idx[i] + 12);

			nvi++;
		}
	}
	
	m_video_pos = 0;
	
	g_MemFree(m_idx);
	m_idx = NULL;

	return TRUE;
}

int KAviFile::VideoStreams()
{
	return m_video_strn;
}
	

int KAviFile::AudioStreams()
{
	return m_audio_strn;
}

void KAviFile::GetBitmapInfoHeader(PBITMAPINFOHEADER pBmpInfoHead)
{
	*pBmpInfoHead = m_BitmapInfoHeader;
}

void KAviFile::GetWaveFormat(PWAVEFORMATEX pWavFmt)
{
	*pWavFmt = m_WaveFormatEx;
}

void KAviFile::GetCompressMethod(char* method)
{
	memcpy(method, m_compressor, 5);
}

int KAviFile::NextFrame(unsigned char* buffer)
{
	int nNeedBytes;
	int nReadBytes;
	
	if (!m_video_index)		   
		return -1;
	
	if (m_video_pos < 0 || m_video_pos >= m_video_frames)
		return -2;
	
	nNeedBytes = m_video_index[m_video_pos].len;
	
	m_AviFile.Seek(m_video_index[m_video_pos].pos, FILE_BEGIN);

	nReadBytes = m_AviFile.Read(buffer, nNeedBytes);

	m_video_pos++;

	return nReadBytes;
}

int KAviFile::ReadAudio(unsigned char* audbuf, int bytes)
{
	int nr, pos, left, todo;
	
	if (!m_audio_index)
		return -1;
	
	nr = 0; 
	
	// We loop until we parsed enough
	// chunks for the amount we want
	while (bytes > 0)
	{
		left = m_audio_index[m_audio_posc].len - m_audio_posb;
		
		if (left == 0)
		{
			if (m_audio_posc >= m_audio_chunks - 1)
			{
				return nr;
			}
			m_audio_posc++;
			m_audio_posb = 0;
			continue;
		}

		if (bytes < left)
			todo = bytes;
		else
			todo = left;
		
		pos = m_audio_index[m_audio_posc].pos + m_audio_posb;
		
		m_AviFile.Seek(pos, FILE_BEGIN);
		
		m_AviFile.Read(audbuf + nr, todo);

		bytes -= todo;
		nr	  += todo;
		m_audio_posb += todo;
	}

	return nr;
}

int KAviFile::FrameRate()
{
	// Fix for some trailers
	if (m_fps == 0)
		m_fps = 25;
	
	if (m_fps == 23)
		m_fps = 25;

	return m_fps;
}

int KAviFile::CurrentFrame()
{
	return m_video_pos;
}

int KAviFile::TotalFrames()
{
	return m_video_frames;
}

int KAviFile::VideoSeek(int percent)
{
	int frame;

	if (!m_video_index)
		return -1; 

	// limit to 0-100
	if (percent < 0)
		percent = 0;
	if (percent > 100)
		frame = 100;

	// compute the desired frame number
	frame = percent * m_video_frames / 100;
	
	// and go to the next keyframe.
	while (!IsKeyframe(frame))
	{
		frame++;
	}

	m_video_pos = frame;
	
	return frame;
}

int KAviFile::AudioSeek(int percent)
{
	int frame;
	int bytes;
	int n0, n1, n;

	if (!m_audio_index)
		return -1; 

	// limit to 0-100
	if (percent < 0)
		percent = 0;
	if (percent > 100)
		frame = 100;

	// compute the desired frame number
	frame = percent * m_video_frames / 100;

	// and set audio position
	bytes = frame * m_audio_bytes / m_video_frames;
	bytes += bytes % 4;
		
	if (bytes < 0) 
		bytes = 0;
	
	n0 = 0;
	n1 = m_audio_chunks;
	
	while (n0 < n1 - 1)
	{
		n = (n0 + n1) / 2;
		if (m_audio_index[n].tot > bytes)
			n1 = n;
		else
			n0 = n;
	}
	
	m_audio_posc = n0;
	
	if (m_audio_index[n0].len > 1000)
	{
		m_audio_posb = bytes - m_audio_index[n0].tot;
	}
	else
	{
		m_audio_posb = 0;
	}
	
	return frame;
}

BOOL KAviFile::IsKeyframe(int frame)
{
	// we still return 1 to avoid looping on waiting for a keyframe.
	if (!m_video_index)		   
		return 1; 

	if (frame < 0)
		frame = 0;
	
	return m_video_index[frame].flags & AVIIF_KEYFRAME;
}

int KAviFile::NextKeyFrame()
{
	// Allways increment by one
	m_video_pos++;
	
	while (!IsKeyframe(m_video_pos) && m_video_pos < m_video_frames)
		m_video_pos++;
	
	return 1;
}

int KAviFile::PreviousKeyFrame()
{
	// Allways decrement by two since we read the last frame
	m_video_pos--;
	m_video_pos--;
	
	while (!IsKeyframe(m_video_pos) && m_video_pos > 0)
		m_video_pos--;
	
	return 1;
}

int KAviFile::Rewind()
{
	m_video_pos  = 0;
	m_audio_posc = 0;
	m_audio_posb = 0;
	
	return 1;
}

void KAviFile::Close()
{
	if (m_video_index)
	{
		g_MemFree(m_video_index);
		m_video_index = NULL;
	}
	if (m_audio_index)
	{
		g_MemFree(m_audio_index);
		m_audio_index = NULL;
	}
}

