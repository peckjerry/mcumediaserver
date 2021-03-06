#ifndef _TEXTSTREAM_H_
#define _TEXTSTREAM_H_

#include <pthread.h>
#include "config.h"
#include "codecs.h"
#include "rtpsession.h"
#include "text.h"
#include <deque>

class TextStream
{
public:
	TextStream(RTPSession::Listener* listener);
	~TextStream();

	int Init(TextInput *input,TextOutput *output);
	int SetTextCodec(TextCodec::Type codec);
	int StartSending(char* sendTextIp,int sendTextPort,TextCodec::RTPMap& rtpMap);
	int StopSending();
	int StartReceiving(TextCodec::RTPMap& rtpMap);
	int StopReceiving();
	int SetMute(bool isMuted);
	int End();

	int IsSending()	  { return sendingText;  }
	int IsReceiving() { return receivingText;}
	MediaStatistics GetStatistics();

protected:
	int SendText();
	int RecText();

private:
	struct RedHeader
	{
		DWORD offset;
		DWORD ini;
		DWORD size;
		RedHeader(DWORD offset,DWORD ini,DWORD size)
		{
			this->offset = offset;
			this->ini = ini;
			this->size = size;
		}
	};
	typedef std::deque<TextFrame*> RedFrames;
	typedef std::vector<RedHeader> RedHeaders;

private:
	//Funciones propias
	static void *startSendingText(void *par);
	static void *startReceivingText(void *par);
	TextCodec* CreateTextCodec(TextCodec::Type type);
	//Redundant frames
	RedFrames	reds;
	//Los objectos gordos
	RTPSession	rtp;
	TextInput	*textInput;
	TextOutput	*textOutput;

	//Parametros del text
	TextCodec::Type textCodec;
	BYTE		t140Codec;
	
	//Las threads
	pthread_t 	recTextThread;
	pthread_t 	sendTextThread;

	//Controlamos si estamos mandando o no
	volatile int	sendingText;
	volatile int 	receivingText;

	bool		muted;
};
#endif
