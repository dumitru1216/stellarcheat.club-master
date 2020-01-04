#pragma once
#include "NetChannel.h"
#include "../includes.h"
namespace SDK
{
	class ClockDriftManager
	{
	public:
		float clock_offsets[17];   //0x0000
		uint32_t current_clock_offset; //0x0044
		uint32_t server_tick;     //0x0048
		uint32_t client_tick;     //0x004C
	}; //Size: 0x0050

	class CClientState
	{
	public:
		void force_full_update()
		{
			*(uint32_t*)((uintptr_t)this + delta_tick) = -1;
		}

		char pad_0000[156];             //0x0000
		SDK::NetChannel* net_channel;          //0x009C
		uint32_t challenge_nr;        //0x00A0
		char pad_00A4[100];             //0x00A4
		uint32_t signon_state;        //0x0108
		char pad_010C[8];               //0x010C
		float next_cmd_time;          //0x0114
		uint32_t server_count;        //0x0118
		uint32_t current_sequence;    //0x011C
		char pad_0120[8];               //0x0120
		ClockDriftManager clock_drift_man; //0x0128
		uint32_t delta_tick;          //0x0178
		bool paused;                 //0x017C
		char pad_017D[3];               //0x017D
		uint32_t view_entity;         //0x0180
		uint32_t player_slot;         //0x0184
		char level_name[260];        //0x0188
		char m_szLevelNameShort[80];    //0x028C
		char m_szGroupName[80];         //0x02DC
		char pad_032С[92];            //0x032С
		uint32_t max_clients;         //0x0388
		char pad_0314[18820];           //0x0314
		float last_server_tick_time;   //0x4C98
		bool in_simulation;              //0x4C9C
		char pad_4C9D[3];               //0x4C9D
		uint32_t old_tickcount;          //0x4CA0
		float tick_remainder;          //0x4CA4
		float frame_time;              //0x4CA8
		uint32_t last_out_going_command;   //0x4CAC
		uint32_t choked_commands;        //0x4CB0
		uint32_t last_command_ack;      //0x4CB4
		uint32_t command_ack;           //0x4CB8
		uint32_t sound_sequence;      //0x4CBC
		char pad_4CC0[80];              //0x4CC0
		Vector viewangles;              //0x4D10
	}; //Size: 0x4D1C
}