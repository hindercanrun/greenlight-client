#pragma once

namespace online
{
	/*typedef struct _XNADDR
	{
		IN_ADDR ina; // IP address (internal)
		IN_ADDR inaOnline; // Online IP address (external, e.g., via NAT)
		BYTE abEnet[6]; // MAC address
		BYTE abOnline[20]; // Online identifier
		BYTE abExtra[6]; // Reserved/extra data
		BYTE abReserved[8]; // Reserved, usually zero
		WORD wPortOnline; // Online port (e.g., for NAT traversal)
		WORD wFlags; // Status flags
	} XNADDR;*/

	typedef enum _XUSER_SIGNIN_STATE
	{
		eXUserSigninState_NotSignedIn = 0,
		eXUserSigninState_SigningIn,
		eXUserSigninState_SignedInToLive
	} XUSER_SIGNIN_STATE;

	typedef struct _XUser
	{
		DWORD dwUserIndex;
		XUSER_SIGNIN_STATE signinState;
		DWORD lastSigninTime; // Time of the last sign-in attempt
	} XUser;

	typedef struct _XOVERLAPPED
	{
		volatile DWORD Internal; // Operation status (997 for pending, 0 for complete)
		volatile DWORD InternalHigh; // Result data (e.g. # of items or bytes)
		volatile DWORD InternalLow;

		union
		{
			struct
			{
				HANDLE hEvent; // Event handle for signaling
			};

			struct
			{
				DWORD dwCompletionResult;
				DWORD dwExtendedError;
			};
		};
		DWORD dwContext; // controller index
	} XOVERLAPPED, *PXOVERLAPPED;

	typedef struct _XSESSIONID
	{
		BYTE ab[16];
	} XSESSIONID;

	typedef struct _XSESSION_LOCAL_DETAILS
	{
		XNADDR hostAddress;
		XSESSIONID sessionID;
		ULONGLONG qwUserXuid[XUSER_MAX_COUNT]; // usually 4 entries
		DWORD dwUserIndexHost;
		DWORD dwOpenPublicSlots;
		DWORD dwOpenPrivateSlots;
		DWORD dwFlags;
		BYTE bSessionNonce[8];
		BYTE pad[32]; // extra padding to match size
	} XSESSION_LOCAL_DETAILS;

	typedef void* XSESSION_HANDLE;

	typedef struct _XSESSION_MODIFY_SKILL
	{
		FLOAT fPlayerSkill;
		DWORD dwPlayerRating;
	} XSESSION_MODIFY_SKILL;

	typedef uint64_t XUID;

	void changes();
}