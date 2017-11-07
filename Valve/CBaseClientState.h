/*
	  ___           ___           ___           ___
	 /\  \         |\__\         /\  \         /\__\
	/::\  \        |:|  |       /::\  \       /::|  |
   /:/\:\  \       |:|  |      /:/\:\  \     /:|:|  |
  /:/  \:\  \      |:|__|__   /::\~\:\  \   /:/|:|  |__
 /:/__/ \:\__\     /::::\__\ /:/\:\ \:\__\ /:/ |:| /\__\
 \:\  \  \/__/    /:/~~/~~   \/__\:\/:/  / \/__|:|/:/  /
  \:\  \         /:/  /           \::/  /      |:/:/  /
   \:\  \        \/__/            /:/  /       |::/  /
	\:\__\                       /:/  /        /:/  /
	 \/__/                       \/__/         \/__/

revolt (4/2017)
credit: AimTux (https://github.com/AimTuxOfficial/AimTux)
*/

#pragma once


class INetChannel {
public:
	int vtable; //0x0000
	int a;
	int b;
	int c;
	int d;
	unsigned char m_bProcessingMessages; //0x0014
	unsigned char m_bShouldDelete; //0x0015
	char pad_0x0016[0x2]; //0x0016
	int m_nOutSequenceNr; //0x0018
	int m_nInSequenceNr; //0x001C
	int m_nOutSequenceNrAck; //0x0020
	int m_nOutReliableState; //0x0024
	int m_nInReliableState; //0x0028
	int m_nChokedPackets; //0x002C

	// more shit idc about
};

class CBaseClientState
{
public:
	// A1 ? ? ? ? B9 ? ? ? ? 56 FF 50 14 8B 34 85 july 14th
	void ForceFullUpdate()
	{
		*reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + 0x174) = -1;
	}
};