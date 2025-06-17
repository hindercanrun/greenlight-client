#include "..\..\std_include.hpp"
#include "online.hpp"

#include "..\..\utils\hook.hpp"
#include "..\..\utils\string.hpp"

namespace online
{
	namespace bitdemon
	{
		utils::hook::detour bdLogMessage_hook;

		void bdLogMessage_stub(
			game::bdLogMessageType type,
			const char *baseChannel,
			const char *channel,
			const char *file,
			const char *function,
			unsigned int line,
			const char *format,
			...)
		{
			game::Com_Printf(0, "[BitDemon] :: [bdLogMessage] file: %s\n", file);
			game::Com_Printf(0, "[BitDemon] :: [bdLogMessage] function: %s\n", function);
			game::Com_Printf(0, "[BitDemon] :: [bdLogMessage] line: %d\n", line);
			game::Com_Printf(0, "[BitDemon] :: [bdLogMessage] format: %s\n", format);
		}

		utils::hook::detour lobby_connection_get_status_hook;
		utils::hook::detour lobby_service_get_status_hook;
		utils::hook::detour dw_get_lobby_hook;
		utils::hook::detour dwGetOnlineID_hook;
		utils::hook::detour dwSetOnlineID_hook;

		int lobby_connection_get_status(int a1)
		{
			return 1;
		}

		int lobby_service_get_status(int a1)
		{
			return 2;
		}

		void *fakeLobby = (void *)0x82000000; // use any safe dummy address

		int __fastcall dw_get_lobby(int controllerIndex)
		{
			// Optional: simulate per-controller storage if needed
			return 1;
		}

		__int64 dwGetOnlineID_stub(int controllerIndex)
		{
			return 1; // dummy XUID
		}

		bool dwSetOnlineID_stub(int controllerIndex)
		{
			return true; // trick the game
		}

		void stubs()
		{
			//bdLogMessage_hook.create(0x82AC7CA8, bdLogMessage_stub);
			lobby_connection_get_status_hook.create(0x82AB4558, lobby_connection_get_status);
			lobby_service_get_status_hook.create(0x82A892B8, lobby_service_get_status);
			//dw_get_lobby_hook.create(0x8238FF70, dw_get_lobby);
			//dwGetOnlineID_hook.create(0x8238C740, dwGetOnlineID_stub); // dwGetOnlineID
			//dwSetOnlineID_hook.create(0x8238C6B8, dwSetOnlineID_stub); // dwSetOnlineID
		}
	}

	namespace xbox
	{
		utils::hook::detour get_sign_in_state_hook;
		utils::hook::detour enumerate_hook;
		utils::hook::detour get_overlapped_result_hook;
		utils::hook::detour get_addr_for_live_hook;
		utils::hook::detour server_to_in_addr_hook;
		utils::hook::detour addr_to_machine_id_hook;
		utils::hook::detour check_privileges_hook;

		utils::hook::detour addr_to_string_hook;

		utils::hook::detour session_get_details_hook;
		utils::hook::detour session_modify_skill_hook;

		XUSER_SIGNIN_STATE get_sign_in_state(DWORD user_index)
		{
			if (user_index != 0) // only want controller 0 to run this code
				return eXUserSigninState_NotSignedIn;

			static XUser users[4];
			XUser* user = &users[user_index];

			static DWORD time_stamp = 0;
			if (time_stamp == 0)
				time_stamp = GetTickCount();

			DWORD elapsed_time = GetTickCount() - time_stamp;
			if (elapsed_time < 2000)
				user->signinState = eXUserSigninState_NotSignedIn;
			else if (elapsed_time < 4000)
				user->signinState = eXUserSigninState_SigningIn;
			else
				user->signinState = eXUserSigninState_SignedInToLive;

			return user->signinState;
		}

		DWORD WINAPI enumerate(
			HANDLE hEnumerator,
			void* pvBuffer,
			DWORD cbBuffer,
			DWORD* pcItemsReturned,
			void* pOverlapped)
		{
			if (pcItemsReturned)
				*pcItemsReturned = 1;

			if (pvBuffer && cbBuffer >= sizeof(DWORD))
			{
				// Write a fake non-zero server ID (DWORD)
				DWORD* serverList = reinterpret_cast<DWORD*>(pvBuffer);
				serverList[0] = 0x12345678; // Some dummy server ID
			}

			if (pOverlapped)
			{
				XOVERLAPPED* overlapped = reinterpret_cast<XOVERLAPPED*>(pOverlapped);
				overlapped->InternalLow = ERROR_NO_MORE_FILES;
				overlapped->InternalHigh = 1;

				// Signal the async completion event
				if (overlapped->hEvent)
					SetEvent(overlapped->hEvent);

				return ERROR_IO_PENDING;
			}

			return ERROR_NO_MORE_FILES;
		}

		BOOL WINAPI get_overlapped_result(
			PXOVERLAPPED pOverlapped,
			DWORD* pcbResult,
			BOOL bWait)
		{
			if (!pOverlapped)
				return FALSE;

			if (pcbResult)
				*pcbResult = static_cast<DWORD>(pOverlapped->InternalHigh);

			DWORD result = static_cast<DWORD>(pOverlapped->InternalLow);
			if (result == ERROR_IO_INCOMPLETE)
			{
				if (bWait)
				{
					pOverlapped->InternalLow = ERROR_SUCCESS;
					pOverlapped->InternalHigh = 1;

					if (pcbResult)
						*pcbResult = 1;

					return TRUE;
				}

				SetLastError(ERROR_IO_INCOMPLETE);
				return FALSE;
			}

			if (result != ERROR_SUCCESS)
				SetLastError(result);

			return (result == ERROR_SUCCESS);
		}

		game::XNADDR_* get_addr_for_live()
		{
			return game::g_ourXnaddr;
		}

		int server_to_in_addr(unsigned int server, unsigned int serviceId, void *out)
		{
			// Optional: Write a fake secure IP to out
			if (out)
				*(unsigned int *)out = 0xC0A80001; // 192.168.0.1

			return 0; // success
		}

		INT WINAPI addr_to_machine_id(
			const XNADDR* pxnaddr,
			ULONGLONG* pqwMachineId
		) {
			if (!pxnaddr || !pqwMachineId)
				return -1;

			// Stub: generate a fake Machine ID based on MAC address
			const BYTE* mac = pxnaddr->abEnet;

			// Simple hash: just fold MAC into 64-bit integer
			*pqwMachineId =
				((ULONGLONG)mac[0] << 40) |
				((ULONGLONG)mac[1] << 32) |
				((ULONGLONG)mac[2] << 24) |
				((ULONGLONG)mac[3] << 16) |
				((ULONGLONG)mac[4] << 8)  |
				((ULONGLONG)mac[5]);

			return 0; // success
		}

		DWORD check_privileges(
			DWORD index,
			XPRIVILEGE_TYPE type,
			PBOOL result)
		{
			if (!result)
				return -1;

			switch (type)
			{
			case XPRIVILEGE_MULTIPLAYER_SESSIONS:
				*result = TRUE;
				return ERROR_SUCCESS;
			case XPRIVILEGE_COMMUNICATIONS:
				*result = TRUE;
				return ERROR_SUCCESS;
			case XPRIVILEGE_COMMUNICATIONS_FRIENDS_ONLY:
				*result = TRUE;
				return ERROR_SUCCESS;
			case XPRIVILEGE_PROFILE_VIEWING:
				*result = TRUE;
				return ERROR_SUCCESS;
			case XPRIVILEGE_PROFILE_VIEWING_FRIENDS_ONLY:
				*result = TRUE;
				return ERROR_SUCCESS;
			case XPRIVILEGE_USER_CREATED_CONTENT:
				*result = TRUE;
				return ERROR_SUCCESS;
			case XPRIVILEGE_USER_CREATED_CONTENT_FRIENDS_ONLY:
				*result = TRUE;
				return ERROR_SUCCESS;
			case XPRIVILEGE_PURCHASE_CONTENT:
				*result = TRUE;
				return ERROR_SUCCESS;
			case XPRIVILEGE_PRESENCE:
				*result = TRUE;
				return ERROR_SUCCESS;
			case XPRIVILEGE_PRESENCE_FRIENDS_ONLY:
				*result = TRUE;
				return ERROR_SUCCESS;
			default:
				*result = FALSE;
				return -1;
			}
		}

		const char* addr_to_string(const XNADDR* addr)
		{
			// we do not want a 0.0.0.0 ip so hardcode it to what the log file from CUT COPY PASTE says

			const auto* result = "ip: 10.150.10.25, online ip: 64.111.160.21";
			return result;
		}

		DWORD WINAPI session_get_details(
			XSESSION_HANDLE hSession,
			XSESSION_LOCAL_DETAILS* pDetails)
		{
			if (!pDetails)
				return ERROR_INVALID_PARAMETER;

			memset(pDetails, 0, sizeof(XSESSION_LOCAL_DETAILS));

			// Fake host address (XNADDR)
			pDetails->hostAddress.ina.s_addr = 0x6400A8C0; // 192.168.0.100 (little endian)
			pDetails->hostAddress.abEnet[0] = 0x00;
			pDetails->hostAddress.abEnet[1] = 0x11;
				pDetails->hostAddress.abEnet[2] = 0x22;
			pDetails->hostAddress.abEnet[3] = 0x33;
			pDetails->hostAddress.abEnet[4] = 0x44;
			pDetails->hostAddress.abEnet[5] = 0x55;

			// Fill in dummy session ID
			memset(&pDetails->sessionID, 0xCD, sizeof(pDetails->sessionID));

			// Arbitrary 64-bit IDs
			pDetails->qwUserXuid[0] = 0x1000000000000001;
			pDetails->qwUserXuid[1] = 0x1000000000000002;

			pDetails->dwUserIndexHost = 0;
			pDetails->dwOpenPublicSlots = 8;
			pDetails->dwOpenPrivateSlots = 4;

			return ERROR_SUCCESS;
		}

		DWORD session_modify_skill(
			XSESSION_HANDLE hSession,
			DWORD cPlayers,
			const XUID* pqwPlayers,
			const XSESSION_MODIFY_SKILL* pModifySkill,
			XOVERLAPPED* pOverlapped)
		{
			if (pOverlapped)
			{
				pOverlapped->InternalLow = ERROR_SUCCESS;
				pOverlapped->InternalHigh = 0;

				if (pOverlapped->hEvent)
					SetEvent(pOverlapped->hEvent);

				return ERROR_IO_PENDING;
			}

			return ERROR_SUCCESS;
		}

		void stubs()
		{
			get_sign_in_state_hook.create(0x82912118, get_sign_in_state); // XUserGetSigninState
			enumerate_hook.create(0x829185D0, enumerate); // XEnumerate
			get_overlapped_result_hook.create(0x82917C68, get_overlapped_result); // XamEnumerate
			get_addr_for_live_hook.create(0x825D8998, get_addr_for_live); // Xenon_GetXNAddrForLive
			server_to_in_addr_hook.create(0x8292EC50, server_to_in_addr);
			addr_to_machine_id_hook.create(0x8292ECC0, addr_to_machine_id); // XNetXnAddrToMachineId
			check_privileges_hook.create(0x829121A8, check_privileges); // XUserCheckPrivilege
			addr_to_string_hook.create(0x824D1758, addr_to_string); // XNAddrToString
			session_get_details_hook.create(0x82931188, session_get_details); // XSessionGetDetails
			session_modify_skill_hook.create(0x82931348, session_modify_skill); // XSessionModifySkill
		}
	}

	namespace
	{
		utils::hook::detour is_ready_to_party_hook;
		utils::hook::detour storage_try_file_hook;
		utils::hook::detour get_xuid_hook;

		utils::hook::detour is_connected_hook;
		utils::hook::detour is_signed_in_hook;
		utils::hook::detour is_signed_into_live_hook;
		utils::hook::detour do_we_have_ffotd_hook;
		utils::hook::detour validate_ffotd_hook;
		utils::hook::detour do_we_have_playlists_hook;
		utils::hook::detour do_we_have_all_stats_hook;
		utils::hook::detour is_time_synced_hook;
		utils::hook::detour do_we_have_contracts_hook;
		utils::hook::detour do_we_have_current_stats_hook;
		utils::hook::detour do_we_have_online_wad_hook;
		utils::hook::detour do_we_have_playlist_pop_hook;

		bool is_ready_to_party(int controller)
		{
			return true;
		}

		int storage_try_file(game::fileRetryInfo* retryInfo)
		{
			return 1;
		}

		__int64 get_xuid(int controller)
		{
			if (controller != 0)
				return FALSE;

			return 0x0123456789ABCDEF;
		}

		BOOL is_connected(int controller)
		{
			return controller == 0;
		}

		BOOL is_signed_in(int controller)
		{
			if (controller != 0)
				return FALSE;

			return TRUE;
		}

		BOOL is_signed_into_live(int controller)
		{
			if (controller != 0)
				return FALSE;

			return TRUE;
		}

		BOOL do_we_have_ffotd()
		{
			//if (!game::s_haveFFOTD)
			//	return game::s_haveFFOTD = TRUE;

			return TRUE/*game::s_haveFFOTD*/;
		}

		BOOL validate_ffotd()
		{
			return true;
		}

		int do_we_have_playlists()
		{
			return 1;
		}

		BOOL do_we_have_all_stats(int controller)
		{
			//if (!game::byte_84A99C93)
			//	return game::byte_84A99C93 = TRUE;

			//if (!game::byte_84AB7CA2)
			//	return game::byte_84AB7CA2 = TRUE;

			return TRUE;
		}

		BOOL is_time_synced()
		{
			//if (!game::s_UTCSynced)
			//	return game::s_UTCSynced = TRUE;

			//return game::s_UTCSynced;
			return TRUE;
		}

		int do_we_have_contracts()
		{
			return 1;
		}

		int do_we_have_current_stats(int controller)
		{
			static int called = 0;

			if (called == 0)
			{
				called = 1;
				return 0; // false on first call
			}

			return 1; // true on subsequent calls
		}

		BOOL do_we_have_online_wad()
		{
			//if (!game::byte_84C579C8)
			//	return game::byte_84C579C8 = TRUE;

			return /*game::byte_84C579C8*/TRUE;
		}

		int do_we_have_playlist_pop()
		{
			return 1;
		}
	}

	void changes()
	{
		bitdemon::stubs();
		//dcache::stubs();
		xbox::stubs();

		is_ready_to_party_hook.create(0x82337158, is_ready_to_party);

		//storage_try_file_hook = utils::hook::detour((void*)0x82755030, storage_try_file);
		//storage_try_file_hook.install(); // LiveStorage_TryFile

		get_xuid_hook.create(0x82710930, get_xuid);

		is_connected_hook.create(0x82769478, is_connected); // Live_Base_IsConnected
		is_signed_in_hook.create(0x82710880, is_signed_in); // Live_IsSignedIn
		is_signed_into_live_hook.create(0x82710A60, is_signed_into_live); // Live_IsUserSignedInToLive
		//do_we_have_ffotd_hook.create(0x8275FFF8, do_we_have_ffotd); // LiveStorage_DoWeHaveFFOTD
		validate_ffotd_hook.create(0x82760170, validate_ffotd); // LiveStorage_ValidateFFOTD
		do_we_have_playlists_hook.create(0x8275F398, do_we_have_playlists); // LiveStorage_DoWeHavePlaylists
		do_we_have_all_stats_hook.create(0x82754AB0, do_we_have_all_stats); // LiveStorage_DoWeHaveAllStats
		is_time_synced_hook.create(0x827563D0, is_time_synced); // LiveStorage_IsTimeSynced
		do_we_have_contracts_hook.create(0x8275F3C8, do_we_have_contracts); // LLiveStorage_DoWeHaveContracts
		do_we_have_current_stats_hook.create(0x82754A48, do_we_have_current_stats); // LiveStorage_DoWeHaveCurrentStats
		//do_we_have_online_wad_hook.create(0x8275F900, do_we_have_online_wad); // LiveStorage_DoWeHaveOnlineWAD
		//do_we_have_playlist_pop_hook.create(0x8275F900, do_we_have_playlist_pop); // LiveStorage_DoWeHavePlaylistPopulation
	}
}